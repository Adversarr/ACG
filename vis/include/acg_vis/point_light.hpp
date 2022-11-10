#pragma once

#include "acg_core/math.hpp"

namespace acg::visualizer {
struct Light {
  Vec3f light_position_{3.0, 3.0, 3.0};
  Vec3f light_color_{0.5, 0.5, 0.5};
  Vec3f ambient_light_color_{0.2, 0.2, 0.2};
  F32 ambient_light_density_{.01};
};
}
