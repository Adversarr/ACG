#pragma once

#include <acg_core/geometry/mesh.hpp>
#include <acg_core/geometry/particlesystem.hpp>
#include <memory>
#include <optional>

#include "acg_core/math/common.hpp"
#include "camera.hpp"
#include "light.hpp"

// libs
#define GLM_FORCE_RADIANS
#define GLM_FORCE_DEPTH_ZERO_TO_ONE
#include <glm/glm.hpp>
#include <glm/gtc/constants.hpp>

#include "backend/avk.hpp"
#include "backend/buffer_def.hpp"

namespace acg::gui {

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
   *     scene_.AddMesh(mesh, AttrVecTrans<F32, 3>{{1, 0, 0}, {1, 0, 0}, {1, 0, 0}}.transpose(),
   * Vec3f(.5, .6, .7));
   */
  Scene& AddMesh(geometry::SimpleMesh<F32> mesh, std::optional<Field<F32, 3>> opt_normals,
                 Field<F32, 3> colors);

  vk::DeviceSize GetRequiredVertexBufferSize() const;

  vk::DeviceSize GetRequiredIndexBufferSize() const;

  std::pair<std::vector<Vertex>, std::vector<GuiIdx>> Build() const;

private:
  std::vector<geometry::SimpleMesh<F32>> meshes_;

  std::vector<std::optional<Field<F32, 3>>> normals_;

  std::vector<Field<F32, 3>> mesh_colors_;

  std::vector<geometry::Particle<F32>> particles_;

  std::vector<Vec3f> particles_colors_;
};

class Scene2 {
public:
  Scene2() = default;

private:
  Light light_;

  Camera camera_;

  // For Mesh: Vertex, Index, normal, and color.
  std::vector<geometry::SimpleMesh<F32>> meshes_;
  std::vector<types::RgbField> mesh_colors_;
  std::vector<Field<F32, 3>> normals_;

  // For particles: Color, position, radius
  std::vector<types::Position<F32, 3>> particle_positions_;
  std::vector<F32> particle_radiuses_;
  std::vector<types::Rgb> particle_colors_;

  // For wireframes: Color, verteces.
  std::vector<types::PositionField<F32, 3>> wireframe_vertex_lists_;
  std::vector<Field<Idx, 2>> wireframe_index_lists_;
  std::vector<types::RgbField> wireframe_colors_;

  // TODO: Arrows
public:
  Scene2 SetCamera(const Camera& camera);
  Scene2 SetLight(const Light& light);

  void AddMesh(const geometry::SimpleMesh<F32>& mesh, std::optional<Field<F32, 3>> normals,
               const Field<F32, 3>& colors);

  void AddLines(const types::PositionField<F32, 3>& vertices, const Field<Idx, 2>& indices,
                const types::RgbField& colors);

  void AddParticles(const types::PositionField<F32, 3>& positions, const Field<F32, 1>& radiuses,
                    const types::RgbField& colors);

  void AddParticle(const types::Position<F32, 3>& position, const F32& radius,
                   const types::Rgb& color);
};

class Canvas {
private:
  std::unique_ptr<Scene2> scene_;

  std::vector<types::Position<F32, 3>> circle_positions_;
  std::vector<F32> circle_radiuses_;
  std::vector<types::Rgb> circle_colors_;
};

}  // namespace acg::gui
