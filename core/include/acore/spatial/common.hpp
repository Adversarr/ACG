#pragma once
#include <acore/math/common.hpp>

namespace acg::spatial {

/****************************************
 * @brief Axis Aligned Bounding Box
 ****************************************/

template <typename F, int dim> struct AABB {
  using bound_type = acg::Vec<F, dim>;
  bound_type lower_bound;
  bound_type upper_bound;
  AABB(bound_type lb, bound_type ub) : lower_bound(lb), upper_bound(ub) {}

  AABB(const AABB &) = default;

  inline bool Intersect(const AABB<F, dim> &another) const {
    auto crossing
        = (upper_bound - another.lower_bound).array() * (another.upper_bound - lower_bound).array();
    return !(crossing.array() < 0).any();
  }

  inline bool Contain(const AABB<F, dim> &another) const {
    return (lower_bound.array() <= another.lower_bound.array()).all()
           && (upper_bound.array() >= another.upper_bound.array()).all();
  }

  inline AABB<F, dim> Merge(const AABB<F, dim>& another) const {
    auto lb = lower_bound.array().cwiseMin(another.lower_bound.array());
    auto ub = upper_bound.array().cwiseMax(another.upper_bound.array());
    return AABB<F, dim>(lb, ub);
  }
};
}  // namespace acg::spatial
