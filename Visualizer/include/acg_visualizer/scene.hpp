#pragma once

#include <memory>
#include <acg_core/mesh.hpp>
#include "acg_visualizer/camera.hpp"


// libs
#define GLM_FORCE_RADIANS
#define GLM_FORCE_DEPTH_ZERO_TO_ONE
#include <glm/glm.hpp>
#include <glm/gtc/constants.hpp>

#include "_vk.hpp"

namespace acg::visualizer::details {

struct Vertex {
  // Note: In our program, no texture is needed.
  glm::vec3 position;
  glm::vec3 color;
  glm::vec3 normal;

  static std::vector<vk::VertexInputBindingDescription> GetBindingDescriptions();

  static std::vector<vk::VertexInputAttributeDescription> GetAttributeDescriptions();
};


// TODO: may add multiple lights support.
struct Ubo {
  alignas(16) glm::mat4 model;
  alignas(16) glm::mat4 view;
  alignas(16) glm::mat4 projection;
//  alignas(16) glm::vec4 ambient_light_color{1.f, 1.f, 1.f, .02f};  // w is intensity
//  alignas(16) glm::vec4  light_position;
//  alignas(16) glm::vec4  light_color;
};

using VertexIndex = uint32_t;


class Scene {
public:
  void Reset();

  template<typename T>
  void AddMesh(const geometry::TriangleMesh<T>& mesh, Eigen::Vector3f color);

  template<typename T>
  void AddColoredMesh(const geometry::TriangleMesh<T> & mesh, Eigen::MatrixX3f color_per_vertex);

  [[nodiscard]] inline const std::vector<Vertex>& GetVertices() const noexcept;

private:
  std::vector<Vertex> vertices_;
};

inline const std::vector<Vertex>& Scene::GetVertices() const noexcept {
  return vertices_;
}

}