#include "acg_visualizer/scene.hpp"

namespace acg::visualizer::details {

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
  
  return {desc1, desc2};
}
}