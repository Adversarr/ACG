#pragma once

#include "./common.hpp"
#include "./invariants.hpp"

namespace acg::physics::elastic {
template <typename F, int dim> class StVK : public HyperElasticModel<F, dim, StVK<F, dim>> {
public:
  template <typename T> StVK(T&& matrix, F lambda, F mu)
      : deformation_gradient(matrix), lambda_(lambda), mu_(mu) {}

private:
  HyperElasticResult<F, dim> ComputeGradientImpl() const noexcept {
    // Page 72
    auto invariants = SmithInvariants{deformation_gradient}.ComputeVarientWithGrad();
    // BUG: This is Neo-Hookean.
    //
    // // Phi = 1/2 mu (I2 - 3) - mu Log[I3] + 1/2 lambda (Log[I3])^2
    //
    // F logi3 = log(invariants.i3_);
    // F i3_scale = (logi3 * lambda_ - mu_) / invariants.i3_;
    // F i2_scale = mu_ * static_cast<F>(.5);
    // F energy = .5 * mu_ * (invariants.i2_ - 3) - mu_ * logi3 + .5 * lambda_ * logi3 * logi3;
    // HyperElasticResult<F, dim> result;
    // result.energy = energy;
    // result.grad = i2_scale * invariants.i2_grad_ + i3_scale * invariants.i3_grad_;

    // NOTE: See "Analytic Eigensystems for Isotropic Distortion Energies"
    //       for more details
    // Phi = 0.125 (lambda (I2 - 3)^2 + mu (8 I1 I3 + I2 ^ 2 + 2 I1^2 I2
    //                                      - 4I2 - I1^4 + 6))
    HyperElasticResult<F, dim> result;
    auto i1 = invariants.i1_, i2 = invariants.i2_, i3 = invariants.i3_;
    // TODO: This expression is SOO UGLY.
    result.energy
        = 0.125
          * (lambda_ * (i2 - 3) * (i2 - 3)
             + mu_ * (8 * i1 * i3 + i2 * i2 + 2 * i1 * i1 * i2 - 4 * i2 - i1 * i1 * i1 * i1 + 6));
    // TODO: implement the grad.
    return result;
  }
  HyperElasticResultWithHessian<F, dim> ComputeHessianImpl() const noexcept;
  Mat<F, dim, dim> deformation_gradient;
  F lambda_, mu_;

  friend HyperElasticModel<F, dim, StVK<F, dim>>;
};
}  // namespace acg::physics::elastic
