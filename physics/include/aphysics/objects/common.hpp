#pragma once
#include <acore/common.hpp>

namespace acg::physics {

// NOTE: Whenever you implement a new PO, add the type here.
enum class PhysicsObjectType {
  kCloth,
  kHyperElasticSoftbody,
  kLaglangianFluid,
  kEulerRegularFluid,
};

struct PhysicsObject {
  PhysicsObjectType type_;
  Index object_; // ID for object
  Index id_;     // ID for component

  // Use SELECT_ALL_COMPONENT for every component.
  static constexpr Index ALL_COMPONENT = std::numeric_limits<Index>::max();

  inline PhysicsObject(PhysicsObjectType type, Index o, Index i) noexcept
      : type_(type), object_(o), id_(i) {}

  inline PhysicsObject(PhysicsObjectType type, Index i) noexcept
      : type_(type), object_(ALL_COMPONENT), id_(i) {}
};
} // namespace acg::physics
