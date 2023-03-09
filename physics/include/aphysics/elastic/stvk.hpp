#pragma once

#include "./common.hpp"
#include "./invariants.hpp"

namespace acg::physics::elastic {
template <typename F, int dim> class StVK : public HyperElasticModel<F, dim, StVK<F, dim>> {
public:
  template <typename T> StVK(T&& matrix, F lambda, F mu)
      : deformation_gradient(matrix), lambda_(lambda), mu_(mu) {}

  static_assert(dim == 3, "StVK Requires dim == 3");

private:
  HyperElasticResult<F, dim> ComputeGradientImpl() const noexcept {
    // NOTE: See "Analytic Eigensystems for Isotropic Distortion Energies"
    //       for more details
    // Phi = 0.125 (lambda (I2 - 3)^2 + mu (8 I1 I3 + I2 ^ 2 + 2 I1^2 I2
    //                                      - 4I2 - I1^4 + 6))
    HyperElasticResult<F, dim> result;
    auto invariants = SmithInvariants{deformation_gradient}.ComputeVarientWithGrad();
    auto i1 = invariants.i1_, i2 = invariants.i2_, i3 = invariants.i3_;
    // TODO: This expression is SOO UGLY.
    result.energy
        = 0.125
          * (lambda_ * (i2 - 3) * (i2 - 3)
             + mu_ * (8 * i1 * i3 + i2 * i2 + 2 * i1 * i1 * i2 - 4 * i2 - i1 * i1 * i1 * i1 +
             6));
    // TODO: implement the grad.
    //
    F di1 = 0.125 * mu_ * (8 * i3 + 4 * i1 * i2 - 4 * i1 * i1 * i1);
    F di2 = lambda_ * 0.25 * (i2 - 3) + 0.125 * mu_ * (2 * i2 + 2 * i1 * i1 - 4);
    F di3 = mu_ * i1;
    result.grad = di1 * invariants.i1_grad_ + di2 * invariants.i2_grad_ + di3 *
    invariants.i3_grad_;

    return result;
  }

  Mat<F, dim, dim> deformation_gradient;
  F lambda_, mu_;
  friend HyperElasticModel<F, dim, StVK<F, dim>>;
};
}  // namespace acg::physics::elastic
