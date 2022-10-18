#pragma once
#include "scene.hpp"

#define GLM_FORCE_RADIANS
#define GLM_FORCE_DEPTH_ZERO_TO_ONE
#include <glm/glm.hpp>

namespace acg::visualizer {

class Camera {
public:
  explicit Camera() = default;

  [[nodiscard]] glm::mat4 GetView() const;
  [[nodiscard]] glm::mat4 GetProjection(float width, float height, bool inverted_y_axis=true) const;

  // TODO: add camera movement handles.

private:
  glm::vec3 position_{1.0f, 1.0f, 2.0f};
  glm::vec3 front_{-1.0f, -1.0f, -2.0f};
  glm::vec3 up_{0.0f, 0.0f, 1.0f};
  float fov_{45.0};
  float near_{0.1f};
  float far_{10.0f};
};
}