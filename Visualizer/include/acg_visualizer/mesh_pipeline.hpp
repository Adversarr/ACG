#pragma once
#include "./renderer.hpp"

namespace acg::visualizer::details {

class MeshPipeline {

public:
  explicit MeshPipeline(Renderer& renderer, bool is_present = true);
  
  // ~MeshPipeline();
  void Init();

  void Cleanup();

  void RecreateSwapchain();

  void SetCamera(const Camera& cam, bool all_update=false);
  
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

  void CreateCommandPool();
  void CreateRenderPass();
  void CreateDescriptorSetLayout();
  void CreateGraphicsPipeline();
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
  bool is_dst_present_{false};
  bool is_render_pass_begin_{false};
  vk::ClearColorValue background_color_
    {std::array{0.0f, 0.0f, 0.0f, 1.0f}};
  vk::ClearDepthStencilValue depth_stencil_value_ {{1.0f, 0}};
  std::vector<vk::Framebuffer> framebuffers_;
  vk::Image depth_image_;
  vk::DeviceMemory depth_image_memory_;
  vk::ImageView depth_image_view_;

  vk::RenderPass render_pass_;
  vk::PipelineLayout pipeline_layout_;
  vk::Pipeline graphics_pipeline_;

  std::vector<Renderer::BufMem> uniform_buffers_;  // Uniform Buffer Memories.
  vk::DescriptorPool descriptor_pool_;
  vk::DescriptorSetLayout descriptor_set_layout_;
  std::vector<vk::DescriptorSet> descriptor_sets_;

  Renderer& renderer_;
  vk::CommandPool command_pool_;
  std::vector<vk::CommandBuffer> command_buffers_;
};

}