#include "acg_gui/backend/graphics_context.hpp"

#include "acg_core/init.hpp"
#include "acg_gui/backend/context.hpp"
#include "acg_gui/backend/window.hpp"
#include "acg_utils/log.hpp"
namespace acg::gui {

std::unique_ptr<VkGraphicsContext> graphics_context_instance;

void VkGraphicsContext::CreateSwapchain(bool verbose) {
  auto &support = VkContext2::Instance().system_info_.physical_device_info;
  auto format = ChooseSwapSurfaceFormat(support.surface_formats);
  auto present_mode = ChooseSwapPresentMode(support.surface_present_modes, verbose);
  auto extent = ChooseSwapExtent(support.surface_capabilities);
  if (verbose) {
    ACG_INFO("Extent: {}x{}", extent.width, extent.height);
    ACG_INFO("Present mode: {}", vk::to_string(present_mode));
    ACG_INFO("Format: {}, Colorspace: {}", vk::to_string(format.format),
             vk::to_string(format.colorSpace));
  }

  auto max_sc_size = support.surface_capabilities.maxImageCount;
  if (swapchain_size_ > max_sc_size && max_sc_size != 0) {
    ACG_WARN("Required Swapchain size is greater than max supported, {} > {}", swapchain_size_,
             max_sc_size);
    swapchain_size_ = max_sc_size;
  }

  vk::SwapchainCreateInfoKHR info;
  info.setSurface(VkContext2::Instance().surface_)
      .setMinImageCount(swapchain_size_)
      .setImageFormat(format.format)
      .setImageColorSpace(format.colorSpace)
      .setImageExtent(extent)
      .setImageArrayLayers(1)
      .setImageUsage(vk::ImageUsageFlagBits::eColorAttachment);
  auto queue_family_indices
      = std::array{support.graphics_family.value(), support.present_family.value()};
  if (queue_family_indices[0] != queue_family_indices[1]) {
    info.setImageSharingMode(vk::SharingMode::eConcurrent)
        .setQueueFamilyIndices(queue_family_indices);
  } else {
    info.setImageSharingMode(vk::SharingMode::eExclusive);
  }

  // TODO: old swapchain available here.
  info.setPreTransform(support.surface_capabilities.currentTransform)
      .setCompositeAlpha(vk::CompositeAlphaFlagBitsKHR::eOpaque)
      .setPresentMode(present_mode)
      .setClipped(VK_TRUE)
      .setOldSwapchain(VK_NULL_HANDLE);

  auto &device = VkContext2::Instance().device_;
  auto result = VkContext2::Instance().device_.createSwapchainKHR(&info, nullptr, &swapchain_);
  ACG_CHECK(result == vk::Result::eSuccess, "Failed to create swapchain, reason: {}",
            vk::to_string(result));

  swapchain_images_ = device.getSwapchainImagesKHR(swapchain_);
  swapchain_image_format_ = format.format;
  swapchain_extent_ = extent;
}

vk::SurfaceFormatKHR VkGraphicsContext::ChooseSwapSurfaceFormat(
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

vk::PresentModeKHR VkGraphicsContext::ChooseSwapPresentMode(
    const std::vector<vk::PresentModeKHR> &modes, bool verbose) {
  auto best = vk::PresentModeKHR::eFifo;
  for (const auto &mode : modes) {
    if (mode == vk::PresentModeKHR::eMailbox) {
      best = mode;
      break;
    }
  }
  if (best != vk::PresentModeKHR::eMailbox) {
    fmt::memory_buffer buffer;
    for (const auto &mode : modes) {
      fmt::format_to(std::back_inserter(buffer), "{} ", vk::to_string(mode));
    }

    if (verbose) {
      ACG_INFO("Not using mailbox present mode, now: {}, available: {}", vk::to_string(best),
               fmt::to_string(buffer));
    }
  } else {
    if (verbose) {
      ACG_INFO("Present Mode: {}", vk::to_string(best));
    }
  }
  return best;
}

vk::Extent2D VkGraphicsContext::ChooseSwapExtent(vk::SurfaceCapabilitiesKHR &capacities) {
  if (capacities.currentExtent.width != std::numeric_limits<uint32_t>::max()) {
    return capacities.currentExtent;
  } else {
    auto [width, height] = Window::Instance().GetWindowSize();
    width = std::clamp(width, capacities.minImageExtent.width, capacities.maxImageExtent.height);
    height = std::clamp(height, capacities.minImageExtent.height, capacities.maxImageExtent.height);
    return {width, height};
  }
}

void VkGraphicsContext::CreateCommandPool() {
  vk::CommandPoolCreateInfo info;
  info.setQueueFamilyIndex(
      VkContext2::Instance().system_info_.physical_device_info.graphics_family.value());
  info.setFlags(vk::CommandPoolCreateFlagBits::eResetCommandBuffer);
  auto result
      = VkContext2::Instance().device_.createCommandPool(&info, nullptr, &graphics_command_pool_);
  ACG_CHECK(result == vk::Result::eSuccess, "Cannot create command pool.");
}

void VkGraphicsContext::CreateImageViews() {
  for (auto &image : swapchain_images_) {
    swapchain_image_views_.emplace_back(
        CreateImageView(image, swapchain_image_format_, vk::ImageAspectFlagBits::eColor));
  }
}

vk::ImageView VkGraphicsContext::CreateImageView(vk::Image image, vk::Format format,
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
  return VkContext2::Instance().device_.createImageView(view_info);
}

void VkGraphicsContext::CreateSyncObjects() {
  vk::SemaphoreCreateInfo semaphore_info;
  vk::FenceCreateInfo fence_info;
  fence_info.flags = vk::FenceCreateFlagBits::eSignaled;

  auto &device = VkContext2::Instance().device_;
  for (size_t i = 0; i < swapchain_size_; ++i) {
    syncs_.emplace_back(RenderSyncObjects{device.createSemaphore(semaphore_info),
                                          device.createSemaphore(semaphore_info),
                                          device.createFence(fence_info)});
  }
}

void VkGraphicsContext::CleanupSwapchain() {
  auto &device = VkContext2::Instance().device_;
  for (const auto &view : swapchain_image_views_) {
    device.destroy(view);
  }
  swapchain_image_views_.clear();
  device.destroy(swapchain_);
  swapchain_ = VK_NULL_HANDLE;
}

void VkGraphicsContext::RecreateSwapchain(bool verbose) {
  Window::Instance().UpdateWindowSize();
  VkContext2::Instance().device_.waitIdle();
  CleanupSwapchain();
  CreateSwapchain(verbose);
  CreateImageViews();
}

bool VkGraphicsContext::BeginDraw() {
  auto &device = VkContext2::Instance().device_;
  ACG_CHECK(!draw_started_, "Draw has already started.");
  // Timeout = infinity.
  ACG_CHECK(device.waitForFences(syncs_[current_frame_].in_flight_fence, VK_TRUE,
                                 std::numeric_limits<uint64_t>::max())
                == vk::Result::eSuccess,
            "Failed to wait for fence.");

  // Acquire Image
  auto rv = device.acquireNextImageKHR(swapchain_, std::numeric_limits<uint64_t>::max(),
                                       syncs_[current_frame_].image_available);

  if (rv.result == vk::Result::eErrorOutOfDateKHR) {
    return false;
  }
  // Check acquire the image successfully.
  ACG_CHECK(rv.result == vk::Result::eSuccess || rv.result == vk::Result::eSuboptimalKHR,
            "Acquire next image failed, with result = {}", vk::to_string(rv.result));
  current_image_index_ = rv.value;
  device.resetFences(syncs_[current_frame_].in_flight_fence);
  draw_started_ = true;
  return true;
}

vk::CommandBuffer VkGraphicsContext::BeginSingleTimeCommand() const {
  vk::CommandBufferAllocateInfo alloc_info{};
  alloc_info.level = vk::CommandBufferLevel::ePrimary;
  alloc_info.commandPool = graphics_command_pool_;
  alloc_info.commandBufferCount = 1;
  auto buf = VkContext2::Instance().device_.allocateCommandBuffers(alloc_info).front();
  vk::CommandBufferBeginInfo begin_info{};
  begin_info.flags = vk::CommandBufferUsageFlagBits::eOneTimeSubmit;
  buf.begin(begin_info);
  return buf;
}

void VkGraphicsContext::EndSingleTimeCommand(vk::CommandBuffer buffer) const {
  buffer.end();
  vk::SubmitInfo submit_info;
  submit_info.setCommandBuffers(buffer);
  VkContext2::Instance().graphics_queue_.submit(submit_info);
  VkContext2::Instance().graphics_queue_.waitIdle();
  VkContext2::Instance().device_.freeCommandBuffers(graphics_command_pool_, buffer);
}
void VkGraphicsContext::TransitionImageLayout(vk::Image image, vk::Format /*format*/,
                                              vk::ImageLayout oldLayout,
                                              vk::ImageLayout newLayout) const {
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

void VkGraphicsContext::CopyBufferToBuffer(vk::Buffer src, vk::Buffer dst,
                                           vk::DeviceSize size) const {
  auto cmd_buf = BeginSingleTimeCommand();
  vk::BufferCopy copy_region;
  copy_region.setSize(size);
  cmd_buf.copyBuffer(src, dst, copy_region);
  EndSingleTimeCommand(cmd_buf);
}

void VkGraphicsContext::Hooker::Hook() {
  acg::details::InitHook hook;
  hook.on_init = []() { graphics_context_instance.reset(new VkGraphicsContext); };
  hook.on_exit = []() { graphics_context_instance.reset(); };
  hook.name = "Vk Graphics Context Init";
  hook.priority = 8;
  acg::details::add_hook(hook);
}

VkGraphicsContext::VkGraphicsContext() {
  CreateCommandPool();
  CreateSwapchain();
  CreateImageViews();
  CreateSyncObjects();
}

VkGraphicsContext::~VkGraphicsContext() {
  CleanupSwapchain();
  auto device = VkContext2::Instance().device_;
  for (auto &sync : syncs_) {
    device.destroy(sync.image_available);
    device.destroy(sync.render_finished);
    device.destroy(sync.in_flight_fence);
  }
  syncs_.clear();
  device.destroyCommandPool(graphics_command_pool_);
}

}  // namespace acg::gui
