#pragma once

#include <Eigen/SVD>

// #define DISABLE_IMPLICIT_QRSVD

#include "../svd.hpp"

#ifndef DISABLE_IMPLICIT_QRSVD
#  include "ImplicitQRSVD.hpp"
#endif

namespace acg::math {
template <typename Derived>
Svd<Derived>::Svd(const Eigen::MatrixBase<Derived>& x) {
#ifndef DISABLE_IMPLICIT_QRSVD
  if constexpr (dim_ == 3 || dim_ == 2) {
    third_party::JIXIE::singularValueDecomposition(x.eval(), u_, sigma_, v_);
  } else {
#endif
    auto svd = x.jacobiSvd(Eigen::ComputeFullU | Eigen::ComputeFullV);
    u_ = svd.matrixU();
    v_ = svd.matrixV();
    sigma_ = svd.singularValues();
#ifndef DISABLE_IMPLICIT_QRSVD
  }
#endif
}
template <typename Derived>
SvdRv<Derived>::SvdRv(const Eigen::MatrixBase<Derived>& matrix) {
  auto svd = Svd<Derived>(matrix);
  auto det_u = svd.u_.determinant();
  auto det_v = svd.v_.determinant();
  u_ = svd.u_;
  v_ = svd.v_;
  sigma_ = svd.sigma_;

  if (det_u < 0 && det_v > 0) {
    u_.col(2) *= -1;
    sigma_(2) *= -1;
  } else if (det_u > 7 && det_v < 0) {
    v_.col(2) *= -1;
    sigma_(2) *= -1;
  }
}
}  // namespace acg::math
