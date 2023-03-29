#pragma once
#include <acore/geometry/common.hpp>
#include <acore/math/common.hpp>

namespace acg::physics {

/**
 * @brief Lagrange fluid representation, discrete fluid as particles.
 *
 * @tparam Scalar
 */
template <typename Scalar, int dim> struct LagrangeFluid {
  Field<Scalar, dim> position_;
  Field<Scalar> mass_;
  Field<Scalar> volumn_;
  Field<Scalar, dim> velocity_;
  Scalar rho_, e_;
  LagrangeFluid() = default;
  explicit LagrangeFluid(Index particle_count);
};

/**
 * @brief Euler fluid representation, fluid velocity field is discreted as
 *  Regular Grid.
 *
 * @tparam Scalar
 */
template <typename Scalar, int dim> struct EulerFluidRegular {
  Field<Scalar> mass_;
  Field<Scalar, dim> velocity_;
  Vec<Scalar, dim> lower_bound_;
  Vec<Scalar, dim> upper_bound_;
  // NOTE: Grid size and Div count should match.
  Vec<Index, dim> div_count_;
  Scalar grid_size_;
  EulerFluidRegular() = default;
  EulerFluidRegular(Vec<Index, dim> div_count, Vec<Scalar, dim> lower_bound,
                    Vec<Scalar, dim> upper_bound);
};

} // namespace acg::physics
