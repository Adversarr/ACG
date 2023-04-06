#pragma once
#include <acore/math/common.hpp>

#include <Eigen/Geometry> // IWYU pragma: export

namespace acg::spatial {

using Eigen::AlignedBox;

/****************************************
 * @brief Axis Aligned Bounding Box
 ****************************************/

template <typename F, int dim> struct AABB {
  using bound_type = acg::Vec<F, dim>;
  bound_type lower_bound_;
  bound_type upper_bound_;
  AABB(bound_type lb, bound_type ub) : lower_bound_(lb), upper_bound_(ub) {
    ACG_DEBUG_CHECK((lb.array() <= ub.array()).all(), "LB > UB.");
  }

  AABB(const AABB &) = default;

  inline bool Intersect(const AABB<F, dim> &another) const {
    auto crossing = (upper_bound_ - another.lower_bound_).array() *
                    (another.upper_bound_ - lower_bound_).array();
    return !(crossing.array() < 0).any();
  }

  inline bool Contain(const AABB<F, dim> &another) const {
    return (lower_bound_.array() <= another.lower_bound_.array()).all() &&
           (upper_bound_.array() >= another.upper_bound_.array()).all();
  }

  inline AABB<F, dim> Merge(const AABB<F, dim> &another) const {
    bound_type lb = lower_bound_.array().cwiseMin(another.lower_bound_.array());
    bound_type ub = upper_bound_.array().cwiseMax(another.upper_bound_.array());
    return AABB<F, dim>(lb, ub);
  }
};

} // namespace acg::spatial
