#pragma once
#include <acore/math/decompositions/polar.hpp>
#include <acore/math/traits.hpp>

#include "common.hpp"

namespace acg::physics::elastic {

template<typename Scalar, int dim = 3> struct InvariantResult {
  Scalar i1_, i2_, i3_;
};

template <typename F, Index dim = 3> struct InvariantGradientResult {
  F i1_, i2_, i3_;
  Vec<F, dim * dim> i1_grad_;
  Vec<F, dim * dim> i2_grad_;
  Vec<F, dim * dim> i3_grad_;
};

template <typename F, Index dim = 3> struct InvariantHessianResult {
  InvariantGradientResult<F, dim> inv_grad_result_;
  Mat<F, dim * dim, dim * dim> i1_hessian_;
  Mat<F, dim * dim, dim * dim> i2_hessian_;
  Mat<F, dim * dim, dim * dim> i3_hessian_;
};

template <typename Scalar, int dim> struct CauchyGreenInvariants {
  explicit CauchyGreenInvariants(Mat<Scalar, dim, dim> dg) noexcept;

  InvariantResult<Scalar, dim> ComputeValue();

  InvariantGradientResult<Scalar, dim> ComputeGrad() const;

  InvariantHessianResult<Scalar, dim> ComputeHessian() const;
  Mat<Scalar, dim, dim> f_;

  /**
   * 1. F's Frob Norm 2
   * 2. F^T F 's Frob Norm 2
   * 3. det (F^T F).
   */
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
 *        Relation-ship is
 *          C1 = S2.
 *          C3^2 = S3 (in most cases C3 > 0, for no inversion is happening)
 *          C2 = .5 (S2^2 - S1^4) + S1^2 S2 + 4 S1 S3
 *
 * @tparam Derived
 * @param dg
 * @return
 */
template <typename Scalar, int dim> struct SmithInvariants {
  explicit SmithInvariants(Mat<Scalar, dim, dim> dg) noexcept;

  Mat<Scalar, 3, 3> f_;

  math::PolarDecompositionRv<Mat<Scalar, dim, dim>> polar_result_;

  InvariantResult<Scalar, dim> ComputeValue() const;
  InvariantGradientResult<Scalar, dim> ComputeGrad() const;
  InvariantHessianResult<Scalar, dim> ComputeHessian() const;
};
}  // namespace acg::physics::elastic
#include "details/invarients-inl.hpp"
