//
// Created by jerryyang on 2022/10/25.
//
#pragma once
#define GLM_FORCE_RADIANS
#define GLM_FORCE_DEPTH_ZERO_TO_ONE
#include <glm/glm.hpp>
#include <vector>
#include "avk.hpp"

namespace acg::gui {

namespace details {

constexpr auto VK_INDEX_TYPE = vk::IndexType::eUint32;
using GuiIndex = uint32_t;


}  // namespace details


using GuiIndex = details::GuiIndex ;

/**
 * @brief Vertex Buffer.
 *
 */
struct Vertex {
  // Note: In our program, no texture is needed.
  glm::vec3 position_;
  glm::vec3 color_;
  glm::vec3 normal_;

  Vertex() = default;

  inline Vertex(const glm::vec3 &position, const glm::vec3 &color, const glm::vec3 &normal)
      : position_(position), color_(color), normal_(normal) {}

  static std::vector<vk::VertexInputBindingDescription> GetBindingDescriptions();

  static std::vector<vk::VertexInputAttributeDescription> GetAttributeDescriptions();
};


struct Point {
  glm::vec3 position;
  glm::vec3 color;

  inline Point(const glm::vec3& position, const glm::vec3& color):
    position(position), color(color) {

  }
  static std::vector<vk::VertexInputBindingDescription> GetBindingDescriptions();

  static std::vector<vk::VertexInputAttributeDescription> GetAttributeDescriptions();
};
/**
 * @brief Content of Uniform Buffer
 *
 */
struct Ubo {
  alignas(16) glm::mat4 mvp;
  // alignas(16) glm::vec3 eye_position;
  // alignas(16) glm::vec4 ambient_light_color{1.f, 1.f, 1.f, .01f};  // w is intensity
  // alignas(16) glm::vec3 light_color{1.0, 1.0, 1.0};
  // alignas(16) glm::vec3 light_position{2.0, 0.0, 1.0};

  // // Different Shader may use this option for different usage.
  // alignas(16) int options[4] = {0, 0, 0, 0};
};

}  // namespace acg::gui
