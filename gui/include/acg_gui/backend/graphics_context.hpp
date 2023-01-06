#pragma once

#include <VkBootstrap.h>

#include <optional>
#include <vulkan/vulkan.hpp>

namespace acg::gui {

class VkGraphicsContext {
  VkGraphicsContext() = default;

public:
  VkGraphicsContext(const VkGraphicsContext&) = delete;
  VkGraphicsContext(VkGraphicsContext&&) = delete;

  vk::Queue graphics_queue_, present_queue_;

  struct RenderSyncObjects {
    vk::Semaphore image_available;
    vk::Semaphore render_finished;
    vk::Fence in_flight_fence;
  };
  std::optional<uint32_t> present_family_;
  std::optional<uint32_t> graphics_family_;

  vkb::Swapchain swapchain_;
  vk::Format swapchain_format_;
  vk::Extent2D swapchain_extent_;

  std::vector<vk::Image> swapchain_images_;
  std::vector<vk::ImageView> swapchain_image_views_;

  std::vector<RenderSyncObjects> syncs_;
  size_t current_frame_{0};
  uint32_t swapchain_size_{3};
  bool draw_started_{false};
  uint32_t current_image_index_;
  static void Init();

  static void Destroy();

  static std::unique_ptr<VkGraphicsContext>& Instance();
};
}  // namespace acg::gui
