#pragma once
#include <functional>
#include <imgui.h>
#include <optional>

#include <vulkan/vulkan.hpp>

namespace acg {
namespace gui {
namespace details {

class UiPass {
public:
  struct Config {
    bool is_ui_only;
    bool enable_node_editor = true;

    std::optional<std::function<void()>> call_on_init{std::nullopt};

    void Hook();
  };

  explicit UiPass(Config config);

  ~UiPass();

  void RecreateSwapchain();

  vk::CommandBuffer &Render(ImDrawData *data);

private:
  void CreateFramebuffers();

  void DestroyFramebuffers();

  void CleanupSwapchain();

  void Cleanup();

  bool enable_node_editor_ = false;

  vk::CommandPool command_pool_;
  std::vector<vk::CommandBuffer> command_buffers_;
  vk::RenderPass render_pass_;
  vk::DescriptorPool descriptor_pool_;
  std::vector<vk::Framebuffer> framebuffers_;
};

} // namespace details
} // namespace gui
} // namespace acg
