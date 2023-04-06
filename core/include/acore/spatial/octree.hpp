#pragma once

#include <Eigen/Geometry>
#include <acore/math/func.hpp>
#include <acore/math/indexer.hpp>

namespace acg::spatial {

template <typename Scalar, typename Object, int dim, int subd = 4, int max_depth = 4>
class BoundedOctree {
  using BoundingBox = Eigen::AlignedBox<Scalar, dim>;
  using StorageType = std::pair<BoundingBox, Object>;
  constexpr static int sub_tree_size = acg::math::pow<dim>(subd);

  // Root node has depth 0
  size_t depth_{0};

  // Large leafs of this tree contain.
  std::vector<StorageType> leafs_;

  // Range of the tree, assert that leafs inside range.
  BoundingBox range_;

  // Childs
  std::array<std::unique_ptr<BoundedOctree>, sub_tree_size> child_tree_;

  NdRangeIndexer<3> idxer_;

  BoundedOctree()
      : idxer_(std::make_from_tuple<NdRangeIndexer<dim>>(
            utils::god::tuple_duplicate<dim>(subd))) {
    child_tree_.fill(nullptr);
  }

  explicit BoundedOctree(BoundingBox box) : range_(box), BoundedOctree() {}

  void Insert(BoundingBox box, Object object_id);

private:
  
};

} // namespace acg::spatial
