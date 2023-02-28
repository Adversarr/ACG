#pragma once
#include <acore/math/common.hpp>

namespace acg::spatial {

/****************************************
 * Axis Aligned Bounding Box
 ****************************************/
template <typename T = void, typename F = acg::Float, int dim = 3> struct AABB;
template <typename F, int dim> struct AABB<void, F, dim> {
  using bound_type = acg::Vec<F, dim>;
  bound_type lower_bound;
  bound_type upper_bound;
  AABB(bound_type lb, bound_type ub) : lower_bound(lb), upper_bound(ub) {}

  AABB(const AABB&) = default;

  AABB(AABB&&) = default;
};

template <typename T, typename F, int dim> struct AABB : AABB<void, F, dim> {
  using bound_type = acg::Vec<F, dim>;
  T data;
  AABB(bound_type lb, bound_type ub, const T& d) : AABB<void, F, dim>(lb, ub), data(d) {}
  AABB(const AABB&) = default;
  AABB(AABB&&) = default;
};

}  // namespace acg::spatial
