#pragma once
#include <acore/math/func.hpp>
#include <aphysics/elastic/invariants.hpp>

#include "./common.hpp"

namespace acg::physics::elastic {

template <typename Scalar, int dim> class OgdenNeoHookean {
public:
  OgdenNeoHookean(Mat<Scalar, dim, dim> dg, Scalar lambda, Scalar mu) noexcept
      : lambda_(lambda), mu_(mu), deformation_gradient_(std::move(dg)) {}

  HyperElasticResult<Scalar, dim> ComputeGradient() const noexcept;

  Scalar ComputeEnergy() const noexcept;

  HyperElasticResultWithHessian<Scalar, dim> ComputeHessian() const noexcept;

private:
  Scalar lambda_, mu_;
  Mat<Scalar, dim, dim> deformation_gradient_;
};

template <typename Scalar, int dim> class SNHNeoHookean {
  // Page 86
  //
  // $$
  //  Phi = mu / 2 (I2 - 3) - mu (I3 - 1) + lambda / 2 (I3 - 1) ^ 2
  // $$
public:
  SNHNeoHookean(Mat<Scalar, dim, dim> dg, Scalar lambda, Scalar mu) noexcept
      : lambda_(lambda), mu_(mu), deformation_gradient_(std::move(dg)) {}

  Scalar ComputeEnergy() const noexcept;

  HyperElasticResult<Scalar, dim> ComputeGradient() const noexcept;

  HyperElasticResultWithHessian<Scalar, dim> ComputeHessian() const noexcept;

private:
  Scalar lambda_, mu_;
  Mat<Scalar, dim, dim> deformation_gradient_;
};

} // namespace acg::physics::elastic

#ifdef ACG_INCLUDE_IMPL
#include "details/neohookean-inl.hpp"
#endif