#pragma once

#include <acore/math/ndrange.hpp>
#include <acore/math/utilities.hpp>
#include <autils/log.hpp>
#include <iostream>
#include <queue>

#include "../subdivision.hpp"

namespace acg::spatial {

template <typename F, typename D, int dim, UInt32 subdivision_count,
          UInt32 max_depth>
size_t SubDivisionAABB<F, D, dim, subdivision_count, max_depth>::Insert(
    const std::pair<AABB<F, dim>, D> &data) {
  auto ind = PutAABBData(data);
  size_t parent = EnsureEntry(data.first);
  if (parent == InvalidSubscript) {
    large_leafs_.push_back(ind);
    return parent;
  }
  auto generate_sequence = FindVisitSequence(data.first);
  auto it = generate_sequence.begin();
  auto child = *(++it);

  while (child != InvalidSubscript && it != generate_sequence.end()) {
    auto ret = EnsureSubDivision(parent, child);
    parent = ret;
    child = *(++it);
  }

  // Now, parent is the index for aabb store.
  ACG_DEBUG_CHECK(parent < nodes_.size() && parent >= 0, "Invalid node id.");
  auto &node = nodes_.at(parent);
  node.leafs_.push_back(ind);
  return parent;
}

template <typename F, typename D, int dim, UInt32 subdivision_count,
          UInt32 max_depth>
size_t
SubDivisionAABB<F, D, dim, subdivision_count, max_depth>::EnsureSubDivision(
    size_t parent, size_t child) {
  ACG_DEBUG_CHECK(parent < nodes_.size() && parent >= 0, "Invalid node id.");
  auto &pnode = nodes_.at(parent);
  ACG_DEBUG_CHECK(child < pnode.sub_nodes_.size() && child >= 0,
                  "Invalid node id.");
  auto &cid = pnode.sub_nodes_.at(child);
  if (cid == InvalidSubscript) {
    auto position = pnode.GetChildAABB(to_tuple(indexer_.Backward(child)));
    auto child_id = PutNode(pnode.unit_ / subdivision_count, position);
    cid = child_id;
  }
  return cid;
}

template <typename F, typename D, int dim, UInt32 subdivision_count,
          UInt32 max_depth>
void SubDivisionAABB<F, D, dim, subdivision_count, max_depth>::Clear() {
  large_leafs_.clear();
  nodes_.clear();
  entry_nodes_.clear();
  data_.clear();
}

template <typename F, typename D, int dim, UInt32 subdivision_count,
          UInt32 max_depth>
size_t SubDivisionAABB<F, D, dim, subdivision_count, max_depth>::EnsureEntry(
    const AABB<F, dim> &aabb) {
  auto ub_ceil = aabb.upper_bound_.array().ceil().eval();
  auto lb_floor = aabb.lower_bound_.array().floor().eval();
  if ((ub_ceil - unit_ != lb_floor).any()) {
    // Too Large
    return InvalidSubscript;
  }

  auto possible_entry = FindEntry(aabb);
  if (possible_entry != InvalidSubscript) {
    return possible_entry;
  }

  // Otherwise, create a new entry.
  auto entry_id = PutNode(unit_ / static_cast<F>(subdivision_count),
                          {lb_floor.matrix(), ub_ceil.matrix()});
  entry_nodes_.push_back(entry_id);
  return entry_id;
};

template <typename F, typename D, int dim, UInt32 subdivision_count,
          UInt32 max_depth>
size_t SubDivisionAABB<F, D, dim, subdivision_count, max_depth>::PutNode(
    F local_unit, const AABB<F, dim> &range) {
  nodes_.emplace_back(local_unit, range);
  return nodes_.size() - 1;
}

template <typename F, typename D, int dim, UInt32 subdivision_count,
          UInt32 max_depth>
size_t SubDivisionAABB<F, D, dim, subdivision_count, max_depth>::FindEntry(
    const AABB<F, dim> &aabb) const {
  // Foreach entry, test if the entry can hold the AABB
  for (const auto &i : entry_nodes_) {
    ACG_DEBUG_CHECK(i >= 0 && i < nodes_.size(), "Invalid subscript.");
    if (nodes_.at(i).box_.Contain(aabb)) {
      return i;
    }
  }
  return InvalidSubscript;
}

template <typename F, typename D, int dim, UInt32 subdivision_count,
          UInt32 max_depth>
size_t SubDivisionAABB<F, D, dim, subdivision_count, max_depth>::PutAABBData(
    const std::pair<AABB<F, dim>, D> &aabb) {
  auto id = data_.size();
  data_.push_back(aabb);
  return id;
}

template <typename F, typename D, int dim, UInt32 subdivision_count,
          UInt32 max_depth>
AABB<F, dim>
SubDivisionAABB<F, D, dim, subdivision_count, max_depth>::Node::GetChildAABB(
    utils::god::IndexTuple<dim> index) const {
  auto local_index_nd = std::make_from_tuple<Vec<Index, dim>>(index);
  auto units = Vec<F, dim>::Constant(unit_);
  auto lb = (box_.lower_bound_ +
             units.cwiseProduct(local_index_nd.template cast<F>()))
                .eval();
  auto ub = lb + units;
  return AABB<F, dim>(lb, ub);
}

template <typename F, typename D, int dim, UInt32 subdivision_count,
          UInt32 max_depth>
std::array<size_t, max_depth>
SubDivisionAABB<F, D, dim, subdivision_count, max_depth>::FindVisitSequence(
    const AABB<F, dim> &aabb) {
  // First, find parent.
  std::array<size_t, max_depth> seq;
  seq.fill(InvalidSubscript);
  auto it = seq.begin();
  auto id = FindEntry(aabb);
  *it = id;
  if (id != InvalidSubscript) {
    ACG_DEBUG_CHECK(id < nodes_.size() && id >= 0, "Invalid node id.");
    Node &entry_node = nodes_.at(id);
    Vec<F, dim> local_lb =
        (aabb.lower_bound_ - entry_node.box_.lower_bound_) / entry_node.unit_;
    Vec<F, dim> local_ub =
        (aabb.upper_bound_ - entry_node.box_.lower_bound_) / entry_node.unit_;
    ++it;
    while (id != InvalidSubscript && it != seq.end()) {
      Vec<Index, dim> local_index_l =
          local_lb.array().floor().template cast<Index>();
      Vec<Index, dim> local_index_u =
          local_ub.array().ceil().template cast<Index>();
      // Too large for this cell.
      if ((local_index_l.array() + static_cast<Index>(1) !=
           local_index_u.array())
              .any()) {
        break;
      }

      // Find the local index.
      auto index = std::apply(indexer_, make_tuple_from_vector(local_index_l));
      *it = index;
      ++it;

      // Update Local Coordinate
      local_lb =
          (local_lb - local_index_l.template cast<F>()) * subdivision_count;
      local_ub =
          (local_ub - local_index_l.template cast<F>()) * subdivision_count;
    }
  }
  return seq;
}

template <typename F, typename D, int dim, UInt32 subdivision_count,
          UInt32 max_depth>
std::pair<Field<F, dim>, types::topology::LineList>
SubDivisionAABB<F, D, dim, subdivision_count, max_depth>::Visualize() const {
  Field<F, dim> position =
      FieldBuilder<F, dim>(nodes_.size() * 8).Placeholder();
  types::topology::LineList lines =
      FieldBuilder<Index, 2>(nodes_.size() * 12).Placeholder();
  auto pacc = view(position);
  auto iacc = view(lines);
  Index np = 0, nl = 0;
  for (const auto &node : nodes_) {
    auto l = node.box_.lower_bound_;
    auto u = node.box_.upper_bound_;
    iacc(nl++) = Vec2<Index>{np, np + 1};
    iacc(nl++) = Vec2<Index>{np + 1, np + 2};
    iacc(nl++) = Vec2<Index>{np + 2, np + 3};
    iacc(nl++) = Vec2<Index>{np + 3, np + 0};
    iacc(nl++) = Vec2<Index>{np + 4, np + 5};
    iacc(nl++) = Vec2<Index>{np + 5, np + 6};
    iacc(nl++) = Vec2<Index>{np + 6, np + 7};
    iacc(nl++) = Vec2<Index>{np + 7, np + 4};
    iacc(nl++) = Vec2<Index>{np + 0, np + 4};
    iacc(nl++) = Vec2<Index>{np + 1, np + 5};
    iacc(nl++) = Vec2<Index>{np + 2, np + 6};
    iacc(nl++) = Vec2<Index>{np + 3, np + 7};
    pacc(np++) = l;
    pacc(np++) = Vec3f{l.x(), u.y(), l.z()};
    pacc(np++) = Vec3f{u.x(), u.y(), l.z()};
    pacc(np++) = Vec3f{u.x(), l.y(), l.z()};
    pacc(np++) = Vec3f{l.x(), l.y(), u.z()};
    pacc(np++) = Vec3f{l.x(), u.y(), u.z()};
    pacc(np++) = Vec3f{u.x(), u.y(), u.z()};
    pacc(np++) = Vec3f{u.x(), l.y(), u.z()};
  }
  return {std::move(position), std::move(lines)};
}

template <typename F, typename D, int dim, UInt32 subdivision_count,
          UInt32 max_depth>
std::pair<AABB<F, dim>, D> &
SubDivisionAABB<F, D, dim, subdivision_count, max_depth>::Get(size_t id) {
  ACG_DEBUG_CHECK(id < data_.size() && id >= 0, "Invalid id");
  return data_.at(id);
}

template <typename F, typename D, int dim, UInt32 subdivision_count,
          UInt32 max_depth>
const std::pair<AABB<F, dim>, D> &
SubDivisionAABB<F, D, dim, subdivision_count, max_depth>::Get(size_t id) const {
  ACG_DEBUG_CHECK(id < data_.size() && id >= 0, "Invalid id");
  return data_.at(id);
}
template <typename F, typename D, int dim, UInt32 subdivision_count,
          UInt32 max_depth>
std::vector<size_t>
SubDivisionAABB<F, D, dim, subdivision_count, max_depth>::Query(
    const AABB<F, dim> &aabb) const {
  std::vector<size_t> retval;
  // 1. Large leafs.
  for (auto id : large_leafs_) {
    const auto &entity = Get(id);

    if (entity.first.Intersect(aabb)) {
      retval.push_back(id);
    }
  }

  // 2. for each entry...
  for (auto id : entry_nodes_) {
    QueryVisit(retval, id, aabb);
  }
  return retval;
}
template <typename F, typename D, int dim, UInt32 subdivision_count,
          UInt32 max_depth>
void SubDivisionAABB<F, D, dim, subdivision_count, max_depth>::QueryVisit(
    std::vector<size_t> &retval, size_t node, const AABB<F, dim> &aabb) const {
  ACG_DEBUG_CHECK(node < nodes_.size() && node >= 0, "Invalid node id.");
  const auto &n = nodes_.at(node);
  if (n.box_.Intersect(aabb)) {
    for (auto leaf: n.leafs_) {
      retval.push_back(leaf);
    }

    for (auto id : n.sub_nodes_) {
      if (id != InvalidSubscript) {
        QueryVisit(retval, id, aabb);
      }
    }
  }
}

template <typename F, typename D, int dim, UInt32 subdivision_count,
          UInt32 max_depth>
std::vector<std::pair<size_t, size_t>>
SubDivisionAABB<F, D, dim, subdivision_count, max_depth>::QueryInternal()
    const {
  std::vector<std::pair<size_t, size_t>> retval;
  // 1. Large leafs.
  for (auto id : large_leafs_) {
    const auto &entity = Get(id);
    auto leaf_intersect = Query(entity.first);
    for (auto another : leaf_intersect) {
      retval.push_back({id, another});
    }
  }

  // 2. for each entry...
  for (auto id : entry_nodes_) {
    QueryVisitInternal(retval, id);
  }
  return retval;
}
template <typename F, typename D, int dim, UInt32 subdivision_count,
          UInt32 max_depth>
void SubDivisionAABB<F, D, dim, subdivision_count, max_depth>::
    QueryVisitInternal(std::vector<std::pair<size_t, size_t>> &retval,
                       size_t node) const {
  ACG_DEBUG_CHECK(node < nodes_.size() && node >= 0, "Invalid node id.");
  const auto &n = nodes_.at(node);
  for (auto l : n.leafs_) {
    std::vector<size_t> intersect;
    QueryVisit(intersect, node, Get(l).first);
    for (auto r : intersect) {
      retval.push_back({l, r});
    }
  }

  for (auto id : n.sub_nodes_) {
    if (id != InvalidSubscript) {
      QueryVisitInternal(retval, id);
    }
  }
}
} // namespace acg::spatial
