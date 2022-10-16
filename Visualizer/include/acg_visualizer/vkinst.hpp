#pragma once

#include "_vk.hpp"
#include "acg_visualizer/vkwindow.hpp"
#include <acg_utils/def.hpp>
#include <optional>

namespace acg::visualizer::details {

extern const std::vector<const char *> validation_layers;

struct SwapChainSupportDetails {
  vk::SurfaceCapabilitiesKHR capabilities;
  std::vector<vk::SurfaceFormatKHR> formats;
  std::vector<vk::PresentModeKHR> present_modes;
};

struct QueueFamilyIndices {
  std::optional<uint32_t> graphics_family{std::nullopt};
  std::optional<uint32_t> present_family{std::nullopt};
  [[nodiscard]] bool IsComplete() const { return graphics_family.has_value() && present_family.has_value(); }
};


class Visualizer {
public:
  struct VkInstBuilder {
    bool enable_validation { utils::get_build_type() == utils::BuildType::kDebug };

    // Default for triple buffering
    int swapchain_size{3};

    VkInstBuilder& SetValidation(bool enable);

    [[nodiscard]] Visualizer Build() const;
  };
  
public:
  Visualizer(Visualizer&& ) = default;
  Visualizer(const Visualizer& ) = delete;

  ~Visualizer();

  bool RunOnce();

private:
  explicit Visualizer(bool init=false);

  bool enable_validation{true};

  vk::Instance instance_;
  vk::Device device_;
  vk::PhysicalDevice physical_device_;

  vk::DebugUtilsMessengerEXT debug_messenger_;

  vk::SurfaceKHR surface_;
  vk::Queue graphics_queue_;
  vk::Queue present_queue_;

  vk::SwapchainKHR swapchain_;
  std::vector<vk::Image> swapchain_images_;
  vk::Format swapchain_image_format_;
  vk::Extent2D swapchain_extent_;
  std::vector<vk::ImageView> swapchain_image_views_;
  std::vector<vk::Framebuffer> swapchain_framebuffers_;

  vk::CommandPool command_pool_;
  vk::RenderPass render_pass_;
  QueueFamilyIndices device_related_indices_;

  const std::vector<const char *> device_extension_enabled = { 
    VK_KHR_SWAPCHAIN_EXTENSION_NAME,  
  };

  std::string title_{"ACG Visualizer (Vulkan)"};
private:
  std::unique_ptr<VkWindow> window_{nullptr};

  [[nodiscard]] std::vector<const char *> GetRequiredExtensions() const;

  vk::DebugUtilsMessengerCreateInfoEXT PopulateDebugMessengerCreateInfo();

  static bool IsValidationSupport();
  // Initializers:
  void Init();
  void CreateInstance();
  void SetupDebugMessenger();
  void CreateSurface();
  void PickPhysicalDevice();
  void CreateLogicalDevice();
  void CreateCommandPool();

  // Physical Device Checkers:
  [[nodiscard]] bool IsSuitable(const vk::PhysicalDevice& physical_device) const;
  [[nodiscard]] bool CheckDeviceExtensionSupport(const vk::PhysicalDevice& physical_device) const;
  [[nodiscard]] QueueFamilyIndices FindQueueFamilies(const vk::PhysicalDevice& physical_device) const;
  [[nodiscard]] SwapChainSupportDetails QuerySwapChainSupport(const vk::PhysicalDevice& physical_device) const;

  // Swapchain:
  void CreateSwapchain();
  void CleanupSwapchain();
  [[nodiscard]] vk::SurfaceFormatKHR ChooseSurfaceFormat(const std::vector<vk::SurfaceFormatKHR> &available_formats) const;
  [[nodiscard]] vk::PresentModeKHR ChooseSwapPresentMode(const std::vector<vk::PresentModeKHR>& available_present_modes) const;
  [[nodiscard]] vk::Extent2D ChooseSwapExtent(const vk::SurfaceCapabilitiesKHR &capabilities) const;

  void CreateImageViews();
  void CreateRenderPass();

  void CreateDescriptorLayout();
};

  


}