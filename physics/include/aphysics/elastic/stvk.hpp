#pragma once

#include <acore/math/func.hpp>

#include "./common.hpp"

namespace acg::physics::elastic {
template <typename Scalar, int dim> class StVK {
public:
  template <typename T> StVK(T&& matrix, Scalar lambda, Scalar mu)
      : deformation_gradient_(matrix), lambda_(lambda), mu_(mu) {}

  static_assert(dim == 3, "StVK Requires dim == 3");
  
  HyperElasticResult<Scalar, dim> ComputeGradient() const noexcept;

  Mat<Scalar, dim, dim> deformation_gradient_;
  Scalar lambda_, mu_;
};
}  // namespace acg::physics::elastic

#ifdef ACG_INCLUDE_IMPL
#include "details/stvk-inl.hpp"
#endif