#pragma once
#include <acg_utils/result.hpp>
#include <acg_core/geometry/common.hpp>
#include <acg_core/geometry/mesh.hpp>
#include <acg_core/geometry/particlesystem.hpp>
#include <acg_core/math/access.hpp>
#include <memory>
#include <optional>
#include <vector>

#include "acg_core/math/common.hpp"
#include "backend/buffer_def.hpp"
#include "camera.hpp"
#include "light.hpp"

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

  struct Mesh {
    int id;
    // mesh data.
    geometry::topology::TriangleList faces;
    types::PositionField<float, 3> vertices;
    types::RgbaField color;
    Field<float, 3> normals;
    // Reserve for future use.
    Field<float, 2> uv;

    bool enable_wireframe;
    bool use_uniform_color;
    bool use_double_side_lighting;
    int specular_shiness;

    // Push constants:
    Mat4x4f model;
    // Instancing
    Ui32 instance_count;
    Field<float, 3> instance_position;
    Field<float, 4> instance_rotation;

    Mesh(const geometry::topology::TriangleList& face_indices,
         const types::PositionField<float, 3>& vertices);

    Mesh& SetEnableWireframe(bool enable = true) {
      enable_wireframe = enable;
      return *this;
    }

    inline Mesh& SetDoubleSideLighting(bool val = true) {
      use_double_side_lighting = val;
      return *this;
    }

    inline Mesh& SetSpecularShiness(int shineness) {
      specular_shiness = shineness;
      return *this;
    }

    inline Mesh& SetColor(types::RgbaField const& val) {
      use_uniform_color = false;
      color = val;
      return *this;
    }

    inline Mesh& SetUniformColor(types::Rgba const& val) {
      use_uniform_color = true;
      color = val;
      return *this;
    }

    inline Mesh& SetNormals(const Field<float, 3>& val) {
      normals = val;
      return *this;
    }

    inline Mesh& SetUV(const Field<float, 2>& val) {
      uv = val;
      return *this;
    }

    inline Mesh& SetModel(const Mat4x4f& val) {
      model = val;
      return *this;
    }

    inline Mesh& SetInstanceCount(Ui32 count) {
      instance_count = count;
      return *this;
    }

    inline Mesh& SetInstancePositions(const Field<float, 3>& positions) {
      instance_position = positions;
      return *this;
    }

    inline Mesh& SetInstanceRotations(const Field<float, 4>& rotations) {
      instance_rotation = rotations;
      return *this;
    }
  };

  struct Particles {
    types::PositionField<float, 3> positions;
    types::RgbaField colors;
    bool use_uniform_color;
    F32 radius;

    inline Particles& SetColor(acg::types::RgbaField const& color) {
      use_uniform_color = false;
      colors = color;
      return *this;
    }

    inline Particles& SetUniformColor(types::Rgba color) {
      use_uniform_color = true;
      colors = color;
      return *this;
    }

    inline Particles& SetRadius(F32 r) {
      radius = r;
      return *this;
    }
  };

  struct Wireframe {
    geometry::topology::LineList indices;
    types::PositionField<float, 3> positions;
    types::RgbField colors;
  };

private:
  std::optional<Light> light_;

  std::optional<Camera> camera_;

  std::vector<Mesh> meshes_;
  std::vector<Particles> particles_;
  std::vector<Wireframe> wireframe_;

  // TODO: Arrows
public:
  /**
   * @brief: add a single and simple mesh to the scene, will automatically compute the nomral for
   * the mesh.
   *
   * @return: Reference to the internal object
   */
  Mesh& AddMesh(const geometry::topology::TriangleList& indices,
                const types::PositionField<float, 3>& positions,
                std::optional<Field<float, 3>> opt_normals = std::nullopt);

  /**
   * @brief: Add a mesh-based particle using instancing rendering.
   */
  Mesh& AddMeshParticles(const types::PositionField<float, 3>& positions, F32 radius = 1.0f);

  Result<Mesh&> GetMesh(int id);

  Particles& AddParticles(const types::PositionField<float, 3>& positions, F32 radius = 1.0f);

  Result<Particles&> GetParticles(Idx id);

  Wireframe& AddWireframe(const geometry::topology::LineList& indices,
                          types::PositionField<float, 3> positions, const types::RgbField& colors);

  /**
   * @brief: remove all the objects.
   */
  void Clear();
};

}  // namespace acg::gui
