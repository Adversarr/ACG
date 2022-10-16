#include "acg_visualizer/renderer.hpp"

#include <set>
#include <vector>
#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>
#include <co/fastream.h>

#include <acg_utils/debugger.hpp>
#include <acg_utils/log.hpp>
#include <fstream>

std::vector<char> read_file(std::string path) {
  std::ifstream input_file{path, std::ios::ate | std::ios::binary};
  ACG_CHECK(input_file.is_open(), "Failed to open file.");
  size_t buffer_size = input_file.tellg();

  input_file.seekg(0);
  std::vector<char> buffer(buffer_size);
  input_file.read(buffer.data(), buffer_size);
  ACG_CHECK(input_file, "Failed to read from file: " + path);

  input_file.close();  // (optional) Explicitly close input file
  return buffer;
}

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

Renderer::Renderer(bool init) {
  if (init) {
    Init();
  }
}

Renderer::~Renderer() {
  device_.waitIdle();
  CleanupSwapchain();

  // Clean Up Device
  device_.destroy(descriptor_pool_);
  device_.destroy(descriptor_set_layout_);
  device_.destroy(render_pass_);
  device_.destroyCommandPool(command_pool_);
  device_.destroy();

  // Clean Up Instance
  if (enable_validation) {
    instance_.destroy(debug_messenger_);
  }
  instance_.destroy(surface_);
  instance_.destroy();

  window_ = nullptr;
}

void Renderer::Init() {
  window_ = std::make_unique<VkWindow>(title_);
  CreateInstance();
  SetupDebugMessenger();
  CreateSurface();
  PickPhysicalDevice();
  CreateLogicalDevice();
  CreateCommandPool();
  CreateSwapchain();
  CreateCommandBuffers();
  CreateImageViews();
  CreateRenderPass();
  CreateDescriptorLayout();
  CreateDescriptorPool();
  // TODO: CreateDescriptorLayout.
  CreateGraphicsPipeline();
//  CreateFramebuffers();
  // TODO: Vertex, index, ubo...
//  CreateSyncObjects();

  is_inited_ = true;
}

