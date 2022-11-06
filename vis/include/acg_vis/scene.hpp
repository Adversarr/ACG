#pragma once

#include <memory>
#include <acg_core/geometry/mesh.hpp>
#include <acg_core/geometry/particlesystem.hpp>
#include "acg_core/math.hpp"
#include "acg_vis/camera.hpp"


// libs
#define GLM_FORCE_RADIANS
#define GLM_FORCE_DEPTH_ZERO_TO_ONE
#include <glm/glm.hpp>
#include <glm/gtc/constants.hpp>

#include "avk.hpp"
#include "buffer_def.hpp"
#include "point_light.hpp"
namespace acg::visualizer::details {

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
  Scene& AddParticleSystem(const geometry::ParticleSystem<F32>& ps);

private:
  VertIdxBuffer meshes_;
};



}
