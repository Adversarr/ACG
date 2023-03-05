#pragma once
#include <acore/math/decompositions/polar.hpp>
#include <acore/math/traits.hpp>

#include "common.hpp"

namespace acg::physics::elastic {

template <typename F, Index dim = 3> struct InvariantWithGradientResult {
  F i1_, i2_, i3_;
  Vec<F, dim * dim> i1_grad_;
  Vec<F, dim * dim> i2_grad_;
  Vec<F, dim * dim> i3_grad_;
};

template <typename F, Index dim = 3> struct InvariantHessianResult {
  InvariantWithGradientResult<F, dim> inv_grad_result_;
  Mat<F, dim * dim, dim * dim> i1_hessian_;
  Mat<F, dim * dim, dim * dim> i2_hessian_;
  Mat<F, dim * dim, dim * dim> i3_hessian_;
};

template <typename F> struct CauchyGreenInvariants {
  explicit CauchyGreenInvariants(const Ref<Mat3x3<F>>& deformation_gradient) noexcept;

  // I = || F ||_F^2
  F i1_;

  // II = || F' F ||_F ^2
  F i2_;

  // III = det (F' F)
  F i3_;
};

/**
 * @brief Smith Invariants are derived in "Dynamic Deformables..."
 *        (SIGGRAPH 2022 Course.)
 *
 *          F := Deformation Gradient.
 *          F = R * S (by polar decomposition)
 *          F' = Transpose(F)
 *
 *        Invariants are:
 *
 *        1. Tr(S)
 *        2. Tr(F' F)
 *        3. Determinant(F)
 *
 * @tparam Derived
 * @param dg
 * @return
 */
template <typename Derived> struct SmithInvariants {
  using Scalar = typename Trait<Derived>::Scalar;
  static constexpr Index dim_ = Trait<Derived>::cols;
  static_assert(dim_ == Trait<Derived>::rows, "Rows != cols.");
  explicit SmithInvariants(const Eigen::MatrixBase<Derived>& dg) noexcept;

  Mat<Scalar, 3, 3> f_;

  math::PolarDecompositionRv<Derived> polar_result_;

  InvariantWithGradientResult<Scalar, dim_> ComputeVarientWithGrad() const;

  InvariantHessianResult<Scalar, dim_> ComputeVariantWithHessian() const;
};
}  // namespace acg::physics::elastic
#include "details/invarients-inl.hpp"
