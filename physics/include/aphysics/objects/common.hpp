#pragma once
#include "../common.hpp"

namespace acg::physics {
enum class PhysicsObjectType {
  kCloth,
  kSoftbody,
  kLaglangianFluid,
  kEulerFluid,
};

struct PhysicsObject {
  PhysicsObjectType type_;
  Index id_;
  Index entity_;
};
}
