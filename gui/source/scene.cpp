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

Scene2::Mesh* Scene2::AddMesh() {
  meshes_.emplace_back(std::make_unique<Mesh>(meshes_.size()));
  return meshes_.back().get();
}

Scene2::Mesh* Scene2::AddMesh(const types::topology::TriangleList& indices,
                              const types::PositionField<float, 3>& positions,
                              std::optional<Field<float, 3>> opt_normals) {
  auto* mesh = AddMesh();
  mesh->SetIndices(indices).SetVertices(positions);
  if (opt_normals.has_value()) {
    mesh->SetNormals(opt_normals.value());
  } else {
    mesh->ComputeDefaultNormal();
  }
  return mesh;
}

Scene2::Particles* Scene2::AddMeshParticles() {
  mesh_particles_.emplace_back(std::make_unique<Particles>(mesh_particles_.size()));
  mesh_particles_.back()->SetUseInstanceRendering(true);
  return mesh_particles_.back().get();
}

Scene2::Particles* Scene2::AddMeshParticles(const types::PositionField<float, 3>& positions,
                                            Float32 radius) {
  auto *mp = AddMeshParticles();
  mp->SetPositions(positions).SetRadius(radius);
  return mp;
}

Scene2::Mesh* Scene2::GetMesh(size_t id) {
  ACG_CHECK(id < meshes_.size(), "mesh-id out of range: {} of {}", id, meshes_.size());
  return meshes_[id].get();
}

Scene2::Particles* Scene2::AddParticles() {
  particles_.emplace_back(std::make_unique<Particles>(particles_.size()));
  return particles_.back().get();
}

Scene2::Particles* Scene2::AddParticles(const types::PositionField<float, 3>& positions,
                                        Float32 radius) {
  auto* p = AddParticles();
  p->SetPositions(positions).SetRadius(radius);
  return p;
}

Scene2::Particles* Scene2::GetParticles(size_t id) {
  ACG_CHECK(id < particles_.size(), "particle-id out of range: {} of {}", id, particles_.size());
  return particles_[id].get();
}

Scene2::Wireframe* Scene2::GetWireframe(size_t id) {
  ACG_CHECK(id < wireframe_.size(), "particle-id out of range: {} of {}", id, wireframe_.size());
  return wireframe_[id].get();
}

Scene2::Wireframe* Scene2::AddWireframe() {
  wireframe_.emplace_back(std::make_unique<Wireframe>(wireframe_.size()));
  return wireframe_.back().get();
}

Scene2::Wireframe* Scene2::AddWireframe(const types::topology::LineList& indices,
                                        types::PositionField<float, 3> positions,
                                        const types::RgbField& colors) {
  auto* wireframe = AddWireframe();
  wireframe->SetIndices(indices).SetPositions(positions).SetColors(colors);
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
size_t Scene2::GetWireframeCount() const { return wireframe_.size(); }
size_t Scene2::GetMeshCount() const { return meshes_.size(); }
std::vector<std::unique_ptr<Scene2::Particles>>& Scene2::GetMeshParticles() { return mesh_particles_; }
Scene2::Particles* Scene2::GetMeshParticle(size_t i) { return mesh_particles_[i].get(); }
size_t Scene2::GetMeshParticleCount() const { return mesh_particles_.size(); }
std::vector<std::unique_ptr<Scene2::Mesh>>& Scene2::GetMesh() { return meshes_; }
std::vector<std::unique_ptr<Scene2::Particles>>& Scene2::GetParticles() {
  return particles_;
}

std::vector<std::unique_ptr<Scene2::Wireframe>>& Scene2::GetWireframe() {
  return wireframe_;
}

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
Scene2::Mesh& Scene2::Mesh::SetIndices(const types::topology::TriangleList& val) {
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

Scene2::Mesh& Scene2::Mesh::ComputeDefaultNormal() {
  acg::geometry::Normal<Float32> kern_normal(faces, vertices);
  auto normal = kern_normal.PerVertex(geometry::NormalPerVertexMode::kArea);
  SetNormals(normal);
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
  ACG_CHECK(positions.cols() == colors.cols() || colors.cols() == 1, "#Color error");
}
Scene2::Wireframe& Scene2::Wireframe::SetIndices(const types::topology::LineList& ind) {
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
