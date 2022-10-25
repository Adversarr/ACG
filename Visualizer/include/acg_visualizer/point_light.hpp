#pragma once

#include "acg_core/math.hpp"
#include "glm/glm.hpp"

namespace acg::visualizer {
class Light {
public:
  Light();

private:
  Vec3f light_position;
  Vec3f light_color;
  Vec3f ambient_light_color;
};
}