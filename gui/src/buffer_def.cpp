#include "acg_gui/backend/buffer_def.hpp"
namespace acg::gui {

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

std::vector<vk::VertexInputBindingDescription> Point::GetBindingDescriptions() {
  vk::VertexInputBindingDescription binding_description;
  binding_description.setBinding(0)
      .setInputRate(vk::VertexInputRate::eVertex)
      .setStride(sizeof(Point));

  return {binding_description};
}

std::vector<vk::VertexInputAttributeDescription> Point::GetAttributeDescriptions() {
  vk::VertexInputAttributeDescription desc1;
  vk::VertexInputAttributeDescription desc2;
  desc1.setBinding(0)
      .setLocation(0)
      .setFormat(vk::Format::eR32G32B32Sfloat)
      .setOffset(offsetof(Point, position));

  desc2.setBinding(0)
      .setLocation(1)
      .setFormat(vk::Format::eR32G32B32Sfloat)
      .setOffset(offsetof(Point, color));

  return {desc1, desc2};
}

}  // namespace acg::gui
