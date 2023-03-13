#pragma once
#include "aphysics/objects/common.hpp"
namespace acg::physics {

template <typename Scalar, int dim> struct PositionLimit {
  Vec<Scalar, dim> value_;
  PhysicsObject object_;
  PositionLimit(PhysicsObject type, Vec<Scalar, dim> v) : object_(type), value_(v) {}
};

template <typename Scalar, int dim> struct VelocityLimit {
  Vec<Scalar, dim> value_;
  PhysicsObject object_;
  VelocityLimit(PhysicsObject type, Vec<Scalar, dim> v) : object_(type), value_(v) {}
};

}  // namespace acg::physics
