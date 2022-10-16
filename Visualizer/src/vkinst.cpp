#include "acg_visualizer/vkinst.hpp"

#include <set>
#include <vector>
#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>
#include <co/fastream.h>

#include <acg_utils/debugger.hpp>
#include <acg_utils/log.hpp>

VKAPI_ATTR VkResult VKAPI_CALL vkCreateDebugUtilsMessengerEXT(
    VkInstance instance, const VkDebugUtilsMessengerCreateInfoEXT *pCreateInfo,
    const VkAllocationCallbacks *pAllocator, VkDebugUtilsMessengerEXT *pMessenger) {
  auto func = (PFN_vkCreateDebugUtilsMessengerEXT)vkGetInstanceProcAddr(
      instance, "vkCreateDebugUtilsMessengerEXT");
  if (func != nullptr) {
    return func(instance, pCreateInfo, pAllocator, pMessenger);
  } else {
    return VK_ERROR_EXTENSION_NOT_PRESENT;
  }
}

VKAPI_ATTR void VKAPI_CALL
vkDestroyDebugUtilsMessengerEXT(VkInstance instance, VkDebugUtilsMessengerEXT messenger,
                                VkAllocationCallbacks const *pAllocator) {
  auto func = (PFN_vkDestroyDebugUtilsMessengerEXT)vkGetInstanceProcAddr(
      instance, "vkDestroyDebugUtilsMessengerEXT");
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

  switch (messageSeverity) {
    case VK_DEBUG_UTILS_MESSAGE_SEVERITY_ERROR_BIT_EXT:
      spdlog::error("{}", buffer.data());
      break;
    case VK_DEBUG_UTILS_MESSAGE_SEVERITY_WARNING_BIT_EXT:
      spdlog::warn("{}", buffer.data());
      break;
    case VK_DEBUG_UTILS_MESSAGE_SEVERITY_INFO_BIT_EXT:
      spdlog::info("{}", buffer.data());
      break;
    case VK_DEBUG_UTILS_MESSAGE_SEVERITY_VERBOSE_BIT_EXT:
      spdlog::debug("{}", buffer.data());
      break;
    default:
      // Not valid bit.
      break;
  };

  return false;
}

