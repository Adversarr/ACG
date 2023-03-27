#pragma once

#include "aphysics/objects/common.hpp"
#include <acore/math/common.hpp>
namespace acg::physics {

/**
 * @brief Represents object_ should have the given position
 * 
 * @tparam Scalar 
 * @tparam dim 
 */
template <typename Scalar, int dim> struct PositionStaticConstraint {
  Vec<Scalar, dim> value_;
  PhysicsObject object_;
  PositionStaticConstraint(PhysicsObject type, Vec<Scalar, dim> v)
      : object_(type), value_(v) {}
};

template <typename Scalar, int dim> struct ExternForceConstraint {
  Vec<Scalar, dim> value_;
  PhysicsObject object_;
};

/**
 * @brief Represents object_ should have the given velocity.
 * 
 * @tparam Scalar 
 * @tparam dim 
 */
template <typename Scalar, int dim> struct VelocityStaticLimit {
  Vec<Scalar, dim> value_;
  PhysicsObject object_;
  VelocityStaticLimit(PhysicsObject type, Vec<Scalar, dim> v)
      : object_(type), value_(v) {}
};

} // namespace acg::physics
