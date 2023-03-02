#pragma once
#include <acore/geometry/common.hpp>
#include <acore/geometry/mesh.hpp>
#include <acore/geometry/particle.hpp>
#include <acore/math/access.hpp>
#include <memory>
#include <optional>
#include <vector>

#include "acore/math/common.hpp"
#include "backend/buffer_def.hpp"
#include "camera.hpp"
#include "light.hpp"

namespace acg::gui {

class Scene {
  // todo: add marching cube support.
public:

  void Reset();

  /**
   * @brief Add a particle to the scene.
   *
   * @param particle: contains the position and radius
   * @param color: the particle's color, in RGB (r, g, b \in [0, 1])
   */
  Scene& AddParticle(const geometry::Particle<Float32>& particle, const Vec3f& color);

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
   *     AttrVec<Index , 3> f = AttrVecTrans<Index , 3>{{0, 1, 2}}.transpose();
   *     AttrVec<F32, 3> c = AttrVecTrans<F32, 3>{{7, .7, .7}}.transpose();
   *     geometry::Mesh<F32> mesh(v, f);
   *     scene_.AddMesh(mesh, AttrVecTrans<F32, 3>{{1, 0, 0}, {1, 0, 0}, {1, 0, 0}}.transpose(),
   * Vec3f(.5, .6, .7));
   */
  Scene& AddMesh(geometry::SimpleMesh<Float32> mesh, std::optional<Field<Float32, 3>> opt_normals,
                 Field<Float32, 3> colors);

  vk::DeviceSize GetRequiredVertexBufferSize() const;

  vk::DeviceSize GetRequiredIndexBufferSize() const;

  std::pair<std::vector<Vertex>, std::vector<GuiIndex >> Build() const;

private:
  std::vector<geometry::SimpleMesh<Float32>> meshes_;

  std::vector<std::optional<Field<Float32, 3>>> normals_;

  std::vector<Field<Float32, 3>> mesh_colors_;

  std::vector<geometry::Particle<Float32>> particles_;

  std::vector<Vec3f> particles_colors_;
};


/**
 * NOTE: Use this as default.
 */
class Scene2 {
public:
  Scene2() = default;

  struct Mesh ;
  struct Particles;
  struct Wireframe;

private:
  std::optional<Light> light_;

  std::optional<Camera> camera_;

  // Internal data:
  std::vector<Mesh> meshes_;
  std::vector<Particles> mesh_particles_;
  std::vector<Particles> particles_;
  std::vector<Wireframe> wireframe_;

public:
  // MESH API:

  /**
   * @brief add a single and simple mesh to the scene, will automatically compute the nomral for
   * the mesh.
   *
   * @return Reference to the internal object
   */
  Mesh& AddMesh(const geometry::topology::TriangleList& indices,
                const types::PositionField<float, 3>& positions,
                std::optional<Field<float, 3>> opt_normals = std::nullopt);

  Mesh& AddMesh();
  
  Mesh& GetMesh(size_t id);
  
  size_t GetMeshCount() const;

  
  /**
   * @brief Add a mesh-based particle using instancing rendering.
   */
  Particles& AddMeshParticles();
  Particles& AddMeshParticles(const types::PositionField<float, 3>& positions, Float32 radius = 1.0f);

  std::vector<Particles>& GetMeshParticles();

  Particles& GetMeshParticle(size_t i);

  size_t GetMeshParticleCount() const;

  std::vector<Mesh>& GetMesh();

  // PARTICLE API

  /**
   * @brief add a group of particles into scene.
   */
  Particles& AddParticles(const types::PositionField<float, 3>& positions, Float32 radius = 1.0f);

  Particles& AddParticles();

  Particles& GetParticles(size_t id);

  size_t GetParticlesCount() const;

  std::vector<Particles>& GetParticles();

  // WIREFRAME API
  Wireframe& AddWireframe();

  Wireframe& AddWireframe(const geometry::topology::LineList& indices,
                          types::PositionField<float, 3> positions, const types::RgbField& colors);

  Wireframe& GetWireframe(size_t id);

  std::vector<Wireframe>& GetWireframe();

  size_t GetWireframeCount() const;

  /**
   * @brief: remove all the objects.
   */
  void ClearMesh();
  void ClearMeshParticles();
  void ClearParticles();
  void ClearWireframe();
  void Clear();
};

struct Scene2::Mesh{
  bool update_flag;
  size_t id;
  // mesh data.
  geometry::topology::TriangleList faces;
  types::PositionField<float, 3> vertices;
  types::RgbaField color;
  Field<float, 3> normals;
  // Reserve for future use.
  Field<float, 2> uv;
  
  bool enable_wireframe = false;
  bool use_uniform_color = false;
  bool use_double_side_lighting = false;
  int specular_shiness = 32;

  // Push constants:
  Mat4x4f model{Mat4x4f::Identity()};
  // Instancing
  size_t instance_count = 1;
  Field<float, 3> instance_position;
  Field<float, 4> instance_rotation;

  explicit Mesh(size_t id) : id(id) {}

  Mesh& operator=(const Mesh&) = default;

  Mesh(const Mesh&) = default;

  Mesh& SetIndices(const geometry::topology::TriangleList& val);

  Mesh& SetVertices(const types::PositionField<float, 3>& val);

  Mesh& SetEnableWireframe(bool enable = true);

  Mesh& SetDoubleSideLighting(bool val = true);

  Mesh& SetSpecularShiness(int shineness);

  Mesh& SetColor(types::RgbaField const& val);

  Mesh& SetUniformColor(types::Rgba const& val);

  Mesh& SetNormals(const Field<float, 3>& val);

  Mesh& SetUV(const Field<float, 2>& val);

  Mesh& SetModel(const Mat4x4f& val);

  Mesh& SetInstanceCount(size_t count);

  Mesh& SetInstancePositions(const Field<float, 3>& positions);

  Mesh& SetInstanceRotations(const Field<float, 4>& rotations);

  void MarkUpdate();
};


struct Scene2::Wireframe {
  size_t id;
  geometry::topology::LineList indices;
  types::PositionField<float, 3> positions;
  types::RgbField colors;
  bool update_flag{true};

  Wireframe(const Wireframe&) = default;

  Wireframe& operator=(const Wireframe&) = default;

  explicit Wireframe(size_t id) : id(id) {}

  Wireframe& SetIndices(const geometry::topology::LineList& ind);

  Wireframe& SetPositions(const types::PositionField<float, 3>& pos);

  Wireframe& SetColors(const types::RgbField& color);

  void MarkUpdate();
};

struct Scene2::Particles  {
  size_t id;
  bool update_flag;
  types::PositionField<float, 3> positions;
  types::RgbaField colors;
  bool use_uniform_color = true;
  Float32 radius = 1.0f;
  bool use_instance_rendering = false;
  
  Particles(const Particles&) = default;

  Particles& operator=(const Particles&) = default;

  explicit Particles(size_t id) : id(id) {}

  Particles& SetUseInstanceRendering(bool value = false);

  Particles& SetPositions(const types::PositionField<float, 3>& val);

  Particles& SetColor(acg::types::RgbaField const& color);

  Particles& SetUniformColor(types::Rgba color);

  Particles& SetRadius(Float32 r);

  void MarkUpdate();
};

}  // namespace acg::gui
