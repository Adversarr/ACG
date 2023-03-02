#pragma once

#include <acore/geometry/common.hpp>
#include <acore/math/access.hpp>
#include <autils/god/god.hpp>
#include <autils/log.hpp>
#include <vector>

#include "common.hpp"

namespace acg::spatial {

template <typename F, typename D, int dim, UInt32 subdivision_count = 4,
          UInt32 max_depth = 4>
class SubDivisionAABB {
public:
  static constexpr UInt32 subdivision_count_total_ =
      utils::god::pow<dim>(subdivision_count);
  static_assert(max_depth > 0, "Max depth should be greater than 0.");
  static_assert(utils::god::pow<max_depth>(subdivision_count_total_) <
                    std::numeric_limits<size_t>::max(),
                "subdivision too large.");

  struct Node {
    AABB<void, F, dim> box_;
    F unit_;
    size_t depth_;
    size_t sub_nodes_[subdivision_count_total_];
    std::vector<size_t> leafs_;

    AABB<void, F, dim> GetChildAABB(utils::god::IndexTuple<dim> index) const;

    size_t GetAABBIndex(const AABB<D, F, dim> &aabb) const;

    Node(const Node &rhs)
        : box_(rhs.box_), unit_(rhs.unit_), depth_(rhs.depth_),
          leafs_(rhs.leafs_.begin(), rhs.leafs_.end()) {
      std::copy(std::begin(rhs.sub_nodes_), std::end(rhs.sub_nodes_),
                std::begin(sub_nodes_));
    }
    Node(Node &&rhs)
        : box_(rhs.box_), unit_(rhs.unit_), depth_(rhs.depth_),
          leafs_(std::move(rhs.leafs_)) {
      std::copy(std::begin(rhs.sub_nodes_), std::end(rhs.sub_nodes_),
                std::begin(sub_nodes_));
    }

    explicit Node(F unit, AABB<void, F, dim> box, size_t depth)
        : box_(box), unit_(unit), depth_(depth) {
      std::fill(std::begin(sub_nodes_), std::end(sub_nodes_), InvalidSubscript);
    }
  };

  size_t CountLeaf() const;

  size_t Insert(const AABB<D, F, dim> &aabb);

  std::pair<Field<F, dim>, geometry::topology::LineList> Visualize() const;

  void Clear();

  template <typename B, typename E> void Insert(B beg, E end);

  std::vector<size_t> Query(const AABB<void> &aabb) const;

  explicit SubDivisionAABB(F unit_length = 1.0, F epsilon = 1e-6)
      : epsilon_(epsilon), unit_(unit_length),
        indexer_(std::make_from_tuple<NdRangeIndexer<dim>>(
            utils::god::duplicate<dim>(subdivision_count))) {}

  // private:

  // @brief Query the entry node's id for aabb.
  size_t FindEntry(const AABB<D, F, dim> &aabb) const;

  // @brief return the sequence for visit given node.
  //
  // [parent = Entry, Sub1, Sub2, ... , Destination]
  //
  // for large leafs, return [InvalidSubscript ... ]
  std::array<size_t, max_depth> FindVisitSequence(const AABB<D, F, dim> &aabb);

  size_t PutAABBData(const AABB<D> &aabb);

  size_t PutNode(F local_unit, const AABB<void, F, dim> &range, size_t depth);

  size_t EnsureSubDivision(const size_t parent, const size_t child);

  size_t EnsureEntry(const AABB<D> &aabb);

  std::vector<AABB<D>> data_;
  std::vector<Node> nodes_;
  std::vector<size_t> entry_nodes_;

  // @brief for those AABB larger than U x U x U, store them directly.
  std::vector<size_t> large_leafs_;

  // @brief Indicates the maximal length for the root aabb.
  const F epsilon_ = std::numeric_limits<F>::min();
  const F unit_;
  NdRangeIndexer<dim> indexer_;
};

} // namespace acg::spatial
#include "details/subdivision-inl.hpp"
