#include "acg_gui/backend/mesh_ppl.hpp"

#include <acg_utils/raw_fileio.hpp>

#include "acg_gui/convent.hpp"
namespace acg::gui::details {

std::vector<vk::VertexInputBindingDescription> MeshVertex::GetBindingDescriptions() {
  vk::VertexInputBindingDescription binding;
  binding.setBinding(0);
  binding.setStride(sizeof(MeshVertex));
  binding.setInputRate(vk::VertexInputRate::eVertex);
  return {binding};
}

std::vector<vk::VertexInputAttributeDescription> MeshVertex::GetAttributeDescriptions() {
  vk::VertexInputAttributeDescription desc1;
  desc1.binding = 0;
  desc1.location = 0;
  desc1.format = vk::Format::eR32G32B32Sfloat;
  desc1.offset = offsetof(MeshVertex, position_);

  vk::VertexInputAttributeDescription desc2;
  desc2.binding = 0;
  desc2.location = 1;
  desc2.format = vk::Format::eR32G32B32A32Sfloat;
  desc2.offset = offsetof(MeshVertex, color_);

  vk::VertexInputAttributeDescription desc3;
  desc3.binding = 0;
  desc3.location = 2;
  desc3.format = vk::Format::eR32G32B32Sfloat;
  desc3.offset = offsetof(MeshVertex, normal_);

  vk::VertexInputAttributeDescription desc4;
  desc4.binding = 0;
  desc4.location = 3;
  desc4.format = vk::Format::eR32G32Sfloat;
  desc4.offset = offsetof(MeshVertex, uv_);

  return {desc1, desc2, desc3, desc4};
}

std::vector<vk::VertexInputBindingDescription> MeshInstance::GetBindingDescriptions() {
  vk::VertexInputBindingDescription binding;
  binding.setBinding(1);
  binding.setStride(sizeof(MeshInstance));
  binding.setInputRate(vk::VertexInputRate::eInstance);
  return {binding};
}

std::vector<vk::VertexInputAttributeDescription> MeshInstance::GetAttributeDescriptions() {
  vk::VertexInputAttributeDescription desc1;
  desc1.binding = 1;
  desc1.location = 4;
  desc1.format = vk::Format::eR32G32B32Sfloat;
  desc1.offset = offsetof(MeshInstance, position);

  vk::VertexInputAttributeDescription desc2;
  desc2.binding = 1;
  desc2.location = 5;
  desc2.format = vk::Format::eR32G32B32A32Sfloat;
  desc2.offset = offsetof(MeshInstance, rotation);

  vk::VertexInputAttributeDescription desc3;
  desc3.binding = 1;
  desc3.location = 6;
  desc3.format = vk::Format::eR32G32B32A32Sfloat;
  desc3.offset = offsetof(MeshInstance, color);
  return {desc1, desc2, desc3};
}

MeshPipeline::MeshPipeline(const GraphicsRenderPass &pass, Config config) : config_(config) {
  Init(pass);
}

MeshPipeline::~MeshPipeline() { Destroy(); }

void MeshPipeline::Init(const GraphicsRenderPass &graphics_pass) {
  CreateUniformBuffers();
  CreateDescriptorSetLayout();
  CreateDescriptorSets(graphics_pass);
  CreateGraphicsPipeline(graphics_pass);
}

void MeshPipeline::Destroy() {
  for (auto ub : uniform_buffers_) {
    VkContext2::Instance().DestroyBufferWithMemory(ub);
  }
  uniform_buffers_.clear();
  VkContext2::Instance().device_.destroy(pipeline_layout_);
  VkContext2::Instance().device_.destroy(pipeline_);
  VkContext2::Instance().device_.destroy(descriptor_set_layout_);
}

void MeshPipeline::CreateUniformBuffers() {
  auto buffer_size = static_cast<vk::DeviceSize>(sizeof(MeshUniform));
  uniform_buffers_.clear();
  for (size_t i = 0; i < VkGraphicsContext::Instance().swapchain_size_; ++i) {
    auto buffer = VkContext2::Instance().CreateBufferWithMemory(
        buffer_size, vk::BufferUsageFlagBits::eUniformBuffer,
        vk::MemoryPropertyFlagBits::eHostVisible | vk::MemoryPropertyFlagBits::eHostCoherent);
    uniform_buffers_.push_back(buffer);
  }
}

void MeshPipeline::CreateDescriptorSetLayout() {
  vk::DescriptorSetLayoutBinding ubo_layout_binding;
  ubo_layout_binding.setBinding(0)
      .setDescriptorCount(1)
      .setDescriptorType(vk::DescriptorType::eUniformBuffer)
      .setStageFlags(vk::ShaderStageFlagBits::eVertex | vk::ShaderStageFlagBits::eFragment);

  vk::DescriptorSetLayoutCreateInfo layout_create_info;
  layout_create_info.setBindings(ubo_layout_binding);
  descriptor_set_layout_
      = VkContext2::Instance().device_.createDescriptorSetLayout(layout_create_info);
}

void MeshPipeline::CreateGraphicsPipeline(const GraphicsRenderPass &graphics_pass) {
  vk::ShaderModule vert_module, frag_module;
  {
    auto code = acg::utils::io::read_binary_file(SPV_HOME "mesh_pc.vert.spv");
    vk::ShaderModuleCreateInfo info;
    info.setPCode(reinterpret_cast<uint32_t *>(code.data())).setCodeSize(code.size());
    vert_module = VkContext2::Instance().device_.createShaderModule(info);
    code = acg::utils::io::read_binary_file(SPV_HOME "mesh_pc.frag.spv");
    info.setPCode(reinterpret_cast<uint32_t *>(code.data())).setCodeSize(code.size());
    frag_module = VkContext2::Instance().device_.createShaderModule(info);
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
  auto vertex_binding_desc = MeshVertex::GetBindingDescriptions();
  auto vertex_attr_desc = MeshVertex::GetAttributeDescriptions();
  auto instance_binding_desc = MeshInstance::GetBindingDescriptions();
  auto instance_attr_desc = MeshInstance::GetAttributeDescriptions();
  std::move(instance_binding_desc.begin(), instance_binding_desc.end(), std::back_inserter(vertex_binding_desc));
  std::move(instance_attr_desc.begin(), instance_attr_desc.end(), std::back_inserter(vertex_attr_desc));
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
      .setPolygonMode(vk::PolygonMode::eFill)
      .setCullMode(config_.cull_mode_)
      .setFrontFace(config_.front_face_)
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
      .setSrcColorBlendFactor(vk::BlendFactor::eSrcAlpha)
      .setDstColorBlendFactor(vk::BlendFactor::eOneMinusSrcAlpha)
      .setColorBlendOp(vk::BlendOp::eAdd)
      .setSrcAlphaBlendFactor(vk::BlendFactor::eOne)
      .setDstAlphaBlendFactor(vk::BlendFactor::eZero)
      .setAlphaBlendOp(vk::BlendOp::eAdd);
  if (config_.enable_color_blending) {
    color_blend_attachment.setBlendEnable(VK_TRUE);
  } else {
    color_blend_attachment.setBlendEnable(VK_FALSE);
  }

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

  // setup push constants
  vk::PushConstantRange push_constant;
  // this push constant range starts at the beginning
  push_constant.offset = 0;
  // this push constant range takes up the size of a MeshPushConstants struct
  push_constant.size = sizeof(MeshPushConstants);
  // this push constant range is accessible only in the vertex shader
  push_constant.stageFlags = vk::ShaderStageFlagBits::eVertex | vk::ShaderStageFlagBits::eFragment;
  pipeline_layout_info.setPPushConstantRanges(&push_constant).setPushConstantRangeCount(1);

  pipeline_layout_ = VkContext2::Instance().device_.createPipelineLayout(pipeline_layout_info);
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

  auto rv = VkContext2::Instance().device_.createGraphicsPipeline(VK_NULL_HANDLE, info);
  ACG_CHECK(rv.result == vk::Result::eSuccess, "Failed to create graphics pipeline");
  pipeline_ = rv.value;

  VkContext2::Instance().device_.destroy(vert_module);
  VkContext2::Instance().device_.destroy(frag_module);
}

void MeshPipeline::BeginPipeline(vk::CommandBuffer &current_command_buffer) {
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

void MeshPipeline::EndPipeline(vk::CommandBuffer & /* current_command_buffer */) {
  // Nothing todo.
}

void MeshPipeline::SetCamera(const Camera &cam) {
  auto extent = VkGraphicsContext::Instance().swapchain_extent_;
  ubo_.projection = cam.GetProjection(extent.width, extent.height);
  ubo_.view = cam.GetView();
  ubo_.eye_position = to_glm(cam.GetPosition());
}

void MeshPipeline::SetLight(const Light &light) {
  ubo_.ambient_light_color = glm::vec4(to_glm(light.ambient_light_color_), light.light_density_);
  ubo_.point_light_color = glm::vec4(to_glm(light.light_color_), light.light_density_);
  ubo_.point_light_pos = to_glm(light.light_position_);
  ubo_.parallel_light_color
      = glm::vec4(to_glm(light.parallel_light_color_), light.parallel_light_density_);
  ubo_.parallel_light_dir = to_glm(light.parallel_light_dir_);
}

void MeshPipeline::UpdateUbo(bool fast) {
  if (!fast) {
    VkContext2::Instance().device_.waitIdle();
  }

  for (auto ub : uniform_buffers_) {
    VkContext2::Instance().CopyHostToBuffer(&ubo_, ub, sizeof(ubo_));
  }
}

void MeshPipeline::CreateDescriptorSets(const GraphicsRenderPass &pass) {
  auto swapchain_size = VkGraphicsContext::Instance().swapchain_size_;
  std::vector<vk::DescriptorSetLayout> layouts(swapchain_size, descriptor_set_layout_);
  vk::DescriptorSetAllocateInfo alloc_info;
  alloc_info.setDescriptorPool(pass.GetDescriptorPool())
      .setSetLayouts(layouts)
      .setDescriptorSetCount(swapchain_size);
  ubo_descriptor_sets_ = VkContext2::Instance().device_.allocateDescriptorSets(alloc_info);

  for (size_t i = 0; i < swapchain_size; ++i) {
    vk::DescriptorBufferInfo buffer_info;
    buffer_info.setBuffer(uniform_buffers_[i].GetBuffer())
        .setOffset(0)
        .setRange(sizeof(MeshUniform));
    vk::WriteDescriptorSet desc_write;
    desc_write.setDstSet(ubo_descriptor_sets_[i])
        .setDstBinding(0)
        .setDstArrayElement(0)
        .setDescriptorCount(1)
        .setDescriptorType(vk::DescriptorType::eUniformBuffer)
        .setPBufferInfo(&buffer_info);
    VkContext2::Instance().device_.updateDescriptorSets(desc_write, {});
  }
}

}  // namespace acg::gui::details
