#include "acg_gui/backend/context.hpp"

#include <acg_core/init.hpp>
#include <acg_utils/common.hpp>
#include <acg_utils/log.hpp>
#include <set>
#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>

#include "acg_gui/backend/avk.hpp"
#include "acg_gui/backend/window.hpp"
VKAPI_ATTR VkResult VKAPI_CALL vkCreateDebugUtilsMessengerEXT(
    VkInstance instance, const VkDebugUtilsMessengerCreateInfoEXT *pCreateInfo,
    const VkAllocationCallbacks *pAllocator, VkDebugUtilsMessengerEXT *pMessenger) {
  auto func = reinterpret_cast<PFN_vkCreateDebugUtilsMessengerEXT>(
      vkGetInstanceProcAddr(instance, "vkCreateDebugUtilsMessengerEXT"));
  if (func != nullptr) {
    return func(instance, pCreateInfo, pAllocator, pMessenger);
  } else {
    return VK_ERROR_EXTENSION_NOT_PRESENT;
  }
}

VKAPI_ATTR void VKAPI_CALL
vkDestroyDebugUtilsMessengerEXT(VkInstance instance, VkDebugUtilsMessengerEXT messenger,
                                VkAllocationCallbacks const *pAllocator) {
  auto func = reinterpret_cast<PFN_vkDestroyDebugUtilsMessengerEXT>(
      vkGetInstanceProcAddr(instance, "vkDestroyDebugUtilsMessengerEXT"));
  if (func != nullptr) {
    func(instance, messenger, pAllocator);
  }
}

VKAPI_ATTR VkBool32 VKAPI_CALL debug_message_callback(
    VkDebugUtilsMessageSeverityFlagBitsEXT messageSeverity,
    VkDebugUtilsMessageTypeFlagsEXT messageTypes,
    VkDebugUtilsMessengerCallbackDataEXT const *pCallbackData, void * /*pUserData*/) {
  auto cstr_to_string = [](const char *c) { return (c == nullptr) ? "Null" : std::string_view(c); };

  fmt::memory_buffer buffer;
  fmt::format_to(
      std::back_inserter(buffer),
      "{}: {}:\n"
      "messageIDName   = <{}>\n"
      "messageIdNumber = {}\n"
      "message         = <{}>\n",
      vk::to_string(static_cast<vk::DebugUtilsMessageSeverityFlagBitsEXT>(messageSeverity)),
      vk::to_string(static_cast<vk::DebugUtilsMessageTypeFlagsEXT>(messageTypes)),
      cstr_to_string(pCallbackData->pMessageIdName), pCallbackData->messageIdNumber,
      cstr_to_string(pCallbackData->pMessage));

  if (0 < pCallbackData->queueLabelCount) {
    fmt::format_to(std::back_inserter(buffer), "\tQueue Labels:\n");
    for (uint32_t i = 0; i < pCallbackData->queueLabelCount; i++) {
      fmt::format_to(std::back_inserter(buffer), "\t\tlabelName = <{}>\n",
                     cstr_to_string(pCallbackData->pQueueLabels[i].pLabelName));
    }
  }
  if (0 < pCallbackData->cmdBufLabelCount) {
    fmt::format_to(std::back_inserter(buffer), "\tCommandBuffer Labels:\n");
    for (uint32_t i = 0; i < pCallbackData->cmdBufLabelCount; i++) {
      fmt::format_to(std::back_inserter(buffer), "\t\tlabelName = <{}>\n",
                     cstr_to_string(pCallbackData->pCmdBufLabels[i].pLabelName));
    }
  }
  if (0 < pCallbackData->objectCount) {
    fmt::format_to(std::back_inserter(buffer), "\tObjects:\n");
    for (uint32_t i = 0; i < pCallbackData->objectCount; i++) {
      fmt::format_to(
          std::back_inserter(buffer),
          "\t\tObject {}\n"
          "\t\t\tobjectType   = {}\n"
          "\t\t\tobjectHandle = {}\n"
          "\t\t\tobjectName   = <{}>\n",
          i, vk::to_string(static_cast<vk::ObjectType>(pCallbackData->pObjects[i].objectType)),
          pCallbackData->pObjects[i].objectHandle,
          cstr_to_string(pCallbackData->pObjects[i].pObjectName));
    }
  }
  auto s = fmt::to_string(buffer);

  switch (messageSeverity) {
    case VK_DEBUG_UTILS_MESSAGE_SEVERITY_ERROR_BIT_EXT:
      ACG_ERROR("{}", s);
      break;
    case VK_DEBUG_UTILS_MESSAGE_SEVERITY_WARNING_BIT_EXT:
      ACG_WARN("{}", s);
      break;
    case VK_DEBUG_UTILS_MESSAGE_SEVERITY_INFO_BIT_EXT:
      ACG_INFO("{}", s);
      break;
    case VK_DEBUG_UTILS_MESSAGE_SEVERITY_VERBOSE_BIT_EXT:
      ACG_DEBUG("{}", s);
      break;
    default:
      // Not valid bit.
      break;
  };

  return false;
}

