#include "acg_gui/backend/graphics_pass.hpp"

#include <fstream>

#include "acg_gui/convent.hpp"

namespace acg::gui::details {

GraphicsRenderPass::GraphicsRenderPass() { Init(); }

void GraphicsRenderPass::Init() {
  if (is_inited_) {
    return;
  }
  CreateCommandPool();
  CreateRenderPass();
  CreateDescriptorSetLayout();
  CreateDepthResources();
  CreateFramebuffers();
  CreateCommandBuffers();
  CreateDescriptorPool();
  CreateDescriptorSets();
  is_inited_ = true;
}

void GraphicsRenderPass::CreateCommandPool() {
  vk::CommandPoolCreateInfo pool_info;
  pool_info.setQueueFamilyIndex(get_vk_context().GetGraphicsFamily())
      .setFlags(vk::CommandPoolCreateFlagBits::eResetCommandBuffer);
  command_pool_ = get_vk_context().GetDevice().createCommandPool(pool_info);
}

void GraphicsRenderPass::CreateRenderPass() {
  vk::AttachmentDescription color_attachment;
  color_attachment.setFormat(get_vk_context().GetSwapchainImageFormat())
      .setSamples(vk::SampleCountFlagBits::e1)
      .setLoadOp(vk::AttachmentLoadOp::eClear)
      .setStoreOp(vk::AttachmentStoreOp::eStore)
      .setStencilLoadOp(vk::AttachmentLoadOp::eDontCare)
      .setStencilStoreOp(vk::AttachmentStoreOp::eDontCare)
      .setInitialLayout(vk::ImageLayout::eUndefined)
      .setFinalLayout(vk::ImageLayout::eColorAttachmentOptimal);

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
  render_pass_ = get_vk_context().GetDevice().createRenderPass(info);
}

void GraphicsRenderPass::CreateDescriptorSetLayout() {
  vk::DescriptorSetLayoutBinding ubo_layout_binding;
  ubo_layout_binding.setBinding(0)
      .setDescriptorCount(1)
      .setDescriptorType(vk::DescriptorType::eUniformBuffer)
      .setPImmutableSamplers(nullptr)
      .setStageFlags(vk::ShaderStageFlagBits::eVertex | vk::ShaderStageFlagBits::eFragment);

  vk::DescriptorSetLayoutCreateInfo layout_create_info;
  layout_create_info.setBindings(ubo_layout_binding);
  descriptor_set_layout_
      = get_vk_context().GetDevice().createDescriptorSetLayout(layout_create_info);
}

void GraphicsRenderPass::CreateDepthResources() {
  auto depth_format = FindDepthFormat();
  ACG_DEBUG_LOG("Depth format = {}", vk::to_string(depth_format));
  std::tie(depth_image_, depth_image_memory_) = get_vk_context().CreateImage(
      get_vk_context().GetSwapchainExtent().width, get_vk_context().GetSwapchainExtent().height,
      depth_format, vk::ImageTiling::eOptimal, vk::ImageUsageFlagBits::eDepthStencilAttachment,
      vk::MemoryPropertyFlagBits::eDeviceLocal);
  depth_image_view_ = get_vk_context().CreateImageView(depth_image_, depth_format,
                                                       vk::ImageAspectFlagBits::eDepth);
}

void GraphicsRenderPass::CreateFramebuffers() {
  for (auto imageview : get_vk_context().GetSwapchainImageviews()) {
    auto attachments = std::array{imageview, depth_image_view_};

    vk::FramebufferCreateInfo info;
    info.setRenderPass(render_pass_)
        .setAttachments(attachments)
        .setWidth(get_vk_context().GetSwapchainExtent().width)
        .setHeight(get_vk_context().GetSwapchainExtent().height)
        .setLayers(1);
    framebuffers_.emplace_back(get_vk_context().GetDevice().createFramebuffer(info));
  }
}

void GraphicsRenderPass::CreateCommandBuffers() {
  vk::CommandBufferAllocateInfo info;
  info.setCommandPool(command_pool_)
      .setCommandBufferCount(get_vk_context().GetSwapchainSize())
      .setLevel(vk::CommandBufferLevel::ePrimary);
  command_buffers_ = get_vk_context().GetDevice().allocateCommandBuffers(info);
}

void GraphicsRenderPass::CreateDescriptorPool() {
  vk::DescriptorPoolSize pool_size;
  pool_size.setType(vk::DescriptorType::eUniformBuffer)
      .setDescriptorCount(get_vk_context().GetSwapchainSize());

  vk::DescriptorPoolCreateInfo pool_create_info;
  pool_create_info.setPoolSizes(pool_size).setMaxSets(get_vk_context().GetSwapchainSize());

  descriptor_pool_ = get_vk_context().GetDevice().createDescriptorPool(pool_create_info);
}

void GraphicsRenderPass::CreateDescriptorSets() {
  std::vector<vk::DescriptorSetLayout> layouts(get_vk_context().GetSwapchainSize(),
                                               descriptor_set_layout_);
  vk::DescriptorSetAllocateInfo alloc_info;
  alloc_info.setDescriptorPool(descriptor_pool_)
      .setSetLayouts(layouts)
      .setDescriptorSetCount(get_vk_context().GetSwapchainSize());
  descriptor_sets_ = get_vk_context().GetDevice().allocateDescriptorSets(alloc_info);

  for (size_t i = 0; i < get_vk_context().GetSwapchainSize(); ++i) {
    vk::DescriptorBufferInfo buffer_info;
    buffer_info.setBuffer(uniform_buffers_[i]->GetBuffer()).setOffset(0).setRange(sizeof(Ubo));
    vk::WriteDescriptorSet desc_write;
    desc_write.setDstSet(descriptor_sets_[i])
        .setDstBinding(0)
        .setDstArrayElement(0)
        .setDescriptorCount(1)
        .setDescriptorType(vk::DescriptorType::eUniformBuffer)
        .setPBufferInfo(&buffer_info);
    get_vk_context().GetDevice().updateDescriptorSets(desc_write, {});
  }
}

void GraphicsRenderPass::Cleanup() {
  if (!is_inited_) {
    return;
  }
  CleanupSwapchain();
  auto device = get_vk_context().GetDevice();
  for (auto &buf : uniform_buffers_) {
    buf->Release();
  }
  uniform_buffers_.clear();
  device.destroy(descriptor_set_layout_);
  device.destroy(descriptor_pool_);
  device.destroy(render_pass_);
  device.destroy(command_pool_);
}

void GraphicsRenderPass::CleanupSwapchain() {
  auto device = get_vk_context().GetDevice();
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
  CleanupSwapchain();
  CreateDepthResources();
  CreateFramebuffers();
}

vk::CommandBuffer &GraphicsRenderPass::BeginRender() {
  ACG_CHECK(!is_render_pass_begin_, "Render pass has begin.");
  auto extent = get_vk_context().GetSwapchainExtent();
  auto current_index = get_vk_context().GetCurrentIndex();
  auto &current_command_buffer = command_buffers_[current_index];
  auto clear_value = std::array<vk::ClearValue, 2>{background_color_, depth_stencil_value_};

  // Begin Command Buffer
  vk::CommandBufferBeginInfo begin_info;
  begin_info.setFlags(vk::CommandBufferUsageFlagBits::eSimultaneousUse);
  current_command_buffer.begin(begin_info);
  // Begin Render Pass
  vk::RenderPassBeginInfo render_pass_info;
  render_pass_info.renderPass = render_pass_;
  render_pass_info.framebuffer = framebuffers_[get_vk_context().GetAcquiredImageIndex()];
  render_pass_info.renderArea.extent = extent;
  render_pass_info.renderArea.offset.setX(0);
  render_pass_info.renderArea.offset.setY(0);
  render_pass_info.setClearValues(clear_value);
  current_command_buffer.beginRenderPass(render_pass_info, vk::SubpassContents::eInline);
  is_render_pass_begin_ = true;
  return current_command_buffer;
}

vk::CommandBuffer &GraphicsRenderPass::EndRender() {
  ACG_CHECK(is_render_pass_begin_, "Render pass has not begin.");
  auto &current_command_buffer = command_buffers_[get_vk_context().GetCurrentIndex()];
  current_command_buffer.endRenderPass();
  current_command_buffer.end();
  is_render_pass_begin_ = false;
  return current_command_buffer;
}
}  // namespace acg::gui::details
