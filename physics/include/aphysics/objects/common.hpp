#pragma once
#include "../common.hpp"

namespace acg::physics {

// NOTE: Whenever you implement a new PO, add the type here.
enum class PhysicsObjectType {
  kCloth,
  kSoftbody,
  kLaglangianFluid,
  kEulerFluid,
};

struct PhysicsObject {
  PhysicsObjectType type_;
  Index object_;  // ID for object
  Index id_;      // ID for component
};

// Use SELECT_ALL_COMPONENT for every component.
static constexpr Index SELECT_ALL_COMPONENT = std::numeric_limits<Index>::max();

}  // namespace acg::physics
