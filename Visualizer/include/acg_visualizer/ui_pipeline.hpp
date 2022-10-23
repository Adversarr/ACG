#pragma once

#include "_vk.hpp"

#include <imgui.h>
#include <imgui_impl_vulkan.h>
#include <imgui_impl_glfw.h>
#include "renderer.hpp"

namespace acg::visualizer::details {


class UiPipeline {
public:
  explicit UiPipeline(Renderer& renderer);

  void Init();

  void Cleanup();
  
  void RecreateSwapchain();

  void CleanupSwapchain();

  vk::CommandBuffer& Render(ImDrawData* data);

private:
  void CreateFramebuffers();
  bool is_ui_only_{false};
  bool is_inited_{false};

  Renderer& renderer_;
  vk::CommandPool command_pool_;
  std::vector<vk::CommandBuffer> command_buffers_;
  vk::RenderPass render_pass_;
  vk::DescriptorPool imgui_pool_;
  std::vector<vk::Framebuffer> framebuffers_;
};

}