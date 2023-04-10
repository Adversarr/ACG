#pragma once
#include <Eigen/Geometry>
#include <acore/math/coordinate/common.hpp>
#include <acore/math/coordinate/continuous_discrete.hpp>
#include <acore/math/func.hpp>
#include <acore/math/indexer.hpp>
#include <acore/math/view.hpp>
#include <acore/spatial/common.hpp>
#include <memory>

#include <acore/parallel/common.hpp>

namespace acg::spatial {

template <typename Scalar, int dim = 3, int subd = 2, int max_depth = 4,
          size_t cacheline_size = 16>
class BoundedOctree {
public:
  using BoundingBox = AlignedBox<Scalar, dim>;
  using StorageType = std::pair<BoundingBox, Index>;
  constexpr static int sub_tree_size = acg::math::pow<dim>(subd);
  template <typename F> void ForeachTree(F f) const;

  template <typename F> void Foreach(F f) const;

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

  // Root node has depth 0
  size_t depth_{0};

  // Large leafs of this tree contain.
  size_t cache_usage_{0};
  std::array<StorageType, cacheline_size> cached_leafs_;
  std::vector<StorageType> nb_leafs_;

  // Range of the tree, assert that leafs inside range.
  BoundingBox range_;

  // Transform World Space Coordinate to Grid Space.
  ContinuousDiscreteTransformRegular<Scalar, dim> cd_transform_;

  // Bias Transform for lower bound.
  // x' = x - lower
  BiasTransform<Scalar, dim> bias_transform_;

  // Discrete to storage
  DiscreteStorageSequentialTransform<3> disc_store_transform_;

  // Childs
  std::array<std::unique_ptr<BoundedOctree>, sub_tree_size> child_tree_;
};

template <typename Tree> class ParallelMultiOctree {
public:
  using BoundingBox = typename Tree::BoundingBox;
  using StorageType = typename Tree::StorageType;
  using Scalar = typename BoundingBox::Scalar;
  constexpr static int dim = BoundingBox::VectorType::RowsAtCompileTime;

  explicit ParallelMultiOctree(Scalar grid_size)
      : grid_size_(grid_size), cd_transf_(grid_size) {}

  void Insert(std::vector<StorageType> &group) {
    // Sort Inputs
    tf::Taskflow flow;
    auto sort_task = flow.sort(
        group.begin(), group.end(),
        [this](const StorageType &lhs, const StorageType &rhs) {
          return cd_transf_(lhs.first.min()) < cd_transf_(rhs.first.min());
        });
    // Find each Box's entry.
    std::vector<Index> entries(group.size());
    auto entry_task =
        flow.for_each_index(0, static_cast<int>(group.size()), 1, [&](int i) {
          entries[i] = cd_transf_(group[i].first.min());
        });

    // Predict num of unique entries
    std::vector<std::pair<size_t, size_t>> u_entries(
        std::thread::hardware_concurrency());
    auto unique_task = flow.emplace([&]() {
      for (size_t i = 1; i < entries.size(); ++i) {
        if (entries[i] < entries[i - 1]) {
          u_entries.push_back({entries[i - 1], i});
        }
      }
      if (entries.back() != entries[entries.size() - 2]) {
        u_entries.push_back({entries.back(), entries.size() - 1});
      }

      for (auto u: u_entries) {
        if (octrees_.find(u.first) == octrees_.end()) {
          octrees_.insert(u.first, Tree()); // TODO:
        }
      }

      // Prepare the entry trees.
    });

    // Foreach Unique entry. Insert into tree.
    auto insert_task = flow.for_each_index(
        0, static_cast<int>(u_entries.size()) + 1, 1, [&](int i) {
          if (i ==static_cast<int>(u_entries.size())) {
            // Is root
            for (const auto& item: group) {
              if (cd_transf_(item.first.min()) != cd_transf_(item.first.max())) {
                large_leafs_.push_back(item);
              }
            }
          } else {
            auto& child = octrees_[u_entries[i].first];
            for (size_t id = u_entries[i].second; id < entries.size(); ++id) {
              
            }
          }
        });
    sort_task.precede(entry_task);
    executor_.run(flow).wait();
  }

private:
  Scalar grid_size_;
  ContinuousDiscreteTransformRegular<Scalar, dim> cd_transf_;

  Vec<Scalar, dim> single_tree_size_;
  std::map<Index, Tree> octrees_;
  std::vector<StorageType> large_leafs_;
  tf::Executor executor_;
};

} // namespace acg::spatial

#include "details/octree-inl.hpp"
