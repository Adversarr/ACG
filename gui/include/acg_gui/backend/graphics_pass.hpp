#pragma once

#include <vulkan/vulkan.hpp>

#include "buffer_def.hpp"
#include "../camera.hpp"
#include "../point_light.hpp"
#include "vkcontext.hpp"

namespace acg::visualizer {

namespace details {

class GraphicsRenderPass {
public:
  class Builder;

  ~GraphicsRenderPass();

  void RecreateSwapchain();

  void SetUbo(const Camera* cam, const Light* light, bool all_update = false);

  /**
   * @brief Begin render pass, and return the command buffer in use.
   *
   * @return vk::CommandBuffer&
   */
  vk::CommandBuffer& BeginRender();

  /**
   * @brief End render pass.
   *
   * @return vk::CommandBuffer&
   */
  vk::CommandBuffer& EndRender();

private:
  explicit GraphicsRenderPass(bool is_dst_present);

  void Init();
  void Cleanup();
  void CreateCommandPool();
  void CreateRenderPass();
  void CreateDescriptorSetLayout();
  void CreateDepthResources();
  void CreateFramebuffers();
  void CreateUniformBuffers();
  void CreateCommandBuffers();
  void CreateDescriptorPool();
  void CreateDescriptorSets();
  void CleanupSwapchain();

  // Private Helpers:
  vk::Format FindDepthFormat() const;

  // Render pass and Pipeline
  bool is_inited_{false};
  bool is_render_pass_begin_{false};
  const bool is_dst_present_{false};

  vk::ClearColorValue background_color_{std::array{0.0f, 0.0f, 0.0f, 1.0f}};
  vk::ClearDepthStencilValue depth_stencil_value_{{1.0f, 0}};

  std::vector<vk::Framebuffer> framebuffers_;
  vk::Image depth_image_;
  vk::DeviceMemory depth_image_memory_;
  vk::ImageView depth_image_view_;

  vk::RenderPass render_pass_;

  std::vector<std::unique_ptr<VkContext::BufMem>> uniform_buffers_;
  vk::DescriptorPool descriptor_pool_;
  vk::DescriptorSetLayout descriptor_set_layout_;
  std::vector<vk::DescriptorSet> descriptor_sets_;

  vk::CommandPool command_pool_;
  std::vector<vk::CommandBuffer> command_buffers_;
  Ubo ubo_;
};

class GraphicsRenderPass::Builder {
  bool is_dst_present_{false};

public:
  inline Builder& SetIsDstPresent(bool is_present) {
    is_dst_present_ = is_present;
    return *this;
  }
  inline std::unique_ptr<GraphicsRenderPass> Build() const {
    auto retval = std::unique_ptr<GraphicsRenderPass>(
        new GraphicsRenderPass(is_dst_present_));
    retval->Init();
    return retval;
  }
};

}  // namespace details

}  // namespace acg::visualizer
