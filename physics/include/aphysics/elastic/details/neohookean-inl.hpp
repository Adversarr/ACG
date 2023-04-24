#pragma once
#include "../neohookean.hpp"

#include <aphysics/elastic/details/utils-inl.hpp>

namespace acg::physics::elastic {
template <typename Scalar, int dim>
HyperElasticResult<Scalar, dim>
OgdenNeoHookean<Scalar, dim>::ComputeGradient() const noexcept {
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

template <typename Scalar, int dim>
HyperElasticResultWithHessian<Scalar, dim>
OgdenNeoHookean<Scalar, dim>::ComputeHessian() const noexcept {
  HyperElasticResultWithHessian<Scalar, dim> result;
  auto inv = SmithInvariants{deformation_gradient_}.ComputeHessian();
  auto &ig = inv.inv_grad_result_;
  result.energy_ = .5 * mu_ * (ig.i2_ - math::constant<Scalar>(3)) -
                   mu_ * log(ig.i3_) + lambda_ * 0.5 * math::square(ig.i3_ - 1);
  auto di2 = .5 * mu_;
  auto di3 = (lambda_ * (ig.i3_ - math::constant<Scalar>(1)) - mu_ / ig.i3_);
  result.grad = di2 * ig.i2_grad_ + di3 * ig.i3_grad_;
  result.hessian = details::compose_final_hessian<Scalar, dim>(
      0, 0, lambda_ + mu_ / math::square(ig.i3_), 0, di2, di3, inv);
  return result;
}

template <typename Scalar, int dim>
Scalar OgdenNeoHookean<Scalar, dim>::ComputeEnergy() const noexcept {
  auto inv = SmithInvariants{deformation_gradient_}.ComputeValue();
  auto energy = .5 * mu_ * (inv.i2_ - math::constant<Scalar>(3)) -
                mu_ * log(inv.i3_) + lambda_ * 0.5 * math::square(inv.i3_ - 1);
  return energy;
}

template <typename Scalar, int dim>
inline Scalar SNHNeoHookean<Scalar, dim>::ComputeEnergy() const noexcept {
  auto inv = SmithInvariants{deformation_gradient_}.ComputeGrad();
  return .5 * mu_ * (inv.i2_ - 3) - mu_ * (inv.i3_ - 1) +
         lambda_ * 0.5 * math::square(inv.i3_ - 1);
}

template <typename Scalar, int dim>
inline HyperElasticResult<Scalar, dim>
SNHNeoHookean<Scalar, dim>::ComputeGradient() const noexcept {
  HyperElasticResult<Scalar, dim> result;
  auto inv = SmithInvariants{deformation_gradient_}.ComputeGrad();
  result.energy_ = .5 * mu_ * (inv.i2_ - 3) - mu_ * (inv.i3_ - 1) +
                   lambda_ * 0.5 * math::square(inv.i3_ - 1);
  result.grad_ =
      .5 * mu_ * inv.i2_grad_ + (lambda_ * (inv.i3_ - 3) + mu_) * inv.i3_grad_;
  return result;
}

template <typename Scalar, int dim>
inline HyperElasticResultWithHessian<Scalar, dim>
SNHNeoHookean<Scalar, dim>::ComputeHessian() const noexcept {
  HyperElasticResultWithHessian<Scalar, dim> result;
  auto inv = SmithInvariants{deformation_gradient_}.ComputeHessian();
  auto &ig = inv.inv_grad_result_;
  result.energy_ = .5 * mu_ * (ig.i2_ - 3) - mu_ * (ig.i3_ - 1) +
                   lambda_ * 0.5 * math::square(ig.i3_ - 1);
  auto di2 = .5 * mu_;
  auto di3 = (lambda_ * (ig.i3_ - 1) - mu_);
  result.grad = di2 * ig.i2_grad_ + di3 * ig.i3_grad_;
  result.hessian = details::compose_final_hessian<Scalar, dim>(0, 0, lambda_, 0,
                                                               di2, di3, inv);
  return result;
}

} // namespace acg::physics::elastic