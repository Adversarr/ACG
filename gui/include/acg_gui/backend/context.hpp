#pragma once

#include <VkBootstrap.h>

#include <optional>
#include <vulkan/vulkan.hpp>

namespace acg::gui {

class VkContext2 {
  VkContext2() = default;

public:
  struct Config {
    std::string app_name;
    std::string engine_name;
    uint32_t major{1}, minor{3}, patch{0};
    bool enable_validation;
    bool headless_mode;

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
