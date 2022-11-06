#include "acg_vis/mesh_pipeline.hpp"

#include <fstream>
#include <spdlog/spdlog.h>

#include "acg_utils/log.hpp"
#include "glm/ext/matrix_transform.hpp"

static std::vector<char> read_file(std::string path) {
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

namespace acg::visualizer::details {

MeshPipeline::MeshPipeline(VkContext &renderer, bool is_present,
    vk::PolygonMode polygon,
    vk::CullModeFlags cull,
    vk::FrontFace front)
    : is_dst_present_(is_present), 
      polygon_mode_(polygon),
      cull_mode_(cull),
      front_face_(front),
      renderer_(renderer) {}

void MeshPipeline::Init() {
  if (is_inited_) {
    return;
  }
  CreateCommandPool();
  CreateRenderPass();
  CreateDescriptorSetLayout();
  CreateGraphicsPipeline();
  CreateDepthResources();
  CreateFramebuffers();
  CreateUniformBuffers();
  CreateCommandBuffers();
  CreateDescriptorPool();
  CreateDescriptorSets();
  is_inited_ = true;
}

void MeshPipeline::CreateCommandPool() {
  vk::CommandPoolCreateInfo pool_info;
  pool_info.setQueueFamilyIndex(renderer_.GetGraphicsFamily())
      .setFlags(vk::CommandPoolCreateFlagBits::eResetCommandBuffer);
  command_pool_ = renderer_.GetDevice().createCommandPool(pool_info);
}

void MeshPipeline::CreateRenderPass() {
  vk::AttachmentDescription color_attachment;
  color_attachment.setFormat(renderer_.GetSwapchainImageFormat())
      .setSamples(vk::SampleCountFlagBits::e1)
      .setLoadOp(vk::AttachmentLoadOp::eClear)
      .setStoreOp(vk::AttachmentStoreOp::eStore)
      .setStencilLoadOp(vk::AttachmentLoadOp::eDontCare)
      .setStencilStoreOp(vk::AttachmentStoreOp::eDontCare)
      .setInitialLayout(vk::ImageLayout::eUndefined);
  if (is_dst_present_) {
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
  render_pass_ = renderer_.GetDevice().createRenderPass(info);
}

void MeshPipeline::CreateDescriptorSetLayout() {
  vk::DescriptorSetLayoutBinding ubo_layout_binding;
  ubo_layout_binding.setBinding(0)
      .setDescriptorCount(1)
      .setDescriptorType(vk::DescriptorType::eUniformBuffer)
      .setPImmutableSamplers(nullptr)
      .setStageFlags(vk::ShaderStageFlagBits::eVertex);

  vk::DescriptorSetLayoutCreateInfo layout_create_info;
  layout_create_info.setBindings(ubo_layout_binding);
  descriptor_set_layout_ = renderer_.GetDevice().createDescriptorSetLayout(layout_create_info);
}

void MeshPipeline::CreateGraphicsPipeline() {
  // Setup Shaders.
  vk::ShaderModule vert_module, frag_module;
  {
    auto code = read_file(SPV_HOME "/3d.vert.spv");
    vk::ShaderModuleCreateInfo info;
    info.setPCode(reinterpret_cast<Idx *>(code.data())).setCodeSize(code.size());
    vert_module = renderer_.GetDevice().createShaderModule(info);
    code = read_file(SPV_HOME "/3d.frag.spv");
    info.setPCode(reinterpret_cast<Idx *>(code.data())).setCodeSize(code.size());
    frag_module = renderer_.GetDevice().createShaderModule(info);
  }

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
  rasterizer_info.setDepthBiasEnable(VK_FALSE)
      .setRasterizerDiscardEnable(VK_FALSE)
      .setLineWidth(1.0f)
      .setPolygonMode(polygon_mode_)
      .setCullMode(cull_mode_)
      .setFrontFace(front_face_)
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
      .setBlendEnable(VK_FALSE)  // todo: add alpha blending support
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
  pipeline_layout_info.setSetLayouts(descriptor_set_layout_);

  // Depth Testing.
  vk::PipelineDepthStencilStateCreateInfo depth_stencil;
  depth_stencil.setDepthTestEnable(VK_TRUE)
      .setDepthWriteEnable(VK_TRUE)
      .setDepthCompareOp(vk::CompareOp::eLess)
      .setDepthBoundsTestEnable(VK_FALSE)
      .setStencilTestEnable(VK_FALSE);

  pipeline_layout_ = renderer_.GetDevice().createPipelineLayout(pipeline_layout_info);
  vk::GraphicsPipelineCreateInfo info;
  info.setStages(shader_stages)
      .setPVertexInputState(&vertex_input_create_info)
      .setPInputAssemblyState(&input_assembly_info)
      .setPViewportState(&viewport_info)
      .setPRasterizationState(&rasterizer_info)
      .setPMultisampleState(&multi_sample_info)
      .setPDepthStencilState(&depth_stencil)
      .setPColorBlendState(&color_blend_info)
      .setPDynamicState(&dynamic_state_info)
      .setLayout(pipeline_layout_)
      .setRenderPass(render_pass_)
      .setSubpass(0)
      .setBasePipelineHandle(VK_NULL_HANDLE)
      .setBasePipelineIndex(-1);

  auto rv = renderer_.GetDevice().createGraphicsPipeline(VK_NULL_HANDLE, info);
  ACG_CHECK(rv.result == vk::Result::eSuccess, "Failed to create graphics pipeline");
  graphics_pipeline_ = rv.value;

  renderer_.GetDevice().destroy(vert_module);
  renderer_.GetDevice().destroy(frag_module);
}

void MeshPipeline::CreateDepthResources() {
  auto depth_format = FindDepthFormat();
  ACG_DEBUG_LOG("Depth format = {}", vk::to_string(depth_format));
  std::tie(depth_image_, depth_image_memory_) = renderer_.CreateImage(
      renderer_.GetSwapchainExtent().width, renderer_.GetSwapchainExtent().height, depth_format,
      vk::ImageTiling::eOptimal, vk::ImageUsageFlagBits::eDepthStencilAttachment,
      vk::MemoryPropertyFlagBits::eDeviceLocal);
  depth_image_view_
      = renderer_.CreateImageView(depth_image_, depth_format, vk::ImageAspectFlagBits::eDepth);
}

vk::Format MeshPipeline::FindDepthFormat() const {
  vk::Format depth_format = renderer_.FindSupportedFormat(
      {vk::Format::eD32Sfloat, vk::Format::eD32SfloatS8Uint, vk::Format::eD24UnormS8Uint},
      vk::ImageTiling::eOptimal, vk::FormatFeatureFlagBits::eDepthStencilAttachment);
  return depth_format;
}

void MeshPipeline::CreateFramebuffers() {
  for (auto imageview : renderer_.GetSwapchainImageviews()) {
    auto attachments = std::array{imageview, depth_image_view_};

    vk::FramebufferCreateInfo info;
    info.setRenderPass(render_pass_)
        .setAttachments(attachments)
        .setWidth(renderer_.GetSwapchainExtent().width)
        .setHeight(renderer_.GetSwapchainExtent().height)
        .setLayers(1);

    framebuffers_.emplace_back(renderer_.GetDevice().createFramebuffer(info));
  }
}

void MeshPipeline::CreateUniformBuffers() {
  auto buffer_size = static_cast<vk::DeviceSize>(sizeof(Ubo));

  uniform_buffers_.clear();
  for (size_t i = 0; i < renderer_.GetSwapchainSize(); ++i) {
    auto buffer = renderer_.CreateBuffer(
        buffer_size, vk::BufferUsageFlagBits::eUniformBuffer,
        vk::MemoryPropertyFlagBits::eHostVisible | vk::MemoryPropertyFlagBits::eHostCoherent);

    uniform_buffers_.emplace_back(buffer);
  }
}
void MeshPipeline::CreateCommandBuffers() {
  vk::CommandBufferAllocateInfo info;
  info.setCommandPool(command_pool_)
      .setCommandBufferCount(renderer_.GetSwapchainSize())
      .setLevel(vk::CommandBufferLevel::ePrimary);
  command_buffers_ = renderer_.GetDevice().allocateCommandBuffers(info);
}

void MeshPipeline::CreateDescriptorPool() {
  vk::DescriptorPoolSize pool_size;
  pool_size.setType(vk::DescriptorType::eUniformBuffer)
      .setDescriptorCount(renderer_.GetSwapchainSize());

  vk::DescriptorPoolCreateInfo pool_create_info;
  pool_create_info.setPoolSizes(pool_size).setMaxSets(renderer_.GetSwapchainSize());

  descriptor_pool_ = renderer_.GetDevice().createDescriptorPool(pool_create_info);
}

void MeshPipeline::CreateDescriptorSets() {
  std::vector<vk::DescriptorSetLayout> layouts(renderer_.GetSwapchainSize(),
                                               descriptor_set_layout_);
  vk::DescriptorSetAllocateInfo alloc_info;
  alloc_info.setDescriptorPool(descriptor_pool_)
      .setSetLayouts(layouts)
      .setDescriptorSetCount(renderer_.GetSwapchainSize());
  descriptor_sets_ = renderer_.GetDevice().allocateDescriptorSets(alloc_info);

  for (size_t i = 0; i < renderer_.GetSwapchainSize(); ++i) {
    vk::DescriptorBufferInfo buffer_info;
    buffer_info.setBuffer(uniform_buffers_[i].GetBuffer()).setOffset(0).setRange(sizeof(Ubo));
    vk::WriteDescriptorSet desc_write;
    desc_write.setDstSet(descriptor_sets_[i])
        .setDstBinding(0)
        .setDstArrayElement(0)
        .setDescriptorCount(1)
        .setDescriptorType(vk::DescriptorType::eUniformBuffer)
        .setPBufferInfo(&buffer_info);
    renderer_.GetDevice().updateDescriptorSets(desc_write, {});
  }
}

void MeshPipeline::Cleanup() {
  if (! is_inited_) {
    return;
  }
  CleanupSwapchain();
  auto device = renderer_.GetDevice();
  for (auto &buf : uniform_buffers_) {
    buf.Release();
  }
  uniform_buffers_.clear();
  device.destroy(pipeline_layout_);
  device.destroy(graphics_pipeline_);
  device.destroy(descriptor_set_layout_);
  device.destroy(descriptor_pool_);
  device.destroy(render_pass_);
  device.destroy(command_pool_);
}

void MeshPipeline::CleanupSwapchain() {
  auto device = renderer_.GetDevice();
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

void MeshPipeline::RecreateSwapchain() {
  CleanupSwapchain();
  CreateDepthResources();
  CreateFramebuffers();
}

vk::CommandBuffer &MeshPipeline::BeginRender() {
  ACG_CHECK(!is_render_pass_begin_, "Render pass has begin.");
  auto extent = renderer_.GetSwapchainExtent();
  auto current_index = renderer_.GetCurrentIndex();
  auto &current_command_buffer = command_buffers_[current_index];
  auto clear_value = std::array<vk::ClearValue, 2>{background_color_, depth_stencil_value_};

  // Begin Command Buffer
  vk::CommandBufferBeginInfo begin_info;
  begin_info.setFlags(vk::CommandBufferUsageFlagBits::eSimultaneousUse);
  current_command_buffer.begin(begin_info);
  // Begin Render Pass
  vk::RenderPassBeginInfo render_pass_info;
  render_pass_info.renderPass = render_pass_;
  render_pass_info.framebuffer = framebuffers_[renderer_.GetAcquiredImageIndex()];
  render_pass_info.renderArea.extent = extent;
  render_pass_info.renderArea.offset.setX(0);
  render_pass_info.renderArea.offset.setY(0);
  render_pass_info.setClearValues(clear_value); 
  current_command_buffer.beginRenderPass(render_pass_info, vk::SubpassContents::eInline);
  
  // Bind Pipeline
  current_command_buffer.bindPipeline(vk::PipelineBindPoint::eGraphics, graphics_pipeline_);
  // Setup Viewport and scissor.
  vk::Viewport viewport(0.0, 0.0, extent.width, extent.height, 0.0, 1.0);
  current_command_buffer.setViewport(0, viewport);
  vk::Rect2D scissor{{0, 0}, extent};
  current_command_buffer.setScissor(0, scissor);
  
  // Bind UBO inline.
  current_command_buffer.bindDescriptorSets(vk::PipelineBindPoint::eGraphics, pipeline_layout_, 0,
                                            descriptor_sets_[renderer_.GetCurrentIndex()], {});
  is_render_pass_begin_ = true;
  return current_command_buffer;
}


vk::CommandBuffer& MeshPipeline::EndRender() {
  ACG_CHECK(is_render_pass_begin_, "Render pass has not begin.");
  auto &current_command_buffer = command_buffers_[renderer_.GetCurrentIndex()];
  current_command_buffer.endRenderPass();
  current_command_buffer.end();
  is_render_pass_begin_ = false;
  return current_command_buffer;
}

void MeshPipeline::SetCamera(const Camera &camera, bool all_update) {
  auto extent = renderer_.GetSwapchainExtent();
  //TODO: Ubo should be updated explicitely,
  Ubo ubo{};
  ubo.model = camera.GetModel();
  ubo.view = camera.GetView();
  ubo.projection = camera.GetProjection(extent.width, extent.height);
  ubo.eye_position = camera.GetPosition();
  if (all_update) {
    for (auto ub: uniform_buffers_) {
      renderer_.CopyHostToBuffer(&ubo, ub, sizeof(ubo));
    }
  } else {
    renderer_.CopyHostToBuffer(&ubo, uniform_buffers_[renderer_.GetCurrentIndex()], sizeof(ubo));
  }
}


MeshPipeline::~MeshPipeline(){
  Cleanup();
}



}  // namespace acg::visualizer::details
