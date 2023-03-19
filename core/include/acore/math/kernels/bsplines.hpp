#pragma once

#include "../func.hpp"

namespace acg::math {

template <typename Scalar>
struct CubicBSpline {
  Scalar operator()(Scalar v) const noexcept {
    return cubic_bspline(v);
  }

  template <int dim>
  auto operator()(const Vec<Scalar, dim>& v) {
    return v.unaryExpr(std::function(cubic_bspline<Scalar>)).prod();
  }
};
}  // namespace acg::math
