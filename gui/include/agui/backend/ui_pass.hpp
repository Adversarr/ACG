#pragma once
#include <imgui.h>

#include <vulkan/vulkan.hpp>

namespace acg {
namespace gui {
namespace details {

class UiPass {
public:
  struct Config {
    bool is_ui_only;

    void Hook();
  };

  explicit UiPass(Config config);

  ~UiPass();

  void RecreateSwapchain();

  [[nodiscard("The recorded command buffer should not be discarded.")]] vk::CommandBuffer& Render(
      ImDrawData* data);

private:
  void CreateFramebuffers();
  void DestroyFramebuffers();

  void CleanupSwapchain();
  
  void Cleanup();

  vk::CommandPool command_pool_;
  std::vector<vk::CommandBuffer> command_buffers_;
  vk::RenderPass render_pass_;
  vk::DescriptorPool descriptor_pool_;
  std::vector<vk::Framebuffer> framebuffers_;
};

}  // namespace details
}  // namespace gui
}  // namespace acg
