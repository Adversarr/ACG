#pragma once

#include <acore/math/common.hpp>
#include <type_traits>

namespace acg::physics::elastic {

template <typename Scalar, int dim> struct HyperElasticResult {
  Scalar energy;
  Vec<Scalar, dim * dim> grad;
};

/**
 * @brief
 *
 * @tparam Scalar
 */
template <typename Scalar, int dim> struct HyperElasticResultWithHessian {
  Scalar energy;
  Vec<Scalar, dim * dim> grad;
  Mat<Scalar, dim * dim, dim * dim> hessian;
};

/**
 * @brief For any `Elastic Model`, it can computes the value of energy
 *
 * @tparam F Float point type.
 * @tparam Derived Implements the model.
 */
template <typename F, int dim, typename Derived> class HyperElasticModel {
  static_assert(dim == 2 || dim == 3, "HyperElasticModel requires dim == 2 or 3.");

public:
  struct WithHessianFlag {};
  struct WithoutHessianFlag {};
  HyperElasticResult<F, dim> ComputeGradient() const noexcept {
    return static_cast<const Derived*>(this)->ComputeGradientImpl();
  }

  HyperElasticResultWithHessian<F, dim> ComputeHessian() const noexcept {
    return static_cast<const Derived*>(this)->ComputeHessianImpl();
  }

private:
  HyperElasticResult<F, dim> ComputeGradientImpl() const noexcept {
    static_assert(!std::is_same_v<Derived, Derived>, "Call to unimplemented Gradient Function.");
  }

  HyperElasticResultWithHessian<F, dim> ComputeHessianImpl() const noexcept {
    static_assert(!std::is_same_v<Derived, Derived>, "Call to unimplemented Hessian Function.");
  }
};

template <typename Derived>
auto compose_pfpx(const Eigen::MatrixBase<Derived> & rinv);
}  // namespace acg::physics::elastic
#include "details/common-inl.hpp"
