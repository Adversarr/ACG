#include "acg_vis/scene.hpp"

#include <vector>
#include <vulkan/vulkan.hpp>

#include "acg_vis/buffer_def.hpp"
#include "acg_vis/convent.hpp"

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
  desc1.offset = offsetof(Vertex, position);

  vk::VertexInputAttributeDescription desc2;
  desc2.binding = 0;
  desc2.location = 1;
  desc2.format = vk::Format::eR32G32B32Sfloat;
  desc2.offset = offsetof(Vertex, color);

  vk::VertexInputAttributeDescription desc3;
  desc3.binding = 0;
  desc3.location = 2;
  desc3.format = vk::Format::eR32G32B32Sfloat;
  desc3.offset = offsetof(Vertex, normal);

  return {desc1, desc2, desc3};
}

vk::DeviceSize Scene::GetRequiredVertexBufferSize() const {
  return vertices_.size() * sizeof(vertices_.front());
}

vk::DeviceSize Scene::GetRequiredIndexBufferSize() const {
  return indices_.size() * sizeof(indices_.front());
}

const std::vector<Vertex>& Scene::GetVertices() const { return vertices_; }

const std::vector<Idx>& Scene::GetIndices() const { return indices_; }

Scene& Scene::AddMesh(const geometry::Mesh<F32>& mesh, Vec3f color) {
  int beg = vertices_.size();
  vertices_.reserve(vertices_.size() + mesh.GetVertices().size());
  for (Vec3f vert : mesh.GetVertices().colwise()) {
    Vertex v;
    v.position = to_glm(vert);
    v.color = to_glm(color);
    v.normal = glm::vec3(0);
    vertices_.push_back(v);
  }
  for (const auto& i : mesh.GetIndices().colwise()) {
    indices_.push_back(i.x() + beg);
    indices_.push_back(i.y() + beg);
    indices_.push_back(i.z() + beg);
  }

  return *this;
}

Scene& Scene::AddMesh(const geometry::Mesh<F32>& mesh,
                      const geometry::Mesh<F32>::StateType& normal, Vec3f color) {
  int beg = vertices_.size();
  vertices_.reserve(vertices_.size() + mesh.GetVertices().size());
  int i = 0;
  for (Vec3f vert : mesh.GetVertices().colwise()) {
    Vertex v;
    v.position = to_glm(vert);
    v.color = to_glm(color);
    v.normal = to_glm(Vec3f(normal.col(i)));
    vertices_.push_back(v);
    i++;
  }
  for (const auto& i : mesh.GetIndices().colwise()) {
    indices_.push_back(i.x() + beg);
    indices_.push_back(i.y() + beg);
    indices_.push_back(i.z() + beg);
  }
  return *this;
}

void Scene::Reset() {
  vertices_.clear();
  indices_.clear();
}

}  // namespace acg::visualizer
