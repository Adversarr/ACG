#pragma once
#include "agui/backend/avk.hpp"
#include "agui/backend/context.hpp"
#include "agui/backend/graphics_pass.hpp"
#include "agui/camera.hpp"
#include "agui/light.hpp"
namespace acg::gui::details {

struct WireframePoint {
  glm::vec3 position;
  glm::vec3 color;

  inline WireframePoint(const glm::vec3& position, const glm::vec3& color)
      : position(position), color(color) {}
  
  inline WireframePoint() = default;

  static std::vector<vk::VertexInputBindingDescription> GetBindingDescriptions();

  static std::vector<vk::VertexInputAttributeDescription> GetAttributeDescriptions();
};

class WireframePipeline {
public:
  struct Config {};

  void SetCamera(const Camera& cam);

  void CleanUp();

  void CreateDescriptorSetLayout();

  void CreateDescriptorSets(const GraphicsRenderPass& pass);

  void CreateUniformBuffers();

  void CreateGraphicsPipeline(const GraphicsRenderPass& graphics_pass);

  void BeginPipeline(vk::CommandBuffer& current_command_buffer);

  void EndPipeline(vk::CommandBuffer& current_command_buffer);

  void UpdateUbo(bool fast = false);

  ~WireframePipeline();

  explicit WireframePipeline(const GraphicsRenderPass& render_pass, Config config);

private:
  std::vector<vk::DescriptorSet> ubo_descriptor_sets_;
  vk::DescriptorSetLayout descriptor_set_layout_;
  vk::PipelineLayout pipeline_layout_;
  vk::Pipeline pipeline_;
  std::vector<BufferWithMemory> uniform_buffers_;
  Ubo ubo_;
};
}  // namespace acg::gui::details
