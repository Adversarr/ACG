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
  bool IsComplete() const { return graphics_family.has_value() && present_family.has_value(); }
};

struct RenderSyncObjects {
  vk::Semaphore image_available;
  vk::Semaphore render_finished;
  vk::Fence in_flight_fence;
};

class Renderer {
public:
  class BufMem {
  public:
    BufMem(Renderer& renderer, vk::Buffer buffer, vk::DeviceMemory memory,
      vk::DeviceSize size);
    BufMem(const BufMem&) = default;  // disable copy
    BufMem(BufMem&&) = default;       // enable  move

    vk::Buffer GetBuffer();

    vk::DeviceSize GetSize() const;
    vk::DeviceMemory GetMemory();

    void Release();

  private:
    vk::Buffer buffer_{VK_NULL_HANDLE};
    vk::DeviceMemory memory_{VK_NULL_HANDLE};
    vk::DeviceSize size_;
    Renderer& renderer_;
  };

  struct Builder {
    bool enable_validation{utils::get_build_type() == utils::BuildType::kDebug};

    uint32_t swapchain_size{3 /*Default for triple buffering.*/};

    Builder& SetValidation(bool enable);

    Builder& SetSwapchainSize(uint32_t size);

    std::unique_ptr<Renderer> Build() const;
  };


  Renderer(Renderer&&) = delete;

  Renderer(const Renderer&) = delete;

  ~Renderer();

  /**
   * @brief Get the Instance object
   *
   * @return const vk::Instance&
   */
  const vk::Instance& GetInstance() const;

  /**
   * @brief Get the Device object
   *
   * @return const vk::Device&
   */
  const vk::Device& GetDevice() const;

  /**
   * @brief Get the Physical Device object
   *
   * @return const vk::PhysicalDevice&
   */
  const vk::PhysicalDevice& GetPhysicalDevice() const;

  /**
   * @brief Get the Surface object
   *
   * @return const vk::SurfaceKHR&
   */
  const vk::SurfaceKHR& GetSurface() const;

  /**
   * @brief Get the Graphics Queue object
   *
   * @return const vk::Queue&
   */
  const vk::Queue& GetGraphicsQueue() const;

  /**
   * @brief Get the Present Queue object
   *
   * @return const vk::Queue&
   */
  const vk::Queue& GetPresentQueue() const;

  /**
   * @brief Get the Swapchain Imageviews object
   *
   * @return const std::vector<vk::ImageView>
   */
  std::vector<vk::ImageView> GetSwapchainImageviews() const;

  /**
   * @brief Get the Window object
   *
   * @return const std::unique_ptr<Window>&
   */
  const std::unique_ptr<Window>& GetWindow() const;

  /**
   * @brief Get the Swapchain Size object
   *
   * @return uint32_t
   */
  uint32_t GetSwapchainSize() const;

  /**
   * @brief Get the Acquired Image Index object
   *
   * @return uint32_t
   */
  uint32_t GetAcquiredImageIndex() const;

  /**
   * @brief Get the Graphics Family object
   *
   * @return uint32_t
   */
  uint32_t GetGraphicsFamily() const;

  /**
   * @brief Get the Present Family object
   *
   * @return uint32_t
   */
  uint32_t GetPresentFamily() const;

  /**
   * @brief Get the Swapchain Extent object
   *
   * @return vk::Extent2D
   */
  vk::Extent2D GetSwapchainExtent() const;

  /**
   * @brief Get the Swapchain Image Format object
   *
   * @return vk::Format
   */
  vk::Format GetSwapchainImageFormat() const;

  /**
   * @brief Get current frame in rendering.
   * @return
   */
  size_t GetCurrentIndex() const;

  /**
   * @brief Create a Image View
   *
   * @param image
   * @param format
   * @param aspectFlags
   * @return vk::ImageView
   */
  vk::ImageView CreateImageView(vk::Image image, vk::Format format,
                                vk::ImageAspectFlags aspectFlags);

  /**
   * @brief Create a Image with memory.
   *
   * @param width
   * @param height
   * @param format
   * @param tiling
   * @param usage
   * @param properties
   * @return std::pair<vk::Image, vk::DeviceMemory>
   */
  std::pair<vk::Image, vk::DeviceMemory> CreateImage(uint32_t width, uint32_t height,
                                                     vk::Format format, vk::ImageTiling tiling,
                                                     vk::ImageUsageFlags usage,
                                                     vk::MemoryPropertyFlags properties);

  /**
   * @brief Find the proper memory type for given type and properties.
   *
   * @param type_filter
   * @param properties
   * @return uint32_t
   */
  uint32_t FindMemoryType(uint32_t type_filter, vk::MemoryPropertyFlags properties);

  /**
   * @brief Find supported format from candidates.
   *
   * @param candidates
   * @param tiling
   * @param features
   * @return vk::Format
   */
  vk::Format FindSupportedFormat(const std::vector<vk::Format>& candidates, vk::ImageTiling tiling,
                                 vk::FormatFeatureFlags features);

