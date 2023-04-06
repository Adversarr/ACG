#pragma once

#include <acore/math/common.hpp>
#include <type_traits>

namespace acg::physics::elastic {

template <typename Scalar, int dim> struct HyperElasticResult {
  Scalar energy_;
  Vec<Scalar, dim * dim> grad_;
};

/**
 * @brief
 *
 * @tparam Scalar
 */
template <typename Scalar, int dim> struct HyperElasticResultWithHessian {
  Scalar energy_;
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
  static_assert(dim == 2 || dim == 3,
                "HyperElasticModel requires dim == 2 or 3.");
};

/**
 * @brief Given Inverse of restpose matrix, compute pfpx.
 * 
 * @tparam Derived 
 * @param rinv 
 * @return auto 
 */
template <typename Derived>
auto compose_pfpx(const Eigen::MatrixBase<Derived> &rinv);
} // namespace acg::physics::elastic

#include "details/common-inl.hpp" // IWYU pragma: export
