#pragma once

#include <memory>
#include <acg_core/geometry/mesh.hpp>
#include <acg_core/geometry/particlesystem.hpp>
#include "acg_core/math.hpp"


// libs
#define GLM_FORCE_RADIANS
#define GLM_FORCE_DEPTH_ZERO_TO_ONE
#include <glm/glm.hpp>
#include <glm/gtc/constants.hpp>

#include <vulkan/vulkan.hpp>
#include "buffer_def.hpp"


namespace acg::visualizer {

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
  Scene& AddMesh(const geometry::Mesh<F32>& mesh,
    Vec3f color);

  Scene& AddMesh(const geometry::Mesh<F32>& mesh,
                 const geometry::Mesh<F32>::StateType& normal,
                 Vec3f color);
  /**
   * @brief add a given triangle mesh to the scene, with given color map.
   * 
   * @param mesh 
   * @param color_per_vertex 
   */
  Scene& AddColoredMesh(const geometry::Mesh<F32> & mesh, 
    Eigen::MatrixX3<F32> color_per_vertex);

  Scene& AddParticle(Vec3f center, F32 radius, Vec3f color);


  vk::DeviceSize GetRequiredVertexBufferSize() const;

  vk::DeviceSize GetRequiredIndexBufferSize() const;

  const std::vector<Vertex>& GetVertices() const;

  const std::vector<Idx>& GetIndices() const;


private:
  std::vector<Vertex> vertices_;

  std::vector<Idx> indices_;

};



}
