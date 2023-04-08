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

template <typename Scalar, int dim = 3, int subd = 2, int max_depth = 4,
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
  ContinuousDiscreteTransformRegular<Scalar, dim> cd_transform_;

  // Bias Transform for lower bound.
  // x' = x - lower
  BiasTransform<Scalar, dim> bias_transform_;

  // Discrete to storage
  DiscreteStorageSequentialTransform<3> disc_store_transform_;

  // Childs
  std::array<std::unique_ptr<BoundedOctree>, sub_tree_size> child_tree_;

public:
  template <typename F> void ForeachTree(F f) const {
    f(*this);
    if (depth_ < max_depth - 1) {
      for (const auto &child : child_tree_) {
        if (child) {
          child->ForeachTree(f);
        }
      }
    }
  }

  template <typename F> void Foreach(F f) const {
    for (const auto &[lbox, lindex] : nb_leafs_) {
      f(lbox, lindex);
    }
    for (size_t i = 0; i < cache_usage_; ++i) {
      f(cached_leafs_.at(i).first, cached_leafs_.at(i).second);
    }
    if (depth_ == max_depth - 1) {
      return;
    }
    for (const auto &child : child_tree_) {
      if (child) {
        child->Foreach(f);
      }
    }
  }

  size_t TreeSize() const noexcept {
    if (depth_ == max_depth - 1) {
      return 1;
    } else {
      size_t c = 1;
      for (const auto &child : child_tree_) {
        if (child) {
          c += child->TreeSize();
        }
      }
      return c;
    }
  }

  size_t Size() const noexcept {
    size_t base = nb_leafs_.size() + cache_usage_;
    if (depth_ == max_depth - 1) {
      return base;
    }
    for (const auto &child : child_tree_) {
      if (child) {
        base += child->Size();
      }
    }
    return base;
  }

  Scalar GetGridSize() const noexcept { return cd_transform_.grid_size_; }

  Vec<Scalar, dim> GetLowerBound() const { return bias_transform_.bias_; }

  BoundingBox GetBoundingBox() const { return range_; }

  explicit BoundedOctree(BoundingBox box, size_t depth = 0)
      : range_(box), depth_(depth), bias_transform_(-box.min()),
        disc_store_transform_(IndexVec<dim>::Constant(subd)) {
    Vec<Scalar, dim> lb = box.min();
    Vec<Scalar, dim> ub = box.max();
    Scalar gs = (ub - lb).x();
    assert(((ub - lb).array() == gs).all() &&
           "Invalid bound (different dim's grid size varies).");
    cd_transform_ = ContinuousDiscreteTransformRegular<Scalar, dim>(
        gs / static_cast<Scalar>(subd));
  }

  void Insert(BoundingBox box, Index object_id) {
    // Check range can contain the box.
    assert(range_.contains(box) && "This tree cannot contain the box.");

    // If this is the deepest node, insert directly.
    if (depth_ == max_depth - 1) {
      InsertThis(std::make_pair(box, object_id));
      return;
    }

    // Get the lower bound of the box, and compute the corresponding index.
    Vec<Scalar, 3> lb = box.min();
    Vec<Scalar, 3> lb_biased = bias_transform_.Forward(lb);
    IndexVec<3> child_index = cd_transform_.Forward(lb_biased);
    if (CanChildTreeHold(child_index, box)) {
      // Box is small enough, the child can hold the box. Make a tree.
      MakeTree(child_index)->Insert(box, object_id);
    } else {
      // Otherwise, insert as this leaf
      InsertThis(std::make_pair(box, object_id));
    }
  }

  void Insert(StorageType box_with_id) {
    Insert(box_with_id.first, box_with_id.second);
  }

  bool CanHold(BoundingBox box) const noexcept {
    return static_cast<bool>(range_.contains(box));
  }

  bool Intersect(BoundingBox box) const noexcept {
    return range_.intersects(box);
  }

  // Query the intersecting bounding box.
  std::vector<Index> Query(BoundingBox box) const noexcept {
    std::vector<Index> result;
    Query(box, result);
    return result;
  }

  void Query(BoundingBox box, std::vector<Index> &result) const noexcept {
    for (const auto &[lbox, lindex] : nb_leafs_) {
      if (box.intersects(lbox)) {
        result.push_back(lindex);
      }
    }
    for (size_t i = 0; i < cache_usage_; ++i) {
      const auto &[b, ind] = cached_leafs_.at(i);
      if (box.intersects(b)) {
        result.push_back(ind);
      }
    }
    if (depth_ == max_depth - 1) {
      return;
    }
    for (const auto &child : child_tree_) {
      if (child && child->Intersect(box)) {
        child->Query(box, result);
      }
    }
  }

  void Plot(Field<Scalar, 3> &position,
            types::topology::LineList &lines) const noexcept {
    Index size = TreeSize();
    position.resize(3, size * 8);
    lines.resize(2, size * 12);
    auto pacc = view(position);
    auto iacc = view(lines);
    Index np = 0, nl = 0;
    ForeachTree([&](const BoundedOctree &tree) -> void {
      auto l = tree.GetBoundingBox().min();
      auto u = tree.GetBoundingBox().max();
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
    });
  }

  void Clear(bool all_internal = false) {
    cache_usage_ = 0;
    nb_leafs_.clear();

    if (depth_ == max_depth - 1) {
      return;
    }
    for (auto &child : child_tree_) {
      if (all_internal) {
        child.reset();
      } else {
        if (child) {
          child->Clear(all_internal);
        }
      }
    }
  }

private:
  void InsertThis(StorageType store) {
    if (cache_usage_ == cached_leafs_.size()) {
      cached_leafs_[cache_usage_] = store;
      cache_usage_++;
    } else {
      nb_leafs_.push_back(store);
    }
  }

  bool CanChildTreeHold(IndexVec<dim> child_tree_index,
                        BoundingBox box) const noexcept {
    Vec<Scalar, dim> lower =
        bias_transform_.Backward(cd_transform_.Backward(child_tree_index));
    Vec<Scalar, dim> upper =
        Vec<Scalar, dim>::Constant(cd_transform_.grid_size_) + lower;
    auto child_range = BoundingBox(lower, upper);
    return child_range.contains(box);
  }

  BoundedOctree *MakeTree(IndexVec<dim> child_tree_index) {
    assert((child_tree_index.array() >= 0).all() &&
           "Internal error: Invalid Child Index");
    size_t index =
        static_cast<size_t>(disc_store_transform_.Forward(child_tree_index));

    assert(index < child_tree_.size() &&
           "Internal error: Invalid Child Index.");
    if (!static_cast<bool>(child_tree_[index])) {
      Vec<Scalar, dim> lower =
          bias_transform_.Backward(cd_transform_.Backward(child_tree_index));
      Vec<Scalar, dim> upper =
          Vec<Scalar, dim>::Constant(cd_transform_.grid_size_) + lower;
      child_tree_[index].reset(
          new BoundedOctree(BoundingBox(lower, upper), depth_ + 1));
    }

    return child_tree_[index].get();
  }
};

} // namespace acg::spatial
