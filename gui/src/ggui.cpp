#include "acg_gui/ggui.hpp"

#include "glm/gtc/type_ptr.hpp"

namespace acg::gui::details {

GGui::GGui(const GGui::Config& config)
    : staging_buffer_(VkContext2::Instance().CreateBufferWithMemory(
        config.staging_buffer_size, vk::BufferUsageFlagBits::eTransferSrc,
        vk::MemoryPropertyFlagBits::eHostCoherent | vk::MemoryPropertyFlagBits::eHostVisible)) {
  ACG_DEBUG_LOG("Init ggui");
  // Render pass:
  UiPass::Config ui_config;
  ui_config.is_ui_only = false;
  ui_pass_ = std::make_unique<UiPass>(ui_config);
  GraphicsRenderPass::Config gr_config;
  gr_config.is_present = false;
  gr_config.max_descriptor_set_count = 24;
  gr_config.required_descriptor_sizes.push_back({vk::DescriptorType::eUniformBuffer, 48});
  graphics_pass_ = std::make_unique<GraphicsRenderPass>(gr_config);
  // Pipeline:
  MeshPipeline::Config mp_config;
  mp_config.cull_mode_ = vk::CullModeFlagBits::eNone;
  mp_config.enable_color_blending = true;
  mesh_pipeline_ = std::make_unique<MeshPipeline>(*graphics_pass_, mp_config);
  PointPipeline::Config pp_config;
  pp_config.enable_color_blending = true;
  point_pipeline_ = std::make_unique<PointPipeline>(*graphics_pass_, pp_config);
  WireframePipeline::Config wf_config;
  wireframe_pipeline_ = std::make_unique<WireframePipeline>(*graphics_pass_, wf_config);

  // Staging buffer init
  InitStagingBuffer();
}

void GGui::InitStagingBuffer() {
  auto* p_data = VkContext2::Instance().device_.mapMemory(staging_buffer_.GetMemory(), 0,
                                                          staging_buffer_.GetSize());
  staging_buffer_.SetMappedMemory(p_data);
}

void GGui::DestroyStagingBuffer() {
  VkContext2::Instance().device_.unmapMemory(staging_buffer_.GetMemory());
  VkContext2::Instance().DestroyBufferWithMemory(staging_buffer_);
}

void GGui::ReserveBuffer(vk::DeviceSize size, BufferID id) {
  auto& buffer = GetAllocatedBuffer(id);
  VkContext2::Instance().DestroyBufferWithMemory(buffer);
  buffer = VkContext2::Instance().CreateBufferWithMemory(size, buffer.usage_, buffer.properties_);
}

BufferID GGui::CreateVertexBuffer(vk::DeviceSize size) {
  auto id = allocated_buffers_.size();
  auto buffer = VkContext2::Instance().CreateBufferWithMemory(
      size, vk::BufferUsageFlagBits::eTransferDst | vk::BufferUsageFlagBits::eVertexBuffer,
      vk::MemoryPropertyFlagBits::eDeviceLocal);
  allocated_buffers_.push_back(buffer);
  return id;
}

BufferID GGui::CreateIndexBuffer(vk::DeviceSize size) {
  auto id = allocated_buffers_.size();
  auto buffer = VkContext2::Instance().CreateBufferWithMemory(
      size, vk::BufferUsageFlagBits::eTransferDst | vk::BufferUsageFlagBits::eIndexBuffer,
      vk::MemoryPropertyFlagBits::eDeviceLocal);
  allocated_buffers_.push_back(buffer);
  return id;
}

BufferID GGui::CreateInstanceBuffer(vk::DeviceSize size) { return CreateVertexBuffer(size); }

void GGui::PrepareBuffers() {
  for (const auto& mesh : scene_.GetMesh()) {
    // collect info
    if (mesh.id >= mesh_render_info_.size()) {
      mesh_render_info_.emplace_back();
    }
    auto& info = mesh_render_info_[mesh.id];
    info.vertex_count = mesh.vertices.cols();
    info.index_count = mesh.faces.cols() * 3;
    info.instance_count = mesh.instance_count;
    Mat4x4f model_t = mesh.model.transpose();
    info.model = glm::make_mat4x4(model_t.data());
    info.specular = mesh.specular_shiness;
    info.use_double_side_lighting = mesh.use_double_side_lighting;
    PrepareMeshBuffer(info);
  }

  for (const auto& part : scene_.GetParticles()) {
    if (part.id >= particle_render_info_.size()) {
      particle_render_info_.emplace_back();
    }
    auto& info = particle_render_info_[part.id];
    if (part.colors.cols() > 0) {
      Vec4f color = part.colors.col(0);
      info.color = glm::make_vec4<float>(color.data());
    } else {
      info.color = glm::vec4(0.8, 0.8, 0.8, 1.0);
    }
    info.use_uniform_color = part.use_uniform_color;
    info.size = part.radius;
    info.vertex_count = part.positions.cols();
    PrepareParticleBuffer(info);
  }

  for (const auto& wf : scene_.GetWireframe()) {
    if (wf.id >= wireframe_render_info_.size()) {
      wireframe_render_info_.emplace_back();
    }
    auto& info = wireframe_render_info_[wf.id];
    info.index_count = wf.indices.cols() * 2;
    info.vertex_count = wf.positions.cols();
    PrepareWireframeBuffer(info);
  }
}

void GGui::FillBuffers() {}

void GGui::RenderOnce() {
  auto result = acg::gui::VkGraphicsContext::Instance().BeginDraw();
  while (!result) {
    RecreateSwapchain();
    result = acg::gui::VkGraphicsContext::Instance().BeginDraw();
  }
  auto cbuf = graphics_pass_->BeginRender();
  // TODO: Graphics Draw.
  graphics_pass_->EndRender();
  ImGui_ImplVulkan_NewFrame();
  ImGui_ImplGlfw_NewFrame();
  ImGui::NewFrame();
  if (ui_draw_callback_.has_value()) {
    (*ui_draw_callback_)();
  } else {
    ImGui::ShowUserGuide();
  }
  ImGui::End();
  ImGui::Render();
  auto* data = ImGui::GetDrawData();
  auto ui_cbuf = ui_pass_->Render(data);
  auto result2 = acg::gui::VkGraphicsContext::Instance().EndDraw({cbuf, ui_cbuf});
  if (result2) {
    RecreateSwapchain();
  }
}

}  // namespace acg::gui::details
