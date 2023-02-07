#pragma once

#include <optional>

#include "acg_utils/result.hpp"
#include "context.hpp"

namespace acg::gui {

class VkGraphicsContext {
public:
  struct Hooker {
    bool is_present{false};

    void Hook();
  };

private:
  explicit VkGraphicsContext(Hooker config);

  static vk::SurfaceFormatKHR ChooseSwapSurfaceFormat(
      const std::vector<vk::SurfaceFormatKHR>& formats);
  static vk::PresentModeKHR ChooseSwapPresentMode(const std::vector<vk::PresentModeKHR>& modes,
                                                  bool verbose = false);

  static vk::Extent2D ChooseSwapExtent(vk::SurfaceCapabilitiesKHR& capacities);

  void CreateCommandPool();
  void CreateImageViews();
  void CreateSyncObjects();
  void CreateSwapchain(bool verbose = false);

  void CleanupSwapchain();

public:
  static vk::ImageView CreateImageView(vk::Image image, vk::Format format,
                                       vk::ImageAspectFlags aspectFlags);
  void RecreateSwapchain(bool verbose = false);
  ~VkGraphicsContext();
  VkGraphicsContext(const VkGraphicsContext&) = delete;
  VkGraphicsContext(VkGraphicsContext&&) = delete;

  struct RenderSyncObjects {
    vk::Semaphore image_available;
    vk::Semaphore render_finished;
    vk::Fence in_flight_fence;
  };
  
  vk::Semaphore data_available;
  Hooker config_;

  // Swapchain
  vk::SwapchainKHR swapchain_;
  vk::Format swapchain_image_format_;
  vk::Extent2D swapchain_extent_;
  vk::CommandPool graphics_command_pool_;

  std::vector<vk::Image> swapchain_images_;
  std::vector<vk::ImageView> swapchain_image_views_;

  std::vector<RenderSyncObjects> syncs_;
  size_t current_frame_{0};
  uint32_t swapchain_size_{3};
  bool draw_started_{false};
  uint32_t current_image_index_{0};

  static VkGraphicsContext& Instance();

  bool BeginDraw();

  bool EndDraw(std::vector<vk::CommandBuffer> command_buffers);

  [[nodiscard]] vk::CommandBuffer BeginSingleTimeCommand() const;
  
  void EndSingleTimeCommand(vk::CommandBuffer buffer) const;
  
  void TransitionImageLayout(vk::Image image, vk::Format format, vk::ImageLayout oldLayout,
                             vk::ImageLayout newLayout) const;
  
  void CopyBufferToBuffer(vk::Buffer src, vk::Buffer dst, vk::DeviceSize size) const;

  void CopyHostToBuffer(const void* mem_data, BufferWithMemory& buffer_with_memory,
                        size_t size) const;
};

}  // namespace acg::gui