namespace acg::visualizer::details {

const std::vector<const char *> validation_layers = {"VK_LAYER_KHRONOS_validation"};

Visualizer::Visualizer(bool init) {
  if (init) {
    Init();
  }
}

Visualizer::~Visualizer() {
  device_.waitIdle();
  // Clean Up Device
  CleanupSwapchain();

  device_.destroyCommandPool(command_pool_);
  device_.destroy();

  // Clean Up Instance
  if (enable_validation) {
    instance_.destroy(debug_messenger_);
  }
  instance_.destroy(surface_);
  instance_.destroy();
}

void Visualizer::Init() {
  window_ = std::make_unique<VkWindow>(title_);
  CreateInstance();
  SetupDebugMessenger();
  CreateSurface();
  PickPhysicalDevice();
  CreateLogicalDevice();
  CreateCommandPool();
  CreateSwapchain();
  CreateImageViews();
  CreateRenderPass();
  CreateDescriptorLayout();
  CreateGraphicsPipeline();
  CreateFramebuffers();
  CreateVertexBuffer();
  CreateIndexBuffer();
  CreateUniformBuffer();
  CreateDescriptorPool();
  CreateDescriptorSets();
  CreateCommandBuffers();
  CreateSyncObjects();
}

void Visualizer::CreateInstance() {
  if (enable_validation) {
    // TODO: test validation support.
    ACG_CHECK(IsValidationSupport() == true,
              "No Validation suppport, please turn off validation layer!");
  }

  vk::ApplicationInfo appInfo;
  appInfo.pApplicationName = "ACG Vulkan Viewer";
  appInfo.applicationVersion = VK_MAKE_VERSION(1, 0, 0);
  appInfo.pEngineName = "No Engine";
  appInfo.engineVersion = VK_MAKE_VERSION(1, 0, 0);
  appInfo.apiVersion = VK_API_VERSION_1_3;

  vk::InstanceCreateInfo createInfo;
  createInfo.pApplicationInfo = &appInfo;

  // Setup extensions for vulkan instance.
  auto extensions = GetRequiredExtensions();
  createInfo.enabledExtensionCount = static_cast<uint32_t>(extensions.size());
  createInfo.ppEnabledExtensionNames = extensions.data();
  if constexpr (utils::get_platform_type() == utils::PlatformType::kApple) {
    createInfo.flags = vk::InstanceCreateFlagBits::eEnumeratePortabilityKHR;
  } else {
    /* Do nothing, only mac need the flag above. */
  }

  // Setup Debug Messenger.
  if (enable_validation) {
    createInfo.enabledLayerCount = static_cast<uint32_t>(validation_layers.size());
    createInfo.ppEnabledLayerNames = validation_layers.data();
  } else {
    createInfo.enabledLayerCount = 0;
    createInfo.pNext = nullptr;
  }

  instance_ = vk::createInstance(createInfo);
}

std::vector<const char *> Visualizer::GetRequiredExtensions() const {
  uint32_t glfwExtensionCount = 0;
  const char **glfwExtensions;
  glfwExtensions = glfwGetRequiredInstanceExtensions(&glfwExtensionCount);
  std::vector<const char *> extensions(glfwExtensions, glfwExtensions + glfwExtensionCount);
  if (enable_validation) {
    extensions.push_back(VK_EXT_DEBUG_UTILS_EXTENSION_NAME);
  }
  if constexpr (utils::get_platform_type() == utils::PlatformType::kApple) {
    extensions.push_back(VK_KHR_PORTABILITY_ENUMERATION_EXTENSION_NAME);
  }

  return extensions;
}

bool Visualizer::IsValidationSupport() {
  auto available_layers = vk::enumerateInstanceLayerProperties();
  for (const auto &layer_name : validation_layers) {
    bool found = false;
    for (const auto &layer_prop : available_layers) {
      if (std::strcmp(layer_name, layer_prop.layerName)) {
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

void Visualizer::SetupDebugMessenger() {
  if (!enable_validation) return;
  vk::DebugUtilsMessengerCreateInfoEXT info = PopulateDebugMessengerCreateInfo();
  debug_messenger_ = instance_.createDebugUtilsMessengerEXT(info);
}

vk::DebugUtilsMessengerCreateInfoEXT Visualizer::PopulateDebugMessengerCreateInfo() {
  vk::DebugUtilsMessengerCreateInfoEXT info;
  info.messageSeverity = vk::DebugUtilsMessageSeverityFlagBitsEXT::eError
                         | vk::DebugUtilsMessageSeverityFlagBitsEXT::eInfo;
  info.messageType = vk::DebugUtilsMessageTypeFlagBitsEXT::eGeneral
                     | vk::DebugUtilsMessageTypeFlagBitsEXT::ePerformance
                     | vk::DebugUtilsMessageTypeFlagBitsEXT::eValidation;
  info.pfnUserCallback = debug_message_callback;
  info.pUserData = nullptr;  // Optional
  return info;
}

void Visualizer::CreateSurface() { surface_ = window_->CreateWindowSurface(instance_); }

void Visualizer::PickPhysicalDevice() {
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

bool Visualizer::IsSuitable(const vk::PhysicalDevice &physical_device) const {
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

bool Visualizer::CheckDeviceExtensionSupport(const vk::PhysicalDevice &physical_device) const {
  auto extensions = physical_device.enumerateDeviceExtensionProperties();
  std::set<std::string_view> required{device_extension_enabled.begin(),
                                      device_extension_enabled.end()};
  for (const auto &ext : extensions) {
    required.erase(ext.extensionName);
  }
  return required.empty();
}

QueueFamilyIndices Visualizer::FindQueueFamilies(const vk::PhysicalDevice &physical_device) const {
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

SwapChainSupportDetails Visualizer::QuerySwapChainSupport(
    const vk::PhysicalDevice &physical_device) const {
  SwapChainSupportDetails details;
  details.capabilities = physical_device.getSurfaceCapabilitiesKHR(surface_);
  details.formats = physical_device.getSurfaceFormatsKHR(surface_);
  details.present_modes = physical_device.getSurfacePresentModesKHR(surface_);
  return details;
}

void Visualizer::CreateLogicalDevice() {
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
  std::vector<const char *> enabled_extension = device_extension_enabled;
  if constexpr (utils::get_platform_type() == utils::PlatformType::kApple) {
    // For apple users, we need vk_khr portablities enabled.
    enabled_extension.push_back("VK_KHR_portability_subset");
  }
  device_create_info.enabledExtensionCount = enabled_extension.size();
  device_create_info.ppEnabledExtensionNames = enabled_extension.data();

  // Setup Validation Layer to enabled layers.
  if (enable_validation) {
    device_create_info.enabledLayerCount = static_cast<uint32_t>(validation_layers.size());
    device_create_info.ppEnabledLayerNames = validation_layers.data();
  } else {
    device_create_info.enabledLayerCount = 0;
  }

  // create logical device from physical device.
  device_ = physical_device_.createDevice(device_create_info);

  // get the queue of logical device.
  graphics_queue_ = device_.getQueue(indices.graphics_family.value(), 0);
  present_queue_ = device_.getQueue(indices.present_family.value(), 0);
  spdlog::info("Created Vulkan device. graphics_family={} present_family={}",
               indices.graphics_family.value(), indices.present_family.value());
}

void Visualizer::CreateCommandPool() {
  vk::CommandPoolCreateInfo pool_info;
  pool_info.queueFamilyIndex = device_related_indices_.graphics_family.value();
  pool_info.flags = vk::CommandPoolCreateFlagBits::eResetCommandBuffer
                    | vk::CommandPoolCreateFlagBits::eTransient;
  command_pool_ = device_.createCommandPool(pool_info);
}

Visualizer Visualizer::VkInstBuilder::Build() const {
  Visualizer inst(false);
  inst.enable_validation = enable_validation;
  inst.Init();
  return std::move(inst);
}

bool Visualizer::RunOnce() {
  if (window_->ShouldClose()) {
    return false;
  }
  glfwPollEvents();
  return true;
}

void Visualizer::CreateSwapchain() {
  SwapChainSupportDetails support_details = QuerySwapChainSupport(physical_device_);
  auto format = ChooseSurfaceFormat(support_details.formats);
  auto present_mode = ChooseSwapPresentMode(support_details.present_modes);
  auto extent = ChooseSwapExtent(support_details.capabilities);

  // Enable triple buffering.
  uint32_t image_count = support_details.capabilities.minImageCount + 1;
  if (support_details.capabilities.maxImageCount > 0
      && image_count == support_details.capabilities.maxImageCount) {
    image_count = support_details.capabilities.maxImageCount;
  }

  vk::SwapchainCreateInfoKHR info;
  info.surface = surface_;
  info.minImageCount = image_count;
  info.imageFormat = format.format;
  info.imageColorSpace = format.colorSpace;
  info.imageExtent = extent;
  info.imageArrayLayers = 1;  // layer for each image.(> 1 for VR apps.)
  info.imageUsage = vk::ImageUsageFlagBits::eColorAttachment;

  // if graphics and present are not same, we have to use concurrent
  // sharing mode.
  auto indices = FindQueueFamilies(physical_device_);
  std::array<uint32_t, 2> queue_family_indices
      = {indices.graphics_family.value(), indices.present_family.value()};
  if (indices.graphics_family.value() != indices.present_family.value()) {
    info.imageSharingMode = vk::SharingMode::eConcurrent;
    info.queueFamilyIndexCount = 2;
    info.pQueueFamilyIndices = queue_family_indices.data();
  } else {
    info.imageSharingMode = vk::SharingMode::eExclusive;
    info.queueFamilyIndexCount = 0;
    info.pQueueFamilyIndices = nullptr;
  }

  // Static Transform Operation applied on image in swapchain.
  info.preTransform = support_details.capabilities.currentTransform;

  // enable alpha.
  info.compositeAlpha = vk::CompositeAlphaFlagBitsKHR::eOpaque;

  // set present mode.
  info.presentMode = present_mode;

  // we won't process the covered pixels.
  info.clipped = VK_TRUE;

  // after resize the window size, swapchain should be reconstructred.
  // reconstruring need old swapchain, here just left nullptr.
  info.oldSwapchain = VK_NULL_HANDLE;

  // Create swapchain.
  swapchain_ = device_.createSwapchainKHR(info);
  swapchain_images_ = device_.getSwapchainImagesKHR(swapchain_);
  swapchain_image_format_ = format.format;
  swapchain_extent_ = extent;
}
vk::SurfaceFormatKHR Visualizer::ChooseSurfaceFormat(
    const std::vector<vk::SurfaceFormatKHR> &available_formats) const {
  if (available_formats.size() == 1 && available_formats.front() == vk::Format::eUndefined) {
    return {vk::Format::eB8G8R8A8Unorm, vk::ColorSpaceKHR::eSrgbNonlinear};
  }

  for (const auto &format : available_formats) {
    if (format.format == vk::Format::eB8G8R8A8Unorm
        && format.colorSpace == vk::ColorSpaceKHR::eSrgbNonlinear) {
      return format;
    }
  }

  return available_formats.front();
}

vk::PresentModeKHR Visualizer::ChooseSwapPresentMode(
    const std::vector<vk::PresentModeKHR> &available_present_modes) const {
  auto best = vk::PresentModeKHR::eFifo;
  for (const auto &mode : available_present_modes) {
    if (mode == vk::PresentModeKHR::eMailbox) {
      return mode;
    } else if (mode == vk::PresentModeKHR::eImmediate) {
      best = vk::PresentModeKHR::eImmediate;
    }
  }
  return best;
}

vk::Extent2D Visualizer::ChooseSwapExtent(const vk::SurfaceCapabilitiesKHR &capabilities) const {
  if (capabilities.currentExtent.width != std::numeric_limits<uint32_t>::max()) {
    return capabilities.currentExtent;
  } else {
    vk::Extent2D actual = {window_->GetWindowSize().first, window_->GetWindowSize().second};
    actual.width = std::max(capabilities.minImageExtent.width,
                            std::min(capabilities.maxImageExtent.width, actual.width));
    actual.height = std::max(capabilities.minImageExtent.height,
                             std::min(capabilities.maxImageExtent.height, actual.height));
    return actual;
  }
}

void Visualizer::CleanupSwapchain() {
  for (const auto &buf : swapchain_framebuffers_) {
    device_.destroy(buf);
  }
  swapchain_framebuffers_.clear();
  for (const auto &view : swapchain_image_views_) {
    device_.destroyImageView(view);
  }
  swapchain_image_views_.clear();
  device_.destroy(swapchain_);
}


void Visualizer::CreateImageViews() {
  for (auto &swapchain_image : swapchain_images_) {
    vk::ImageViewCreateInfo info;
    info.image = swapchain_image;
    info.viewType = vk::ImageViewType::e2D;
    info.format = swapchain_image_format_;
    info.components.r = vk::ComponentSwizzle::eIdentity;
    info.components.g = vk::ComponentSwizzle::eIdentity;
    info.components.b = vk::ComponentSwizzle::eIdentity;
    info.components.a = vk::ComponentSwizzle::eIdentity;
    info.subresourceRange.aspectMask = vk::ImageAspectFlagBits::eColor;
    info.subresourceRange.baseMipLevel = 0;
    info.subresourceRange.levelCount = 1;
    info.subresourceRange.baseArrayLayer = 0;
    info.subresourceRange.layerCount = 1;
    swapchain_image_views_.push_back(device_.createImageView(info));
  }
}

void Visualizer::CreateRenderPass() {
  vk::AttachmentDescription color_attachment;
  color_attachment.format = swapchain_image_format_;
  color_attachment.samples = vk::SampleCountFlagBits::e1;
  color_attachment.loadOp = vk::AttachmentLoadOp::eClear;
  color_attachment.storeOp = vk::AttachmentStoreOp::eStore;

  // after rendering, the content will not be copy back.
  color_attachment.stencilLoadOp = vk::AttachmentLoadOp::eDontCare;
  color_attachment.stencilStoreOp = vk::AttachmentStoreOp::eDontCare;

  color_attachment.initialLayout = vk::ImageLayout::eUndefined;
  color_attachment.finalLayout = vk::ImageLayout::ePresentSrcKHR;

  vk::AttachmentReference color_attachment_ref;
  color_attachment_ref.attachment = 0;
  color_attachment_ref.layout = vk::ImageLayout::eAttachmentOptimal;

  vk::SubpassDescription subpass;
  subpass.pipelineBindPoint = vk::PipelineBindPoint::eGraphics;
  subpass.colorAttachmentCount = 1;
  subpass.pColorAttachments = &color_attachment_ref;

  vk::SubpassDependency dependency;
  dependency.srcSubpass = VK_SUBPASS_EXTERNAL;
  dependency.dstSubpass = 0;
  dependency.srcStageMask = vk::PipelineStageFlagBits::eColorAttachmentOutput;
  dependency.srcAccessMask = vk::AccessFlagBits::eNone;
  dependency.dstStageMask = vk::PipelineStageFlagBits::eColorAttachmentOutput;
  dependency.dstAccessMask = vk::AccessFlagBits::eColorAttachmentWrite;

  vk::RenderPassCreateInfo render_pass_info;
  render_pass_info.attachmentCount = 1;
  render_pass_info.pAttachments = &color_attachment;
  render_pass_info.subpassCount = 1;
  render_pass_info.pSubpasses = &subpass;
  render_pass_info.dependencyCount = 1;
  render_pass_info.pDependencies = &dependency;

  render_pass_ = device_.createRenderPass(render_pass_info);
}

void VulkanRender::CreateDescriptorLayout() {
  vk::DescriptorSetLayoutBinding ubo_layout_binding;
  ubo_layout_binding.binding = 0;
  ubo_layout_binding.descriptorType = vk::DescriptorType::eUniformBuffer;
  ubo_layout_binding.descriptorCount = 1;
  ubo_layout_binding.pImmutableSamplers = nullptr;
  ubo_layout_binding.stageFlags = vk::ShaderStageFlagBits::eVertex;

  vk::DescriptorSetLayoutCreateInfo layout_info;
  layout_info.bindingCount = 1;
  layout_info.pBindings = &ubo_layout_binding;

  descriptor_set_layout_ = device_.createDescriptorSetLayout(layout_info);
}

void Visualizer::CreateUniformBuffer() {
  vk::DeviceSize buf_size = sizeof(UniformBufferObject);
  uniform_buffers_.clear();
  uniform_buffers_memory_.clear();
  for (size_t i = 0; i < MAX_FRAMES_IN_FLIGHT; ++i) {
    auto [b, m] = CreateBuffer(
        buf_size, vk::BufferUsageFlagBits::eUniformBuffer,
        vk::MemoryPropertyFlagBits::eHostVisible | vk::MemoryPropertyFlagBits::eHostCoherent);
    uniform_buffers_.emplace_back(b);
    uniform_buffers_memory_.emplace_back(m);
  }
}

}  // namespace acg::visualizer::details