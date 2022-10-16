#pragma once

#include <memory>
#include <acg_core/mesh.hpp>


// libs
#define GLM_FORCE_RADIANS
#define GLM_FORCE_DEPTH_ZERO_TO_ONE
#include <glm/glm.hpp>
#include <glm/gtc/constants.hpp>

#include "_vk.hpp"

namespace acg::visualizer::details {

struct Vertex {
  // Note: In our program, no texture is needed.
  alignas(16) glm::vec3 position;
  alignas(16) glm::vec3 color;
  alignas(16) glm::vec2 normal;

  static std::vector<vk::VertexInputBindingDescription> GetBindingDescriptions();

  static std::vector<vk::VertexInputAttributeDescription> GetAttributeDescriptions();

  inline bool operator==(const Vertex &other) const {
    return position == other.position && color == other.color && normal == other.normal;
  }
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