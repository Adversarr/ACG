#pragma once
#include "scene.hpp"

#define GLM_FORCE_RADIANS
#define GLM_FORCE_DEPTH_ZERO_TO_ONE
#include <glm/glm.hpp>

namespace acg::visualizer {

class Camera {
public:
  explicit Camera() = default;

  glm::mat4 GetView() const;
  glm::mat4 GetProjection(float width, float height, bool inverted_y_axis=true) const;
  glm::mat4 GetModel() const;
  glm::vec3 GetPosition() const;

  glm::vec3 GetFront() const;

  // TODO: add camera movement handles.
  void SetPosition(glm::vec3 position);
  
  void SetFrontDirection(glm::vec3 front);

private:
  // Extra Rotation/transform applied to each model.
  glm::vec3 model_rotate_axis_{0.0f, 0.0f, 1.0f};
  float model_rotate_angle_{0.0f};
  
  glm::vec3 position_{3.0f, 0.0f, 1.0f};
  
  glm::vec3 front_{-3.0f, 0.0f, -1.0f};

  // Up is always the positive direction of z-axis
  glm::vec3 up_{0.0f, 0.0f, 1.0f};
  float fov_{45.0};
  float near_{0.1f};
  float far_{10.0f};
};
}
