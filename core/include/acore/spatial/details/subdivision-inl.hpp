#pragma once

#include <acore/math/ndrange.hpp>
#include <acore/math/utilities.hpp>
#include <autils/log.hpp>
#include <iostream>

#include "../subdivision.hpp"

namespace acg::spatial {

template <typename F, typename D, int dim, UInt32 subdivision_count,
          UInt32 max_depth>
size_t SubDivisionAABB<F, D, dim, subdivision_count, max_depth>::Insert(
    const AABB<D, F, dim> &aabb) {
  auto ind = PutAABBData(aabb);
  size_t parent = EnsureEntry(aabb);
  if (parent == InvalidSubscript) {
    large_leafs_.push_back(ind);
    return parent;
  }
  auto generate_sequence = FindVisitSequence(aabb);

  ACG_DEBUG_CHECK(generate_sequence.front() == parent,
                  "Generated sequence does not start with entry. {} v.s. {}, "
                  "Input = {} by {}",
                  generate_sequence.front(), parent,
                  fmt::streamed(aabb.lower_bound.transpose()),
                  fmt::streamed(aabb.upper_bound.transpose()));
  auto it = generate_sequence.begin();
  auto child = *(++it);

  while (child != InvalidSubscript && it != generate_sequence.end()) {
    auto ret = EnsureSubDivision(parent, child);
    // Check the ensured child:
    parent = ret;
    child = *(++it);
  }

  // Now, parent is the index for aabb store.
  auto &node = nodes_[parent];
  node.leafs_.push_back(ind);
  return parent;
}

template <typename F, typename D, int dim, UInt32 subdivision_count,
          UInt32 max_depth>
size_t
SubDivisionAABB<F, D, dim, subdivision_count, max_depth>::EnsureSubDivision(
    const size_t parent, const size_t child) {
  if (nodes_[parent].sub_nodes_[child] == InvalidSubscript) {
    auto position = nodes_[parent].GetChildAABB(indexer_[child]);
    auto child_id = PutNode(nodes_[parent].unit_ / subdivision_count, position,
                            nodes_[parent].depth_ + 1);
    nodes_[parent].sub_nodes_[child] = child_id;
  }
  return nodes_[parent].sub_nodes_[child];
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
    const AABB<D> &aabb) {
  auto ub_ceil = aabb.upper_bound.array().ceil().eval();
  auto lb_floor = aabb.lower_bound.array().floor().eval();
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
                          {lb_floor.matrix(), ub_ceil.matrix()}, 0);
  // ACG_INFO("Entry {} by {} created for {} by {}",
  //   fmt::streamed(nodes_[entry_id].box_.lower_bound.transpose()),
  //   fmt::streamed(nodes_[entry_id].box_.upper_bound.transpose()),
  //   fmt::streamed(aabb.lower_bound.transpose()),
  //   fmt::streamed(aabb.lower_bound.transpose()));
  entry_nodes_.push_back(entry_id);
  return entry_id;
};

template <typename F, typename D, int dim, UInt32 subdivision_count,
          UInt32 max_depth>
size_t SubDivisionAABB<F, D, dim, subdivision_count, max_depth>::PutNode(
    F local_unit, const AABB<void, F, dim> &range, size_t depth) {
  // ACG_WARN("Putting a new node: unit={} depth={}, lb={}, ub={}", local_unit,
  //          depth, fmt::streamed(range.lower_bound.transpose()),
  //          fmt::streamed(range.upper_bound.transpose()));
  nodes_.emplace_back(local_unit, range, depth);
  return nodes_.size() - 1;
}

template <typename F, typename D, int dim, UInt32 subdivision_count,
          UInt32 max_depth>
size_t SubDivisionAABB<F, D, dim, subdivision_count, max_depth>::FindEntry(
    const AABB<D, F, dim> &aabb) const {
  // Foreach entry, test if the entry can hold the AABB
  for (const auto &i : entry_nodes_) {
    if (nodes_[i].box_.Contain(aabb)) {
      return i;
    }
  }
  return InvalidSubscript;
}

