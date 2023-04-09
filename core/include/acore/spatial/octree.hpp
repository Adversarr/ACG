#pragma once

#include <acore/math/view.hpp>
#include <Eigen/Geometry>
#include <acore/math/coordinate/common.hpp>
#include <acore/math/coordinate/continuous_discrete.hpp>
#include <acore/math/func.hpp>
#include <acore/math/indexer.hpp>
#include <acore/spatial/common.hpp>
#include <memory>

namespace acg::spatial {

template<typename Scalar, int dim = 3, int subd = 2, int max_depth = 4,
  size_t cacheline_size = 16>
class BoundedOctree {
  using BoundingBox = AlignedBox<Scalar, dim>;
  using StorageType = std::pair<BoundingBox, Index>;
  constexpr static int sub_tree_size = acg::math::pow<dim>(subd);
  
  // Root node has depth 0
  size_t depth_{0};
  
  // Large leafs of this tree contain.
  size_t cache_usage_{0};
  std::array<StorageType, cacheline_size> cached_leafs_;
  std::vector<StorageType> nb_leafs_;
  
  // Range of the tree, assert that leafs inside range.
  BoundingBox range_;
  
  // Transform World Space Coordinate to Grid Space.
  ContinuousDiscreteTransformRegular <Scalar, dim> cd_transform_;
  
  // Bias Transform for lower bound.
  // x' = x - lower
  BiasTransform<Scalar, dim> bias_transform_;
  
  // Discrete to storage
  DiscreteStorageSequentialTransform<3> disc_store_transform_;
  
  // Childs
  std::array<std::unique_ptr<BoundedOctree>, sub_tree_size> child_tree_;
 
 public:
  template<typename F>
  void ForeachTree(F f) const;
  
  template<typename F>
  void Foreach(F f) const;
  
  size_t TreeSize() const noexcept;
  
  size_t Size() const noexcept;
  
  Scalar GetGridSize() const noexcept;
  
  Vec<Scalar, dim> GetLowerBound() const;
  
  BoundingBox GetBoundingBox() const;
  
  explicit BoundedOctree(BoundingBox box, size_t depth = 0);
  
  void Insert(BoundingBox box, Index object_id);
  
  void Insert(StorageType box_with_id);
  
  bool CanHold(BoundingBox box) const noexcept;
  
  bool Intersect(BoundingBox box) const noexcept;
  
  // Query the intersecting bounding box.
  std::vector<Index> Query(BoundingBox box) const noexcept;
  
  void Query(BoundingBox box, std::vector<Index> &result) const noexcept;
  
  void Plot(Field<Scalar, 3> &position,
            types::topology::LineList &lines) const noexcept;
  
  void Clear(bool all_internal = false);
 
 private:
  void InsertThis(StorageType store);
  
  bool CanChildTreeHold(IndexVec<dim> child_tree_index,
                        BoundingBox box) const noexcept;
  
  BoundedOctree *MakeTree(IndexVec<dim> child_tree_index);
};

} // namespace acg::spatial

#include "details/octree-inl.hpp"