namespace acg::gui {

namespace details {

std::unique_ptr<VkContext2> context2_instance;

}

void VkContext2::Hooker::Hook() {
  acg::details::InitHook hook;
  using namespace details;
  hook.on_init = [this]() {
    ACG_CHECK(context2_instance.get() == nullptr, "Vk context double initialize");
    context2_instance.reset(new VkContext2(*this));
  };
  hook.on_exit = []() { context2_instance.reset(); };
  hook.priority = 10;
  hook.name = "Vk Context2 Init";
  acg::details::add_hook(hook);
}

VkContext2 &VkContext2::Instance() {
  ACG_CHECK(details::context2_instance.get() != nullptr, "Access to uninitialized vk context");
  return *details::context2_instance;
}

void VkContext2::CreateInstance() {
  system_info_.available_layers = vk::enumerateInstanceLayerProperties();
  system_info_.available_extensions = vk::enumerateInstanceExtensionProperties();
  system_info_.validation_layers_available = CheckLayerSupport({"VK_LAYER_KHRONOS_validation"});
  system_info_.debug_utils_available = CheckExtensionSupport({VK_EXT_DEBUG_UTILS_EXTENSION_NAME});
  ACG_CHECK(!init_config_.enable_validation || system_info_.validation_layers_available,
            "Vulkan validation required with validation layer unavailable");

  ACG_CHECK(!init_config_.enable_validation || system_info_.debug_utils_available,
            "Vulkan validation required with debug utils unavailable");
  vk::ApplicationInfo app_info;
  app_info.setPApplicationName(init_config_.app_name.c_str());
  app_info.setApplicationVersion(init_config_.app_version);
  app_info.setEngineVersion(init_config_.engine_version);
  app_info.setApiVersion(init_config_.api_version);
  vk::InstanceCreateInfo create_info;
  create_info.setPApplicationInfo(&app_info);

  // Setup Extensions.
  uint32_t glfw_extension_count = 0;
  const char **glfw_extensions;
  glfw_extensions = glfwGetRequiredInstanceExtensions(&glfw_extension_count);
  std::vector<const char *> extensions(glfw_extensions, glfw_extensions + glfw_extension_count);
  if (init_config_.enable_validation) {
    extensions.push_back(VK_EXT_DEBUG_UTILS_EXTENSION_NAME);
  }
  if constexpr (utils::get_platform_type() == utils::PlatformType::kApple) {
    extensions.push_back(VK_KHR_PORTABILITY_ENUMERATION_EXTENSION_NAME);
  }
  ACG_CHECK(CheckExtensionSupport(extensions), "Some extensions not support");
  create_info.setPEnabledExtensionNames(extensions);

  // If apple, set flag.
  if constexpr (utils::get_platform_type() == utils::PlatformType::kApple) {
    create_info.setFlags(vk::InstanceCreateFlagBits::eEnumeratePortabilityKHR);
  }

  auto &enabled_layers = init_config_.enabled_layers;
  if (init_config_.enable_validation) {
    enabled_layers.push_back("VK_LAYER_KHRONOS_validation");
  }
  ACG_CHECK(CheckLayerSupport(enabled_layers), "Some layer not support");
  create_info.setPEnabledLayerNames(enabled_layers);

  auto result = vk::createInstance(&create_info, nullptr, &instance_);
  ACG_CHECK(result == vk::Result::eSuccess, "Vk instance create error: {}", vk::to_string(result));

  if (init_config_.enable_validation) {
    vk::DebugUtilsMessengerCreateInfoEXT info;
    info.messageSeverity = vk::DebugUtilsMessageSeverityFlagBitsEXT::eError
                           | vk::DebugUtilsMessageSeverityFlagBitsEXT::eWarning;
    info.messageType = vk::DebugUtilsMessageTypeFlagBitsEXT::eGeneral
                       | vk::DebugUtilsMessageTypeFlagBitsEXT::ePerformance
                       | vk::DebugUtilsMessageTypeFlagBitsEXT::eValidation;
    info.pfnUserCallback = debug_message_callback;
    info.pUserData = nullptr;  // Optional
    auto result = instance_.createDebugUtilsMessengerEXT(&info, nullptr, &debug_messenger_);
    ACG_CHECK(result == vk::Result::eSuccess, "Create debug utils error: {}",
              vk::to_string(result));
  }
}

bool VkContext2::CheckLayerSupport(const std::vector<const char *> &layers_require) {
  for (const auto &layer_name : layers_require) {
    bool found = false;
    for (const auto &layer_prop : system_info_.available_layers) {
      if (strcmp(layer_name, layer_prop.layerName) != 0) {
        found = true;
        break;
      }
    }
    if (!found) {
      return false;
    }
  }
  return true;
}

bool VkContext2::CheckExtensionSupport(const std::vector<const char *> &extensions_require) {
  for (const auto &ext_name : extensions_require) {
    bool found = false;
    for (const auto &ext_name_sup : system_info_.available_extensions) {
      if (strcmp(ext_name, ext_name_sup.extensionName) != 0) {
        found = true;
        break;
      }
    }
    if (!found) {
      return false;
    }
  }
  return true;
}

void VkContext2::CreatePhysicalDevice() {
  auto &e = init_config_.enabled_device_extensions;
  if constexpr (acg::utils::get_platform_type() == utils::PlatformType::kApple) {
    if (auto it = std::find_if(
            e.begin(), e.end(),
            [](const char *ext) { return strcmp(ext, "VK_KHR_portability_subset") == 0; });
        it == e.end()) {
      ACG_DEBUG_LOG("Add portability subset extension automatically for Macos.");
      e.push_back("VK_KHR_portability_subset");
    }
  }
  if (!init_config_.headless_mode) {
    // Initialize surface.
    surface_ = Window::Instance().CreateWindowSurface(instance_);
    ACG_DEBUG_LOG("Created window surface.");

    if (auto it = std::find_if(
            e.begin(), e.end(),
            [](const char *ext) { return strcmp(ext, VK_KHR_SWAPCHAIN_EXTENSION_NAME) == 0; });
        it == e.end()) {
      ACG_INFO("Non-headless mode require swapchain extension support, add it automatically.");
      e.push_back(VK_KHR_SWAPCHAIN_EXTENSION_NAME);
    }

    if (!init_config_.require_graphics_queue) {
      ACG_WARN("Non-headless mode require graphics queue.");
      init_config_.require_graphics_queue = true;
    }
    if (!init_config_.require_present_queue) {
      ACG_WARN("Non-headless mode require present queue.");
      init_config_.require_present_queue = true;
    }
  }
  auto devices = instance_.enumeratePhysicalDevices();

  std::vector<vk::PhysicalDevice> available_devices;
  for (const auto &d : devices) {
    SystemInfo::PhysicalDeviceInfo device_info = GetDeviceInfo(d);
    std::set<std::string_view> required(e.begin(), e.end());
    for (const auto &p : device_info.extension_properties) {
      required.erase(p.extensionName);
    }
    if (!required.empty()) {
      // not capable.
      ACG_DEBUG_LOG("Physical Device {} not capable because of extension",
                    device_info.properties.deviceName);
      continue;
    }

    if (init_config_.require_graphics_queue && !device_info.graphics_family.has_value()) {
      ACG_DEBUG_LOG("Physical Device {} not capable: No graphics family available.",
                    device_info.properties.deviceName);
      continue;
    }
    if (init_config_.require_present_queue && !device_info.present_family.has_value()) {
      ACG_DEBUG_LOG("Physical Device {} not capable: No present family available.",
                    device_info.properties.deviceName);
      continue;
    }
    if (init_config_.require_compute_queue && !device_info.compute_family.has_value()) {
      ACG_DEBUG_LOG("Physical Device {} not capable: No compute family available.",
                    device_info.properties.deviceName);
      continue;
    }
    if (init_config_.require_transfer_queue && !device_info.transfer_family.has_value()) {
      ACG_DEBUG_LOG("Physical Device {} not capable: No transfer family available.",
                    device_info.properties.deviceName);
      continue;
    }

    if (!init_config_.headless_mode) {
      // Check Swapchain Support
      if (device_info.surface_formats.empty()) {
        ACG_DEBUG_LOG("No surface format available.");
        continue;
      }

      if (device_info.surface_present_modes.empty()) {
        ACG_DEBUG_LOG("No surface present mode available.");
        continue;
      }
    }
    available_devices.push_back(d);
  }

  if (!init_config_.rank_function.has_value()) {
    init_config_.rank_function = [](vk::PhysicalDevice device) {
      auto type = device.getProperties().deviceType;
      switch (type) {
        case vk::PhysicalDeviceType::eCpu:
          return 1.0;
        case vk::PhysicalDeviceType::eIntegratedGpu:
          return 3.0;
        case vk::PhysicalDeviceType::eVirtualGpu:
          return 4.0;
        case vk::PhysicalDeviceType::eDiscreteGpu:
          return 5.0;
        default:
          return 0.0;
      }
    };
  }

  std::sort(
      available_devices.begin(), available_devices.end(),
      [rank = init_config_.rank_function.value()](
          const vk::PhysicalDevice &l, const vk::PhysicalDevice &r) { return rank(l) > rank(r); });
  ACG_CHECK(!available_devices.empty(), "No device available.");

  physical_device_ = available_devices.front();
  system_info_.physical_device_info = GetDeviceInfo(physical_device_);

  ACG_INFO("Physical device picked. {}", system_info_.physical_device_info.properties.deviceName);
}

VkContext2::SystemInfo::PhysicalDeviceInfo VkContext2::GetDeviceInfo(
    vk::PhysicalDevice device) const {
  VkContext2::SystemInfo::PhysicalDeviceInfo info;
  info.properties = device.getProperties();
  info.extension_properties = device.enumerateDeviceExtensionProperties();
  int i = 0;
  for (const auto &family : device.getQueueFamilyProperties()) {
    if (family.queueCount > 0) {
      if (family.queueFlags & vk::QueueFlagBits::eGraphics) {
        info.graphics_family = i;
      }
      if (family.queueFlags & vk::QueueFlagBits::eCompute) {
        info.compute_family = i;
      }
      if (family.queueFlags & vk::QueueFlagBits::eTransfer) {
        info.transfer_family = i;
      }
    }

    if (surface_) {
      bool has_surface_support = device.getSurfaceSupportKHR(i, surface_);
      if (has_surface_support) {
        info.present_family = i;
      }

      info.surface_capabilities = device.getSurfaceCapabilitiesKHR(surface_);
      info.surface_formats = device.getSurfaceFormatsKHR(surface_);
      info.surface_present_modes = device.getSurfacePresentModesKHR(surface_);
    }
    i += 1;
  }

  return info;
}

void VkContext2::CreateDevice() {
  std::vector<vk::DeviceQueueCreateInfo> queue_create_info;
  std::set<uint32_t> unique_queue_families;
  // Foreach Family, if required, add to set:
  if (init_config_.require_graphics_queue) {
    unique_queue_families.emplace(system_info_.physical_device_info.graphics_family.value());
  }
  if (init_config_.require_present_queue) {
    unique_queue_families.emplace(system_info_.physical_device_info.present_family.value());
  }
  if (init_config_.require_compute_queue) {
    unique_queue_families.emplace(system_info_.physical_device_info.compute_family.value());
  }
  if (init_config_.require_transfer_queue) {
    unique_queue_families.emplace(system_info_.physical_device_info.transfer_family.value());
  }

  float queue_priority = 1.0f;
  for (auto family : unique_queue_families) {
    vk::DeviceQueueCreateInfo info;
    info.setQueueFamilyIndex(family).setQueueCount(1).setPQueuePriorities(&queue_priority);
    queue_create_info.emplace_back(info);
  }

  vk::DeviceCreateInfo device_create_info;
  device_create_info.setQueueCreateInfos(queue_create_info)
      .setPEnabledFeatures(&init_config_.physical_device_features)
      .setPEnabledExtensionNames(init_config_.enabled_device_extensions)
      .setPEnabledLayerNames(init_config_.enabled_layers);

  auto result = physical_device_.createDevice(&device_create_info, nullptr, &device_);
  ACG_CHECK(result == vk::Result::eSuccess, "Failed to create vulkan logical device: {}",
            vk::to_string(result));
  // setup queue:
  if (system_info_.physical_device_info.graphics_family && init_config_.require_graphics_queue) {
    graphics_queue_
        = device_.getQueue(system_info_.physical_device_info.graphics_family.value(), 0);
  }
  if (system_info_.physical_device_info.present_family && init_config_.require_present_queue) {
    present_queue_ = device_.getQueue(system_info_.physical_device_info.present_family.value(), 0);
  }
  if (system_info_.physical_device_info.compute_family && init_config_.require_compute_queue) {
    compute_queue_ = device_.getQueue(system_info_.physical_device_info.compute_family.value(), 0);
  }
  if (system_info_.physical_device_info.transfer_family && init_config_.require_transfer_queue) {
    transfer_queue_
        = device_.getQueue(system_info_.physical_device_info.transfer_family.value(), 0);
  }
}

VkContext2::VkContext2(Hooker config) {
  init_config_ = config;
  CreateInstance();
  CreatePhysicalDevice();
  CreateDevice();
}

VkContext2::~VkContext2() {
  device_.destroy();
  if (static_cast<bool>(debug_messenger_)) {
    instance_.destroy(debug_messenger_);
  }

  if (static_cast<bool>(surface_)) {
    instance_.destroy(surface_);
  }
  instance_.destroy();
}

acg::Result<std::pair<vk::Image, vk::DeviceMemory>> VkContext2::CreateImage(
    uint32_t width, uint32_t height, vk::Format format, vk::ImageTiling tiling,
    vk::ImageUsageFlags usage, vk::MemoryPropertyFlags properties) const {
  vk::ImageCreateInfo image_info{};
  image_info.imageType = vk::ImageType::e2D;
  image_info.extent.width = width;
  image_info.extent.height = height;
  image_info.extent.depth = 1;
  image_info.mipLevels = 1;
  image_info.arrayLayers = 1;
  image_info.format = format;
  image_info.tiling = tiling;
  image_info.initialLayout = vk::ImageLayout::eUndefined;
  image_info.usage = usage;
  image_info.samples = vk::SampleCountFlagBits::e1;
  image_info.sharingMode = vk::SharingMode::eExclusive;

  auto image = device_.createImage(image_info);

  auto mem_requirements = device_.getImageMemoryRequirements(image);

  vk::MemoryAllocateInfo alloc_info{};
  alloc_info.allocationSize = mem_requirements.size;
  auto mt = FindMemoryType(mem_requirements.memoryTypeBits, properties);
  if (mt.HasValue()) {
    alloc_info.memoryTypeIndex = mt.Value();
  } else {
    return mt.Error();
  }

  auto mem = device_.allocateMemory(alloc_info);
  device_.bindImageMemory(image, mem, 0);
  return Result(std::pair{image, mem});
}

acg::Result<uint32_t> VkContext2::FindMemoryType(uint32_t type_filter,
                                                 vk::MemoryPropertyFlags properties) const {
  auto memory_properties = physical_device_.getMemoryProperties();
  for (uint32_t i = 0; i < memory_properties.memoryTypeCount; ++i) {
    if ((type_filter & (1 << i))
        && (memory_properties.memoryTypes[i].propertyFlags & properties) == properties) {
      return i;
    }
  }
  return Result<uint32_t>(acg::Status::kNotFound);
}

BufferWithMemory VkContext2::CreateBufferWithMemory(vk::DeviceSize size, vk::BufferUsageFlags usage,
                                                    vk::MemoryPropertyFlags properties) const {
  vk::BufferCreateInfo buffer_info;
  buffer_info.size = size;
  buffer_info.usage = usage;
  buffer_info.sharingMode = vk::SharingMode::eExclusive;
  auto buf = device_.createBuffer(buffer_info);
  auto mem_requirements = device_.getBufferMemoryRequirements(buf);
  vk::MemoryAllocateInfo alloc_info(mem_requirements.size,
                                    FindMemoryType(mem_requirements.memoryTypeBits, properties));
  auto mem = device_.allocateMemory(alloc_info);
  device_.bindBufferMemory(buf, mem, 0);
  return BufferWithMemory(buf, mem, size);
}

void VkContext2::DestroyBufferWithMemory(BufferWithMemory &bufmem) const {
  device_.destroy(bufmem.GetBuffer());
  device_.free(bufmem.GetMemory());
}
void VkContext2::CopyHostToBuffer(const void *mem_data, BufferWithMemory &buffer_with_memory,
                                  size_t size) const {
  void *mapped = nullptr;
  if (buffer_with_memory.IsMapped()) {
    mapped = buffer_with_memory.GetMappedMemory();
  } else {
    mapped = device_.mapMemory(buffer_with_memory.GetMemory(), 0, VK_WHOLE_SIZE);
  }
  memcpy(mapped, mem_data, size);
  if (!buffer_with_memory.IsMapped()) {
    device_.unmapMemory(buffer_with_memory.GetMemory());
  }
}

acg::Result<vk::Format> VkContext2::FindSupportedFormat(const std::vector<vk::Format> &candidates,
                                                        vk::ImageTiling tiling,
                                                        vk::FormatFeatureFlags features) const {
  for (const auto &format : candidates) {
    auto props = physical_device_.getFormatProperties(format);

    if (tiling == vk::ImageTiling::eLinear && (props.linearTilingFeatures & features) == features) {
      return format;
    } else if (tiling == vk::ImageTiling::eOptimal
               && (props.optimalTilingFeatures & features) == features) {
      return format;
    }
  }
  return {acg::Status::kNotFound};
}


}  // namespace acg::gui
