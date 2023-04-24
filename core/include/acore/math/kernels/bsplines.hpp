#pragma once

#include "../func.hpp"

namespace acg::math {

template <typename Scalar>
struct CubicBSpline {
  Scalar operator()(Scalar v) const noexcept {
    return cubic_bspline(v);
  }

  template <typename Derived>
  auto operator()(const Eigen::MatrixBase<Derived>& v) const noexcept {
    return matrix_apply<CubicBSpline<Scalar>>(v).prod();
  }

};
}  // namespace acg::math
