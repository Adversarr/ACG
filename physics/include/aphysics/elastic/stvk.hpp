#pragma once

#include <acore/math/func.hpp>

#include "./common.hpp"
#include "./invariants.hpp"
namespace acg::physics::elastic {
template <typename Scalar, int dim> class StVK : public HyperElasticModel<Scalar, dim, StVK<Scalar, dim>> {
public:
  template <typename T> StVK(T&& matrix, Scalar lambda, Scalar mu)
      : deformation_gradient(matrix), lambda_(lambda), mu_(mu) {}

  static_assert(dim == 3, "StVK Requires dim == 3");

private:
  HyperElasticResult<Scalar, dim> ComputeGradientImpl() const noexcept {
    // NOTE: See "Analytic Eigensystems for Isotropic Distortion Energies"
    //       for more details
    // Phi = 0.125 (lambda (I2 - 3)^2 + mu (8 I1 I3 + I2 ^ 2 + 2 I1^2 I2
    //                                      - 4I2 - I1^4 + 6))
    HyperElasticResult<Scalar, dim> result;
    auto invariants = SmithInvariants{deformation_gradient}.ComputeGrad();
    auto i1 = invariants.i1_, i2 = invariants.i2_, i3 = invariants.i3_;
    using namespace acg::math;
    result.energy
        = 0.125
          * (lambda_ * math::square(i2 - 3)
             + mu_ * (8 * i1 * i3 + square(i2) + 2 * square(i1) * i2 - 4 * i2 - pow<4>(i1) + 6));
    Scalar di1 = 0.125 * mu_ * (8 * i3 + 4 * i1 * i2 - 4 * pow<3>(i1));
    Scalar di2 = lambda_ * 0.25 * (i2 - 3) + 0.125 * mu_ * (2 * i2 + 2 * square(i1) - 4);
    Scalar di3 = mu_ * i1;
    result.grad = di1 * invariants.i1_grad_ + di2 * invariants.i2_grad_ + di3 * invariants.i3_grad_;
    return result;
  }

  Mat<Scalar, dim, dim> deformation_gradient;
  Scalar lambda_, mu_;
  friend HyperElasticModel<Scalar, dim, StVK<Scalar, dim>>;
};
}  // namespace acg::physics::elastic
