#pragma once

#include <acg_utils/common.hpp>
#include <optional>
#include <vulkan/vulkan.hpp>

namespace acg::gui {

class VkContext2 {
public:
  struct Hooker {
    std::string app_name;
    std::string engine_name;
    uint32_t api_version{VK_API_VERSION_1_3};
    uint32_t app_version{VK_MAKE_VERSION(1, 0, 0)};
    uint32_t engine_version{VK_MAKE_VERSION(1, 0, 0)};
    bool enable_validation{acg::utils::is_debug_mode};
    bool headless_mode{false};

    bool require_graphics_queue = true;
    bool require_present_queue = true;
    bool require_compute_queue = false;
    bool require_transfer_queue = false;
    std::vector<const char*> enabled_layers;
    std::vector<const char*> enabled_device_extensions;

    vk::PhysicalDeviceFeatures physical_device_features;
    std::optional<std::function<double(vk::PhysicalDevice)> > rank_function;
    void Hook();
  };

  struct SystemInfo {
    std::vector<vk::LayerProperties> available_layers;
    std::vector<vk::ExtensionProperties> available_extensions;
    bool validation_layers_available = false;
    bool debug_utils_available = false;

    struct PhysicalDeviceInfo {
      vk::PhysicalDeviceProperties properties;
      std::vector<vk::ExtensionProperties> extension_properties;
      std::optional<uint32_t> graphics_family;
      std::optional<uint32_t> present_family;
      std::optional<uint32_t> compute_family;
      std::optional<uint32_t> transfer_family;

      vk::SurfaceCapabilitiesKHR surface_capabilities;
      std::vector<vk::SurfaceFormatKHR> surface_formats;
      std::vector<vk::PresentModeKHR> surface_present_modes;

    } physical_device_info;

  };

  VkContext2() = delete;
  VkContext2(VkContext2&&) = delete;
  VkContext2(VkContext2 const&) = delete;

  ~VkContext2();
  static VkContext2& Instance();

protected:
  explicit VkContext2(Hooker config);

  bool CheckLayerSupport(const std::vector<const char* >& layers_require);
  bool CheckExtensionSupport(const std::vector<const char* >& extensions_require);
  void CreateInstance();
  void CreatePhysicalDevice();
  void CreateDevice();

  // Config
  Hooker init_config_;
  // Instance
  vk::Instance instance_;
  vk::DebugUtilsMessengerEXT debug_messenger_;
  bool headless_{false};

  // Physical Device
  vk::PhysicalDevice physical_device_;
  std::string physical_device_name_;

  // Device
  vk::Device device_;
  vk::SurfaceKHR surface_;
  vk::Queue graphics_queue_;
  vk::Queue present_queue_;
  vk::Queue compute_queue_;
  vk::Queue transfer_queue_;

  std::vector<vk::QueueFamilyProperties> queue_families_;

  SystemInfo::PhysicalDeviceInfo GetDeviceInfo(vk::PhysicalDevice device);
  SystemInfo system_info_;
};

using VkContext2Hooker = VkContext2::Hooker;

}  // namespace acg::gui
