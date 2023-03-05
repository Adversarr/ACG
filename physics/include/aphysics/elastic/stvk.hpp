#pragma once

#include <iostream>

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
    // Page 72
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

    // TODO: based on cauchy invarient:
    // auto invariants = CauchyGreenInvariants{deformation_gradient}.ComputeVarientWithGrad();
    // std::cout << invariants.i1_ << std::endl;
    // std::cout << invariants.i2_ << std::endl;
    // std::cout << invariants.i3_ << std::endl;
    // // Phi = .25 * II_C - .5 I_C + 3
    // result.energy
    //     = mu_ * (.25 * invariants.i2_ - .5 * invariants.i1_ + 0.75)
    //       + 0.125 * lambda_ * (invariants.i1_ * invariants.i1_ - 6 * invariants.i1_ + 9);
    // std::cout << result.energy << std::endl;
    // result.grad = mu_ * (.25 * invariants.i2_grad_ - .5 * invariants.i1_grad_)
    //               + lambda_ * 0.25 * (invariants.i1_ - 3) * invariants.i1_grad_;
    return result;
  }

  Mat<F, dim, dim> deformation_gradient;
  F lambda_, mu_;
  friend HyperElasticModel<F, dim, StVK<F, dim>>;
};
}  // namespace acg::physics::elastic
