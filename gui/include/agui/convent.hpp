#pragma once

#include <acore/math/common.hpp>
#include <glm/glm.hpp>

namespace acg::gui {

inline glm::vec2 to_glm(Vec2f v) { return {v.x(), v.y()}; }

inline glm::vec3 to_glm(Vec3f v) { return {v.x(), v.y(), v.z()}; }

inline glm::vec4 to_glm(Vec4f v) { return {v.x(), v.y(), v.z(), v.w()}; }

inline Vec2f to_eigen(glm::vec2 v) { return {v.x, v.y}; }

inline Vec3f to_eigen(glm::vec3 v) { return {v.x, v.y, v.z}; }

inline Vec4f to_eigen(glm::vec4 v) { return {v.x, v.y, v.z, v.w}; }

}  // namespace acg::gui
