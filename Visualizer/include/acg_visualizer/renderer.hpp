#pragma once

#include <acg_utils/def.hpp>
#include <memory>
#include <optional>

#include "_vk.hpp"
#include "acg_visualizer/camera.hpp"
#include "acg_visualizer/window.hpp"

namespace acg::visualizer::details {

extern const std::vector<const char*> validation_layers;

struct SwapChainSupportDetails {
  vk::SurfaceCapabilitiesKHR capabilities;
  std::vector<vk::SurfaceFormatKHR> formats;
  std::vector<vk::PresentModeKHR> present_modes;
};

struct QueueFamilyIndices {
  std::optional<uint32_t> graphics_family{std::nullopt};
  std::optional<uint32_t> present_family{std::nullopt};
  [[nodiscard]] bool IsComplete() const {
    return graphics_family.has_value() && present_family.has_value();
  }
};
struct RenderSyncObjects {
  vk::Semaphore image_available;
  vk::Semaphore render_finished;
  vk::Fence in_flight_fence;
};

// TODO: Add Imgui Support.

class Renderer {
public:
  struct Builder {
    bool enable_validation{utils::get_build_type() == utils::BuildType::kDebug};

    uint32_t swapchain_size{3 /*Default for triple buffering.*/};

    Builder& SetValidation(bool enable);

    Builder& SetSwapchainSize(uint32_t size);

    [[nodiscard]] std::unique_ptr<Renderer> Build() const;
  };

  explicit Renderer(bool init = false);

public:
  Renderer(Renderer&&) = delete;
  Renderer(const Renderer&) = delete;

  ~Renderer();

  bool RunOnce();

  struct BufferWithMemory {
    vk::Buffer buffer;
    vk::DeviceMemory memory;
  };

private:
  bool is_inited_{false};
  bool enable_validation{true};

  std::unique_ptr<VkWindow> window_{nullptr};
  vk::Instance instance_;
  vk::Device device_;
  vk::PhysicalDevice physical_device_;

  vk::DebugUtilsMessengerEXT debug_messenger_;

  vk::SurfaceKHR surface_;
  vk::Queue graphics_queue_;
  vk::Queue present_queue_;

  vk::CommandPool command_pool_;
  std::vector<vk::CommandBuffer> command_buffers_;
  // Swapchain
  vk::SwapchainKHR swapchain_;
  vk::Format swapchain_image_format_;
  vk::Extent2D swapchain_extent_;
  std::vector<vk::Image> swapchain_images_;
  std::vector<vk::ImageView> swapchain_image_views_;
  std::vector<vk::Framebuffer> swapchain_framebuffers_;
  std::vector<RenderSyncObjects> semaphores_;
  std::vector<BufferWithMemory> uniform_buffers_;  // Uniform Buffer Memories.
  size_t current_frame_{0};
  uint32_t swapchain_size_{3};
  bool draw_started_{false};
  uint32_t current_image_index_;
  vk::ClearColorValue background_color_{std::array{0.0f, 0.0f, 0.0f, 1.0f}};
  vk::Image depth_image_;
  vk::DeviceMemory depth_image_memory_;
  vk::ImageView depth_image_view_;

  // TODO: buffer and buffer memories for ubo, vertices, and indices are created in other class.

  // Render pass and Pipeline
  vk::RenderPass render_pass_;
  vk::DescriptorSetLayout ubo_layout_;
  std::vector<vk::DescriptorSet> ubo_descriptor_sets_;
  vk::DescriptorPool descriptor_pool_;
  vk::PipelineLayout pipeline_layout_;
  vk::Pipeline graphics_pipeline_;

  QueueFamilyIndices device_related_indices_;

  const std::vector<const char*> device_extension_enabled = {
      VK_KHR_SWAPCHAIN_EXTENSION_NAME,
  };

  std::string title_{"ACG Visualizer (Vulkan)"};

private:
  [[nodiscard]] std::vector<const char*> GetRequiredExtensions() const;

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
  void CreateDepthResources();
  vk::Format FindSupportedFormat(const std::vector<vk::Format>& candidates, vk::ImageTiling tiling,
                                 vk::FormatFeatureFlags features);
  vk::Format FindDepthFormat();
  bool HasStencilComponent(vk::Format format);

  // Physical Device Checkers:
  [[nodiscard]] bool IsSuitable(const vk::PhysicalDevice& physical_device) const;
  [[nodiscard]] bool CheckDeviceExtensionSupport(const vk::PhysicalDevice& physical_device) const;
  [[nodiscard]] QueueFamilyIndices FindQueueFamilies(
      const vk::PhysicalDevice& physical_device) const;
  [[nodiscard]] SwapChainSupportDetails QuerySwapChainSupport(
      const vk::PhysicalDevice& physical_device) const;

  [[nodiscard]] vk::SurfaceFormatKHR ChooseSwapSurfaceFormat(
      const std::vector<vk::SurfaceFormatKHR>& formats) const;
  [[nodiscard]] vk::PresentModeKHR ChooseSwapPresentMode(
      const std::vector<vk::PresentModeKHR>& modes) const;
  [[nodiscard]] vk::Extent2D ChooseSwapExtent(vk::SurfaceCapabilitiesKHR& capacities) const;

  void CreateSwapchain();
  void CreateImageViews();
  void CreateGraphicsPipeline();
  void CreateRenderPass();
  void CreateFramebuffers();
  void CreateCommandBuffers();

  void CreateSyncObjects();
  void RecreateSwapchain();
  void CleanupSwapchain();
  void CreateDescriptorPool();
  void CreateUboDescriptorSets();
  void CreateUboDescriptorLayout();
  void CreateUniformBuffers();
  // Memory Management
  Renderer::BufferWithMemory CreateBuffer(vk::DeviceSize size, vk::BufferUsageFlags usage,
                                          vk::MemoryPropertyFlags properties);
  uint32_t FindMemoryType(uint32_t type_filter, vk::MemoryPropertyFlags properties);

  [[nodiscard]] vk::ShaderModule CreateShaderModule(const std::vector<char>& code) const;

public:
  [[nodiscard]] const vk::Instance& GetInstance() const;
  [[nodiscard]] const vk::Device& GetDevice() const;
  [[nodiscard]] const vk::PhysicalDevice& GetPhysicalDevice() const;
  [[nodiscard]] const vk::SurfaceKHR& GetSurface() const;
  [[nodiscard]] const vk::Queue& GetGraphicsQueue() const;
  [[nodiscard]] const vk::Queue& GetPresentQueue() const;
  [[nodiscard]] const vk::CommandPool& GetCommandPool() const;

  vk::CommandBuffer BeginDraw();
  void BeginRenderPass();
  void SetCamera(const Camera& camera);
  void EndRenderPass();
  void EndDrawSubmit();

  vk::ImageView CreateImageView(vk::Image image, vk::Format format,
                                vk::ImageAspectFlags aspectFlags);
  std::pair<vk::Image, vk::DeviceMemory> CreateImage(uint32_t width, uint32_t height,
                                                     vk::Format format, vk::ImageTiling tiling,
                                                     vk::ImageUsageFlags usage,
                                                     vk::MemoryPropertyFlags properties);

  vk::CommandBuffer BeginSingleTimeCommands();
  void TransitionImageLayout(vk::Image image, vk::Format format, vk::ImageLayout oldLayout,
                             vk::ImageLayout newLayout);
  void EndSingleTimeCommands(vk::CommandBuffer buffer);
  void CopyBuffer(vk::Buffer src, vk::Buffer dst, vk::DeviceSize size);
};

}  // namespace acg::visualizer::details
