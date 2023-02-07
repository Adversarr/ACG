#include "acg_gui/backend/wireframe_ppl.hpp"

#include "acg_gui/backend/context.hpp"
#include "acg_gui/backend/graphics_context.hpp"
#include "acg_gui/convent.hpp"
#include "acg_utils/raw_fileio.hpp"

namespace acg::gui::details {

std::vector<vk::VertexInputBindingDescription> WireframePoint::GetBindingDescriptions() {
  vk::VertexInputBindingDescription binding_description;
  binding_description.setBinding(0)
      .setInputRate(vk::VertexInputRate::eVertex)
      .setStride(sizeof(WireframePoint));

  return {binding_description};
}

std::vector<vk::VertexInputAttributeDescription> WireframePoint::GetAttributeDescriptions() {
  vk::VertexInputAttributeDescription desc1;
  vk::VertexInputAttributeDescription desc2;
  desc1.setBinding(0)
      .setLocation(0)
      .setFormat(vk::Format::eR32G32B32Sfloat)
      .setOffset(offsetof(WireframePoint, position));

  desc2.setBinding(0)
      .setLocation(1)
      .setFormat(vk::Format::eR32G32B32Sfloat)
      .setOffset(offsetof(WireframePoint, color));

  return {desc1, desc2};
}

WireframePipeline::WireframePipeline(const GraphicsRenderPass &render_pass, Config /*config*/) {
  CreateUniformBuffers();
  CreateDescriptorSetLayout();
  CreateDescriptorSets(render_pass);
  CreateGraphicsPipeline(render_pass);
}

void WireframePipeline::CreateDescriptorSetLayout() {
  vk::DescriptorSetLayoutBinding ubo_layout_binding;
  ubo_layout_binding.setBinding(0)
      .setDescriptorCount(1)
      .setDescriptorType(vk::DescriptorType::eUniformBuffer)
      .setPImmutableSamplers(nullptr)
      .setStageFlags(vk::ShaderStageFlagBits::eVertex | vk::ShaderStageFlagBits::eFragment);

  vk::DescriptorSetLayoutCreateInfo layout_create_info;
  layout_create_info.setBindings(ubo_layout_binding);
  descriptor_set_layout_
      = VkContext2::Instance().device_.createDescriptorSetLayout(layout_create_info);
}

void WireframePipeline::CleanUp() {
  for (auto ub : uniform_buffers_) {
    VkContext2::Instance().DestroyBufferWithMemory(ub);
  }
  uniform_buffers_.clear();
  auto device = VkContext2::Instance().device_;
  device.destroy(pipeline_layout_);
  device.destroy(pipeline_);
  device.destroy(descriptor_set_layout_);
}

WireframePipeline::~WireframePipeline() { CleanUp(); }

void WireframePipeline::CreateUniformBuffers() {
  auto buffer_size = static_cast<vk::DeviceSize>(sizeof(Ubo));
  uniform_buffers_.clear();
  for (size_t i = 0; i < VkGraphicsContext::Instance().swapchain_size_; ++i) {
    auto buffer = VkContext2::Instance().CreateBufferWithMemory(
        buffer_size, vk::BufferUsageFlagBits::eUniformBuffer,
        vk::MemoryPropertyFlagBits::eHostVisible | vk::MemoryPropertyFlagBits::eHostCoherent);

    uniform_buffers_.emplace_back(std::move(buffer));
  }
}

void WireframePipeline::CreateGraphicsPipeline(const GraphicsRenderPass &graphics_pass) {
  vk::ShaderModule vert_module, frag_module;
  {
    auto code = acg::utils::io::read_binary_file(SPV_HOME "wireframe.vert.spv");
    vk::ShaderModuleCreateInfo info;
    info.setPCode(reinterpret_cast<uint32_t *>(code.data())).setCodeSize(code.size());
    auto device = VkContext2::Instance().device_;
    vert_module = device.createShaderModule(info);
    code = acg::utils::io::read_binary_file(SPV_HOME "wireframe.frag.spv");
    info.setPCode(reinterpret_cast<uint32_t *>(code.data())).setCodeSize(code.size());
    frag_module = device.createShaderModule(info);
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
  auto vertex_binding_desc = WireframePoint::GetBindingDescriptions();
  auto vertex_attr_desc = WireframePoint::GetAttributeDescriptions();
  vertex_input_create_info.setVertexBindingDescriptions(vertex_binding_desc)
      .setVertexAttributeDescriptions(vertex_attr_desc);

  vk::PipelineInputAssemblyStateCreateInfo input_assembly_info;
  input_assembly_info.setTopology(vk::PrimitiveTopology::eLineList)
      .setPrimitiveRestartEnable(VK_FALSE);

  // Setup Viewport
  vk::PipelineViewportStateCreateInfo viewport_info;
  viewport_info.setViewportCount(1).setScissorCount(1);

  // Setup Rasterization
  vk::PipelineRasterizationStateCreateInfo rasterizer_info;
  rasterizer_info.setDepthBiasEnable(VK_FALSE)
      .setRasterizerDiscardEnable(VK_FALSE)
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

  auto dynamic_states = std::array{vk::DynamicState::eViewport, vk::DynamicState::eScissor,
                                   vk::DynamicState::eLineWidth};
  vk::PipelineDynamicStateCreateInfo dynamic_state_info;
  dynamic_state_info.setDynamicStates(dynamic_states);

  vk::PipelineLayoutCreateInfo pipeline_layout_info;
  pipeline_layout_info.setSetLayouts(descriptor_set_layout_);

  // Depth Testing.
  vk::PipelineDepthStencilStateCreateInfo depth_stencil;
  depth_stencil.setDepthTestEnable(VK_TRUE)
      .setDepthWriteEnable(VK_TRUE)
      .setDepthCompareOp(vk::CompareOp::eLessOrEqual)
      .setDepthBoundsTestEnable(VK_FALSE)
      .setStencilTestEnable(VK_FALSE);

  auto device = VkContext2::Instance().device_;
  pipeline_layout_ = device.createPipelineLayout(pipeline_layout_info);
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
      .setRenderPass(graphics_pass.GetRenderPass())
      .setSubpass(0)
      .setBasePipelineHandle(VK_NULL_HANDLE)
      .setBasePipelineIndex(-1);

  auto rv = device.createGraphicsPipeline(VK_NULL_HANDLE, info);
  ACG_CHECK(rv.result == vk::Result::eSuccess, "Failed to create graphics pipeline");
  pipeline_ = rv.value;

  device.destroy(vert_module);
  device.destroy(frag_module);
}

void WireframePipeline::BeginPipeline(vk::CommandBuffer &current_command_buffer) {
  // Bind Pipeline
  auto extent = VkGraphicsContext::Instance().swapchain_extent_;
  current_command_buffer.bindPipeline(vk::PipelineBindPoint::eGraphics, pipeline_);
  // Setup Viewport and scissor.
  vk::Viewport viewport(0.0, 0.0, extent.width, extent.height, 0.0, 1.0);
  current_command_buffer.setViewport(0, viewport);
  vk::Rect2D scissor{{0, 0}, extent};
  current_command_buffer.setScissor(0, scissor);

  // Bind UBO inline.
  current_command_buffer.bindDescriptorSets(
      vk::PipelineBindPoint::eGraphics, pipeline_layout_, 0,
      ubo_descriptor_sets_[VkGraphicsContext::Instance().current_frame_], {});
}

void WireframePipeline::EndPipeline(vk::CommandBuffer & /* current_command_buffer */) {
  // Nothing todo.
}

void WireframePipeline::SetCamera(const Camera &cam) {
  auto extent = VkGraphicsContext::Instance().swapchain_extent_;
  ubo_.mvp = cam.GetProjection(extent.width, extent.height) * cam.GetView() * cam.GetModel();
  ubo_.eye_position = to_glm(cam.GetPosition());
}

void WireframePipeline::UpdateUbo(bool fast) {
  if (!fast) {
    VkContext2::Instance().device_.waitIdle();
  }

  for (auto &ub : uniform_buffers_) {
    VkContext2::Instance().CopyHostToBuffer(&ubo_, ub, sizeof(ubo_));
  }
}

void WireframePipeline::CreateDescriptorSets(const GraphicsRenderPass &pass) {
  auto swapchain_size = VkGraphicsContext::Instance().swapchain_size_;
  std::vector<vk::DescriptorSetLayout> layouts(swapchain_size, descriptor_set_layout_);
  vk::DescriptorSetAllocateInfo alloc_info;
  alloc_info.setDescriptorPool(pass.GetDescriptorPool())
      .setSetLayouts(layouts)
      .setDescriptorSetCount(swapchain_size);
  auto device = VkContext2::Instance().device_;
  ubo_descriptor_sets_ = device.allocateDescriptorSets(alloc_info);

  for (size_t i = 0; i < swapchain_size; ++i) {
    vk::DescriptorBufferInfo buffer_info;
    buffer_info.setBuffer(uniform_buffers_[i].GetBuffer()).setOffset(0).setRange(sizeof(Ubo));
    vk::WriteDescriptorSet desc_write;
    desc_write.setDstSet(ubo_descriptor_sets_[i])
        .setDstBinding(0)
        .setDstArrayElement(0)
        .setDescriptorCount(1)
        .setDescriptorType(vk::DescriptorType::eUniformBuffer)
        .setPBufferInfo(&buffer_info);
    device.updateDescriptorSets(desc_write, {});
  }
}

}  // namespace acg::gui::details
