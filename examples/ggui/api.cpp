#include "api.hpp"
#include <co/god.h>
#include <acg_core/math/common.hpp>
using namespace acg;

void Api::CleanUpCallback() {
  ACG_DEBUG_LOG("Mesh ppl Cleanup.");
  acg::gui::get_vk_context().GetDevice().waitIdle();
  vertex_buffer_->Release();
  indice_buffer_->Release();
  mesh_ppl_.reset(nullptr);
}

void Api::InitCallback() {
  graphics_render_pass_ = details::GraphicsRenderPass::Builder().Build();
  mesh_ppl_ = details::MeshPipeline2::Builder{}.Build(*graphics_render_pass_);
  ACG_DEBUG_LOG("Mesh pipeline created.");
  ui_only_mode_ = false;
  camera_.SetPosition({-10, 0, 0});
  camera_.SetFront({1, 0, 0});
  light_.light_position_ = Vec3f(3, 3, 3);
  mesh_ppl_->SetCamera(camera_);
  mesh_ppl_->SetLight(light_);
  mesh_ppl_->UpdateUbo(true);

  graphics_render_pass_->SetBackgroundColor({0.3, 0.3, 0.3, 1});
}

std::vector<vk::CommandBuffer> Api::DrawScene() {
  scene_.Reset();
  scene_.AddParticle(geometry::Particle<F32>({0, 0, 0}, 1), {.5, .6, .7});
  RefitBuffers();
  auto [vertices, indices] = scene_.Build();
  ACG_DEBUG_LOG("v: {}, i: {}", vertices.size(), indices.size());

  acg::gui::get_vk_context().GetDevice().waitIdle();
  acg::gui::get_vk_context().CopyHostToBuffer(vertices.data(), *vertex_buffer_,
                                              vertices.size() * sizeof(Vertex));
  acg::gui::get_vk_context().CopyHostToBuffer(indices.data(), *indice_buffer_,
                                              indices.size() * sizeof(indices.front()));

  auto cb = graphics_render_pass_->BeginRender();
  mesh_ppl_->BeginPipeline(cb);
  cb.bindVertexBuffers(0, vertex_buffer_->GetBuffer(), static_cast<vk::DeviceSize>(0));
  cb.bindIndexBuffer(indice_buffer_->GetBuffer(), 0, vk::IndexType::eUint32);
  cb.drawIndexed(indices.size(), 1, 0, 0, 0);
  graphics_render_pass_->EndRender();
  return {cb};
}

void Api::RefitBuffers() {
  auto vs = scene_.GetRequiredVertexBufferSize();
  auto is = scene_.GetRequiredIndexBufferSize();
  decltype(vs) buffer_minimum_size_ = 1024;
  vs = std::max(vs, buffer_minimum_size_);
  is = std::max(is, buffer_minimum_size_);

  std::unique_ptr<VkContext::BufMem> new_vb{nullptr}, new_ib{nullptr};

  if (!vertex_buffer_ || vertex_buffer_->GetSize() < vs) {
    vs = god::align_up(vs, buffer_minimum_size_);
    new_vb = get_vk_context().CreateBuffer(
        vs, vk::BufferUsageFlagBits::eVertexBuffer,
        vk::MemoryPropertyFlagBits::eHostCoherent | vk::MemoryPropertyFlagBits::eHostVisible);
    spdlog::info("Created a new vertex buffer. size = {}", vs);
  }
  if (!indice_buffer_ || indice_buffer_->GetSize() < is) {
    is = god::align_up(is, buffer_minimum_size_);
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

