#include "acg_gui/backend/context.hpp"

#include <acg_utils/common.hpp>
#include <acg_utils/log.hpp>

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
  ACG_CHECK(instance_result.has_value(), "Vulkan Context Create Error");
  auto instance = instance_result.value();
  context.instance_ = instance;

  // Pick Physical Device.
  auto physical_device_selector = vkb::PhysicalDeviceSelector{instance};
  if (!config.headless_mode) {
    vk::Instance ipp = instance.instance;
    context.surface_ = details::Window::Instance().CreateWindowSurface(ipp);
    physical_device_selector.set_surface(details::context2_instance->surface_);
  }

  auto pd_result = physical_device_selector.select();
  ACG_CHECK(pd_result.has_value(), "Cannot pick a suitable device.");
  auto physical_device = pd_result.value();
  context.physical_device_ = physical_device;

  // Create Logical Device
  auto ld_result = vkb::DeviceBuilder(physical_device).build();
  ACG_CHECK(ld_result.has_value(), "Logical Device Create Error");
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
  VkContext2::Init(*this);
}

}  // namespace acg::gui
