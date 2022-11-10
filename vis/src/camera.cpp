//
// Created by jerryyang on 2022/10/18.
//

#include "acg_vis/camera.hpp"
#include "acg_vis/aglm.hpp"

#include "acg_vis/convent.hpp"

namespace acg::visualizer {
glm::mat4 Camera::GetView() const {
  return glm::lookAt(to_glm(position_), 
                     to_glm(Vec3f(position_ + front_)), 
                     to_glm(up_));
}

glm::mat4 Camera::GetProjection(float width, float height, bool inverted_y_axis) const {
  auto proj = glm::perspective(glm::radians(fov_), width / height, near_, far_);
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

void Camera::Move(glm::vec3 direction, F64 dt) { position_ = position_ + to_eigen(direction) * dt; }

const Vec3f &Camera::GetCPosition() const {
  return position_;
}
const Vec3f &Camera::GetCFront() const {
  return front_;
}


}  // namespace acg::visualizer