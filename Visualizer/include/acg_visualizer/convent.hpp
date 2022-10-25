#pragma once

#include <glm/glm.hpp>
#include <acg_core/math.hpp>

namespace acg::visualizer {


inline glm::vec2 to_glm(Vec2f v){
  return {v.x(), v.y()};
}

inline glm::vec3 to_glm(Vec3f v){
  return {v.x(), v.y(), v.z()};
}

inline glm::vec4 to_glm(Vec4f v){
  return {v.x(), v.y(), v.z(), v.w()};
}

}