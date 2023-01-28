//
// Created by jerryyang on 2022/10/18.
//

#include "acg_gui/camera.hpp"

#include "acg_gui/convent.hpp"
#define GLM_FORCE_RADIANS
#define GLM_FORCE_DEPTH_ZERO_TO_ONE
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

namespace acg::gui {
glm::mat4 Camera::GetView() const {
  return glm::lookAt(to_glm(position_), to_glm(Vec3f(position_ + front_)), to_glm(up_));
}

glm::mat4 Camera::GetProjection(float width, float height, bool inverted_y_axis) const {
  auto proj = glm::perspective(glm::radians(fov_), width / height, near_, far_);
  if (!is_perspective_) {
    proj = glm::ortho(-width / 2, width / 2, height / 2, -height / 2, near_, far_);
  }
  if (inverted_y_axis) {
    proj[1][1] *= -1;
  }

  return proj;
}

glm::mat4 Camera::GetModel() const {
  return glm::rotate(glm::mat4(1.0f), glm::radians(model_rotate_angle_),
                     to_glm(model_rotate_axis_));
}

Vec3f& Camera::GetPosition() { return position_; }

Vec3f& Camera::GetFront() { return front_; }

Vec3f& Camera::GetUp() { return up_; }

bool Camera::SetPosition(glm::vec3 position) {
  bool changed = to_eigen(position) != position_;
  if (changed) {
    position_ = to_eigen(position);
  }
  return changed;
}

bool Camera::SetFront(glm::vec3 front) {
  bool changed = to_eigen(front) != front_;
  if (changed) {
    front_ = to_eigen(front);
  }
  return changed;
}
void Camera::Move(Vec3f direction, F64 dt) { position_ = position_ + direction * dt; }

const Vec3f& Camera::GetPosition() const { return position_; }

const Vec3f& Camera::GetFront() const { return front_; }

Camera& Camera::SetLookAt(Vec3f look_at) {
  front_ = look_at - position_;
  return *this;
}

Camera& Camera::SetTop(F32 top) {
  top_ = top;
  return *this;
}

Camera& Camera::SetBottom(F32 bottom) {
  bottom_ = bottom;
  return *this;
}

Camera& Camera::SetLeft(F32 left) {
  left_ = left;
  return *this;
}

Camera& Camera::SetRight(F32 right) {
  right_ = right;
  return *this;
}

Camera& Camera::SetProjectionMode(bool is_perspective) {
  is_perspective_ = is_perspective;
  return *this;
}

Camera& Camera::SetPitch(F32 pitch) {
  pitch_ = pitch;
  return *this;
}

Camera& Camera::SetRoll(F32 roll) {
  roll_ = roll;
  return *this;
}

Camera& Camera::SetYaw(F32 yaw) {
  yaw_ = yaw;
  return *this;
}

Camera& Camera::SetUp(Vec3f up) {
  up_ = up;
  return *this;
}

// TODO: compute everything using yrp.

}  // namespace acg::gui
