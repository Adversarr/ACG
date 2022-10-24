#pragma once

#include <memory>
#include <acg_core/geometry/mesh.hpp>
#include "acg_core/math.hpp"
#include "acg_visualizer/camera.hpp"


// libs
#define GLM_FORCE_RADIANS
#define GLM_FORCE_DEPTH_ZERO_TO_ONE
#include <glm/glm.hpp>
#include <glm/gtc/constants.hpp>

#include "_vk.hpp"
#include "point_light.hpp"

namespace acg::visualizer::details {

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

/**
 * @brief An valid mesh for rendering.
 * 
 */
class VertIdxBuffer {
public:
  vk::DeviceSize GetRequiredVertexBufferSize() const;

  vk::DeviceSize GetRequiredIndexBufferSize() const;

  // TODO: Binding Function.
  // TODO: VertIdxBuffer& AppendVertex();
  // TODO: VertIdxBuffer& AppendIdx();

private:
  std::vector<Vertex> vertices;

  std::vector<Idx> indices;
};

class Scene {
  // todo: add marching cube support.
public:
  void Swap(Scene& another);

  void Copy(const Scene& another);

  void Reset();

  /**
   * @brief add a given triangle mesh to the scene for rendering.
   * 
   * @param mesh 
   * @param color
   */
  void AddMesh(const geometry::TriangleMesh<F32>& mesh,
    Vec3f color);

  /**
   * @brief add a given triangle mesh to the scene, with given color map.
   * 
   * @param mesh 
   * @param color_per_vertex 
   */
  void AddColoredMesh(const geometry::TriangleMesh<F32> & mesh, 
    Eigen::MatrixX3<F32> color_per_vertex);

  // TODO: void AddParticleSystem(const )

private:
  VertIdxBuffer meshes_;
};



}