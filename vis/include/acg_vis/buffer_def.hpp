//
// Created by jerryyang on 2022/10/25.
//
#pragma once
#ifndef ACG_BUFFER_DEF_HPP
#define ACG_BUFFER_DEF_HPP
#include <glm/glm.hpp>
#include <vector>
#include <vulkan/vulkan.hpp>
namespace acg::visualizer {

/**
 * @brief Element of Vertex Buffer.
 *
 */
struct Vertex {
  // Note: In our program, no texture is needed.
  glm::vec3 position;
  glm::vec3 color;
  glm::vec3 normal;

  static std::vector<vk::VertexInputBindingDescription> GetBindingDescriptions();

  static std::vector<vk::VertexInputAttributeDescription> GetAttributeDescriptions();
};

/**
 * @brief Content of Uniform Buffer
 *
 */
struct Ubo {
 alignas(16) glm::mat4 model;
 alignas(16) glm::mat4 view;
 alignas(16) glm::mat4 projection;
 alignas(16) glm::vec3 eye_position;
 alignas(16) glm::vec4 ambient_light_color{1.f, 1.f, 1.f, .01f};  // w is intensity
 alignas(16) glm::vec3 light_color{1.0, 1.0, 1.0};
 alignas(16) glm::vec3 light_position{2.0, 0.0, 1.0};
};



}

#endif  // ACG_BUFFER_DEF_HPP
