#include "acg_vis/scene.hpp"

#include <acg_core/geometry/common_models.hpp>
#include <vector>
#include <vulkan/vulkan.hpp>

#include "acg_vis/buffer_def.hpp"
#include "acg_vis/convent.hpp"
#include "spdlog/spdlog.h"

namespace acg::visualizer {

std::vector<vk::VertexInputBindingDescription> Vertex::GetBindingDescriptions() {
  vk::VertexInputBindingDescription binding;
  binding.setBinding(0);
  binding.setStride(sizeof(Vertex));
  binding.setInputRate(vk::VertexInputRate::eVertex);
  return {binding};
}

std::vector<vk::VertexInputAttributeDescription> Vertex::GetAttributeDescriptions() {
  vk::VertexInputAttributeDescription desc1;
  desc1.binding = 0;
  desc1.location = 0;
  desc1.format = vk::Format::eR32G32B32Sfloat;
  desc1.offset = offsetof(Vertex, position_);

  vk::VertexInputAttributeDescription desc2;
  desc2.binding = 0;
  desc2.location = 1;
  desc2.format = vk::Format::eR32G32B32Sfloat;
  desc2.offset = offsetof(Vertex, color_);

  vk::VertexInputAttributeDescription desc3;
  desc3.binding = 0;
  desc3.location = 2;
  desc3.format = vk::Format::eR32G32B32Sfloat;
  desc3.offset = offsetof(Vertex, normal_);

  return {desc1, desc2, desc3};
}

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
  return size * 3 * sizeof(details::IndexType);
}

void Scene::Reset() {
  meshes_.clear();
  normals_.clear();
  mesh_colors_.clear();
  particles_.clear();
  particles_colors_.clear();
}

std::pair<std::vector<Vertex>, std::vector<Idx>> Scene::Build() const {
  std::vector<Vertex> vertices;
  std::vector<Idx> indices;
  for (size_t i = 0; i < meshes_.size(); ++i) {
    const auto& m = meshes_[i];
    const auto& c = mesh_colors_[i];
    const auto& n = normals_[i];
    Idx n_v = m.GetVertices().cols();
    Idx i_offset = vertices.size();
    for (Idx j = 0; j < n_v; ++j) {
      Vec3f position = m.GetVertices().col(j);
      Vec3f color = (c.cols() == 1) ? c : c.col(j);
      Vec3f normal = Vec3f::Zero();
      if (n.has_value()) {
        normal = n.value().col(j);
      }
      vertices.emplace_back(Vertex(to_glm(position), to_glm(color), to_glm(normal)));
    }

    for (auto idx : m.GetFaces().colwise()) {
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
    for (auto idx : m.GetFaces().colwise()) {
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

Scene& Scene::AddMesh(geometry::Mesh<F32> mesh, std::optional<AttrVec<F32, 3>> opt_normals,
                      AttrVec<F32, 3> colors) {
  meshes_.emplace_back(std::move(mesh));
  normals_.emplace_back(std::move(opt_normals));
  mesh_colors_.emplace_back(std::move(colors));
  return *this;
}

}  // namespace acg::visualizer
