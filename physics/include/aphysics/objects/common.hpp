#pragma once
#include <acore/common.hpp>

namespace acg::physics {

// NOTE: Whenever you implement a new PO, add the type here.
enum PhysicsObjectType : int {
  kCloth,
  kHyperElasticSoftbody,
  kLaglangianFluid,
  kEulerRegularFluid,
};

struct PhysicsObject {
  // Use SELECT_ALL for every component.
  static constexpr Index SELECT_ALL = std::numeric_limits<Index>::max();
  PhysicsObjectType type_;
  Index object_{SELECT_ALL};  // ID for object
  Index id_{SELECT_ALL};      // ID for component

  PhysicsObject() = default;

  inline PhysicsObject(PhysicsObjectType type, Index o, Index i) noexcept : type_(type), object_(o), id_(i) {}

  bool operator==(const PhysicsObject& rhs) const noexcept {
    return object_ == rhs.object_ && type_ == rhs.type_ && id_ == rhs.id_;
  }
};
}  // namespace acg::physics

template <> struct std::hash<acg::physics::PhysicsObject> {
  std::size_t operator()(acg::physics::PhysicsObject const& s) const noexcept {
    return (s.object_ ^ (s.id_ << 1)) ^ ((static_cast<int>(s.type_)) << 1);
  }
};
