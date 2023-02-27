#include "agui/scene.hpp"
// libs
#define GLM_FORCE_RADIANS
#define GLM_FORCE_DEPTH_ZERO_TO_ONE
#include <acore/geometry/common_models.hpp>
#include <acore/geometry/normal.hpp>
#include <glm/glm.hpp>
#include <glm/gtc/constants.hpp>
#include <vector>

#include "acore/geometry/common_models.hpp"
#include "agui/backend/avk.hpp"
#include "agui/backend/buffer_def.hpp"
#include "agui/convent.hpp"

namespace acg::gui {

vk::DeviceSize Scene::GetRequiredVertexBufferSize() const {
  vk::DeviceSize size = 0;
  for (const auto& m : meshes_) {
    auto n_vert = m.GetVertices().cols();
    size += n_vert;
  }
  // INFO: 12 is hard coded because we use sphere_20 for particle rendering.
  size += particles_.size() * 12;
  return size * sizeof(Vertex);
}

vk::DeviceSize Scene::GetRequiredIndexBufferSize() const {
  vk::DeviceSize size = 0;
  for (const auto& m : meshes_) {
    size += m.GetFaces().cols();
  }
  // INFO: 20 is hard coded because we use sphere_20 for particle rendering.
  size += particles_.size() * 20;
  return size * 3 * sizeof(details::GuiIndex );
}

void Scene::Reset() {
  meshes_.clear();
  normals_.clear();
  mesh_colors_.clear();
  particles_.clear();
  particles_colors_.clear();
}

std::pair<std::vector<Vertex>, std::vector<GuiIndex >> Scene::Build() const {
  std::vector<Vertex> vertices;
  std::vector<GuiIndex > indices;
  for (size_t i = 0; i < meshes_.size(); ++i) {
    const auto& m = meshes_[i];
    const auto& c = mesh_colors_[i];
    const auto& n = normals_[i];
    Index n_v = m.GetVertices().cols();
    Index i_offset = vertices.size();
    if (n.has_value()) {
      ACG_CHECK(n.value().cols() == n_v, "#Normal {} != #Vertices {}", n.value().cols(), n_v);
    }
    for (Index j = 0; j < n_v; ++j) {
      Vec3f position = m.GetVertices().col(j);
      Vec3f color = (c.cols() == 1) ? c : c.col(j);
      Vec3f normal = Vec3f::Zero();
      if (n.has_value()) {
        normal = n.value().col(j);
      }
      vertices.emplace_back(Vertex(to_glm(position), to_glm(color), to_glm(normal)));
    }

    for (const auto& idx : m.GetFaces().colwise()) {
      indices.push_back(idx.x() + i_offset);
      indices.push_back(idx.y() + i_offset);
      indices.push_back(idx.z() + i_offset);
    }
  }

  for (size_t i = 0; i < particles_.size(); ++i) {
    auto m = geometry::sphere_20(Vec3f::Zero(), particles_[i].GetRadius());
    Index i_offset = vertices.size();
    for (Vec3f position : m.GetVertices().colwise()) {
      vertices.emplace_back(Vertex(to_glm(Vec3f(position + particles_[i].GetCenter())),
                                   to_glm(particles_colors_[i]), to_glm(position)));
    }
    for (const auto& idx : m.GetFaces().colwise()) {
      indices.push_back(idx.x() + i_offset);
      indices.push_back(idx.y() + i_offset);
      indices.push_back(idx.z() + i_offset);
    }
  }

  return {std::move(vertices), std::move(indices)};
}

Scene& Scene::AddParticle(const geometry::Particle<Float32>& particle, const Vec3f& color) {
  particles_.push_back(particle);
  particles_colors_.push_back(color);
  return *this;
}

Scene& Scene::AddMesh(geometry::SimpleMesh<Float32> mesh, std::optional<Field<Float32, 3>> opt_normals,
                      Field<Float32, 3> colors) {
  // normals_.emplace_back(acg::geometry::Normal<F32>{mesh.GetFaces(),
  // mesh.GetVertices()}.PerVertex());
  if (!opt_normals.has_value()) {
    const auto& face = mesh.GetFaces();
    const auto& vert = mesh.GetVertices();
    acg::geometry::Normal<Float32> normal_compute(face, vert);
    auto normal = normal_compute.PerVertex(geometry::NormalPerVertexMode::kArea);
    normals_.push_back(normal);
  } else {
    normals_.emplace_back(opt_normals);
  }
  mesh_colors_.emplace_back(std::move(colors));
  meshes_.emplace_back(std::move(mesh));
  return *this;
}

Scene2::Mesh& Scene2::AddMesh() {
  meshes_.emplace_back(meshes_.size());
  return meshes_.back();
}

Scene2::Mesh& Scene2::AddMesh(const geometry::topology::TriangleList& indices,
                              const types::PositionField<float, 3>& positions,
                              std::optional<Field<float, 3>> opt_normals) {
  decltype(auto) mesh = AddMesh();
  mesh.SetIndices(indices).SetVertices(positions);
  if (opt_normals.has_value()) {
    mesh.SetNormals(opt_normals.value());
  } else {
    acg::geometry::Normal<Float32> kern_normal(indices, positions);
    auto normal = kern_normal.PerVertex(geometry::NormalPerVertexMode::kArea);
    mesh.SetNormals(normal);
  }
  return mesh;
}

Scene2::Particles& Scene2::AddMeshParticles() {
  mesh_particles_.emplace_back(mesh_particles_.size());
  mesh_particles_.back().SetUseInstanceRendering(true);
  return mesh_particles_.back();
}

Scene2::Particles& Scene2::AddMeshParticles(const types::PositionField<float, 3>& positions,
                                            Float32 radius) {
  return AddMeshParticles().SetPositions(positions).SetRadius(radius);
}

Scene2::Mesh& Scene2::GetMesh(size_t id) {
  ACG_CHECK(id < meshes_.size(), "mesh-id out of range: {} of {}", id, meshes_.size());
  return meshes_[id];
}

Scene2::Particles& Scene2::AddParticles() {
  particles_.emplace_back(particles_.size());
  return particles_.back();
}

Scene2::Particles& Scene2::AddParticles(const types::PositionField<float, 3>& positions,
                                        Float32 radius) {
  return AddParticles().SetPositions(positions).SetRadius(radius);
}

Scene2::Particles& Scene2::GetParticles(size_t id) {
  ACG_CHECK(id < particles_.size(), "particle-id out of range: {} of {}", id, particles_.size());
  return particles_[id];
}

Scene2::Wireframe& Scene2::GetWireframe(size_t id) {
  ACG_CHECK(id < wireframe_.size(), "particle-id out of range: {} of {}", id, wireframe_.size());
  return wireframe_[id];
}

Scene2::Wireframe& Scene2::AddWireframe() {
  wireframe_.emplace_back(wireframe_.size());
  return wireframe_.back();
}

Scene2::Wireframe& Scene2::AddWireframe(const geometry::topology::LineList& indices,
                                        types::PositionField<float, 3> positions,
                                        const types::RgbField& colors) {
  auto&& wireframe = AddWireframe().SetIndices(indices).SetPositions(positions).SetColors(colors);
  return wireframe;
}

void Scene2::Clear() {
  ClearMesh();
  ClearParticles();
  ClearWireframe();
}
void Scene2::ClearMesh() { meshes_.clear(); }
void Scene2::ClearMeshParticles() { meshes_.clear(); }
void Scene2::ClearParticles() { particles_.clear(); }
void Scene2::ClearWireframe() { wireframe_.clear(); }
size_t Scene2::GetParticlesCount() const { return particles_.size(); }
std::vector<Scene2::Particles>& Scene2::GetParticles() { return particles_; }
std::vector<Scene2::Wireframe>& Scene2::GetWireframe() { return wireframe_; }
size_t Scene2::GetWireframeCount() const { return wireframe_.size(); }
size_t Scene2::GetMeshCount() const { return meshes_.size(); }
std::vector<Scene2::Particles>& Scene2::GetMeshParticles() { return mesh_particles_; }
Scene2::Particles& Scene2::GetMeshParticle(size_t i) { return mesh_particles_[i]; }
size_t Scene2::GetMeshParticleCount() const { return mesh_particles_.size(); }
std::vector<Scene2::Mesh>& Scene2::GetMesh() { return meshes_; }

void Scene2::Mesh::MarkUpdate() {
  update_flag = true;
  auto vert_count = vertices.cols();
  if (!use_uniform_color) {
    ACG_CHECK(vert_count == color.cols(), "#Vertex != #Color");
  } else {
    ACG_CHECK(color.cols() == 1, "Use Uniform Color => #Color == 1");
  }

  ACG_CHECK(normals.cols() == vertices.cols(), "#Normal != #Vertex");
}
Scene2::Mesh& Scene2::Mesh::SetIndices(const geometry::topology::TriangleList& val) {
  faces = val;
  return *this;
}
Scene2::Mesh& Scene2::Mesh::SetDoubleSideLighting(bool val) {
  use_double_side_lighting = val;
  return *this;
}
Scene2::Mesh& Scene2::Mesh::SetSpecularShiness(int shineness) {
  specular_shiness = shineness;
  return *this;
}
Scene2::Mesh& Scene2::Mesh::SetVertices(const types::PositionField<float, 3>& val) {
  vertices = val;
  return *this;
}
Scene2::Mesh& Scene2::Mesh::SetUniformColor(const types::Rgba& val) {
  use_uniform_color = true;
  color.resize(4, 1);
  color.col(0) = val;
  return *this;
}
Scene2::Mesh& Scene2::Mesh::SetNormals(const Field<float, 3>& val) {
  normals = val;
  return *this;
}
Scene2::Mesh& Scene2::Mesh::SetUV(const Field<float, 2>& val) {
  uv = val;
  return *this;
}
Scene2::Mesh& Scene2::Mesh::SetModel(const Mat4x4f& val) {
  model = val;
  return *this;
}
Scene2::Mesh& Scene2::Mesh::SetColor(const types::RgbaField& val) {
  use_uniform_color = false;
  color = val;
  return *this;
}
Scene2::Mesh& Scene2::Mesh::SetInstanceCount(size_t count) {
  instance_count = count;
  return *this;
}
Scene2::Mesh& Scene2::Mesh::SetEnableWireframe(bool enable) {
  enable_wireframe = enable;
  return *this;
}
Scene2::Mesh& Scene2::Mesh::SetInstancePositions(const Field<float, 3>& positions) {
  instance_position = positions;
  return *this;
}
Scene2::Mesh& Scene2::Mesh::SetInstanceRotations(const Field<float, 4>& rotations) {
  instance_rotation = rotations;
  return *this;
}

void Scene2::Particles::MarkUpdate() {
  update_flag = true;
  if (!use_uniform_color) {
    ACG_CHECK(positions.cols() == colors.cols(), "#Vertex != #Color");
  } else {
    ACG_CHECK(colors.cols() == 1, "Use Uniform Color => #Color == 1");
  }
  ACG_CHECK(radius > 0, "Particle Radius should be greater than zero.");
}
Scene2::Particles& Scene2::Particles::SetUseInstanceRendering(bool value) {
  use_instance_rendering = value;
  return *this;
}
Scene2::Particles& Scene2::Particles::SetPositions(const types::PositionField<float, 3>& val) {
  positions = val;
  return *this;
}
Scene2::Particles& Scene2::Particles::SetColor(const types::RgbaField& color) {
  use_uniform_color = false;
  colors = color;
  return *this;
}
Scene2::Particles& Scene2::Particles::SetUniformColor(types::Rgba color) {
  use_uniform_color = true;
  colors.resize(4, 1);
  colors.col(0) = color;
  return *this;
}
Scene2::Particles& Scene2::Particles::SetRadius(Float32 r) {
  radius = r;
  return *this;
}

void Scene2::Wireframe::MarkUpdate() {
  update_flag = true;
  ACG_CHECK(positions.cols() == colors.cols() || colors.cols() == 1,
      "#Color error");
  // TODO: add debug check.
}
Scene2::Wireframe& Scene2::Wireframe::SetIndices(const geometry::topology::LineList& ind) {
  indices = ind;
  return *this;
}
Scene2::Wireframe& Scene2::Wireframe::SetPositions(const types::PositionField<float, 3>& pos) {
  positions = pos;
  return *this;
}
Scene2::Wireframe& Scene2::Wireframe::SetColors(const types::RgbField& color) {
  colors = color;
  return *this;
}
}  // namespace acg::gui
