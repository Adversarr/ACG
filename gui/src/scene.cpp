#include "acg_gui/scene.hpp"
// libs
#define GLM_FORCE_RADIANS
#define GLM_FORCE_DEPTH_ZERO_TO_ONE
#include <acg_core/geometry/common_models.hpp>
#include <acg_core/geometry/normal.hpp>
#include <glm/glm.hpp>
#include <glm/gtc/constants.hpp>
#include <vector>

#include "acg_core/geometry/common_models.hpp"
#include "acg_gui/backend/avk.hpp"
#include "acg_gui/backend/buffer_def.hpp"
#include "acg_gui/convent.hpp"

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
  return size * 3 * sizeof(details::GuiIdx);
}

void Scene::Reset() {
  meshes_.clear();
  normals_.clear();
  mesh_colors_.clear();
  particles_.clear();
  particles_colors_.clear();
}

std::pair<std::vector<Vertex>, std::vector<GuiIdx>> Scene::Build() const {
  std::vector<Vertex> vertices;
  std::vector<GuiIdx> indices;
  for (size_t i = 0; i < meshes_.size(); ++i) {
    const auto& m = meshes_[i];
    const auto& c = mesh_colors_[i];
    const auto& n = normals_[i];
    Idx n_v = m.GetVertices().cols();
    Idx i_offset = vertices.size();
    if (n.has_value()) {
      ACG_CHECK(n.value().cols() == n_v, "#Normal {} != #Vertices {}", n.value().cols(), n_v);
    }
    for (Idx j = 0; j < n_v; ++j) {
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
    Idx i_offset = vertices.size();
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

Scene& Scene::AddParticle(const geometry::Particle<F32>& particle, const Vec3f& color) {
  particles_.push_back(particle);
  particles_colors_.push_back(color);
  return *this;
}

Scene& Scene::AddMesh(geometry::SimpleMesh<F32> mesh, std::optional<Field<F32, 3>> opt_normals,
                      Field<F32, 3> colors) {
  // normals_.emplace_back(acg::geometry::Normal<F32>{mesh.GetFaces(),
  // mesh.GetVertices()}.PerVertex());
  if (!opt_normals.has_value()) {
    const auto& face = mesh.GetFaces();
    const auto& vert = mesh.GetVertices();
    acg::geometry::Normal<F32> normal_compute(face, vert);
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
    acg::geometry::Normal<F32> kern_normal(indices, positions);
    auto normal = kern_normal.PerVertex(geometry::NormalPerVertexMode::kArea);
    mesh.SetNormals(normal);
  }
  return mesh;
}

Scene2::Particles& Scene2::AddMeshParticles() {
  mesh_particles_.emplace_back(mesh_particles_.size());
  return mesh_particles_.back();
}

Scene2::Particles& Scene2::AddMeshParticles(const types::PositionField<float, 3>& positions,
                                            F32 radius) {
  return AddMeshParticles().SetPositions(positions).SetRadius(radius);
}

Scene2::Mesh& Scene2::GetMesh(size_t id) {
  ACG_CHECK(id < meshes_.size(), "mesh-id out of range: {} of {}", id, meshes_.size());
  return meshes_[id];
}

Scene2::Particles& Scene2::AddParticles() {
  particles_.emplace_back(meshes_.size());
  return particles_.back();
}

Scene2::Particles& Scene2::AddParticles(const types::PositionField<float, 3>& positions,
                                        F32 radius) {
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

}  // namespace acg::gui
