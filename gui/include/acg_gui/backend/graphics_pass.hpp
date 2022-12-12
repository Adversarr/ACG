#pragma once

#include <acg_core/math/common.hpp>
#include <vulkan/vulkan.hpp>

#include "buffer_def.hpp"
#include "vkcontext.hpp"

namespace acg::gui {
namespace details {

class GraphicsRenderPass {
public:
  class Builder;

  ~GraphicsRenderPass();

  void RecreateSwapchain();

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

  void SetBackgroundColor(acg::attr::Rgba color);

  void SetDepthStencilValue(F32 depth);

private:
  explicit GraphicsRenderPass();

  void Init();
  void Cleanup();
  void CreateCommandPool();
  void CreateRenderPass();
  void CreateDescriptorSetLayout();
  void CreateDepthResources();
  void CreateFramebuffers();
  void CreateCommandBuffers();
  void CreateDescriptorPool();
  void CleanupSwapchain();

  // Private Helpers:
  vk::Format FindDepthFormat() const;

  // Render pass and Pipeline
  bool is_inited_{false};
  bool is_render_pass_begin_{false};

  vk::ClearColorValue background_color_{std::array{0.0f, 0.0f, 0.0f, 1.0f}};
  vk::ClearDepthStencilValue depth_stencil_value_{{1.0f, 0}};

  std::vector<vk::Framebuffer> framebuffers_;
  vk::Image depth_image_;
  vk::DeviceMemory depth_image_memory_;
  vk::ImageView depth_image_view_;

  vk::RenderPass render_pass_;

  vk::DescriptorPool descriptor_pool_;
  // UBO Descriptor pool
  vk::DescriptorSetLayout descriptor_set_layout_;
  std::vector<vk::DescriptorSet> descriptor_sets_;

  vk::CommandPool command_pool_;
  std::vector<vk::CommandBuffer> command_buffers_;

public:
  inline vk::RenderPass GetRenderPass() const { return render_pass_; }

  inline vk::DescriptorPool GetDescriptorPool() const { return descriptor_pool_; }
};

class GraphicsRenderPass::Builder {
public:
  inline static std::unique_ptr<GraphicsRenderPass> Build() {
    auto retval = std::unique_ptr<GraphicsRenderPass>(new GraphicsRenderPass);
    retval->Init();
    return retval;
  }
};


inline void GraphicsRenderPass::SetBackgroundColor(acg::attr::Rgba color) {
  background_color_.setFloat32(std::array<F32, 4>{color.x(), color.y(), color.z(), color.w()});
}


inline void GraphicsRenderPass::SetDepthStencilValue(F32 depth) {
  depth_stencil_value_.setDepth(depth);
}

}  // namespace details

}  // namespace acg::gui
