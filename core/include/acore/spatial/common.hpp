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

  AABB(const AABB &) = default;

  AABB(AABB &&) = default;

  template <typename Tp> inline bool Intersect(const AABB<Tp, F, dim> &another) const {
    // HACK: Unroll the loop
    for (Index i = 0; i < dim; ++i) {
      if ((upper_bound(i) - another.lower_bound(i)) * (another.upper_bound(i) - lower_bound(i))
          < 0) {
        return false;
      }
    }

    return true;
  }

  template <typename Tp> inline bool Contain(const AABB<Tp, F, dim> &another) const {
    return (lower_bound.array() <= another.lower_bound.array()).all()
           && (upper_bound.array() >= another.upper_bound.array()).all();
  }
};

template <typename T, typename F, int dim> struct AABB : public AABB<void, F, dim> {
  using bound_type = acg::Vec<F, dim>;
  T data;
  AABB(bound_type lb, bound_type ub, const T &d) : AABB<void, F, dim>(lb, ub), data(d) {}
  AABB(const AABB &) = default;
  AABB(AABB &&) = default;
};

}  // namespace acg::spatial
