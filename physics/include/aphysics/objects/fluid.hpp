#pragma once
#include <acore/geometry/common.hpp>
#include <acore/math/common.hpp>

namespace acg::physics {

template <typename Scalar, int dim> struct LagrangeFluid {
  Field<Scalar, dim> particle_position_;
  Field<Scalar> particle_mass_;
  Field<Scalar> particle_volumn_;
  Field<Scalar, dim> particle_velocity_;
  Scalar rho_, e_;
};

}  // namespace acg::physics
