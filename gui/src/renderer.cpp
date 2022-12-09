#include "acg_gui/backend/vkcontext.hpp"

#include <glm/ext/matrix_transform.hpp>
#include <set>
#include <vector>
#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>
#include <spdlog/spdlog.h>

#include <acg_utils/log.hpp>
#include <acg_utils/singleton.hpp>
#include <acg_gui/scene.hpp>
#include <fstream>
#include <vulkan/vulkan_handles.hpp>

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
      spdlog::error("{}", s);
      break;
    case VK_DEBUG_UTILS_MESSAGE_SEVERITY_WARNING_BIT_EXT:
      spdlog::warn("{}", s);
      break;
    case VK_DEBUG_UTILS_MESSAGE_SEVERITY_INFO_BIT_EXT:
      spdlog::info("{}", s);
      break;
    case VK_DEBUG_UTILS_MESSAGE_SEVERITY_VERBOSE_BIT_EXT:
      spdlog::debug("{}", s);
      break;
    default:
      // Not valid bit.
      break;
  };

  return false;
}

namespace acg::gui {

namespace details {

const std::vector<const char *> validation_layers = {"VK_LAYER_KHRONOS_validation"};

VkContext::~VkContext() {
  device_.waitIdle();
  CleanupSwapchain();
  // Cleanup the semaphores
  for (auto &sync : semaphores_) {
    device_.destroy(sync.image_available);
    device_.destroy(sync.render_finished);
    device_.destroy(sync.in_flight_fence);
  }
  semaphores_.clear();
  for (auto &bm : allocated_buffers_) {
    bm.Release();
  }
  allocated_buffers_.clear();
  device_.destroyCommandPool(command_pool_);
  device_.destroy();

  // Clean Up Instance
  if (enable_validation_ && static_cast<bool>(debug_messenger_)) {
    instance_.destroy(debug_messenger_);
  }
  instance_.destroy(surface_);
  instance_.destroy();
}

void VkContext::Init() {
  if (is_inited_) {
    return;
  }

  window_ = std::make_unique<Window>(title_);
  CreateInstance();
  SetupDebugMessenger();
  CreateSurface();
  PickPhysicalDevice();
  CreateLogicalDevice();

  // Swapchain, ImageViews
  CreateCommandPool();
  CreateSwapchain();
  CreateImageViews();
  CreateSyncObjects();

  is_inited_ = true;
}

void VkContext::CreateInstance() {
  if (enable_validation_) {
    ACG_CHECK(IsValidationSupport() == true,
              "No Validation suppport, please turn off validation layer!");
  }

  vk::ApplicationInfo app_info;
  app_info.pApplicationName = "ACG Vulkan Viewer";
  app_info.applicationVersion = VK_MAKE_VERSION(1, 0, 0);
  app_info.pEngineName = "No Engine";
  app_info.engineVersion = VK_MAKE_VERSION(1, 0, 0);
  app_info.apiVersion = VK_API_VERSION_1_3;

  vk::InstanceCreateInfo create_info;
  create_info.pApplicationInfo = &app_info;

  // Setup extensions for vulkan instance.
  auto extensions = GetRequiredExtensions();
  create_info.enabledExtensionCount = static_cast<uint32_t>(extensions.size());
  create_info.ppEnabledExtensionNames = extensions.data();
  if constexpr (utils::get_platform_type() == utils::PlatformType::kApple) {
    create_info.flags = vk::InstanceCreateFlagBits::eEnumeratePortabilityKHR;
  } else {
    /* Do nothing, only mac need the flag above. */
  }

  // Setup Debug Messenger.
  if (enable_validation_) {
    create_info.enabledLayerCount = static_cast<uint32_t>(validation_layers.size());
    create_info.ppEnabledLayerNames = validation_layers.data();
  } else {
    create_info.enabledLayerCount = 0;
    create_info.pNext = nullptr;
  }

  instance_ = vk::createInstance(create_info);
}

std::vector<const char *> VkContext::GetRequiredExtensions() const {
  uint32_t glfw_extension_count = 0;
  const char **glfw_extensions;
  glfw_extensions = glfwGetRequiredInstanceExtensions(&glfw_extension_count);
  std::vector<const char *> extensions(glfw_extensions, glfw_extensions + glfw_extension_count);
  if (enable_validation_) {
    extensions.push_back(VK_EXT_DEBUG_UTILS_EXTENSION_NAME);
  }
  if constexpr (utils::get_platform_type() == utils::PlatformType::kApple) {
    extensions.push_back(VK_KHR_PORTABILITY_ENUMERATION_EXTENSION_NAME);
  }

  return extensions;
}

bool VkContext::IsValidationSupport() {
  auto available_layers = vk::enumerateInstanceLayerProperties();
  for (const auto &layer_name : validation_layers) {
    bool found = false;
    for (const auto &layer_prop : available_layers) {
      if (std::strcmp(layer_name, layer_prop.layerName) != 0) {
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

void VkContext::SetupDebugMessenger() {
  if (!enable_validation_) return;
  vk::DebugUtilsMessengerCreateInfoEXT info = PopulateDebugMessengerCreateInfo();
  debug_messenger_ = instance_.createDebugUtilsMessengerEXT(info);
}

vk::DebugUtilsMessengerCreateInfoEXT VkContext::PopulateDebugMessengerCreateInfo() {
  vk::DebugUtilsMessengerCreateInfoEXT info;
  info.messageSeverity = vk::DebugUtilsMessageSeverityFlagBitsEXT::eError
                         | vk::DebugUtilsMessageSeverityFlagBitsEXT::eWarning;
  info.messageType = vk::DebugUtilsMessageTypeFlagBitsEXT::eGeneral
                     | vk::DebugUtilsMessageTypeFlagBitsEXT::ePerformance
                     | vk::DebugUtilsMessageTypeFlagBitsEXT::eValidation;
  info.pfnUserCallback = debug_message_callback;
  info.pUserData = nullptr;  // Optional
  return info;
}

void VkContext::CreateSurface() { surface_ = window_->CreateWindowSurface(instance_); }

void VkContext::PickPhysicalDevice() {
  auto devices = instance_.enumeratePhysicalDevices();
  for (const auto &d : devices) {
    auto properties = d.getProperties();
    ACG_DEBUG_LOG("Found Device: id={}, name=<{}>", properties.deviceID, properties.deviceName);
    if (IsSuitable(d)) {
      physical_device_ = d;
      break;
    }
    ACG_DEBUG_LOG("Not Suitable.");
  }

  if (!physical_device_) {
    spdlog::error("No device is suitable.");
    exit(-1);
  }

  spdlog::info("Physical Device Pick: {}", physical_device_.getProperties().deviceName);
}

bool VkContext::IsSuitable(const vk::PhysicalDevice &physical_device) const {
  if (!CheckDeviceExtensionSupport(physical_device)) {
    return false;
  }

  if (!FindQueueFamilies(physical_device).IsComplete()) {
    return false;
  }

  if (auto details = QuerySwapChainSupport(physical_device);
      details.formats.empty() || details.present_modes.empty()) {
    return false;
  }

  return true;
}

bool VkContext::CheckDeviceExtensionSupport(const vk::PhysicalDevice &physical_device) const {
  auto extensions = physical_device.enumerateDeviceExtensionProperties();
  std::set<std::string_view> required{device_extension_enabled_.begin(),
                                      device_extension_enabled_.end()};
  for (const auto &ext : extensions) {
    required.erase(ext.extensionName);
  }
  return required.empty();
}

QueueFamilyIndices VkContext::FindQueueFamilies(const vk::PhysicalDevice &physical_device) const {
  QueueFamilyIndices ret;
  auto family_properties = physical_device.getQueueFamilyProperties();
  int i = 0;
  for (const auto &family : family_properties) {
    if (family.queueCount > 0 && (family.queueFlags & vk::QueueFlagBits::eGraphics)) {
      ret.graphics_family = i;
    }
    auto has_surface_support = physical_device.getSurfaceSupportKHR(i, surface_);
    if (has_surface_support) {
      ret.present_family = i;
    }
    if (ret.IsComplete()) {
      break;
    }
    i += 1;
  }
  return ret;
}

SwapChainSupportDetails VkContext::QuerySwapChainSupport(
    const vk::PhysicalDevice &physical_device) const {
  SwapChainSupportDetails details;
  details.capabilities = physical_device.getSurfaceCapabilitiesKHR(surface_);
  details.formats = physical_device.getSurfaceFormatsKHR(surface_);
  details.present_modes = physical_device.getSurfacePresentModesKHR(surface_);
  return details;
}

void VkContext::CreateLogicalDevice() {
  device_related_indices_ = FindQueueFamilies(physical_device_);
  auto &indices = device_related_indices_;
  std::vector<vk::DeviceQueueCreateInfo> queue_create_infos;
  std::set<int> unique_queue_families = {static_cast<int>(indices.graphics_family.value()),
                                         static_cast<int>(indices.present_family.value())};

  float queue_priority = 1.0;
  for (int queue_family : unique_queue_families) {
    vk::DeviceQueueCreateInfo queue_create_info;
    queue_create_info.queueFamilyIndex = queue_family;
    queue_create_info.queueCount = 1;
    queue_create_info.pQueuePriorities = &queue_priority;
    queue_create_infos.emplace_back(queue_create_info);
  }

  vk::DeviceCreateInfo device_create_info;
  // Queue Create Info
  device_create_info.queueCreateInfoCount = static_cast<uint32_t>(queue_create_infos.size());
  device_create_info.pQueueCreateInfos = queue_create_infos.data();

  // Features: No feature is needed
  vk::PhysicalDeviceFeatures device_features;
  device_create_info.pEnabledFeatures = &device_features;

  // Set Enabled Extension.
  std::vector<const char *> enabled_extension = device_extension_enabled_;
  if constexpr (utils::get_platform_type() == utils::PlatformType::kApple) {
    // For apple users, we need vk_khr portablities enabled.
    enabled_extension.push_back("VK_KHR_portability_subset");
  }
  device_create_info.enabledExtensionCount = enabled_extension.size();
  device_create_info.ppEnabledExtensionNames = enabled_extension.data();

  // Setup Validation Layer to enabled layers.
  if (enable_validation_) {
    device_create_info.enabledLayerCount = static_cast<uint32_t>(validation_layers.size());
    device_create_info.ppEnabledLayerNames = validation_layers.data();
  } else {
    device_create_info.enabledLayerCount = 0;
  }

  // create logical device from physical device.
  auto res = physical_device_.createDevice(&device_create_info, nullptr, &device_);

  // get the queue of logical device.
  graphics_queue_ = device_.getQueue(indices.graphics_family.value(), 0);
  present_queue_ = device_.getQueue(indices.present_family.value(), 0);
  spdlog::info("Created Vulkan device (Result: {}). graphics_family={} present_family={}",
               vk::to_string(res), indices.graphics_family.value(), indices.present_family.value());
}

void VkContext::CreateCommandPool() {
  vk::CommandPoolCreateInfo pool_info;
  pool_info.queueFamilyIndex = device_related_indices_.graphics_family.value();
  pool_info.flags = vk::CommandPoolCreateFlagBits::eResetCommandBuffer;
  command_pool_ = device_.createCommandPool(pool_info);
}

std::unique_ptr<VkContext> VkContext::Builder::Build() const {
  std::unique_ptr<VkContext> inst(new VkContext);
  inst->enable_validation_ = enable_validation;
  inst->swapchain_size_ = swapchain_size;
  inst->Init();
  utils::Singleton<VkContext>::Init(std::move(inst));
  return inst;
}

VkContext::Builder &VkContext::Builder::SetValidation(bool enable) {
  enable_validation = enable;
  return *this;
}
VkContext::Builder &VkContext::Builder::SetSwapchainSize(uint32_t size) {
  if (size < 2) {
    spdlog::error("Cannot set swapchain size less than 2, set to default 3");
    size = 3;
  }
  swapchain_size = size;
  return *this;
}

const vk::Instance &VkContext::GetInstance() const { return instance_; }
const vk::Device &VkContext::GetDevice() const { return device_; }
const vk::PhysicalDevice &VkContext::GetPhysicalDevice() const { return physical_device_; }
const vk::SurfaceKHR &VkContext::GetSurface() const { return surface_; }
const vk::Queue &VkContext::GetGraphicsQueue() const { return graphics_queue_; }
const vk::Queue &VkContext::GetPresentQueue() const { return present_queue_; }

vk::SurfaceFormatKHR VkContext::ChooseSwapSurfaceFormat(
    const std::vector<vk::SurfaceFormatKHR> &formats) {
  ACG_CHECK(!formats.empty(), "No Format Support!");
  if (formats.size() == 1 && formats.front() == vk::Format::eUndefined) {
    return {vk::Format::eB8G8R8A8Unorm, vk::ColorSpaceKHR::eSrgbNonlinear};
  }

  for (const auto &format : formats) {
    if (format.format == vk::Format::eB8G8R8A8Unorm
        && format.colorSpace == vk::ColorSpaceKHR::eSrgbNonlinear) {
      return format;
    }
  }

  return formats.front();
}

vk::PresentModeKHR VkContext::ChooseSwapPresentMode(const std::vector<vk::PresentModeKHR> &modes) {
  auto best = vk::PresentModeKHR::eFifo;
  for (const auto &mode : modes) {
    if (mode == vk::PresentModeKHR::eMailbox) {
      best = mode;
      break;
    }

    // NOTE: Immediate mode leads to visiable tearing
    //
    // if (mode == vk::PresentModeKHR::eImmediate) {
    //   best = mode;
    // }
  }
  if (best != vk::PresentModeKHR::eMailbox) {
    fmt::memory_buffer buffer;
    for (const auto &mode : modes) {
      fmt::format_to(std::back_inserter(buffer), "{} ", vk::to_string(mode));
    }

    spdlog::warn("Not using mailbox present mode, now: {}, available: {}", vk::to_string(best),
                 fmt::to_string(buffer));
  } else {
    spdlog::info("Present Mode: {}", vk::to_string(best));
  }

  return best;
}
vk::Extent2D VkContext::ChooseSwapExtent(vk::SurfaceCapabilitiesKHR &capacities) const {
  if (capacities.currentExtent.width != std::numeric_limits<uint32_t>::max()) {
    return capacities.currentExtent;
  } else {
    auto [width, height] = window_->GetWindowSize();
    width = std::clamp(width, capacities.minImageExtent.width, capacities.maxImageExtent.height);
    height = std::clamp(height, capacities.minImageExtent.height, capacities.maxImageExtent.height);
    return {width, height};
  }
}
void VkContext::CreateSwapchain() {
  auto support = QuerySwapChainSupport(physical_device_);
  auto format = ChooseSwapSurfaceFormat(support.formats);
  auto present_mode = ChooseSwapPresentMode(support.present_modes);
  auto extent = ChooseSwapExtent(support.capabilities);
  ACG_DEBUG_LOG("Extent: {}x{}", extent.width, extent.height);
  ACG_DEBUG_LOG("Present mode: {}", vk::to_string(present_mode));
  ACG_DEBUG_LOG("Format: {}, Colorspace: {}", vk::to_string(format.format),
                vk::to_string(format.colorSpace));

  if (swapchain_size_ > support.capabilities.maxImageCount
      && support.capabilities.maxImageCount != 0) {
    spdlog::warn("Not Support swapchain size {}, use {} instead.", swapchain_size_,
                 support.capabilities.maxImageCount);
    swapchain_size_ = support.capabilities.maxImageCount;
  }
  spdlog::info("Swapchain Size certificated = {}", swapchain_size_);

  vk::SwapchainCreateInfoKHR info;
  info.setSurface(surface_)
      .setMinImageCount(swapchain_size_)
      .setImageFormat(format.format)
      .setImageColorSpace(format.colorSpace)
      .setImageExtent(extent)
      .setImageArrayLayers(1)
      .setImageUsage(vk::ImageUsageFlagBits::eColorAttachment);

  auto queue_family_indices = std::array{device_related_indices_.graphics_family.value(),
                                         device_related_indices_.present_family.value()};

  if (queue_family_indices[0] != queue_family_indices[1]) {
    info.setImageSharingMode(vk::SharingMode::eConcurrent)
        .setQueueFamilyIndexCount(2)
        .setPQueueFamilyIndices(queue_family_indices.data());
  } else {
    info.setImageSharingMode(vk::SharingMode::eExclusive)
        .setQueueFamilyIndexCount(0)
        .setPQueueFamilyIndices(nullptr);
  }

  info.setPreTransform(support.capabilities.currentTransform)
      .setCompositeAlpha(vk::CompositeAlphaFlagBitsKHR::eOpaque)
      .setPresentMode(present_mode)
      .setClipped(VK_TRUE)
      .setOldSwapchain(VK_NULL_HANDLE);

  swapchain_ = device_.createSwapchainKHR(info);
  swapchain_images_ = device_.getSwapchainImagesKHR(swapchain_);
  swapchain_image_format_ = format.format;
  swapchain_extent_ = extent;
}

vk::ImageView VkContext::CreateImageView(vk::Image image, vk::Format format,
                                         vk::ImageAspectFlags aspectFlags) {
  vk::ImageViewCreateInfo view_info{};
  view_info.image = image;
  view_info.viewType = vk::ImageViewType::e2D;
  view_info.format = format;
  view_info.components.r = vk::ComponentSwizzle::eIdentity;
  view_info.components.g = vk::ComponentSwizzle::eIdentity;
  view_info.components.b = vk::ComponentSwizzle::eIdentity;
  view_info.components.a = vk::ComponentSwizzle::eIdentity;
  view_info.subresourceRange.aspectMask = aspectFlags;
  view_info.subresourceRange.baseMipLevel = 0;
  view_info.subresourceRange.levelCount = 1;
  view_info.subresourceRange.baseArrayLayer = 0;
  view_info.subresourceRange.layerCount = 1;
  return device_.createImageView(view_info);
}

void VkContext::CreateImageViews() {
  for (auto &image : swapchain_images_) {
    swapchain_image_views_.emplace_back(
        CreateImageView(image, swapchain_image_format_, vk::ImageAspectFlagBits::eColor));
  }
}

void VkContext::CleanupSwapchain() {
  for (const auto &view : swapchain_image_views_) {
    device_.destroy(view);
  }
  swapchain_image_views_.clear();
  device_.destroy(swapchain_);
  swapchain_ = VK_NULL_HANDLE;
}

void VkContext::CreateSyncObjects() {
  vk::SemaphoreCreateInfo semaphore_info;
  vk::FenceCreateInfo fence_info;
  fence_info.flags = vk::FenceCreateFlagBits::eSignaled;

  for (size_t i = 0; i < swapchain_size_; ++i) {
    semaphores_.emplace_back(RenderSyncObjects{device_.createSemaphore(semaphore_info),
                                               device_.createSemaphore(semaphore_info),
                                               device_.createFence(fence_info)});
  }
}

std::unique_ptr<VkContext::BufMem> VkContext::CreateBuffer(vk::DeviceSize size,
                                                           vk::BufferUsageFlags usage,
                                                           vk::MemoryPropertyFlags properties) {
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
  return std::make_unique<BufMem>(buf, mem, size);
}

uint32_t VkContext::FindMemoryType(uint32_t type_filter, vk::MemoryPropertyFlags properties) {
  auto memory_properties = physical_device_.getMemoryProperties();
  for (uint32_t i = 0; i < memory_properties.memoryTypeCount; ++i) {
    if ((type_filter & (1 << i))
        && (memory_properties.memoryTypes[i].propertyFlags & properties) == properties) {
      return i;
    }
  }
  spdlog::error("Cannot Find Memory type for {}", type_filter);
  exit(-1);
  return -1;
}

bool VkContext::BeginDraw() {
  ACG_CHECK(!draw_started_, "Draw has already started.");
  // Timeout = infinity.
  ACG_CHECK(device_.waitForFences(semaphores_[current_frame_].in_flight_fence, VK_TRUE,
                                  std::numeric_limits<uint64_t>::max())
                == vk::Result::eSuccess,
            "Failed to wait for fence.");

  // Acquire Image
  auto rv = device_.acquireNextImageKHR(swapchain_, std::numeric_limits<uint64_t>::max(),
                                        semaphores_[current_frame_].image_available);

  if (rv.result == vk::Result::eErrorOutOfDateKHR) {
    return false;
  }
  // Check acquire the image successfully.
  ACG_CHECK(rv.result == vk::Result::eSuccess || rv.result == vk::Result::eSuboptimalKHR,
            "Acquire next image failed, with result = {}", vk::to_string(rv.result));
  current_image_index_ = rv.value;
  device_.resetFences(semaphores_[current_frame_].in_flight_fence);
  draw_started_ = true;
  return true;
}

void VkContext::RecreateSwapchain() {
  // when the window is minimized, recreate the swapchain after the application is visiable.
  window_->UpdateWindowSize();
  device_.waitIdle();
  CleanupSwapchain();
  CreateSwapchain();
  CreateImageViews();
}

std::pair<vk::Image, vk::DeviceMemory> VkContext::CreateImage(uint32_t width, uint32_t height,
                                                              vk::Format format,
                                                              vk::ImageTiling tiling,
                                                              vk::ImageUsageFlags usage,
                                                              vk::MemoryPropertyFlags properties) {
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
  alloc_info.memoryTypeIndex = FindMemoryType(mem_requirements.memoryTypeBits, properties);

  auto mem = device_.allocateMemory(alloc_info);
  device_.bindImageMemory(image, mem, 0);
  return {image, mem};
}

void VkContext::TransitionImageLayout(vk::Image image, vk::Format /*format*/,
                                      vk::ImageLayout oldLayout, vk::ImageLayout newLayout) {
  vk::CommandBuffer command_buffer = BeginSingleTimeCommand();

  vk::ImageMemoryBarrier barrier{};
  barrier.oldLayout = oldLayout;
  barrier.newLayout = newLayout;
  barrier.srcQueueFamilyIndex = VK_QUEUE_FAMILY_IGNORED;
  barrier.dstQueueFamilyIndex = VK_QUEUE_FAMILY_IGNORED;
  barrier.image = image;
  barrier.subresourceRange.aspectMask = vk::ImageAspectFlagBits::eColor;
  barrier.subresourceRange.baseMipLevel = 0;
  barrier.subresourceRange.levelCount = 1;
  barrier.subresourceRange.baseArrayLayer = 0;
  barrier.subresourceRange.layerCount = 1;

  vk::PipelineStageFlags source_stage;
  vk::PipelineStageFlags destination_stage;

  if (oldLayout == vk::ImageLayout::eUndefined
      && newLayout == vk::ImageLayout::eTransferDstOptimal) {
    barrier.srcAccessMask = {};
    barrier.dstAccessMask = vk::AccessFlagBits::eTransferWrite;

    source_stage = vk::PipelineStageFlagBits::eTopOfPipe;
    destination_stage = vk::PipelineStageFlagBits::eTransfer;
  } else if (oldLayout == vk::ImageLayout::eTransferDstOptimal
             && newLayout == vk::ImageLayout::eShaderReadOnlyOptimal) {
    barrier.srcAccessMask = vk::AccessFlagBits::eTransferWrite;
    barrier.dstAccessMask = vk::AccessFlagBits::eShaderRead;

    source_stage = vk::PipelineStageFlagBits::eTransfer;
    destination_stage = vk::PipelineStageFlagBits::eFragmentShader;
  } else {
    ACG_CHECK(false, "Unsupported Layout");
  }

  command_buffer.pipelineBarrier(source_stage, destination_stage, {}, {}, {}, barrier);

  EndSingleTimeCommand(command_buffer);
}

vk::CommandBuffer VkContext::BeginSingleTimeCommand(vk::CommandPool command_pool) {
  if (!command_pool) {
    command_pool = command_pool_;
  }
  vk::CommandBufferAllocateInfo alloc_info{};
  alloc_info.level = vk::CommandBufferLevel::ePrimary;
  alloc_info.commandPool = command_pool;
  alloc_info.commandBufferCount = 1;
  auto buf = device_.allocateCommandBuffers(alloc_info).front();
  vk::CommandBufferBeginInfo begin_info{};
  begin_info.flags = vk::CommandBufferUsageFlagBits::eOneTimeSubmit;
  buf.begin(begin_info);
  return buf;
}

void VkContext::EndSingleTimeCommand(vk::CommandBuffer buffer, vk::CommandPool command_pool) {
  buffer.end();
  if (!command_pool) {
    command_pool = command_pool_;
  }
  vk::SubmitInfo submit_info;
  submit_info.setCommandBuffers(buffer);
  graphics_queue_.submit(submit_info);
  graphics_queue_.waitIdle();
  device_.freeCommandBuffers(command_pool, buffer);
}

void VkContext::CopyBuffer(vk::Buffer src, vk::Buffer dst, vk::DeviceSize size) {
  auto cmd_buf = BeginSingleTimeCommand();
  vk::BufferCopy copy_region;
  copy_region.setSize(size);
  cmd_buf.copyBuffer(src, dst, copy_region);
  EndSingleTimeCommand(cmd_buf);
}

vk::Format VkContext::FindSupportedFormat(const std::vector<vk::Format> &candidates,
                                          vk::ImageTiling tiling, vk::FormatFeatureFlags features) {
  for (const auto &format : candidates) {
    auto props = physical_device_.getFormatProperties(format);

    if (tiling == vk::ImageTiling::eLinear && (props.linearTilingFeatures & features) == features) {
      return format;
    } else if (tiling == vk::ImageTiling::eOptimal
               && (props.optimalTilingFeatures & features) == features) {
      return format;
    }
  }
  ACG_CHECK(false, "Failed to find a supported format");
  return {};
}

bool VkContext::HasStencilComponent(vk::Format format) {
  return format == vk::Format::eD32SfloatS8Uint || format == vk::Format::eD24UnormS8Uint;
}

void VkContext::CopyHostToBuffer(const void *mem_data, BufMem &buffer_with_memory, size_t size) {
  auto *data = device_.mapMemory(buffer_with_memory.GetMemory(), 0, buffer_with_memory.GetSize());
  memcpy(data, mem_data, size);
  device_.unmapMemory(buffer_with_memory.GetMemory());
}

bool VkContext::EndDrawSubmitPresent(std::vector<vk::CommandBuffer> command_buffers) {
  vk::SubmitInfo submit_info{};
  auto wait_sem = std::array{semaphores_[current_frame_].image_available};
  auto signal_sem = std::array{semaphores_[current_frame_].render_finished};
  vk::PipelineStageFlags wait_stages = {vk::PipelineStageFlagBits::eColorAttachmentOutput};
  submit_info.setWaitSemaphores(wait_sem)
      .setPWaitDstStageMask(&wait_stages)
      .setCommandBuffers(command_buffers)
      .setSignalSemaphores(signal_sem);
  graphics_queue_.submit(submit_info, semaphores_[current_frame_].in_flight_fence);

  vk::PresentInfoKHR present_info;
  present_info.setWaitSemaphores(signal_sem)
      .setSwapchains(swapchain_)
      .setPImageIndices(&current_image_index_);

  auto result = present_queue_.presentKHR(&present_info);
  bool need_recreate_swapchain{false};
  if (result == vk::Result::eErrorOutOfDateKHR || result == vk::Result::eSuboptimalKHR
      || window_->IsResized()) {
    window_->ResetResizeFlag();
    need_recreate_swapchain = true;
    ACG_DEBUG_LOG("Set Recreate Swapchain.");
  } else {
    ACG_CHECK(result == vk::Result::eSuccess, "Failed to present swapchain image");
  }
  current_frame_ = (current_frame_ + 1) % swapchain_size_;
  draw_started_ = false;
  return need_recreate_swapchain;
}

uint32_t VkContext::GetSwapchainSize() const { return swapchain_size_; }

uint32_t VkContext::GetAcquiredImageIndex() const { return current_image_index_; }

uint32_t VkContext::GetGraphicsFamily() const {
  return device_related_indices_.graphics_family.value();
}

uint32_t VkContext::GetPresentFamily() const {
  return device_related_indices_.present_family.value();
}

vk::Format VkContext::GetSwapchainImageFormat() const { return swapchain_image_format_; }

size_t VkContext::GetCurrentIndex() const { return current_frame_; }

vk::Extent2D VkContext::GetSwapchainExtent() const { return swapchain_extent_; }

std::vector<vk::ImageView> VkContext::GetSwapchainImageviews() const {
  return swapchain_image_views_;
}
const std::unique_ptr<Window> &VkContext::GetWindow() const { return window_; }

VkContext::BufMem::BufMem(vk::Buffer buffer, vk::DeviceMemory memory, vk::DeviceSize size)
    : buffer_(buffer), memory_(memory), size_(size) {}

vk::Buffer VkContext::BufMem::GetBuffer() { return buffer_; }

vk::DeviceMemory VkContext::BufMem::GetMemory() { return memory_; }

VkContext::BufMem::BufMem(BufMem &&m) {
  buffer_ = m.buffer_;
  m.buffer_ = nullptr;
  memory_ = m.memory_;
  m.memory_ = nullptr;
  size_ = m.size_;
  m.size_ = 0;
}

VkContext::BufMem::~BufMem() { Release(); }
vk::DeviceSize VkContext::BufMem::GetSize() const { return size_; }

void VkContext::BufMem::Release() {
  if (buffer_) {
    get_vk_context().GetDevice().destroy(buffer_);
    buffer_ = VK_NULL_HANDLE;
  }

  if (memory_) {
    get_vk_context().GetDevice().free(memory_);
    memory_ = VK_NULL_HANDLE;
  }
}

} 
VkContext &get_vk_context() { return acg::utils::Singleton<VkContext>().GetRef(); }
}  // namespace acg::gui
