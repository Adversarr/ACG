#pragma once
#include <acore/math/func.hpp>
#include <aphysics/elastic/invariants.hpp>

#include "./common.hpp"
#include "details/utils-inl.hpp"

namespace acg::physics::elastic {
template <typename Scalar, int dim>
class OgdenNeoHookean
    : public HyperElasticModel<Scalar, dim, OgdenNeoHookean<Scalar, dim>> {
public:
  OgdenNeoHookean(Mat<Scalar, dim, dim> dg, Scalar lambda, Scalar mu) noexcept
      : lambda_(lambda), mu_(mu), deformation_gradient_(std::move(dg)) {}

private:
  HyperElasticResult<Scalar, dim> ComputeGradientImpl() const noexcept {
    HyperElasticResult<Scalar, dim> result;
    auto inv = SmithInvariants{deformation_gradient_}.ComputeGrad();

    result.energy_ = .5 * mu_ * (inv.i2_ - math::constant<Scalar>(3)) -
                    mu_ * log(inv.i3_) +
                    lambda_ * 0.5 * math::square(inv.i3_ - 1);
    result.grad_ =
        .5 * mu_ * inv.i2_grad_ +
        (lambda_ * (inv.i3_ - math::constant<Scalar>(1)) - mu_ / inv.i3_) *
            inv.i3_grad_;
    return result;
  }
  Scalar ComputeEnergyImpl() const noexcept {
    auto inv = SmithInvariants{deformation_gradient_}.ComputeValue();
    auto energy = .5 * mu_ * (inv.i2_ - math::constant<Scalar>(3)) -
                  mu_ * log(inv.i3_) +
                  lambda_ * 0.5 * math::square(inv.i3_ - 1);
    return energy;
  }

  HyperElasticResultWithHessian<Scalar, dim>
  ComputeHessianImpl() const noexcept {
    HyperElasticResultWithHessian<Scalar, dim> result;
    auto inv = SmithInvariants{deformation_gradient_}.ComputeHessian();
    auto &ig = inv.inv_grad_result_;
    result.energy_ = .5 * mu_ * (ig.i2_ - math::constant<Scalar>(3)) -
                    mu_ * log(ig.i3_) +
                    lambda_ * 0.5 * math::square(ig.i3_ - 1);
    auto di2 = .5 * mu_;
    auto di3 = (lambda_ * (ig.i3_ - math::constant<Scalar>(1)) - mu_ / ig.i3_);
    result.grad = di2 * ig.i2_grad_ + di3 * ig.i3_grad_;
    result.hessian = details::compose_final_hessian<Scalar, dim>(
        0, 0, lambda_ + mu_ / math::square(ig.i3_), 0, di2, di3, inv);
    return result;
  }
  Scalar lambda_, mu_;
  Mat<Scalar, dim, dim> deformation_gradient_;
  friend HyperElasticModel<Scalar, dim, OgdenNeoHookean<Scalar, dim>>;
};

template <typename Scalar, int dim>
class SNHNeoHookean
    : public HyperElasticModel<Scalar, dim, SNHNeoHookean<Scalar, dim>> {
  // Page 86
  //
  // $$
  //  Phi = mu / 2 (I2 - 3) - mu (I3 - 1) + lambda / 2 (I3 - 1) ^ 2
  // $$
public:
  SNHNeoHookean(Mat<Scalar, dim, dim> dg, Scalar lambda, Scalar mu) noexcept
      : lambda_(lambda), mu_(mu), deformation_gradient_(std::move(dg)) {}

private:
  inline Scalar ComputeEnergyImpl() const noexcept {
    auto inv = SmithInvariants{deformation_gradient_}.ComputeGrad();
    return .5 * mu_ * (inv.i2_ - 3) - mu_ * (inv.i3_ - 1) +
           lambda_ * 0.5 * math::square(inv.i3_ - 1);
  }

  inline HyperElasticResult<Scalar, dim> ComputeGradientImpl() const noexcept {
    HyperElasticResult<Scalar, dim> result;
    auto inv = SmithInvariants{deformation_gradient_}.ComputeGrad();
    result.energy_ = .5 * mu_ * (inv.i2_ - 3) - mu_ * (inv.i3_ - 1) +
                    lambda_ * 0.5 * math::square(inv.i3_ - 1);
    result.grad_ = .5 * mu_ * inv.i2_grad_ +
                  (lambda_ * (inv.i3_ - 3) + mu_) * inv.i3_grad_;
    return result;
  }

  inline HyperElasticResultWithHessian<Scalar, dim>
  ComputeHessianImpl() const noexcept {
    HyperElasticResultWithHessian<Scalar, dim> result;
    auto inv = SmithInvariants{deformation_gradient_}.ComputeHessian();
    auto &ig = inv.inv_grad_result_;
    result.energy_ = .5 * mu_ * (ig.i2_ - 3) - mu_ * (ig.i3_ - 1) +
                    lambda_ * 0.5 * math::square(ig.i3_ - 1);
    auto di2 = .5 * mu_;
    auto di3 = (lambda_ * (ig.i3_ - 1) - mu_);
    result.grad = di2 * ig.i2_grad_ + di3 * ig.i3_grad_;
    result.hessian = details::compose_final_hessian<Scalar, dim>(
        0, 0, lambda_, 0, di2, di3, inv);
    return result;
  }
  Scalar lambda_, mu_;
  Mat<Scalar, dim, dim> deformation_gradient_;
  friend HyperElasticModel<Scalar, dim, SNHNeoHookean<Scalar, dim>>;
};

} // namespace acg::physics::elastic
