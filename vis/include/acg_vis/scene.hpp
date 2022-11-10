#pragma once

#include <acg_core/geometry/mesh.hpp>
#include <acg_core/geometry/particlesystem.hpp>
#include <memory>

#include "acg_core/math.hpp"
#include "point_light.hpp"

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

  Scene& AddParticle(const geometry::Particle<F32>& particle, const Vec3f& color);

  Scene& AddMesh(geometry::Mesh<F32> mesh, std::optional<AttrVec<F32, 3>> opt_normals,
                 AttrVec<F32, 3> colors);

  vk::DeviceSize GetRequiredVertexBufferSize() const;

  vk::DeviceSize GetRequiredIndexBufferSize() const;

  std::pair<std::vector<Vertex>, std::vector<Idx>> Build() const;

private:
  std::vector<geometry::Mesh<F32>> meshes_;

  std::vector<std::optional<AttrVec<F32, 3>>> normals_;

  std::vector<AttrVec<F32, 3>> mesh_colors_;

  std::vector<geometry::Particle<F32>> particles_;

  std::vector<Vec3f> particles_colors_;
};

}  // namespace acg::visualizer
