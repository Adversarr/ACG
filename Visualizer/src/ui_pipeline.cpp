#include "acg_visualizer/ui_pipeline.hpp"
#include "acg_utils/log.hpp"

static void check_vk_result(VkResult err) {
  ACG_CHECK(err == VK_SUCCESS, "error detected: {}", vk::to_string((vk::Result)err));
}

namespace acg::visualizer::details {

UiPipeline::UiPipeline(Renderer& renderer):
  renderer_(renderer) {}

void UiPipeline::Init() {
  if (is_inited_) {
    return;
  }
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
  imgui_pool_ = renderer_.GetDevice().createDescriptorPool(pool_info);

  IMGUI_CHECKVERSION();
  ImGui::CreateContext();
  ImGui_ImplGlfw_InitForVulkan(renderer_.GetWindow()->GetWindow(), true);

  { // Command Pool and Command Buffer.
    vk::CommandPoolCreateInfo pool_info;
    pool_info.setQueueFamilyIndex(renderer_.GetGraphicsFamily())
        .setFlags(vk::CommandPoolCreateFlagBits::eResetCommandBuffer);
    command_pool_ = renderer_.GetDevice().createCommandPool(pool_info);

    vk::CommandBufferAllocateInfo info;
    info.setCommandPool(command_pool_)
        .setCommandBufferCount(renderer_.GetSwapchainSize())
        .setLevel(vk::CommandBufferLevel::ePrimary);
    command_buffers_ = renderer_.GetDevice().allocateCommandBuffers(info);
  }

  { // Render Pass
    vk::AttachmentDescription attachment = {};
    attachment.setFormat(renderer_.GetSwapchainImageFormat())
        .setSamples(vk::SampleCountFlagBits::e1)
        .setLoadOp(vk::AttachmentLoadOp::eLoad)
        .setStoreOp(vk::AttachmentStoreOp::eStore)
        .setStencilLoadOp(vk::AttachmentLoadOp::eDontCare)
        .setStencilStoreOp(vk::AttachmentStoreOp::eDontCare)
        .setInitialLayout(
          !is_ui_only_ ? vk::ImageLayout::eColorAttachmentOptimal : vk::ImageLayout::eUndefined)
        .setFinalLayout(vk::ImageLayout::ePresentSrcKHR);

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
    render_pass_ = renderer_.GetDevice().createRenderPass(info);
  }

  // this initializes imgui for Vulkan
  ImGui_ImplVulkan_InitInfo init_info = {};
  init_info.Instance = renderer_.GetInstance();
  init_info.PhysicalDevice = renderer_.GetPhysicalDevice();
  init_info.Device = renderer_.GetDevice();
  init_info.Queue = renderer_.GetGraphicsQueue();
  init_info.QueueFamily = renderer_.GetGraphicsFamily();
  init_info.Allocator = VK_NULL_HANDLE;
  init_info.DescriptorPool = imgui_pool_;
  init_info.MinImageCount = renderer_.GetSwapchainSize();
  init_info.ImageCount = renderer_.GetSwapchainSize();
  init_info.MSAASamples = VK_SAMPLE_COUNT_1_BIT;
  init_info.CheckVkResultFn = check_vk_result;

  ACG_CHECK(ImGui_ImplVulkan_Init(&init_info, render_pass_), "Falied to init ImGui_Vulkan");

  {  // Fonts and textures
    auto command_buffer = renderer_.BeginSingleTimeCommand(command_pool_);
    ImGui_ImplVulkan_CreateFontsTexture(command_buffer);
    renderer_.EndSingleTimeCommand(command_buffer, command_pool_);
    ImGui_ImplVulkan_DestroyFontUploadObjects();
  }

  CreateFramebuffers();
  is_inited_ = true;
}

void UiPipeline::CreateFramebuffers() {
  for (auto imageview : renderer_.GetSwapchainImageviews()) {
    auto attachments = std::array{imageview};

    vk::FramebufferCreateInfo info;
    info.setRenderPass(render_pass_)
        .setAttachments(attachments)
        .setWidth(renderer_.GetSwapchainExtent().width)
        .setHeight(renderer_.GetSwapchainExtent().height)
        .setLayers(1);
    framebuffers_.emplace_back(
      renderer_.GetDevice().createFramebuffer(info));
  }
}

vk::CommandBuffer& UiPipeline::Render(ImDrawData* data) {
  size_t current_index = renderer_.GetCurrentIndex();
  size_t current_image_index = renderer_.GetAcquiredImageIndex();
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
  render_pass_begin_info.renderArea.setExtent(renderer_.GetSwapchainExtent());
  render_pass_begin_info.renderArea.setOffset({0, 0});
  current_command_buffer.beginRenderPass(render_pass_begin_info, vk::SubpassContents::eInline);
  // Draw Imgui.
  ImGui_ImplVulkan_RenderDrawData(data, current_command_buffer);
  // End render pass and end command buffer.
  current_command_buffer.endRenderPass();
  current_command_buffer.end();
  return current_command_buffer;
}

void UiPipeline::Cleanup() {
  if (!is_inited_) {
    return ;
  }
  auto dev = renderer_.GetDevice();
  dev.destroy(render_pass_);
  CleanupSwapchain();
  dev.destroy(command_pool_);
  dev.destroy(imgui_pool_);
  ImGui_ImplVulkan_Shutdown();
  is_inited_ = false;
}

void UiPipeline::CleanupSwapchain(){
  for (auto fb: framebuffers_) {
    renderer_.GetDevice().destroy(fb);
  }
  framebuffers_.clear();
}


void UiPipeline::RecreateSwapchain(){
  CleanupSwapchain();
  CreateFramebuffers();
}

UiPipeline::~UiPipeline() {
  Cleanup();
}


}
