#pragma once

#define GLM_FORCE_RADIANS
#define GLM_FORCE_DEPTH_ZERO_TO_ONE
#include <glm/glm.hpp>

#include <acg_core/math/common.hpp>

namespace acg::visualizer {

class Camera {
public:
  explicit Camera() = default;

  glm::mat4 GetView() const;

  // Get the projection matrix, you should set inverted_y_axis to true, if you are using vulkan
  // backend.
  glm::mat4 GetProjection(float width, float height, bool inverted_y_axis = true) const;

  glm::mat4 GetModel() const;

  Vec3f &GetPosition();

  const Vec3f &GetPosition() const;

  const Vec3f &GetFront() const;

  Vec3f &GetFront();

  Vec3f &GetUp();

  void Move(Vec3f direction, F64 dt);

  // Set the position of the camera. Returns true, if readlly changed.
  bool SetPosition(glm::vec3 position);

  // Set the front direction of the camera. Returns true, if really changed.
  bool SetFront(glm::vec3 front);

private:
  // Extra Rotation/transform applied to each model.
  Vec3f model_rotate_axis_{0.0f, 0.0f, 1.0f};

  float model_rotate_angle_{0.0f};

  Vec3f position_{3.0f, 0.0f, 1.0f};

  Vec3f front_{-3.0f, 0.0f, -1.0f};

  Vec3f up_{0.0f, 0.0f, 1.0f};

  float fov_{45.0};
  float near_{0.1f};
  float far_{100.0f};
};
}  // namespace acg::visualizer
