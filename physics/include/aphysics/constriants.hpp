#pragma once

#include <acore/math/common.hpp>

#include "aphysics/objects/common.hpp"
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
  PositionStaticConstraint(PhysicsObject type, Vec<Scalar, dim> v) : object_(type), value_(v) {}
};

template <typename Scalar, int dim> struct ExternForceConstraint {
  Vec<Scalar, dim> value_;
  PhysicsObject object_;
};

template <typename Scalar> struct GroundConstraint { Scalar z_value_; };

/**
 * @brief Represents object_ should have the given velocity.
 *
 * @tparam Scalar
 * @tparam dim
 */
template <typename Scalar, int dim> struct VelocityStaticLimit {
  Vec<Scalar, dim> value_;
  PhysicsObject object_;
  VelocityStaticLimit(PhysicsObject type, Vec<Scalar, dim> v) : object_(type), value_(v) {}
};

enum CollisionType : int {
  kVertexFace,
  kEdgeEdge,
};


struct CollisionConstraint {
  CollisionType type_;
  PhysicsObject obj0_;
  PhysicsObject obj1_;

  bool operator==(const CollisionConstraint& rhs) const noexcept {
    return type_ == rhs.type_ && obj0_ == rhs.obj0_ && obj1_ == rhs.obj1_;
  }
};
}  // namespace acg::physics

template <> struct std::hash<acg::physics::CollisionConstraint> {
  std::size_t operator()(acg::physics::CollisionConstraint const& s) const noexcept {
    std::size_t h1 = std::hash<acg::physics::PhysicsObject>{}(s.obj0_);
    std::size_t h2 = std::hash<acg::physics::PhysicsObject>{}(s.obj1_);
    std::size_t h3 = s.type_;
    return h1 ^ (h2 << 1) ^ (h3 << 1);  // or use boost::hash_combine
  }
};
