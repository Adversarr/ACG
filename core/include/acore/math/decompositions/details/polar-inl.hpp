#pragma once

#include <acore/math/decompositions/polar.hpp>
namespace acg::math {
template <typename Derived>
PolarDecomposition<Derived>::PolarDecomposition(const Eigen::MatrixBase<Derived>& matrix)
    : svd_(matrix) {
  rot_ = svd_.u_ * svd_.v_.transpose();
  symm_ = svd_.v_ * svd_.sigma_.asDiagonal() * svd_.v_.transpose();
}
template <typename Derived>
PolarDecompositionRv<Derived>::PolarDecompositionRv(const Eigen::MatrixBase<Derived>& matrix)
    : svd_(matrix) {
  rot_ = svd_.u_ * svd_.v_.transpose();
  symm_ = svd_.v_ * svd_.sigma_.asDiagonal() * svd_.v_.transpose();
}
}  // namespace acg::math