  /** Check given format whether has Stencil Component.
   * @brief
   *
   * @param format
   * @return true
   * @return false
   */
  static bool HasStencilComponent(vk::Format format);

  /**
   * @brief Create a Buffer with Memory Allocated.
   *
   * @param size
   * @param usage
   * @param properties
   * @return Renderer::BufMem
   */
  Renderer::BufMem CreateBuffer(vk::DeviceSize size, vk::BufferUsageFlags usage,
                                vk::MemoryPropertyFlags properties);
  
  void DestroyBufmem(BufMem& bufmem);

  /**
   * @brief Copy from Host To Device
   *
   * @param mem_data
   * @param buffer_with_memory
   * @param size
   */
  void CopyHostToBuffer(const void* mem_data, BufMem buffer_with_memory, size_t size);

  /**
   * @brief Try to begin draw.
   *
   * @return true draw begun
   * @return false if need to recreate swapchain.
   */
  bool BeginDraw();

  /**
   * @brief Submit command buffers to graphics queue and present
   *
   * @param command_buffers
   */
  bool EndDrawSubmitPresent(std::vector<vk::CommandBuffer> command_buffers);

  /**
   * @brief Start a single time command.
   *
   * @param command_pool (optional) use the desired command pool
   * @return vk::CommandBuffer
   */
  vk::CommandBuffer BeginSingleTimeCommand(vk::CommandPool command_pool = VK_NULL_HANDLE);

  /**
   * @brief End single time command and submit to graphics queue.
   *
   * @param buffer
   */
  void EndSingleTimeCommand(vk::CommandBuffer buffer,
                            vk::CommandPool command_pool = VK_NULL_HANDLE);
  /**
   * @brief Transit image layout form old to new.
   *
   * @param image
   * @param format
   * @param oldLayout
   * @param newLayout
   */
  void TransitionImageLayout(vk::Image image, vk::Format format, vk::ImageLayout oldLayout,
                             vk::ImageLayout newLayout);

  /**
   * @brief Copy from one buffer to another.
   *
   * @param src
   * @param dst
   * @param size
   */
  void CopyBuffer(vk::Buffer src, vk::Buffer dst, vk::DeviceSize size);

  void RecreateSwapchain();

private:

  /**
   * @brief DO NOT CALL This INITIALIZER DIRECTLY! Construct a new Renderer object
   *
   */
  Renderer() = default;

  // Initializers:
  void Init();
  void CreateInstance();
  void SetupDebugMessenger();
  void CreateSurface();
  void PickPhysicalDevice();
  void CreateLogicalDevice();
  void CreateCommandPool();
  void CreateSwapchain();
  void CreateImageViews();
  void CreateSyncObjects();

  void CleanupSwapchain();

  // Helpers:
  std::vector<const char*> GetRequiredExtensions() const;
  static vk::DebugUtilsMessengerCreateInfoEXT PopulateDebugMessengerCreateInfo();
  static bool IsValidationSupport();
  //  Physical Device Checkers:
  bool IsSuitable(const vk::PhysicalDevice& physical_device) const;

  bool CheckDeviceExtensionSupport(const vk::PhysicalDevice& physical_device) const;

  QueueFamilyIndices FindQueueFamilies(const vk::PhysicalDevice& physical_device) const;

  SwapChainSupportDetails QuerySwapChainSupport(const vk::PhysicalDevice& physical_device) const;
  //  Swapchain Chooser
  static vk::SurfaceFormatKHR ChooseSwapSurfaceFormat(
      const std::vector<vk::SurfaceFormatKHR>& formats);
  static vk::PresentModeKHR ChooseSwapPresentMode(const std::vector<vk::PresentModeKHR>& modes);
  vk::Extent2D ChooseSwapExtent(vk::SurfaceCapabilitiesKHR& capacities) const;

  // Pre-build Options
  bool is_inited_{false};
  bool enable_validation_{true};
  std::string title_{"ACG Visualizer (Vulkan)"};

  // Basic Context
  std::unique_ptr<Window> window_{nullptr};
  vk::Instance instance_;
  vk::Device device_;
  vk::PhysicalDevice physical_device_;
  vk::DebugUtilsMessengerEXT debug_messenger_;
  vk::SurfaceKHR surface_;
  vk::Queue graphics_queue_;
  vk::Queue present_queue_;
  QueueFamilyIndices device_related_indices_;
  // Assists.
  vk::CommandPool command_pool_;
  std::vector<BufMem> allocated_buffers_;

  // Swapchain
  vk::SwapchainKHR swapchain_;
  vk::Format swapchain_image_format_;
  vk::Extent2D swapchain_extent_;
  std::vector<vk::Image> swapchain_images_;
  std::vector<vk::ImageView> swapchain_image_views_;
  // Render Loop
  std::vector<RenderSyncObjects> semaphores_;
  size_t current_frame_{0};
  uint32_t swapchain_size_{3};
  bool draw_started_{false};
  uint32_t current_image_index_;

  const std::vector<const char*> device_extension_enabled_ = {
      VK_KHR_SWAPCHAIN_EXTENSION_NAME,
  };
};

}  // namespace acg::visualizer::details