template <typename F, typename D, int dim, UInt32 subdivision_count,
          UInt32 max_depth>
size_t SubDivisionAABB<F, D, dim, subdivision_count, max_depth>::PutAABBData(
    const AABB<D> &aabb) {
  auto id = data_.size();
  data_.push_back(aabb);
  return id;
}

template <typename F, typename D, int dim, UInt32 subdivision_count,
          UInt32 max_depth>
AABB<void, F, dim>
SubDivisionAABB<F, D, dim, subdivision_count, max_depth>::Node::GetChildAABB(
    utils::god::IndexTuple<dim> index) const {
  auto local_index_nd = std::make_from_tuple<Vec<Index, dim>>(index);
  auto units = Vec<F, dim>::Constant(unit_);
  auto lb =
      (box_.lower_bound + units.cwiseProduct(local_index_nd.template cast<F>()))
          .eval();
  auto ub = lb + units;
  return AABB<void, F, dim>(lb, ub);
}

template <typename F, typename D, int dim, UInt32 subdivision_count,
          UInt32 max_depth>
std::array<size_t, max_depth>
SubDivisionAABB<F, D, dim, subdivision_count, max_depth>::FindVisitSequence(
    const AABB<D, F, dim> &aabb) {
  // First, find parent.
  std::array<size_t, max_depth> seq;
  seq.fill(InvalidSubscript);
  auto it = seq.begin();
  auto id = FindEntry(aabb);
  *it = id;
  if (id != InvalidSubscript) {
    Node &entry_node = nodes_[id];
    Vec<F, dim> local_lb =
        (aabb.lower_bound - entry_node.box_.lower_bound) / entry_node.unit_;
    Vec<F, dim> local_ub =
        (aabb.upper_bound - entry_node.box_.lower_bound) / entry_node.unit_;
    // auto inspect = [&]() {
    //   ACG_WARN("Lower = {}, Upper = {}", fmt::streamed(local_lb.transpose()),
    //            fmt::streamed(local_ub.transpose()));
    // };
    // ACG_INFO("Input = {} by {}, Entry = {} by {}",
    //          fmt::streamed(aabb.lower_bound.transpose()),
    //          fmt::streamed(aabb.upper_bound.transpose()),
    //          fmt::streamed(entry_node.box_.lower_bound.transpose()),
    //          fmt::streamed(entry_node.box_.upper_bound.transpose()));
    // inspect();
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
      // ACG_CHECK(index < subdivision_count_total_,
      //           "Got index greater than total supported! {} > = {}, vec = "
      //           "{} by {},vec_index = {},"
      //           "Entry Unit = {}",
      //           index, subdivision_count_total_,
      //           fmt::streamed(local_lb.transpose()),
      //           fmt::streamed(local_ub.transpose()),
      //           fmt::streamed(local_index_l.transpose()), entry_node.unit_);
      ++it;

      // Update Local Coordinate
      local_lb =
          (local_lb - local_index_l.template cast<F>()) * subdivision_count;
      local_ub =
          (local_ub - local_index_l.template cast<F>()) * subdivision_count;
      // inspect();
    }
  }
  return seq;
}
template <typename F, typename D, int dim, UInt32 subdivision_count,
          UInt32 max_depth>
std::pair<Field<F, dim>, geometry::topology::LineList>
SubDivisionAABB<F, D, dim, subdivision_count, max_depth>::Visualize() const {
  Field<F, dim> position =
      FieldBuilder<F, dim>(nodes_.size() * 8).Placeholder();
  geometry::topology::LineList lines =
      FieldBuilder<Index, 2>(nodes_.size() * 12).Placeholder();

  auto pacc = access(position);
  auto iacc = access(lines);
  Index np = 0, nl = 0;
  for (const auto &node : nodes_) {
    auto l = node.box_.lower_bound;
    auto u = node.box_.upper_bound;
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

  return {position, lines};
}

} // namespace acg::spatial