void Renderer::CreateInstance() {
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

std::vector<const char *> Renderer::GetRequiredExtensions() const {
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

bool Renderer::IsValidationSupport() {
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

void Renderer::SetupDebugMessenger() {
  if (!enable_validation) return;
  vk::DebugUtilsMessengerCreateInfoEXT info = PopulateDebugMessengerCreateInfo();
  debug_messenger_ = instance_.createDebugUtilsMessengerEXT(info);
}

vk::DebugUtilsMessengerCreateInfoEXT Renderer::PopulateDebugMessengerCreateInfo() {
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

void Renderer::CreateSurface() { surface_ = window_->CreateWindowSurface(instance_); }

void Renderer::PickPhysicalDevice() {
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

bool Renderer::IsSuitable(const vk::PhysicalDevice &physical_device) const {
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

bool Renderer::CheckDeviceExtensionSupport(const vk::PhysicalDevice &physical_device) const {
  auto extensions = physical_device.enumerateDeviceExtensionProperties();
  std::set<std::string_view> required{device_extension_enabled.begin(),
                                      device_extension_enabled.end()};
  for (const auto &ext : extensions) {
    required.erase(ext.extensionName);
  }
  return required.empty();
}

QueueFamilyIndices Renderer::FindQueueFamilies(const vk::PhysicalDevice &physical_device) const {
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

SwapChainSupportDetails Renderer::QuerySwapChainSupport(
    const vk::PhysicalDevice &physical_device) const {
  SwapChainSupportDetails details;
  details.capabilities = physical_device.getSurfaceCapabilitiesKHR(surface_);
  details.formats = physical_device.getSurfaceFormatsKHR(surface_);
  details.present_modes = physical_device.getSurfacePresentModesKHR(surface_);
  return details;
}

void Renderer::CreateLogicalDevice() {
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
  auto res = physical_device_.createDevice(&device_create_info, nullptr, &device_);

  // get the queue of logical device.
  graphics_queue_ = device_.getQueue(indices.graphics_family.value(), 0);
  present_queue_ = device_.getQueue(indices.present_family.value(), 0);
  spdlog::info("Created Vulkan device (Result: {}). graphics_family={} present_family={}",
               vk::to_string(res), indices.graphics_family.value(), indices.present_family.value());
}

void Renderer::CreateCommandPool() {
  vk::CommandPoolCreateInfo pool_info;
  pool_info.queueFamilyIndex = device_related_indices_.graphics_family.value();
  pool_info.flags = vk::CommandPoolCreateFlagBits::eResetCommandBuffer
                    | vk::CommandPoolCreateFlagBits::eTransient;
  command_pool_ = device_.createCommandPool(pool_info);
}

std::unique_ptr<Renderer> Renderer::Builder::Build() const {
  auto inst = std::make_unique<Renderer>(false);
  inst->enable_validation = enable_validation;
  inst->swapchain_size_ = swapchain_size;
  inst->Init();
  return inst;
}
Renderer::Builder &Renderer::Builder::SetValidation(bool enable) {
  enable_validation = enable;
  return *this;
}
Renderer::Builder &Renderer::Builder::SetSwapchainSize(uint32_t size) {
  if (size < 2) {
    spdlog::error("Cannot set swapchain size less than 2, set to default 3");
    size = 3;
  }
  swapchain_size = size;
  return *this;
}

bool Renderer::RunOnce() {
  if (window_->ShouldClose()) {
    return false;
  }
  glfwPollEvents();
  return true;
}

const vk::Instance &Renderer::GetInstance() const { return instance_; }
const vk::Device &Renderer::GetDevice() const { return device_; }
const vk::PhysicalDevice &Renderer::GetPhysicalDevice() const { return physical_device_; }
const vk::SurfaceKHR &Renderer::GetSurface() const { return surface_; }
const vk::Queue &Renderer::GetGraphicsQueue() const { return graphics_queue_; }
const vk::Queue &Renderer::GetPresentQueue() const { return present_queue_; }
const vk::CommandPool &Renderer::GetCommandPool() const { return command_pool_; }
vk::SurfaceFormatKHR Renderer::ChooseSwapSurfaceFormat(
    const std::vector<vk::SurfaceFormatKHR> &formats) const {
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

  spdlog::info("Not using optimal format, use format: {}, color space: {}",
               vk::to_string(formats.front().format), vk::to_string(formats.front().colorSpace));
  return formats.front();
}

vk::PresentModeKHR Renderer::ChooseSwapPresentMode(
    const std::vector<vk::PresentModeKHR> &modes) const {
  auto best = vk::PresentModeKHR::eFifo;
  for (const auto &mode : modes) {
    if (mode == vk::PresentModeKHR::eMailbox) {
      best = mode;
      break;
    }

    if (mode == vk::PresentModeKHR::eImmediate) {
      best = mode;
      break;
    }
  }
  if (best != vk::PresentModeKHR::eMailbox) {
    spdlog::warn("Not using mailbox present mode, now: {}", vk::to_string(best));
  } else {
    spdlog::info("Present Mode: {}", vk::to_string(best));
  }

  return best;
}
vk::Extent2D Renderer::ChooseSwapExtent(vk::SurfaceCapabilitiesKHR &capacities) const {
  if (capacities.currentExtent.width != std::numeric_limits<uint32_t>::max()) {
    return capacities.currentExtent;
  } else {
    auto [width, height] = window_->GetWindowSize();
    width = std::clamp(width, capacities.minImageExtent.width, capacities.maxImageExtent.height);
    height = std::clamp(height, capacities.minImageExtent.height, capacities.maxImageExtent.height);
    return {width, height};
  }
}
void Renderer::CreateSwapchain() {
  auto support = QuerySwapChainSupport(physical_device_);
  auto format = ChooseSwapSurfaceFormat(support.formats);
  auto present_mode = ChooseSwapPresentMode(support.present_modes);
  auto extent = ChooseSwapExtent(support.capabilities);

  if (swapchain_size_ > support.capabilities.maxImageCount
      && support.capabilities.maxImageCount != 0) {
    spdlog::warn("Not Support swapchain size {}, use {} instead.", swapchain_size_,
                 support.capabilities.maxImageCount);
    swapchain_size_ = support.capabilities.maxImageCount;
  }

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
      .setOldSwapchain(nullptr);

  swapchain_ = device_.createSwapchainKHR(info);
  swapchain_images_ = device_.getSwapchainImagesKHR(swapchain_);
  swapchain_image_format_ = format.format;
  swapchain_extent_ = extent;
}
void Renderer::CreateImageViews() {
  for (auto &image : swapchain_images_) {
    vk::ImageViewCreateInfo info;
    info.setImage(image)
        .setViewType(vk::ImageViewType::e2D)
        .setFormat(swapchain_image_format_)
        .setComponents(vk::ComponentMapping{})  // default is identity.
        .setSubresourceRange(
            vk::ImageSubresourceRange{vk::ImageAspectFlagBits::eColor, 0, 1, 0, 1});
    swapchain_image_views_.emplace_back(device_.createImageView(info));
  }
}
void Renderer::CreateGraphicsPipeline() {
  auto vert_shader_code = read_file(SPV_HOME "/vert.spv");
  auto frag_shader_code = read_file(SPV_HOME "/frag.spv");

  auto vert_module = CreateShaderModule(vert_shader_code);
  auto frag_module = CreateShaderModule(frag_shader_code);

  vk::PipelineShaderStageCreateInfo vert_stage_info;
  vert_stage_info.setStage(vk::ShaderStageFlagBits::eVertex)
      .setModule(vert_module)
      .setPName("main");

  // TODO: Need more implementation.

  device_.destroy(vert_module);
  device_.destroy(frag_module);
}
void Renderer::CleanupSwapchain() {
  for (const auto &buf : swapchain_framebuffers_) {
    device_.destroy(buf);
  }
  swapchain_framebuffers_.clear();
  for (const auto &view : swapchain_image_views_) {
    device_.destroy(view);
  }
  swapchain_image_views_.clear();
  device_.destroy(swapchain_);
  swapchain_ = nullptr;
}
void Renderer::CreateCommandBuffers() {
  auto allocate_info = vk::CommandBufferAllocateInfo{}
                           .setCommandPool(command_pool_)
                           .setCommandBufferCount(swapchain_size_)
                           .setLevel(vk::CommandBufferLevel::ePrimary);
  command_buffers_ = device_.allocateCommandBuffers(allocate_info);
}
vk::ShaderModule Renderer::CreateShaderModule(const std::vector<char> &code) const {
  vk::ShaderModuleCreateInfo info;
  info.setPCode((uint32_t *)code.data());
  info.setCodeSize(code.size());
  return device_.createShaderModule(info);
}
void Renderer::CreateRenderPass() {
  vk::AttachmentDescription color_attachment;
  color_attachment.setFormat(swapchain_image_format_)
      .setSamples(vk::SampleCountFlagBits::e1)
      .setLoadOp(vk::AttachmentLoadOp::eClear)
      .setStoreOp(vk::AttachmentStoreOp::eStore)
      .setStencilLoadOp(vk::AttachmentLoadOp::eDontCare)
      .setStencilStoreOp(vk::AttachmentStoreOp::eDontCare)
      .setInitialLayout(vk::ImageLayout::eUndefined)
      .setFinalLayout(vk::ImageLayout::ePresentSrcKHR);

  vk::AttachmentReference color_attachment_ref;
  color_attachment_ref.setAttachment(0).setLayout(vk::ImageLayout::eAttachmentOptimal);

  vk::SubpassDescription subpass;
  subpass.setPipelineBindPoint(vk::PipelineBindPoint::eGraphics)
      .setColorAttachmentCount(1)
      .setPColorAttachments(&color_attachment_ref);

  vk::SubpassDependency dependency;
  dependency.setSrcSubpass(VK_SUBPASS_EXTERNAL)
      .setDstSubpass(0)
      .setSrcStageMask(vk::PipelineStageFlagBits::eColorAttachmentOutput)
      .setSrcAccessMask(vk::AccessFlagBits::eNone)
      .setDstStageMask(vk::PipelineStageFlagBits::eColorAttachmentOutput)
      .setDstAccessMask(vk::AccessFlagBits::eColorAttachmentWrite);

  vk::RenderPassCreateInfo info;
  info.setAttachments(color_attachment).setSubpasses(subpass).setDependencies(dependency);
  render_pass_ = device_.createRenderPass(info);
}
void Renderer::CreateDescriptorLayout() {
  vk::DescriptorSetLayoutBinding ubo_layout_binding;
  ubo_layout_binding.setBinding(0)
      .setDescriptorCount(1)
      .setDescriptorType(vk::DescriptorType::eUniformBuffer)
      .setPImmutableSamplers(nullptr)
      .setStageFlags(vk::ShaderStageFlagBits::eVertex);

  vk::DescriptorSetLayoutCreateInfo layout_create_info;
  layout_create_info.setBindingCount(1).setBindings(ubo_layout_binding);

  descriptor_set_layout_ = device_.createDescriptorSetLayout(layout_create_info);
}
void Renderer::CreateDescriptorPool() {
  vk::DescriptorPoolSize pool_size;
  pool_size.setType(vk::DescriptorType::eUniformBuffer)
      .setDescriptorCount(swapchain_size_);

  vk::DescriptorPoolCreateInfo pool_create_info;
  pool_create_info.setPoolSizes(pool_size)
      .setMaxSets(swapchain_size_);

  descriptor_pool_ = device_.createDescriptorPool(pool_create_info);
}

}  // namespace acg::visualizer::details
