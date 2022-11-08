#include "acg_vis/mesh_ui_world.hpp"

#include <acg_core/geometry/common_models.hpp>
#include <acg_core/math.hpp>
#include <acg_utils/log.hpp>
#include <cmath>
namespace acg::visualizer::details {

MeshWorldCtrl::MeshWorldCtrl(int n) : n_(n) {}

MeshWorldCtrl::~MeshWorldCtrl() { CleanUp(); }

int MeshWorldCtrl::RunPhysicsImpl(F64 dt) {
  // TODO: N-body simulation
  return 0;
}

void MeshWorldCtrl::InitCallback() {
  mesh_ppl_ = MeshPipeline::Builder()
                  .SetCullMode(vk::CullModeFlagBits::eNone)
                  .SetPolygonMode(vk::PolygonMode::eFill)
                  .SetIsDstPresent(false)
                  .Build();
  ACG_DEBUG_LOG("Mesh pipeline!");
  ui_only_mode_ = false;
}

void MeshWorldCtrl::CleanUp() {
  ACG_DEBUG_LOG("Mesh ppl Cleanup");
  get_vk_context().GetDevice().waitIdle();
  vertex_buffer_->Release();
  indice_buffer_->Release();
  mesh_ppl_.reset(nullptr);
}

void MeshWorldCtrl::PreRun() {
  for (int i = 0; i < n_; ++i) {
    Vec3f center{0, sin(2.0 * i / n_ * acg::constants::pi<F32>),
                 cos(2.0 * i / n_ * acg::constants::pi<F32>)};

    scene_.AddMesh(acg::geometry::sphere_20(center, 0.2), (Vec3f::Random() * 2) + Vec3f::Ones());
    geo_.push_back(P64(center.cast<F64>(), .2));
    mass_.push_back(1.0);
  }
  vertex_buffer_ = get_vk_context().CreateBuffer(
      scene_.GetRequiredVertexBufferSize(), vk::BufferUsageFlagBits::eVertexBuffer,
      vk::MemoryPropertyFlagBits::eHostCoherent | vk::MemoryPropertyFlagBits::eHostVisible);
  indice_buffer_ = get_vk_context().CreateBuffer(
      scene_.GetRequiredIndexBufferSize(), vk::BufferUsageFlagBits::eIndexBuffer,
      vk::MemoryPropertyFlagBits::eHostCoherent | vk::MemoryPropertyFlagBits::eHostVisible);
  mesh_ppl_->SetCamera(&camera_, true);
}

void MeshWorldCtrl::RecreateSwapchainCallback() { mesh_ppl_->RecreateSwapchain(); }

std::vector<vk::CommandBuffer> MeshWorldCtrl::DrawScene() {
  // TODO: Render the scene.
  const auto& vertices = scene_.GetVertices();
  const auto& indices = scene_.GetIndices();
  get_vk_context().CopyHostToBuffer(vertices.data(), *vertex_buffer_,
                                    scene_.GetRequiredVertexBufferSize());
  get_vk_context().CopyHostToBuffer(indices.data(), *indice_buffer_,
                                    scene_.GetRequiredIndexBufferSize());

  spdlog::info("Begin Render.");
  auto cb = mesh_ppl_->BeginRender();
  cb.bindVertexBuffers(0, vertex_buffer_->GetBuffer(), static_cast<vk::DeviceSize>(0));
  cb.bindIndexBuffer(indice_buffer_->GetBuffer(), 0, vk::IndexType::eUint32);
  cb.drawIndexed(indices.size(), 1, 0, 0, 0);
  return {mesh_ppl_->EndRender()};
}

}  // namespace acg::visualizer::details
