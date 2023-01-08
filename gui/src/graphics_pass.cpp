#include "acg_gui/backend/graphics_pass.hpp"

#include <fstream>

#include "acg_gui/backend/context.hpp"
#include "acg_gui/backend/graphics_context.hpp"
#include "acg_gui/convent.hpp"

namespace acg::gui::details {

GraphicsRenderPass::GraphicsRenderPass(GraphicsRenderPass::InitConfig config)
    : init_config_(config) {
  Init();
}

GraphicsRenderPass::~GraphicsRenderPass() { Destroy(); }

void GraphicsRenderPass::Init() {
  CreateRenderPass();
  CreateDepthResources();
  CreateFramebuffers();
  CreateCommandBuffers();
  CreateDescriptorPool();
}

void GraphicsRenderPass::CreateRenderPass() {
  vk::AttachmentDescription color_attachment;
  color_attachment.setFormat(VkGraphicsContext::Instance().swapchain_image_format_)
      .setSamples(vk::SampleCountFlagBits::e1)
      .setLoadOp(vk::AttachmentLoadOp::eClear)
      .setStoreOp(vk::AttachmentStoreOp::eStore)
      .setStencilLoadOp(vk::AttachmentLoadOp::eDontCare)
      .setStencilStoreOp(vk::AttachmentStoreOp::eDontCare)
      .setInitialLayout(vk::ImageLayout::eUndefined)
      .setFinalLayout(vk::ImageLayout::eColorAttachmentOptimal);
  if (init_config_.is_present) {
    color_attachment.setFinalLayout(vk::ImageLayout::ePresentSrcKHR);
  } else {
    color_attachment.setFinalLayout(vk::ImageLayout::eColorAttachmentOptimal);
  }
  vk::AttachmentReference color_attachment_ref;
  color_attachment_ref.setAttachment(0).setLayout(vk::ImageLayout::eColorAttachmentOptimal);

  vk::AttachmentDescription depth_attachment;
  depth_attachment.setFormat(FindDepthFormat())
      .setSamples(vk::SampleCountFlagBits::e1)
      .setLoadOp(vk::AttachmentLoadOp::eClear)
      .setStoreOp(vk::AttachmentStoreOp::eStore)
      .setStencilStoreOp(vk::AttachmentStoreOp::eDontCare)
      .setStencilLoadOp(vk::AttachmentLoadOp::eDontCare)
      .setInitialLayout(vk::ImageLayout::eUndefined)
      .setFinalLayout(vk::ImageLayout::eDepthStencilAttachmentOptimal);

  vk::AttachmentReference depth_ref;
  depth_ref.setLayout(vk::ImageLayout::eDepthStencilAttachmentOptimal).setAttachment(1);

  vk::SubpassDescription subpass;
  subpass.setPipelineBindPoint(vk::PipelineBindPoint::eGraphics)
      .setColorAttachmentCount(1)
      .setPDepthStencilAttachment(&depth_ref)
      .setPColorAttachments(&color_attachment_ref);

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
  auto result = VkContext2::Instance().device_.createRenderPass(&info, nullptr, &render_pass_);
  ACG_CHECK(result == vk::Result::eSuccess, "Failed to create graphics render pass: {}",
            vk::to_string(result));
}

void GraphicsRenderPass::CreateDepthResources() {
  auto depth_format = FindDepthFormat();
  auto extent = VkGraphicsContext::Instance().swapchain_extent_;
  ACG_DEBUG_LOG("Depth format = {}", vk::to_string(depth_format));
  auto result = VkContext2::Instance().CreateImage(
      extent.width, extent.height, depth_format, vk::ImageTiling::eOptimal,
      vk::ImageUsageFlagBits::eDepthStencilAttachment, vk::MemoryPropertyFlagBits::eDeviceLocal);
  ACG_CHECK(result, "Failed to create depth image");
  std::tie(depth_image_, depth_image_memory_) = result.Value();
  depth_image_view_ = VkGraphicsContext::Instance().CreateImageView(
      depth_image_, depth_format, vk::ImageAspectFlagBits::eDepth);
}

void GraphicsRenderPass::CreateFramebuffers() {
  auto extent = VkGraphicsContext::Instance().swapchain_extent_;
  for (auto imageview : VkGraphicsContext::Instance().swapchain_image_views_) {
    auto attachments = std::array{imageview, depth_image_view_};
    vk::FramebufferCreateInfo info;
    info.setRenderPass(render_pass_)
        .setAttachments(attachments)
        .setWidth(extent.width)
        .setHeight(extent.height)
        .setLayers(1);
    framebuffers_.emplace_back(VkContext2::Instance().device_.createFramebuffer(info));
  }
}

void GraphicsRenderPass::CreateCommandBuffers() {
  vk::CommandBufferAllocateInfo info;
  info.setCommandPool(VkGraphicsContext::Instance().graphics_command_pool_)
      .setCommandBufferCount(VkGraphicsContext::Instance().swapchain_size_)
      .setLevel(vk::CommandBufferLevel::ePrimary);
  command_buffers_ = VkContext2::Instance().device_.allocateCommandBuffers(info);
}

void GraphicsRenderPass::CreateDescriptorPool() {
  vk::DescriptorPoolCreateInfo pool_create_info;
  pool_create_info.setPoolSizes(init_config_.required_descriptor_sizes)
      .setMaxSets(init_config_.max_descriptor_set_count);
  ub_descriptor_pool_ = VkContext2::Instance().device_.createDescriptorPool(pool_create_info);
}

void GraphicsRenderPass::Destroy() {
  DestroySwapchain();
  auto device = VkContext2::Instance().device_;
  device.destroy(ub_descriptor_pool_);
  device.destroy(render_pass_);
}

void GraphicsRenderPass::DestroySwapchain() {
  auto device = VkContext2::Instance().device_;
  // Depth Resources
  device.destroy(depth_image_);
  device.destroy(depth_image_view_);
  device.free(depth_image_memory_);
  // Frame Buffers
  for (auto fb : framebuffers_) {
    device.destroyFramebuffer(fb);
  }
  framebuffers_.clear();
}

void GraphicsRenderPass::RecreateSwapchain() {
  DestroySwapchain();
  CreateDepthResources();
  CreateFramebuffers();
}

vk::CommandBuffer &GraphicsRenderPass::BeginRender() {
  ACG_CHECK(!is_begin_, "Render pass has begin.");
  auto extent = VkGraphicsContext::Instance().swapchain_extent_;
  auto current_index = VkGraphicsContext::Instance().current_frame_;
  auto &current_command_buffer = command_buffers_[current_index];
  auto clear_value = std::array<vk::ClearValue, 2>{background_color_, depth_stencil_value_};

  // Begin Command Buffer
  vk::CommandBufferBeginInfo begin_info;
  begin_info.setFlags(vk::CommandBufferUsageFlagBits::eSimultaneousUse);
  current_command_buffer.begin(begin_info);
  // Begin Render Pass
  vk::RenderPassBeginInfo render_pass_info;
  render_pass_info.renderPass = render_pass_;
  render_pass_info.framebuffer = framebuffers_[VkGraphicsContext::Instance().current_image_index_];
  render_pass_info.renderArea.extent = extent;
  render_pass_info.renderArea.offset.setX(0);
  render_pass_info.renderArea.offset.setY(0);
  render_pass_info.setClearValues(clear_value);
  current_command_buffer.beginRenderPass(render_pass_info, vk::SubpassContents::eInline);
  is_begin_ = true;
  return current_command_buffer;
}

vk::CommandBuffer &GraphicsRenderPass::EndRender() {
  ACG_CHECK(is_begin_, "Render pass has not begin.");
  auto &current_command_buffer = command_buffers_[VkGraphicsContext::Instance().current_frame_];
  current_command_buffer.endRenderPass();
  current_command_buffer.end();
  is_begin_ = false;
  return current_command_buffer;
}

vk::CommandBuffer &GraphicsRenderPass::GetCurrentFrameCommandBuffer() {
  return command_buffers_[VkGraphicsContext::Instance().current_frame_];
}

vk::Format GraphicsRenderPass::FindDepthFormat() {
  acg::Result<vk::Format> depth_format = VkContext2::Instance().FindSupportedFormat(
      {vk::Format::eD32Sfloat, vk::Format::eD32SfloatS8Uint, vk::Format::eD24UnormS8Uint},
      vk::ImageTiling::eOptimal, vk::FormatFeatureFlagBits::eDepthStencilAttachment);
  ACG_CHECK(depth_format.HasValue(), "Failed to find depth format.");
  return depth_format.Value();
}

}  // namespace acg::gui::details
