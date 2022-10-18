#include "acg_visualizer/renderer.hpp"

#include <set>
#include <vector>
#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>
#include <co/fastream.h>

#include <acg_utils/debugger.hpp>
#include <acg_utils/log.hpp>
#include <acg_visualizer/scene.hpp>
#include <fstream>
#include <vulkan/vulkan_handles.hpp>

std::vector<char> read_file(std::string path) {
  std::ifstream input_file{path, std::ios::ate | std::ios::binary};
  ACG_CHECK(input_file.is_open(), "Failed to open file: {}", path);
  size_t buffer_size = input_file.tellg();

  input_file.seekg(0);
  std::vector<char> buffer(buffer_size);
  input_file.read(buffer.data(), buffer_size);
  ACG_CHECK(input_file, "Failed to read from file: {}", path);

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

  // Cleanup the semaphores
  for (auto &sync : semaphores_) {
    device_.destroy(sync.image_available);
    device_.destroy(sync.render_finished);
    device_.destroy(sync.in_flight_fence);
  }
  semaphores_.clear();

  for (auto &buf : uniform_buffers_) {
    device_.destroy(buf.buffer);
    device_.free(buf.memory);
  }
  uniform_buffers_.clear();

  // Clean Up Device
  device_.destroy(pipeline_layout_);
  device_.destroy(graphics_pipeline_);
  device_.destroy(descriptor_pool_);
  device_.destroy(ubo_layout_);
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
  if (is_inited_) {
    return;
  }
  window_ = std::make_unique<VkWindow>(title_);
  CreateInstance();
  SetupDebugMessenger();
  CreateSurface();
  PickPhysicalDevice();
  CreateLogicalDevice();

  // Swapchain, ImageViews
  CreateSwapchain();
  CreateImageViews();
  CreateCommandPool();
  CreateRenderPass();
  CreateDepthResources();

  // UBO Descriptor:
  CreateUboDescriptorLayout();
  // Graphics Pipeline.
  CreateGraphicsPipeline();

  // Init Buffers
  CreateUniformBuffers();
  CreateFramebuffers();
  CreateCommandBuffers();

  // Descriptor Pool and UBO Descriptors.
  CreateDescriptorPool();
  CreateUboDescriptorSets();
  // Command Buffer
  CreateCommandBuffers();
  // Sync
  CreateSyncObjects();

  // TODO: Vertex, index... not processed here.
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

vk::ImageView Renderer::CreateImageView(vk::Image image, vk::Format format,
                                        vk::ImageAspectFlags aspectFlags) {
  // TODO: Reformat.
  vk::ImageViewCreateInfo viewInfo{};
  viewInfo.image = image;
  viewInfo.viewType = vk::ImageViewType::e2D;
  viewInfo.format = format;
  viewInfo.subresourceRange.aspectMask = aspectFlags;
  viewInfo.subresourceRange.baseMipLevel = 0;
  viewInfo.subresourceRange.levelCount = 1;
  viewInfo.subresourceRange.baseArrayLayer = 0;
  viewInfo.subresourceRange.layerCount = 1;
  return device_.createImageView(viewInfo);
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
  // Setup Shaders.
  auto vert_shader_code = read_file(SPV_HOME "/3d.vert.spv");
  auto frag_shader_code = read_file(SPV_HOME "/3d.frag.spv");
  auto vert_module = CreateShaderModule(vert_shader_code);
  auto frag_module = CreateShaderModule(frag_shader_code);
  vk::PipelineShaderStageCreateInfo vert_stage_info;
  vert_stage_info.setStage(vk::ShaderStageFlagBits::eVertex)
      .setModule(vert_module)
      .setPName("main");
  vk::PipelineShaderStageCreateInfo frag_stage_info;
  frag_stage_info.setStage(vk::ShaderStageFlagBits::eFragment)
      .setModule(frag_module)
      .setPName("main");
  auto shader_stages = std::array{vert_stage_info, frag_stage_info};

  // Setup Vertex input
  vk::PipelineVertexInputStateCreateInfo vertex_input_create_info;
  auto vertex_binding_desc = Vertex::GetBindingDescriptions();
  auto vertex_attr_desc = Vertex::GetAttributeDescriptions();
  vertex_input_create_info.setVertexBindingDescriptions(vertex_binding_desc)
      .setVertexAttributeDescriptions(vertex_attr_desc);

  vk::PipelineInputAssemblyStateCreateInfo input_assembly_info;
  input_assembly_info.setTopology(vk::PrimitiveTopology::eTriangleList)
      .setPrimitiveRestartEnable(VK_FALSE);

  // Setup Viewport
  vk::PipelineViewportStateCreateInfo viewport_info;
  viewport_info.setViewportCount(1).setScissorCount(1);

  // Setup Rasterization
  vk::PipelineRasterizationStateCreateInfo rasterizer_info;
  rasterizer_info.setDepthBiasEnable(VK_TRUE)
      .setRasterizerDiscardEnable(VK_FALSE)
      .setLineWidth(1.0f)
      .setPolygonMode(vk::PolygonMode::eFill)          // TODO: Line Only mode support.
      .setCullMode(vk::CullModeFlagBits::eNone)        // TODO: Double mode.
      .setFrontFace(vk::FrontFace::eCounterClockwise)  // TODO: Clock wise support needed.
      .setDepthBiasEnable(VK_FALSE);

  // Setup Multi sampling: No multisampling for better performance.
  vk::PipelineMultisampleStateCreateInfo multi_sample_info;
  multi_sample_info.setSampleShadingEnable(VK_FALSE).setRasterizationSamples(
      vk::SampleCountFlagBits::e1);

  // Setup alpha blending: We do not use color blend. although options are set
  vk::PipelineColorBlendAttachmentState color_blend_attachment;
  color_blend_attachment
      .setColorWriteMask(vk::ColorComponentFlagBits::eR | vk::ColorComponentFlagBits::eG
                         | vk::ColorComponentFlagBits::eB | vk::ColorComponentFlagBits::eA)
      .setBlendEnable(VK_FALSE)
      .setSrcColorBlendFactor(vk::BlendFactor::eSrcAlpha)
      .setDstColorBlendFactor(vk::BlendFactor::eOneMinusSrcAlpha)
      .setColorBlendOp(vk::BlendOp::eAdd)
      .setSrcAlphaBlendFactor(vk::BlendFactor::eOne)
      .setDstAlphaBlendFactor(vk::BlendFactor::eZero)
      .setAlphaBlendOp(vk::BlendOp::eAdd);

  vk::PipelineColorBlendStateCreateInfo color_blend_info;
  color_blend_info.setLogicOpEnable(VK_FALSE)
      .setLogicOp(vk::LogicOp::eCopy)
      .setAttachments(color_blend_attachment)
      .setBlendConstants({0.0f, 0.0f, 0.0f, 0.0f});

  auto dynamic_states = std::array{vk::DynamicState::eViewport, vk::DynamicState::eScissor};
  vk::PipelineDynamicStateCreateInfo dynamic_state_info;
  dynamic_state_info.setDynamicStates(dynamic_states);

  vk::PipelineLayoutCreateInfo pipeline_layout_info;
  pipeline_layout_info.setSetLayouts(ubo_layout_).setPushConstantRangeCount(0);

  // Depth Testing.
  vk::PipelineDepthStencilStateCreateInfo depth_stencil;
  depth_stencil.setDepthTestEnable(VK_TRUE)
      .setDepthWriteEnable(VK_TRUE)
      .setDepthCompareOp(vk::CompareOp::eLess)
      .setDepthBoundsTestEnable(VK_FALSE)
      .setStencilTestEnable(VK_FALSE);

  pipeline_layout_ = device_.createPipelineLayout(pipeline_layout_info);
  vk::GraphicsPipelineCreateInfo info;
  info.setStages(shader_stages)
      .setPVertexInputState(&vertex_input_create_info)
      .setPInputAssemblyState(&input_assembly_info)
      .setPViewportState(&viewport_info)
      .setPRasterizationState(&rasterizer_info)
      .setPColorBlendState(&color_blend_info)
      .setPMultisampleState(&multi_sample_info)
      .setPDepthStencilState(&depth_stencil)
      .setPDynamicState(&dynamic_state_info)
      .setLayout(pipeline_layout_)
      .setRenderPass(render_pass_)
      .setSubpass(0)
      .setBasePipelineHandle(VK_NULL_HANDLE)
      .setBasePipelineIndex(-1);

  auto rv = device_.createGraphicsPipeline(VK_NULL_HANDLE, info);
  ACG_CHECK(rv.result == vk::Result::eSuccess, "Failed to create graphics pipeline");
  graphics_pipeline_ = rv.value;

  device_.destroy(vert_module);
  device_.destroy(frag_module);
}
void Renderer::CleanupSwapchain() {
  device_.destroy(depth_image_view_);
  device_.destroy(depth_image_);
  device_.free(depth_image_memory_);
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

  vk::AttachmentDescription depth_attachment;
  depth_attachment.setFormat(FindDepthFormat())
      .setSamples(vk::SampleCountFlagBits::e1)
      .setLoadOp(vk::AttachmentLoadOp::eClear)
      .setStoreOp(vk::AttachmentStoreOp::eDontCare)
      .setStencilStoreOp(vk::AttachmentStoreOp::eDontCare)
      .setStencilLoadOp(vk::AttachmentLoadOp::eDontCare)
      .setInitialLayout(vk::ImageLayout::eUndefined)
      .setFinalLayout(vk::ImageLayout::eDepthStencilAttachmentOptimal);

  vk::AttachmentReference depth_ref;
  depth_ref.setLayout(vk::ImageLayout::eDepthStencilAttachmentOptimal).setAttachment(1);

  vk::SubpassDescription subpass;
  subpass.setPipelineBindPoint(vk::PipelineBindPoint::eGraphics)
      .setColorAttachmentCount(1)
      .setPColorAttachments(&color_attachment_ref)
      .setPDepthStencilAttachment(&depth_ref);

  vk::SubpassDependency dependency;
  dependency.setSrcSubpass(VK_SUBPASS_EXTERNAL)
      .setDstSubpass(0)
      .setSrcStageMask(vk::PipelineStageFlagBits::eColorAttachmentOutput
                       | vk::PipelineStageFlagBits::eEarlyFragmentTests)
      .setSrcAccessMask(vk::AccessFlagBits::eNone)
      .setDstStageMask(vk::PipelineStageFlagBits::eColorAttachmentOutput
                       | vk::PipelineStageFlagBits::eEarlyFragmentTests)
      .setDstAccessMask(vk::AccessFlagBits::eColorAttachmentWrite
                        | vk::AccessFlagBits::eDepthStencilAttachmentWrite);

  vk::RenderPassCreateInfo info;
  auto attachments = std::array{color_attachment, depth_attachment};
  info.setAttachments(attachments).setSubpasses(subpass).setDependencies(dependency);
  render_pass_ = device_.createRenderPass(info);
}
void Renderer::CreateUboDescriptorLayout() {
  vk::DescriptorSetLayoutBinding ubo_layout_binding;
  ubo_layout_binding.setBinding(0)
      .setDescriptorCount(1)
      .setDescriptorType(vk::DescriptorType::eUniformBuffer)
      .setPImmutableSamplers(nullptr)
      .setStageFlags(vk::ShaderStageFlagBits::eVertex);

  vk::DescriptorSetLayoutCreateInfo layout_create_info;
  layout_create_info.setBindingCount(1).setBindings(ubo_layout_binding);

  ubo_layout_ = device_.createDescriptorSetLayout(layout_create_info);
}
void Renderer::CreateDescriptorPool() {
  vk::DescriptorPoolSize pool_size;
  pool_size.setType(vk::DescriptorType::eUniformBuffer).setDescriptorCount(swapchain_size_);

  vk::DescriptorPoolCreateInfo pool_create_info;
  pool_create_info.setPoolSizes(pool_size).setMaxSets(swapchain_size_);

  descriptor_pool_ = device_.createDescriptorPool(pool_create_info);
}
void Renderer::CreateSyncObjects() {
  vk::SemaphoreCreateInfo semaphore_info;
  vk::FenceCreateInfo fence_info;
  fence_info.flags = vk::FenceCreateFlagBits::eSignaled;

  for (size_t i = 0; i < swapchain_size_; ++i) {
    semaphores_.push_back({.image_available = device_.createSemaphore(semaphore_info),
                           .render_finished = device_.createSemaphore(semaphore_info),
                           .in_flight_fence = device_.createFence(fence_info)});
  }
}
void Renderer::CreateFramebuffers() {
  for (const auto &swapchain_image_view : swapchain_image_views_) {
    auto attachments = std::array{swapchain_image_view, depth_image_view_};
    vk::FramebufferCreateInfo frame_buffer_info;
    frame_buffer_info.setRenderPass(render_pass_)
        .setAttachments(attachments)
        .setWidth(swapchain_extent_.width)
        .setHeight(swapchain_extent_.height)
        .setLayers(1);
    swapchain_framebuffers_.emplace_back(device_.createFramebuffer(frame_buffer_info));
  }
}
void Renderer::CreateUboDescriptorSets() {
  std::vector<vk::DescriptorSetLayout> layouts(swapchain_size_, ubo_layout_);
  vk::DescriptorSetAllocateInfo alloc_info;
  alloc_info.setDescriptorPool(descriptor_pool_)
      .setSetLayouts(layouts)
      .setDescriptorSetCount(swapchain_size_);
  ubo_descriptor_sets_ = device_.allocateDescriptorSets(alloc_info);

  for (size_t i = 0; i < swapchain_size_; ++i) {
    vk::DescriptorBufferInfo buffer_info;
    buffer_info.buffer = uniform_buffers_[i].buffer;
    buffer_info.offset = 0;
    buffer_info.range = sizeof(Ubo);

    vk::WriteDescriptorSet descriptor_write;
    descriptor_write.dstSet = ubo_descriptor_sets_[i];
    descriptor_write.dstBinding = 0;
    descriptor_write.dstArrayElement = 0;
    descriptor_write.descriptorCount = 1;
    descriptor_write.descriptorType = vk::DescriptorType::eUniformBuffer;
    descriptor_write.pBufferInfo = &buffer_info;
    descriptor_write.pImageInfo = nullptr;
    descriptor_write.pTexelBufferView = nullptr;
    device_.updateDescriptorSets(descriptor_write, nullptr);
  }
}
void Renderer::CreateUniformBuffers() {
  vk::DeviceSize buf_size = sizeof(Ubo);
  uniform_buffers_.clear();
  for (size_t i = 0; i < swapchain_size_; ++i) {
    auto b = CreateBuffer(
        buf_size, vk::BufferUsageFlagBits::eUniformBuffer,
        vk::MemoryPropertyFlagBits::eHostVisible | vk::MemoryPropertyFlagBits::eHostCoherent);
    uniform_buffers_.emplace_back(b);
  }
}

Renderer::BufferWithMemory Renderer::CreateBuffer(vk::DeviceSize size, vk::BufferUsageFlags usage,
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
  return {buf, mem};
}

uint32_t Renderer::FindMemoryType(uint32_t type_filter, vk::MemoryPropertyFlags properties) {
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

vk::CommandBuffer Renderer::BeginDraw() {
  ACG_CHECK(!draw_started_, "Draw has already started.");
  // Timeout = infinity.
  ACG_CHECK(device_.waitForFences(semaphores_[current_frame_].in_flight_fence, VK_TRUE,
                                  std::numeric_limits<uint64_t>::max())
                == vk::Result::eSuccess,
            "Failed to wait for fence.");

  // Acquire Image
  auto rv
      = device_.acquireNextImageKHR(swapchain_, std::numeric_limits<uint64_t>::max(),
                                    semaphores_[current_frame_].image_available, VK_NULL_HANDLE);

  if (rv.result == vk::Result::eErrorOutOfDateKHR) {
    RecreateSwapchain();
    return nullptr;
  }

  ACG_CHECK(rv.result == vk::Result::eSuccess || rv.result == vk::Result::eSuboptimalKHR,
            "Acquire next image failed, with result = {}", vk::to_string(rv.result));
  current_image_index_ = rv.value;
  draw_started_ = true;
  device_.resetFences(semaphores_[current_frame_].in_flight_fence);
  command_buffers_[current_frame_].reset();
  vk::CommandBufferBeginInfo begin_info;
  begin_info.flags = vk::CommandBufferUsageFlagBits::eOneTimeSubmit;
  begin_info.pInheritanceInfo = nullptr;
  command_buffers_[current_frame_].begin(begin_info);

  return command_buffers_[current_frame_];
}

void Renderer::BeginRenderPass() {
  ACG_CHECK(draw_started_, "Draw not started, cannot begin render pass.");
  auto &cmd_buffer = command_buffers_[current_frame_];
  vk::RenderPassBeginInfo render_pass_info;
  render_pass_info.renderPass = render_pass_;
  render_pass_info.framebuffer = swapchain_framebuffers_[current_image_index_];
  render_pass_info.renderArea.offset.setX(0);
  render_pass_info.renderArea.offset.setY(0);
  render_pass_info.renderArea.extent = swapchain_extent_;
  auto clear_value
      = std::array<vk::ClearValue, 2>{background_color_, vk::ClearDepthStencilValue{{1.0f, 0}}};
  render_pass_info.setClearValues(clear_value);
  cmd_buffer.beginRenderPass(render_pass_info, vk::SubpassContents::eInline);
  // Do some basic stuff...
  cmd_buffer.bindPipeline(vk::PipelineBindPoint::eGraphics, graphics_pipeline_);
  vk::Viewport viewport(0.0, 0.0, swapchain_extent_.width, swapchain_extent_.height, 0.0, 1.0);
  cmd_buffer.setViewport(0, viewport);
  vk::Rect2D scissor{{0, 0}, swapchain_extent_};
  cmd_buffer.setScissor(0, scissor);
}

void Renderer::RecreateSwapchain() {
  // when the window is minimized, recreate the swapchain after the application is visiable.
  window_->UpdateWindowSize();
  device_.waitIdle();
  CleanupSwapchain();
  CreateSwapchain();
  CreateImageViews();
  CreateFramebuffers();
  CreateDepthResources();
}

std::pair<vk::Image, vk::DeviceMemory> Renderer::CreateImage(uint32_t width, uint32_t height,
                                                             vk::Format format,
                                                             vk::ImageTiling tiling,
                                                             vk::ImageUsageFlags usage,
                                                             vk::MemoryPropertyFlags properties) {
  vk::ImageCreateInfo imageInfo{};
  imageInfo.imageType = vk::ImageType::e2D;
  imageInfo.extent.width = width;
  imageInfo.extent.height = height;
  imageInfo.extent.depth = 1;
  imageInfo.mipLevels = 1;
  imageInfo.arrayLayers = 1;
  imageInfo.format = format;
  imageInfo.tiling = tiling;
  imageInfo.initialLayout = vk::ImageLayout::eUndefined;
  imageInfo.usage = usage;
  imageInfo.samples = vk::SampleCountFlagBits::e1;
  imageInfo.sharingMode = vk::SharingMode::eExclusive;

  auto image = device_.createImage(imageInfo);

  auto mem_requirements = device_.getImageMemoryRequirements(image);

  vk::MemoryAllocateInfo allocInfo{};
  allocInfo.allocationSize = mem_requirements.size;
  allocInfo.memoryTypeIndex = FindMemoryType(mem_requirements.memoryTypeBits, properties);

  auto mem = device_.allocateMemory(allocInfo);
  device_.bindImageMemory(image, mem, 0);
  return {image, mem};
}

void Renderer::TransitionImageLayout(vk::Image image, vk::Format format, vk::ImageLayout oldLayout,
                                     vk::ImageLayout newLayout) {
  vk::CommandBuffer commandBuffer = BeginSingleTimeCommands();

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

  vk::PipelineStageFlags sourceStage;
  vk::PipelineStageFlags destinationStage;

  if (oldLayout == vk::ImageLayout::eUndefined
      && newLayout == vk::ImageLayout::eTransferDstOptimal) {
    barrier.srcAccessMask = {};
    barrier.dstAccessMask = vk::AccessFlagBits::eTransferWrite;

    sourceStage = vk::PipelineStageFlagBits::eTopOfPipe;
    destinationStage = vk::PipelineStageFlagBits::eTransfer;
  } else if (oldLayout == vk::ImageLayout::eTransferDstOptimal
             && newLayout == vk::ImageLayout::eShaderReadOnlyOptimal) {
    barrier.srcAccessMask = vk::AccessFlagBits::eTransferWrite;
    barrier.dstAccessMask = vk::AccessFlagBits::eShaderRead;

    sourceStage = vk::PipelineStageFlagBits::eTransfer;
    destinationStage = vk::PipelineStageFlagBits::eFragmentShader;
  } else {
    ACG_CHECK(false, "Unsupported Layout");
  }

  commandBuffer.pipelineBarrier(sourceStage, destinationStage, {}, {}, {}, barrier);

  EndSingleTimeCommands(commandBuffer);
}

vk::CommandBuffer Renderer::BeginSingleTimeCommands() {
  vk::CommandBufferAllocateInfo allocInfo{};
  allocInfo.level = vk::CommandBufferLevel::ePrimary;
  allocInfo.commandPool = command_pool_;
  allocInfo.commandBufferCount = 1;

  auto buf = device_.allocateCommandBuffers(allocInfo).front();

  vk::CommandBufferBeginInfo beginInfo{};
  beginInfo.flags = vk::CommandBufferUsageFlagBits::eOneTimeSubmit;
  buf.begin(beginInfo);

  return buf;
}

void Renderer::EndSingleTimeCommands(vk::CommandBuffer buffer) {
  buffer.end();
  vk::SubmitInfo submit_info;
  submit_info.setCommandBuffers(buffer);
  graphics_queue_.submit(submit_info);
  graphics_queue_.waitIdle();
  device_.freeCommandBuffers(command_pool_, buffer);
}

void Renderer::CopyBuffer(vk::Buffer src, vk::Buffer dst, vk::DeviceSize size) {
  auto cmd_buf = BeginSingleTimeCommands();
  vk::BufferCopy copy_region;
  copy_region.setSize(size);
  cmd_buf.copyBuffer(src, dst, copy_region);
  EndSingleTimeCommands(cmd_buf);
}
void Renderer::CreateDepthResources() {
  auto depth_format = FindDepthFormat();
  std::tie(depth_image_, depth_image_memory_) = CreateImage(
      swapchain_extent_.width, swapchain_extent_.height, depth_format, vk::ImageTiling::eOptimal,
      vk::ImageUsageFlagBits::eDepthStencilAttachment, vk::MemoryPropertyFlagBits::eDeviceLocal);

  depth_image_view_ = CreateImageView(depth_image_, depth_format, vk::ImageAspectFlagBits::eDepth);
}

vk::Format Renderer::FindSupportedFormat(const std::vector<vk::Format> &candidates,
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

vk::Format Renderer::FindDepthFormat() {
  return FindSupportedFormat(
      {vk::Format::eD32Sfloat, vk::Format::eD32SfloatS8Uint, vk::Format::eD24UnormS8Uint},
      vk::ImageTiling::eOptimal, vk::FormatFeatureFlagBits::eDepthStencilAttachment);
}

bool Renderer::HasStencilComponent(vk::Format format) {
  return format == vk::Format::eD32SfloatS8Uint || format == vk::Format::eD24UnormS8Uint;
}

}  // namespace acg::visualizer::details
