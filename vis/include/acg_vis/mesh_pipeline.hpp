#pragma once

#include "./renderer.hpp"
#include "buffer_def.hpp"
#include "camera.hpp"
#include "point_light.hpp"

namespace acg::visualizer::details {

class MeshPipeline {
public:
  class Builder;

  ~MeshPipeline();

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
  /**
   * @brief Construct a new Mesh Pipeline object
   *
   * @param renderer
   * @param is_present
   * @param polygon
   * @param cull
   * @param front
   */
  explicit MeshPipeline(VkContext& renderer, bool is_present = true,
                        vk::PolygonMode polygon = vk::PolygonMode::eFill,
                        vk::CullModeFlags cull = vk::CullModeFlagBits::eNone,
                        vk::FrontFace front = vk::FrontFace::eClockwise);

  void Init();
  void Cleanup();
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
  bool is_render_pass_begin_{false};
  const bool is_dst_present_{false};
  const vk::PolygonMode polygon_mode_{vk::PolygonMode::eFill};
  const vk::CullModeFlags cull_mode_{vk::CullModeFlagBits::eNone};
  const vk::FrontFace front_face_{vk::FrontFace::eCounterClockwise};

  vk::ClearColorValue background_color_{std::array{0.0f, 0.0f, 0.0f, 1.0f}};
  vk::ClearDepthStencilValue depth_stencil_value_{{1.0f, 0}};
  std::vector<vk::Framebuffer> framebuffers_;
  vk::Image depth_image_;
  vk::DeviceMemory depth_image_memory_;
  vk::ImageView depth_image_view_;

  vk::RenderPass render_pass_;
  vk::PipelineLayout pipeline_layout_;
  vk::Pipeline graphics_pipeline_;

  std::vector<std::unique_ptr<VkContext::BufMem>> uniform_buffers_;  // Uniform Buffer Memories.
  vk::DescriptorPool descriptor_pool_;
  vk::DescriptorSetLayout descriptor_set_layout_;
  std::vector<vk::DescriptorSet> descriptor_sets_;

  VkContext& renderer_;
  vk::CommandPool command_pool_;
  std::vector<vk::CommandBuffer> command_buffers_;

  Ubo ubo_;
};

class MeshPipeline::Builder {
private:
  bool is_dst_present_{false};
  vk::PolygonMode polygon_mode_{vk::PolygonMode::eFill};
  vk::CullModeFlags cull_mode_{vk::CullModeFlagBits::eNone};
  vk::FrontFace front_face_{vk::FrontFace::eCounterClockwise};

public:
  inline Builder& SetIsDstPresent(bool is_present) {
    is_dst_present_ = is_present;
    return *this;
  }
  inline Builder& SetCullMode(vk::CullModeFlags cull_mode) {
    cull_mode_ = cull_mode;
    return *this;
  }
  inline Builder& SetPolygonMode(vk::PolygonMode mode) {
    polygon_mode_ = mode;
    return *this;
  }
  inline Builder& SetFrontFace(vk::FrontFace face) {
    front_face_ = face;
    return *this;
  }

  inline std::unique_ptr<MeshPipeline> Build() const {
    auto retval = std::unique_ptr<MeshPipeline>(
        new MeshPipeline(get_vk_context(), is_dst_present_, polygon_mode_, cull_mode_, front_face_));
    retval->Init();
    return retval;
  }
};

}  // namespace acg::visualizer::details
