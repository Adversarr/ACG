#pragma once

#include <Eigen/SVD>

#include "../svd.hpp"
#include "svd3.hpp"

namespace acg::math {
template <typename Derived> Svd<Derived>::Svd(const Eigen::MatrixBase<Derived>& x) {
  auto svd = x.jacobiSvd(Eigen::ComputeThinU | Eigen::ComputeThinV);
  u_ = svd.matrixU();
  v_ = svd.matrixV();
  sigma_ = svd.singularValues();
}

// template <typename F> SmallSVD<F, 3, 3>::SmallSVD(const Ref<Mat<F, 3, 3>>& x) {
//   details::svd3::Svd3Context<F> ctx;
//   sigma_.setZero();
//   ctx.svd(
//       x(0, 0), x(0, 1), x(0, 2), x(1, 0), x(1, 1), x(1,2), x(2, 0),x(2, 1),x(2, 2),
//       u_(0, 0), u_(0, 1), u_(0, 2), u_(1, 0), u_(1, 1), u_(1,2), u_(2, 0),u_(2, 1),u_(2, 2),
//       sigma_(0, 0), sigma_(0, 1), sigma_(0, 2), sigma_(1, 0), sigma_(1, 1), sigma_(1,2),
//       sigma_(2, 0),sigma_(2, 1),sigma_(2, 2), v_(0, 0), v_(0, 1), v_(0, 2), v_(1, 0), v_(1, 1),
//       v_(1,2), v_(2, 0),v_(2, 1),v_(2, 2)
//       );
// }

template <typename Derived> SvdRv<Derived>::SvdRv(const Eigen::MatrixBase<Derived>& matrix) {
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
