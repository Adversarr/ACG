#pragma once
#include <acore/math/func.hpp>
#include <aphysics/elastic/invariants.hpp>

#include "./common.hpp"
#include "details/utils-inl.hpp"

namespace acg::physics::elastic {
template <typename F, int dim> class OgdenNeoHookean
    : public HyperElasticModel<F, dim, OgdenNeoHookean<F, dim>> {
public:
  template <typename T> OgdenNeoHookean(T&& dg, F lambda, F mu) noexcept
      : lambda_(lambda), mu_(mu), deformation_gradient_(dg) {}

private:
  inline HyperElasticResult<F, dim> ComputeGradientImpl() const noexcept {
    HyperElasticResult<F, dim> result;
    auto inv = SmithInvariants{deformation_gradient_}.ComputeGrad();

    result.energy = .5 * mu_ * (inv.i2_ - math::constant<F>(3)) - mu_ * log(inv.i3_)
                    + lambda_ * 0.5 * math::square(inv.i3_ - 1);
    result.grad = .5 * mu_ * inv.i2_grad_
                  + (lambda_ * (inv.i3_ - math::constant<F>(1)) - mu_ / inv.i3_) * inv.i3_grad_;
    return result;
  }

  inline HyperElasticResultWithHessian<F, dim> ComputeHessianImpl() const noexcept {
    HyperElasticResultWithHessian<F, dim> result;
    auto inv = SmithInvariants{deformation_gradient_}.ComputeHessian();
    auto& ig = inv.inv_grad_result_;
    result.energy = .5 * mu_ * (ig.i2_ - math::constant<F>(3)) - mu_ * log(ig.i3_)
                    + lambda_ * 0.5 * math::square(ig.i3_ - 1);
    auto di2 = .5 * mu_;
    auto di3 = (lambda_ * (ig.i3_ - math::constant<F>(1)) - mu_ / ig.i3_);
    result.grad = di2 * ig.i2_grad_ + di3 * ig.i3_grad_;
    result.hessian = details::compose_final_hessian<F, dim>(
        0, 0, lambda_ + mu_ / math::square(ig.i3_), 0, di2, di3, inv);
    return result;
  }
  F lambda_, mu_;
  Mat<F, dim, dim> deformation_gradient_;
  friend HyperElasticModel<F, dim, OgdenNeoHookean<F, dim>>;
};

template <typename F, int dim> class SNHNeoHookean : public HyperElasticModel<F, dim, SNHNeoHookean<F, dim>>{
  // Page 86
  //
  // $$
  //  Phi = mu / 2 (I2 - 3) - mu (I3 - 1) + lambda / 2 (I3 - 1) ^ 2
  // $$
public:
  template <typename T> SNHNeoHookean(T&& dg, F lambda, F mu) noexcept
      : lambda_(lambda), mu_(mu), deformation_gradient_(dg) {}

private:
  inline HyperElasticResult<F, dim> ComputeGradientImpl() const noexcept {
    HyperElasticResult<F, dim> result;
    auto inv = SmithInvariants{deformation_gradient_}.ComputeGrad();
    result.energy = .5 * mu_ * (inv.i2_ - 3) - mu_ * (inv.i3_ - 1)
                    + lambda_ * 0.5 * math::square(inv.i3_ - 1);
    result.grad
        = .5 * mu_ * inv.i2_grad_ + (lambda_ * (inv.i3_ - 3) + mu_) * inv.i3_grad_;
    return result;
  }

  inline HyperElasticResultWithHessian<F, dim> ComputeHessianImpl() const noexcept {
    HyperElasticResultWithHessian<F, dim> result;
    auto inv = SmithInvariants{deformation_gradient_}.ComputeHessian();
    auto& ig = inv.inv_grad_result_;
    result.energy
        = .5 * mu_ * (ig.i2_ - 3) - mu_ * (ig.i3_ - 1) + lambda_ * 0.5 * math::square(ig.i3_ - 1);
    auto di2 = .5 * mu_;
    auto di3 = (lambda_ * (ig.i3_ - 1) - mu_);
    result.grad = di2 * ig.i2_grad_ + di3 * ig.i3_grad_;
    result.hessian = details::compose_final_hessian<F, dim>(0, 0, lambda_, 0, di2, di3, inv);
    return result;
  }
  F lambda_, mu_;
  Mat<F, dim, dim> deformation_gradient_;
  friend HyperElasticModel<F, dim, SNHNeoHookean<F, dim>>;
};

}  // namespace acg::physics::elastic
