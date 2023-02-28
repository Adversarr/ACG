#pragma once

#include <autils/meta.hpp>
#include <vector>

#include "common.hpp"

namespace acg::spatial {

template <typename F, typename D, int dim, UInt32 subdivision_count = 2> class SubDivisionAABB {
public:
  static constexpr UInt32 subdivision_count_total_ = utils::ctpow<dim>(subdivision_count);

  struct Node {
    AABB<void, F, dim> box;
    std::array<std::unique_ptr<Node>, subdivision_count_total_> sub_nodes;
    std::vector<Index> leafs;
    
    // TODO: start here.
  };

  Index CountLeaf() const;

  void Insert(AABB<D> aabb);

  std::vector<Index> Query(AABB<void> aabb) const;

private:
  std::vector<AABB<D>> data_;
  std::vector<Node> entry_nodes_;

  // @brief for those AABB larger than U x U x U, store them directly.
  std::vector<Index> large_leafs_;

  // @brief Indicates the maximal length for the root aabb.
  const F unit_length_;
};

}  // namespace acg::spatial
#include "details/subdivision-inl.hpp"
