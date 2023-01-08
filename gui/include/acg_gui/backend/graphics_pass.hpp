#pragma once

#include <acg_core/math/common.hpp>
#include <vulkan/vulkan.hpp>

#include "buffer_def.hpp"
#include "vkcontext.hpp"

namespace acg::gui {
namespace details {

class GraphicsRenderPass {
public:
  struct InitConfig {
    std::vector<vk::DescriptorPoolSize> required_descriptor_sizes;
    uint32_t max_descriptor_set_count;
    bool is_present{true};
  };

  ~GraphicsRenderPass();

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

  void SetBackgroundColor(acg::types::Rgba color);

  void SetDepthStencilValue(F32 depth);

  explicit GraphicsRenderPass(InitConfig config);

  vk::CommandBuffer & GetCurrentFrameCommandBuffer();

  void RecreateSwapchain();

private:
  void Init();
  void Destroy();
  void CreateRenderPass();
  void CreateDepthResources();
  void CreateFramebuffers();
  void CreateCommandBuffers();
  void CreateDescriptorPool();
  void DestroySwapchain();

  // Private Helpers:
  static vk::Format FindDepthFormat();

  // Initialize config
  const InitConfig init_config_;
  // Render pass and Pipeline
  bool is_begin_{false};

  vk::ClearColorValue background_color_{std::array{0.0f, 0.0f, 0.0f, 1.0f}};
  vk::ClearDepthStencilValue depth_stencil_value_{{1.0f, 0}};

  std::vector<vk::Framebuffer> framebuffers_;
  vk::Image depth_image_;
  vk::DeviceMemory depth_image_memory_;
  vk::ImageView depth_image_view_;

  vk::RenderPass render_pass_;

  // UBO Descriptor pool
  vk::DescriptorPool ub_descriptor_pool_;
  
  // Command buffers
  std::vector<vk::CommandBuffer> command_buffers_;

public:
  inline vk::RenderPass GetRenderPass() const { return render_pass_; }

  inline vk::DescriptorPool GetDescriptorPool() const { return ub_descriptor_pool_; }
};

inline void GraphicsRenderPass::SetBackgroundColor(acg::types::Rgba color) {
  background_color_.setFloat32(std::array<F32, 4>{color.x(), color.y(), color.z(), color.w()});
}


inline void GraphicsRenderPass::SetDepthStencilValue(F32 depth) {
  depth_stencil_value_.setDepth(depth);
}

}  // namespace details

}  // namespace acg::gui
