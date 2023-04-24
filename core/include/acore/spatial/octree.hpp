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
#include <taskflow/algorithm/reduce.hpp>
#include <taskflow/algorithm/sort.hpp>

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

  ParallelMultiOctree(Scalar grid_size, size_t threads):
    executor_(threads),
    ParallelMultiOctree(grid_size) {}

  explicit ParallelMultiOctree(Scalar grid_size)
      : grid_size_(grid_size), cd_transf_(grid_size) {}

  void SetFromPairs(std::vector<StorageType> &group) {
    octrees_.clear();
    tf::Taskflow flow;
    StorageType largest_box;
    largest_box.first.setEmpty();
    flow.reduce(group.begin(), group.end(), largest_box,
                [](const StorageType &lhs, const StorageType &rhs) {
                  StorageType merged;
                  merged.first = lhs.first.merged(rhs.first);
                  return merged;
                });
    // Get the result:
    executor_.run(flow).wait();

    // Create transform.
    IndexVec<dim> lbound = (largest_box.first.min() / grid_size_)
                               .array()
                               .floor()
                               .template cast<Index>();
    IndexVec<dim> ubound = (largest_box.first.max() / grid_size_)
                               .array()
                               .ceil()
                               .template cast<Index>();
    ds_transf_ = DiscreteStorageSequentialTransform<3>(ubound - lbound);
    l_transf_.bias_ = -lbound;
    auto combined =
        combine_transform(combine_transform(cd_transf_, l_transf_), ds_transf_);
    for (auto [i] : NdRange<1>((ubound - lbound).prod())) {
      // Make tree.
      Vec3<Scalar> tree_lower = combined.Backward(i);
      Vec3<Scalar> tree_upper = tree_lower + Vec3<Scalar>::Constant(grid_size_);
      octrees_.push_back(Tree(BoundingBox(tree_lower, tree_upper)));
    }

    using InsertType = std::pair<StorageType, Index>;
    std::vector<InsertType> tree_node;
    tree_node.reserve(group.size() / 1.5);
    flow.clear();
    flow.emplace([&]() {
      for (const auto &p : group) {
        auto l = combined(p.first.min());
        auto u = combined(p.first.max());
        if (l == u) {
          tree_node.push_back(std::make_pair(p, l));
        }
      }
    });
    flow.emplace([&]() {
      for (const auto &p : group) {
        auto l = combined(p.first.min());
        auto u = combined(p.first.max());
        if (l != u) {
          large_leafs_.push_back(p);
        }
      }
    });
    executor_.run(flow).wait();
    flow.clear();
    // Sort Inputs
    tf::Task sort_task =
        flow.sort(tree_node.begin(), tree_node.end(),
                  [&](const InsertType &lhs, const InsertType &rhs) {
                    return lhs.second < rhs.second;
                  });
    // Find entries.
    std::vector<Index> entries((ubound - lbound).prod(), -1);
    entries[tree_node.front().second] = 0;
    tf::Task entry_task = flow.for_each_index(
        0, static_cast<int>(tree_node.size() - 1), 1, [&](int i) {
          if (tree_node[i + 1].second != tree_node[i].second) {
            entries[tree_node[i + 1].second] = i + 1;
          }
        });

    // Insert all the nodes.
    tf::Task insert_task =
        flow.for_each(entries.begin(), entries.end(), [&](Index beg) {
          if (beg < 0) {
            return;
          }
          auto entry = tree_node[beg].second;
          for (auto i = beg; i < tree_node.size(); ++i) {
            if (tree_node[i].second != entry) {
              break;
            }
            octrees_[entry].Insert(tree_node[i].first);
          }
        });

    sort_task.precede(entry_task);
    entry_task.precede(insert_task);
    executor_.run(flow).wait();
  }

  std::vector<Index> Query(BoundingBox query) const noexcept {
    std::vector<Index> result;
    for (const auto &box : large_leafs_) {
      if (query.intersects(box.first)) {
        result.push_back(box.second);
      }
    }

    for (const auto &tree : octrees_) {
      tree.Query(query, result);
    }

    return result;
  }

private:
  // Size for each tree.
  Scalar grid_size_;
  // Transforms
  ContinuousDiscreteTransformRegular<Scalar, dim> cd_transf_;
  BiasTransform<Index, dim> l_transf_;
  DiscreteStorageSequentialTransform<dim> ds_transf_;
  // Containers
  std::vector<Tree> octrees_;
  std::vector<StorageType> large_leafs_;
  // Par Runner
  tf::Executor executor_;
};

} // namespace acg::spatial

#include "details/octree-inl.hpp"
