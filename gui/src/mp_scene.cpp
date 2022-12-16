#include "acg_gui/mp_scene.hpp"

#include <acg_core/geometry/common_models.hpp>
#include <acg_core/math/all.hpp>
#include <acg_utils/log.hpp>
#include <cmath>
#include <acg_utils/align.hpp>


namespace acg::gui::details {

void MPWorldCtrl::InitCallback() {
  mesh_ppl_ = MeshPipelineWithRenderPass::Builder()
                  .SetCullMode(vk::CullModeFlagBits::eNone)
                  .SetPolygonMode(vk::PolygonMode::eFill)
                  .SetIsDstPresent(false)
                  .Build();
  ACG_DEBUG_LOG("Mesh pipeline created.");
  ui_only_mode_ = false;
  camera_.SetPosition({-10, 0, 0});
  camera_.SetFront({1, 0, 0});
  light_.light_position_ = Vec3f(3, 0, 0);
  light_.ambient_light_color_ = Vec3f(1, 1, 1);
  light_.ambient_light_density_ = 0.5;
  light_.light_color_ = Vec3f(0.7, .7, .7);
  mesh_ppl_->SetUbo(&camera_, &light_, true);
}

void MPWorldCtrl::CleanUpCallback() {
  ACG_DEBUG_LOG("Mesh ppl Cleanup.");
  acg::gui::get_vk_context().GetDevice().waitIdle();
  vertex_buffer_->Release();
  indice_buffer_->Release();
  mesh_ppl_.reset(nullptr);
}

std::vector<vk::CommandBuffer> MPWorldCtrl::DrawScene() {
  if (update_camera_) {
    mesh_ppl_->SetUbo(&camera_, &light_, true);
    update_camera_ = false;
  }
  auto [vertices, indices] = scene_.Build();
  acg::gui::get_vk_context().GetDevice().waitIdle();
  acg::gui::get_vk_context().CopyHostToBuffer(vertices.data(), *vertex_buffer_,
                                              vertices.size() * sizeof(Vertex));
  acg::gui::get_vk_context().CopyHostToBuffer(indices.data(), *indice_buffer_,
                                              indices.size() * sizeof(indices.front()));

  auto cb = mesh_ppl_->BeginRender();
  cb.bindVertexBuffers(0, vertex_buffer_->GetBuffer(), static_cast<vk::DeviceSize>(0));
  cb.bindIndexBuffer(indice_buffer_->GetBuffer(), 0, vk::IndexType::eUint32);
  cb.drawIndexed(indices.size(), 1, 0, 0, 0);
  return {mesh_ppl_->EndRender()};
}

void MPWorldCtrl::RecreateSwapchainCallback() {
  mesh_ppl_->RecreateSwapchain();
  update_camera_ = true;
}

void MPWorldCtrl::RefitBuffers() {
  auto vs = scene_.GetRequiredVertexBufferSize();
  auto is = scene_.GetRequiredIndexBufferSize();
  vs = std::max(vs, buffer_minimum_size_);
  is = std::max(is, buffer_minimum_size_);

  std::unique_ptr<VkContext::BufMem> new_vb{nullptr}, new_ib{nullptr};

  if (!vertex_buffer_ || vertex_buffer_->GetSize() < vs) {
    vs = acg::utils::align_up(vs, buffer_minimum_size_);
    new_vb = get_vk_context().CreateBuffer(
        vs, vk::BufferUsageFlagBits::eVertexBuffer,
        vk::MemoryPropertyFlagBits::eHostCoherent | vk::MemoryPropertyFlagBits::eHostVisible);
    spdlog::info("Created a new vertex buffer. size = {}", vs);
  }
  if (!indice_buffer_ || indice_buffer_->GetSize() < is) {
    is = acg::utils::align_up(is, buffer_minimum_size_);
    new_ib = get_vk_context().CreateBuffer(
        is, vk::BufferUsageFlagBits::eIndexBuffer,
        vk::MemoryPropertyFlagBits::eHostCoherent | vk::MemoryPropertyFlagBits::eHostVisible);
    spdlog::info("Created a new index buffer. size = {}", is);
  }

  if (new_vb || new_ib) {
    get_vk_context().GetDevice().waitIdle();
    vertex_buffer_.swap(new_vb);
    indice_buffer_.swap(new_ib);
  }
}

}  // namespace acg::gui::details
