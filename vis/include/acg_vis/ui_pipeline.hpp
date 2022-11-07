#pragma once

#include <imgui.h>
#include <imgui_impl_glfw.h>
#include <imgui_impl_vulkan.h>

#include <vulkan/vulkan.hpp>
#include "renderer.hpp"

namespace acg::visualizer::details {

class UiPipeline {
public:
  class Builder;
  UiPipeline(const UiPipeline&) = delete;
  UiPipeline(UiPipeline&&) = delete;
  ~UiPipeline();

  void RecreateSwapchain();

  [[nodiscard("The recorded command buffer should not be discarded.")]]
  vk::CommandBuffer& Render(ImDrawData* data);

private:
  void Init();
  void CleanupSwapchain();
  void Cleanup();

  explicit UiPipeline();

  void CreateFramebuffers();
  bool is_ui_only_{false};
  bool is_inited_{false};

  VkContext& renderer_;
  vk::CommandPool command_pool_;
  std::vector<vk::CommandBuffer> command_buffers_;
  vk::RenderPass render_pass_;
  vk::DescriptorPool imgui_pool_;
  std::vector<vk::Framebuffer> framebuffers_;
};

class UiPipeline::Builder {
  bool is_ui_only_ = false; 

public:
  Builder() = default;

  inline Builder& SetIsUIOnly(bool is_ui_only) {
    is_ui_only_ = is_ui_only;
    return *this;
  }

  inline std::unique_ptr<UiPipeline> Build() const {
    auto inst = std::unique_ptr<UiPipeline>(new UiPipeline);
    inst->is_ui_only_ = is_ui_only_;
    inst->Init();
    return inst;
  }
};

}  // namespace acg::visualizer::details
