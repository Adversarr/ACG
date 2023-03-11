#pragma once
#include <Eigen/Geometry>
#include <acore/math/constants.hpp>

#include "../invariants.hpp"
namespace acg::physics::elastic {

namespace details {
template <typename T> inline decltype(auto) make_vector_hat3d(T&& vec) {
  using Scalar = typename Trait<std::decay_t<T>>::Scalar;
  return Mat<Scalar, 3, 3>{{0, -vec.z(), vec.y()}, {vec.z(), 0, -vec.x()}, {-vec.y(), vec.x(), 0}};
}
}  // namespace details

template <typename Derived>
CauchyGreenInvariants<Derived>::CauchyGreenInvariants(const Eigen::MatrixBase<Derived>& dg) noexcept
    : f_(dg) {}

template <typename Derived>
InvariantGradientResult<typename CauchyGreenInvariants<Derived>::Scalar,
                            CauchyGreenInvariants<Derived>::dim_>
CauchyGreenInvariants<Derived>::ComputeGrad() const {
  InvariantGradientResult<Scalar, dim_> result;
  result.i1_ = f_.array().square().sum();
  result.i2_ = (f_ * f_.transpose()).array().square().sum();
  auto detf = f_.determinant();
  result.i3_ = detf * detf;
  result.i1_grad_ = 2 * f_.reshaped();
  result.i2_grad_ = 2 * (f_ * (f_.transpose() * f_ - Mat<Scalar, dim_, dim_>::Identity())).reshaped();
  // TODO: i3 grad.
  return result;
};

template <typename Derived>
SmithInvariants<Derived>::SmithInvariants(const Eigen::MatrixBase<Derived>& dg) noexcept
    : f_(dg), polar_result_(dg) {}

template <typename Derived> InvariantGradientResult<typename SmithInvariants<Derived>::Scalar,
                                                        SmithInvariants<Derived>::dim_>
SmithInvariants<Derived>::ComputeGrad() const {
  InvariantGradientResult<Scalar> result;
  result.i1_ = polar_result_.symm_.trace();
  result.i2_ = f_.array().square().sum();
  // result.i2_ = (f_.transpose() * f_).trace();
  result.i3_ = f_.determinant();

  result.i1_grad_ = polar_result_.rot_.reshaped();
  result.i2_grad_ = f_.reshaped() * 2;
  if constexpr (dim_ == 3) {
    auto f0 = f_.col(0);
    auto f1 = f_.col(1);
    auto f2 = f_.col(2);
    auto f0xf1 = f0.cross(f1);
    auto f1xf2 = f1.cross(f2);
    auto f2xf0 = f2.cross(f0);
    result.i3_grad_.block(0, 0, 3, 1) = f1xf2;
    result.i3_grad_.block(3, 0, 3, 1) = f2xf0;
    result.i3_grad_.block(6, 0, 3, 1) = f0xf1;
  } else /* dim == 2 */ {
    result.i3_grad_ = Vec4<Scalar>{f_(1, 1), -f_(1, 0), -f_(0, 1), f_(0, 0)};
  }
  return result;
}

template <typename Derived>
InvariantHessianResult<typename SmithInvariants<Derived>::Scalar, SmithInvariants<Derived>::dim_>
SmithInvariants<Derived>::ComputeHessian() const {
  InvariantHessianResult<Scalar, dim_> result;
  // II: Same for 2, 3 d.
  result.inv_grad_result_ = ComputeGrad();
  result.i2_hessian_ = Mat<Scalar, dim_ * dim_, dim_ * dim_>::Identity() * 2;

  if constexpr (dim_ == 2) {
    // Invariant 1.
    // H[I] = DR / DF, See Page 69 for more details.
    const auto& svd = polar_result_.svd_;
    const Mat2x2<Scalar> inner{{0, -1}, {1, 0}};
    auto q0 = (constants::rsqrt2<Scalar> * svd.u_ * inner * svd.v_.transpose()).eval();
    // NOTE: Assert that q0 is column majored.
    auto vec_q0 = q0.reshaped();
    auto lambda0 = static_cast<Scalar>(2) / (svd.sigma_.array().sum());
    result.i1_hessian_ = lambda0 * vec_q0 * vec_q0.transpose();

    // Invariant 2
    result.i3_hessian_.setZero();
    result.i3_hessian_(0, 3) = 1;
    result.i3_hessian_(1, 2) = -1;
    result.i3_hessian_(2, 1) = -1;
    result.i3_hessian_(3, 0) = 1;

  } else /* dim == 3*/ {
    const Mat3x3<Scalar> t0{{0, -1, 0}, {1, 0, 0}, {0, 0, 0}};
    const Mat3x3<Scalar> t1{{0, 0, 0}, {0, 0, 1}, {0, -1, 0}};
    const Mat3x3<Scalar> t2{{0, 0, 1}, {0, 0, 0}, {-1, 0, 0}};

    const auto& svd = polar_result_.svd_;
    auto t0_vec = (constants::rsqrt2<Scalar> * svd.u_ * t0 * svd.v_.transpose()).reshaped();
    auto t1_vec = (constants::rsqrt2<Scalar> * svd.u_ * t1 * svd.v_.transpose()).reshaped();
    auto t2_vec = (constants::rsqrt2<Scalar> * svd.u_ * t2 * svd.v_.transpose()).reshaped();

    result.i1_hessian_.setZero();
    auto s0 = svd.sigma_.x(), s1 = svd.sigma_.y(), s2 = svd.sigma_.z();
    result.i1_hessian_ += (2 / (s0 + s1)) * (t0_vec * t0_vec.transpose());
    result.i1_hessian_ += (2 / (s1 + s2)) * (t1_vec * t1_vec.transpose());
    result.i1_hessian_ += (2 / (s0 + s2)) * (t2_vec * t2_vec.transpose());

    auto f0_hat = details::make_vector_hat3d(f_.col(0));
    auto f1_hat = details::make_vector_hat3d(f_.col(1));
    auto f2_hat = details::make_vector_hat3d(f_.col(2));

    (result.i3_hessian_.template block<3, 3>(0, 0)).setZero();
    (result.i3_hessian_.template block<3, 3>(3, 3)).setZero();
    (result.i3_hessian_.template block<3, 3>(6, 6)).setZero();
    (result.i3_hessian_.template block<3, 3>(0, 3)) = -f2_hat;
    (result.i3_hessian_.template block<3, 3>(0, 6)) = f1_hat;
    (result.i3_hessian_.template block<3, 3>(3, 0)) = f2_hat;
    (result.i3_hessian_.template block<3, 3>(3, 6)) = -f0_hat;
    (result.i3_hessian_.template block<3, 3>(6, 0)) = -f1_hat;
    (result.i3_hessian_.template block<3, 3>(6, 3)) = f0_hat;
  }

  return result;
}
}  // namespace acg::physics::elastic
