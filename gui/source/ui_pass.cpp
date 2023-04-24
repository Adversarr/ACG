#include "agui/backend/ui_pass.hpp"

#include "agui/backend/context.hpp"
#include "agui/backend/graphics_context.hpp"
#include "agui/backend/window.hpp"
#include "agui/imnodes/imnodes.hpp"
#include "autils/log.hpp"
#include "imgui_impl_glfw.h"
#include "imgui_impl_vulkan.h"

static void check_vk_result(VkResult err) {
  ACG_CHECK(err == VK_SUCCESS, "error detected: {}", vk::to_string((vk::Result)err));
}
namespace acg::gui::details {

UiPass::UiPass(Config config) {
  enable_node_editor_ = config.enable_node_editor;
  vk::DescriptorPoolSize pool_sizes[] = {{vk::DescriptorType::eSampler, 1000},
                                         {vk::DescriptorType::eCombinedImageSampler, 1000},
                                         {vk::DescriptorType::eSampledImage, 1000},
                                         {vk::DescriptorType::eStorageImage, 1000},
                                         {vk::DescriptorType::eUniformTexelBuffer, 1000},
                                         {vk::DescriptorType::eStorageTexelBuffer, 1000},
                                         {vk::DescriptorType::eUniformBuffer, 1000},
                                         {vk::DescriptorType::eStorageBuffer, 1000},
                                         {vk::DescriptorType::eUniformBufferDynamic, 1000},
                                         {vk::DescriptorType::eStorageBufferDynamic, 1000},
                                         {vk::DescriptorType::eInputAttachment, 1000}};

  vk::DescriptorPoolCreateInfo pool_info = {};
  pool_info.setFlags(vk::DescriptorPoolCreateFlagBits::eFreeDescriptorSet)
      .setMaxSets(1000 * IM_ARRAYSIZE(pool_sizes))
      .setPoolSizes(pool_sizes);
  descriptor_pool_ = VkContext2::Instance().device_.createDescriptorPool(pool_info);

  IMGUI_CHECKVERSION();
  if (config.call_on_init) {
    config.call_on_init.value()();
  }
  ImGui::CreateContext();
  if (enable_node_editor_) {
    ImNodes::CreateContext();
  }
  ImGui_ImplGlfw_InitForVulkan(Window::Instance().GetWindow(), true);

  {  // Command Pool and Command Buffer.
    vk::CommandPoolCreateInfo pool_info;
    pool_info
        .setQueueFamilyIndex(
            VkContext2::Instance().system_info_.physical_device_info.graphics_family.value())
        .setFlags(vk::CommandPoolCreateFlagBits::eResetCommandBuffer);
    command_pool_ = VkContext2::Instance().device_.createCommandPool(pool_info);

    vk::CommandBufferAllocateInfo info;
    info.setCommandPool(command_pool_)
        .setCommandBufferCount(VkGraphicsContext::Instance().swapchain_size_)
        .setLevel(vk::CommandBufferLevel::ePrimary);
    command_buffers_ = VkContext2::Instance().device_.allocateCommandBuffers(info);
  }

  {  // Render Pass
    vk::AttachmentDescription attachment = {};
    attachment.setFormat(VkGraphicsContext::Instance().swapchain_image_format_)
        .setSamples(vk::SampleCountFlagBits::e1)
        .setStoreOp(vk::AttachmentStoreOp::eStore)
        .setStencilLoadOp(vk::AttachmentLoadOp::eDontCare)
        .setStencilStoreOp(vk::AttachmentStoreOp::eDontCare)
        .setFinalLayout(vk::ImageLayout::ePresentSrcKHR);

    if (config.is_ui_only) {
      attachment.setLoadOp(vk::AttachmentLoadOp::eClear)
          .setInitialLayout(vk::ImageLayout::eUndefined);
    } else {
      attachment.setLoadOp(vk::AttachmentLoadOp::eLoad)
          .setInitialLayout(vk::ImageLayout::eColorAttachmentOptimal);
    }

    vk::AttachmentReference color_attachment = {};
    color_attachment.setAttachment(0).setLayout(vk::ImageLayout::eColorAttachmentOptimal);
    vk::SubpassDescription subpass;
    subpass.setPipelineBindPoint(vk::PipelineBindPoint::eGraphics)
        .setColorAttachments(color_attachment);
    vk::SubpassDependency subpass_dep;
    subpass_dep.setSrcSubpass(VK_SUBPASS_EXTERNAL)
        .setDstSubpass(0)
        .setSrcStageMask(vk::PipelineStageFlagBits::eColorAttachmentOutput)
        .setDstStageMask(vk::PipelineStageFlagBits::eColorAttachmentOutput)
        .setSrcAccessMask(vk::AccessFlagBits::eColorAttachmentWrite)
        .setDstAccessMask(vk::AccessFlagBits::eColorAttachmentWrite);
    vk::RenderPassCreateInfo info;
    info.setAttachments(attachment).setSubpasses(subpass).setDependencies(subpass_dep);
    render_pass_ = VkContext2::Instance().device_.createRenderPass(info);
  }

  // this initializes imgui for Vulkan
  ImGui_ImplVulkan_InitInfo init_info = {};
  init_info.Instance = VkContext2::Instance().instance_;
  init_info.PhysicalDevice = VkContext2::Instance().physical_device_;
  init_info.Device = VkContext2::Instance().device_;
  init_info.Queue = VkContext2::Instance().graphics_queue_;
  init_info.QueueFamily
      = VkContext2::Instance().system_info_.physical_device_info.graphics_family.value();
  init_info.Allocator = VK_NULL_HANDLE;
  init_info.DescriptorPool = descriptor_pool_;
  init_info.MinImageCount = VkGraphicsContext::Instance().swapchain_size_;
  init_info.ImageCount = VkGraphicsContext::Instance().swapchain_size_;
  init_info.MSAASamples = VK_SAMPLE_COUNT_1_BIT;
  init_info.CheckVkResultFn = check_vk_result;

  ACG_CHECK(ImGui_ImplVulkan_Init(&init_info, render_pass_), "Falied to init ImGui_Vulkan");

  {  // Fonts and textures
    auto command_buffer = VkContext2::Instance().BeginSingleTimeCommand(command_pool_);
    ImGui_ImplVulkan_CreateFontsTexture(command_buffer);
    VkContext2::Instance().EndSingleTimeCommand(command_buffer, command_pool_,
                                                VkContext2::Instance().graphics_queue_);
    ImGui_ImplVulkan_DestroyFontUploadObjects();
  }

  CreateFramebuffers();
}

void UiPass::CreateFramebuffers() {
  for (auto imageview : VkGraphicsContext::Instance().swapchain_image_views_) {
    auto attachments = std::array{imageview};

    auto extent = VkGraphicsContext::Instance().swapchain_extent_;
    vk::FramebufferCreateInfo info;
    info.setRenderPass(render_pass_)
        .setAttachments(attachments)
        .setWidth(extent.width)
        .setHeight(extent.height)
        .setLayers(1);
    framebuffers_.emplace_back(VkContext2::Instance().device_.createFramebuffer(info));
  }
}

void UiPass::RecreateSwapchain() {
  DestroyFramebuffers();
  CreateFramebuffers();
}

UiPass::~UiPass() {
  DestroyFramebuffers();
  auto device = VkContext2::Instance().device_;
  device.destroy(render_pass_);
  device.destroy(descriptor_pool_);
  device.destroy(command_pool_);
  ImGui_ImplVulkan_Shutdown();
  ImGui_ImplGlfw_Shutdown();
  ImNodes::DestroyContext();
  if (enable_node_editor_) {
    ImGui::DestroyContext();
  }
}

void UiPass::DestroyFramebuffers() {
  auto device = VkContext2::Instance().device_;
  for (auto fb : framebuffers_) {
    device.destroy(fb);
  }
  framebuffers_.clear();
}

vk::CommandBuffer& UiPass::Render(ImDrawData* data) {
  size_t current_index = VkGraphicsContext::Instance().current_frame_;
  size_t current_image_index = VkGraphicsContext::Instance().current_image_index_;
  auto& current_command_buffer = command_buffers_[current_index];
  // Begin Command Buffer.
  vk::CommandBufferBeginInfo begininfo;
  current_command_buffer.begin(begininfo);
  // Begin Render pass.
  vk::RenderPassBeginInfo render_pass_begin_info;
  vk::ClearColorValue empty_color{std::array{0.0f, 0.0f, 0.0f, 1.0f}};
  vk::ClearValue clear_values[] = {empty_color};
  render_pass_begin_info.setFramebuffer(framebuffers_[current_image_index])
      .setRenderPass(render_pass_)
      .setClearValueCount(1)
      .setPClearValues(clear_values);
  render_pass_begin_info.renderArea.setExtent(VkGraphicsContext::Instance().swapchain_extent_);
  render_pass_begin_info.renderArea.setOffset({0, 0});
  current_command_buffer.beginRenderPass(render_pass_begin_info, vk::SubpassContents::eInline);
  // Draw Imgui.
  ImGui_ImplVulkan_RenderDrawData(data, current_command_buffer);
  // End render pass and end command buffer.
  current_command_buffer.endRenderPass();
  current_command_buffer.end();
  return current_command_buffer;
}

}  // namespace acg::gui::details
