#include "acg_gui/backend/context.hpp"

#include <acg_core/init.hpp>
#include <acg_utils/common.hpp>
#include <acg_utils/log.hpp>

#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>

#include "acg_gui/backend/window.hpp"

namespace acg::gui {

namespace details {

std::unique_ptr<VkContext2> context2_instance;

}

void VkContext2::Init(Config config) {
  // Create Vulkan Instance.
  auto instance_builder = vkb::InstanceBuilder();
  ACG_CHECK(details::context2_instance.get() == nullptr, "Vulkan Context Has Initialized.");
  details::context2_instance.reset(new VkContext2);
  instance_builder.set_engine_name(config.engine_name.c_str());
  instance_builder.set_app_name(config.app_name.c_str());
  instance_builder.set_app_version(config.app_version);
  instance_builder.set_engine_version(config.engine_version);
  instance_builder.require_api_version(config.api_version);

  // const char** glfw_extensions;
  // uint32_t glfw_extension_count;
  // glfw_extensions = glfwGetRequiredInstanceExtensions(&glfw_extension_count);
  if constexpr (acg::utils::get_platform_type() == utils::PlatformType::kApple) {
    instance_builder.enable_extension(VK_KHR_PORTABILITY_ENUMERATION_EXTENSION_NAME);
  }

  auto& context = VkContext2::Instance();

  if (config.enable_validation) {
    auto info = SystemInfo();
    if (!info.validation_layers_available) {
    }
    instance_builder.use_default_debug_messenger();
    instance_builder.request_validation_layers(true);
  }

  if (config.headless_mode) {
    instance_builder.set_headless();
  }

  auto instance_result = instance_builder.build();
  ACG_CHECK(instance_result.has_value(), "Vulkan Context Create Error: {}",
            vk::to_string(vk::Result(instance_result.full_error().vk_result)));
  auto instance = instance_result.value();
  context.instance_ = instance;

  // Pick Physical Device.
  auto physical_device_selector = vkb::PhysicalDeviceSelector{instance};
  if (!config.headless_mode) {
    vk::Instance ipp = instance.instance;
    context.surface_ = Window::Instance().CreateWindowSurface(ipp);
    physical_device_selector.set_surface(details::context2_instance->surface_);
    ACG_INFO("Physical Device Create with surface.");
  } else {
    ACG_INFO("Physical device headless mode requrested.");
  }

  auto pd_result = physical_device_selector.select();
  ACG_CHECK(pd_result, "Cannot pick a suitable device. {}",
            vk::to_string(vk::Result(pd_result.full_error().vk_result)));
  auto physical_device = pd_result.value();
  context.physical_device_ = physical_device;

  // Create Logical Device
  auto ld_result = vkb::DeviceBuilder(physical_device).build();
  ACG_CHECK(ld_result.has_value(), "Logical Device Create Error:{}",
            vk::to_string(vk::Result(ld_result.full_error().vk_result)));
  context.logical_device_ = ld_result.value();
}

VkContext2& VkContext2::Instance() {
  ACG_DEBUG_CHECK(details::context2_instance.get() != nullptr,
                  "You cannot access to uninitialized vkcontext");
  return *(details::context2_instance);
}

vkb::SystemInfo VkContext2::SystemInfo() {
  auto info = vkb::SystemInfo::get_system_info();
  ACG_CHECK(info.has_value(), "vkb get system info error: {}", info.error().message());
  return info.value();
}

void VkContext2::Destroy() {
  auto& context = Instance();
  vkb::destroy_device(context.logical_device_);
  vkb::destroy_surface(context.instance_, context.surface_);
  vkb::destroy_instance(context.instance_);
}

void VkContext2::Config::Hook() {
  acg::details::InitHook hook;
  hook.on_init = [this]() { VkContext2::Init(*this); };
  hook.on_exit = []() { VkContext2::Destroy(); };
  hook.name = "Vulkan Context2";
  hook.priority = 10;
  acg::details::add_hook(hook);
}

}  // namespace acg::gui
