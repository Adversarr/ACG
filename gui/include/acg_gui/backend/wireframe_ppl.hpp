#pragma once
#include "acg_gui/backend/avk.hpp"
#include "acg_gui/backend/graphics_pass.hpp"
#include "acg_gui/camera.hpp"
#include "acg_gui/light.hpp"

namespace acg::gui::details {

class WireframePipeline {
public:
  class Builder;

  void SetCamera(const Camera& cam);

  void CleanUp();

  void CreateDescriptorSetLayout();

  void CreateDescriptorSets(const GraphicsRenderPass& pass);

  void CreateUniformBuffers();

  void CreateGraphicsPipeline(const GraphicsRenderPass& graphics_pass);

  void BeginPipeline(vk::CommandBuffer& current_command_buffer);

  void EndPipeline(vk::CommandBuffer& current_command_buffer);

  void UpdateUbo(bool fast = false);

  void Recreate(const GraphicsRenderPass& graphics_pass);

  ~WireframePipeline();

private:
  WireframePipeline() = default;
  void Init(const GraphicsRenderPass& graphics_pass);
  bool is_inited_{false};
  std::vector<vk::DescriptorSet> ubo_descriptor_sets_;

  vk::DescriptorSetLayout descriptor_set_layout_;
  vk::PipelineLayout pipeline_layout_;
  vk::Pipeline pipeline_;
  std::vector<std::unique_ptr<VkContext::BufMem>> uniform_buffers_;
  Ubo ubo_;
};

class WireframePipeline::Builder {
public:
  std::unique_ptr<WireframePipeline> Build(const GraphicsRenderPass& render_pass) const;
};

}  // namespace acg::gui::details
