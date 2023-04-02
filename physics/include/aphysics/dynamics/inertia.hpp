#pragma once
#include <acore/math/common.hpp>

namespace acg::physics::dynamics {

template<typename Scalar, int dim> struct InertiaPosition {
  Scalar dt_;

  Scalar k_{1};

  Vec<Scalar, dim> grav_accel_;

  explicit InertiaPosition(Scalar dt) : 
    dt_(dt), grav_accel_(Vec<Scalar, dim>::Zero()) {
    grav_accel_(grav_accel_.size() - 1) = -9.8;
  }

  /**
   * @brief Compute the inertia position.
   * 
   * @param position 
   * @param velocity 
   * @return auto 
   */
  auto operator()(const Field<Scalar, dim>& position, const Field<Scalar, dim>& velocity){
    Field<Scalar, dim> iner = (position + dt_ * velocity);
    iner.colwise() += grav_accel_ * dt_ * dt_ * k_;
    return iner;
  }
};

}