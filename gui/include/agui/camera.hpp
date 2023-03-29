#pragma once

#define GLM_FORCE_RADIANS
#define GLM_FORCE_DEPTH_ZERO_TO_ONE
#include <acore/math/common.hpp>
#include <glm/glm.hpp>

namespace acg::gui {

class Camera {
public:
  explicit Camera() = default;

  glm::mat4 GetView() const;

  // Get the projection matrix, you should set inverted_y_axis to true, if you are using vulkan
  // backend.
  glm::mat4 GetProjection(float width, float height, bool inverted_y_axis = true) const;

  glm::mat4 GetModel() const;

  Vec3f& GetPosition();

  const Vec3f& GetPosition() const;

  const Vec3f& GetFront() const;

  Vec3f& GetFront();

  Vec3f& GetUp();

  void Move(Vec3f direction, Float64 dt);
  // Set the position of the camera.
  bool SetPosition(glm::vec3 position);

  // Set the front direction of the camera.
  bool SetFront(glm::vec3 front);

  Camera& SetFov(Float32 angle);
  Float32 GetFov() const;
  Camera& SetUp(Vec3f up);

  Camera& SetLeft(Float32 left);
  Camera& SetRight(Float32 right);
  Camera& SetTop(Float32 top);
  Camera& SetBottom(Float32 bottom);

  Camera& SetLookAt(Vec3f look_at);

  Camera& SetProjectionMode(bool is_perspective = true);

  Camera& SetYaw(Float32 yaw);
  Camera& SetRoll(Float32 roll);
  Camera& SetPitch(Float32 pitch);

  Float32 GetYaw() const;
  Float32 GetRoll() const;
  Float32 GetPitch() const;

  bool& GetProjectionMode() { return is_perspective_; }

private:
  bool is_perspective_{true};
  // Extra Rotation/transform applied to each model.
  Vec3f model_rotate_axis_{0.0f, 0.0f, 1.0f};
  float model_rotate_angle_{0.0f};

  Vec3f position_{3.0f, 0.0f, 1.0f};
  Vec3f front_{-3.0f, 0.0f, -1.0f};
  Vec3f up_{0.0f, 0.0f, 1.0f};

  float fov_{45.0};
  float near_{0.1f};
  float far_{100.0f};

  float yaw_{0}, roll_{0}, pitch_{0};
  float left_, right_, top_, bottom_;
};
}  // namespace acg::gui
