#pragma once

#include <VkBootstrap.h>

#include <optional>
#include <vulkan/vulkan.hpp>
#include <acg_utils/common.hpp>

namespace acg::gui {

class VkContext2 {
  VkContext2() = default;

public:
  struct Config {
    std::string app_name;
    std::string engine_name;
    uint32_t api_version{VK_API_VERSION_1_3};
    uint32_t app_version{VK_MAKE_VERSION(1, 0, 0)};
    uint32_t engine_version{VK_MAKE_VERSION(1, 0, 0)};
    bool enable_validation{acg::utils::is_debug_mode};
    bool headless_mode{false};

    void Hook();
  };

  // No copy and move
  VkContext2(const VkContext2&) = delete;

  VkContext2(VkContext2&&) = delete;

  vkb::Instance instance_;

  vkb::PhysicalDevice physical_device_;

  vkb::Device logical_device_;

  vk::SurfaceKHR surface_;

private:
  static void Init(Config config);

  static void Destroy();

public:
  static VkContext2& Instance();

  static vkb::SystemInfo SystemInfo();
};

using VkContext2Hooker = VkContext2::Config;

}  // namespace acg::gui
