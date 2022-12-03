#pragma once

#include <acg_core/geometry/mesh.hpp>
#include <acg_core/geometry/particlesystem.hpp>
#include <memory>
#include <optional>

#include "acg_core/math.hpp"

// libs
#define GLM_FORCE_RADIANS
#define GLM_FORCE_DEPTH_ZERO_TO_ONE
#include <glm/glm.hpp>
#include <glm/gtc/constants.hpp>
#include "backend/avk.hpp"
#include "backend/buffer_def.hpp"

namespace acg::visualizer {

class Scene {
  // todo: add marching cube support.
public:
  void Swap(Scene& another);

  void Copy(const Scene& another);

  void Reset();

  /**
  * @brief Add a particle to the scene.
  *
  * @param particle: contains the position and radius
  * @param color: the particle's color, in RGB (r, g, b \in [0, 1])
  */
  Scene& AddParticle(const geometry::Particle<F32>& particle, const Vec3f& color);

  /**
  * @brief Add a triangle mesh to the scene.
  *
  * @param mesh: triangle mesh
  * @param opt_normals: simply use std::nullopt here. i.e. AddMesh(mesh, std::nullopt, colors);
  * @param colors: color for each vertex (3xn) or the whole mesh (3x1)
  *
  * @example AddMesh(mesh, std::nullopt, Vec3f(0.4, 0.5, 0.6));
  * @example 
  *     AttrVec<F32, 3> v = AttrVecTrans<F32, 3>{{0, 0, 1}, {0, 0, 0}, {0, 1, 0}}.transpose();
  *     AttrVec<Idx, 3> f = AttrVecTrans<Idx, 3>{{0, 1, 2}}.transpose();
  *     AttrVec<F32, 3> c = AttrVecTrans<F32, 3>{{7, .7, .7}}.transpose();
  *     geometry::Mesh<F32> mesh(v, f);
  *     scene_.AddMesh(mesh, AttrVecTrans<F32, 3>{{1, 0, 0}, {1, 0, 0}, {1, 0, 0}}.transpose(), Vec3f(.5, .6, .7));
  */
  Scene& AddMesh(geometry::Mesh<F32> mesh, std::optional<AttrVec<F32, 3>> opt_normals,
                 AttrVec<F32, 3> colors);

  vk::DeviceSize GetRequiredVertexBufferSize() const;

  vk::DeviceSize GetRequiredIndexBufferSize() const;

  std::pair<std::vector<Vertex>, std::vector<IndexType>> Build() const;

private:
  std::vector<geometry::Mesh<F32>> meshes_;

  std::vector<std::optional<AttrVec<F32, 3>>> normals_;

  std::vector<AttrVec<F32, 3>> mesh_colors_;

  std::vector<geometry::Particle<F32>> particles_;

  std::vector<Vec3f> particles_colors_;
};

}  // namespace acg::visualizer
