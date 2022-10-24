//
// Created by jerryyang on 2022/10/18.
//

#include "acg_visualizer/camera.hpp"
#define GLM_FORCE_RADIANS
#define GLM_FORCE_DEPTH_ZERO_TO_ONE
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

namespace acg::visualizer{
glm::mat4 Camera::GetView() const {
  return glm::lookAt(position_, position_ + front_, up_);
}

glm::mat4 Camera::GetProjection(float width, float height, bool inverted_y_axis) const {
  auto proj =  glm::perspective(glm::radians(fov_), width / height, near_, far_);
  if (inverted_y_axis) {
    proj[1][1] *= -1;
  }

  return proj;
}


glm::mat4 Camera::GetModel() const {
  return glm::rotate(glm::mat4(1.0f), glm::radians(model_rotate_angle_), model_rotate_axis_);
}

glm::vec3 Camera::GetPosition() const {
  return position_;
}

glm::vec3 Camera::GetFront() const {
  return front_;
}

void Camera::SetPosition(glm::vec3 position){
  position_ = position;
}

void Camera::SetFrontDirection(glm::vec3 front) {
  front_ = front;
}


}
