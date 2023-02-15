#include "api.hpp"

#include <acore/math/common.hpp>
#include <autils/align.hpp>
using namespace acg;

// void Api::CleanUpCallback() {
//   ACG_DEBUG_LOG("Mesh ppl Cleanup.");
//   acg::gui::get_vk_context().GetDevice().waitIdle();
//   vertex_buffer_->Release();
//   indice_buffer_->Release();
//   wireframe_indice_buffer_->Release();
//   wireframe_vertex_buffer_->Release();
//   mesh_ppl_.reset(nullptr);
//   wireframe_pipeline_.reset(nullptr);
// }

// void Api::InitCallback() {
//   graphics_render_pass_ = details::GraphicsRenderPass::Builder().Build();
//   mesh_ppl_ = details::MeshPipeline::Builder{}.Build(*graphics_render_pass_);
//   ACG_DEBUG_LOG("Mesh pipeline created.");
//   wireframe_pipeline_ = details::WireframePipeline::Builder{}.Build(*graphics_render_pass_);
//   ACG_DEBUG_LOG("Wireframe pipeline created.");
//   ui_only_mode_ = false;
//   camera_.SetPosition({-10, 0, 0});
//   camera_.SetFront({1, 0, 0});
//   light_.light_position_ = Vec3f(3, 3, 3);
//   light_.ambient_light_color_ = Vec3f(.3, .3, .3);
//   light_.ambient_light_density_ = 1;
//   mesh_ppl_->SetCamera(camera_);
//   wireframe_pipeline_->SetCamera(camera_);
//   mesh_ppl_->SetLight(light_);
//   mesh_ppl_->UpdateUbo(true);
//   wireframe_pipeline_->UpdateUbo(true);

//   wireframe_vertex_buffer_ = get_vk_context().CreateBuffer(
//       10240, vk::BufferUsageFlagBits::eVertexBuffer,
//       vk::MemoryPropertyFlagBits::eHostCoherent | vk::MemoryPropertyFlagBits::eHostVisible);

//   wireframe_indice_buffer_ = get_vk_context().CreateBuffer(
//       10240, vk::BufferUsageFlagBits::eIndexBuffer,
//       vk::MemoryPropertyFlagBits::eHostCoherent | vk::MemoryPropertyFlagBits::eHostVisible);
//   graphics_render_pass_->SetBackgroundColor({0.3, 0.3, 0.3, 1});
// }

// std::vector<vk::CommandBuffer> Api::DrawScene() {
//   scene_.Reset();
//   scene_.AddParticle(geometry::Particle<F32>({0, 0, 0}, 1), {.4, .4, .7});
//   RefitBuffers();
//   auto [vertices, indices] = scene_.Build();
//   acg::gui::get_vk_context().GetDevice().waitIdle();
//   acg::gui::get_vk_context().CopyHostToBuffer(vertices.data(), *vertex_buffer_,
//                                               vertices.size() * sizeof(Vertex));
//   acg::gui::get_vk_context().CopyHostToBuffer(indices.data(), *indice_buffer_,
//                                               indices.size() * sizeof(indices.front()));

//   std::vector<Point> line_vertices;
//   std::vector<uint32_t> line_indices;
//   for (size_t i = 0; i * 3 < indices.size(); ++i) {
//     line_indices.push_back(indices[i * 3]);
//     line_indices.push_back(indices[i * 3 + 1]);
//     line_indices.push_back(indices[i * 3 + 1]);
//     line_indices.push_back(indices[i * 3 + 2]);
//     line_indices.push_back(indices[i * 3]);
//     line_indices.push_back(indices[i * 3 + 2]);
//   }

//   ACG_INFO("Wireframe Index Buffer size = {}", line_indices.size() * sizeof(uint32_t));
//   for (const auto& vert : vertices) {
//     line_vertices.push_back(Point(vert.position_, vert.color_));
//   }
//   ACG_INFO("Wireframe Vertex Buffer size = {}", line_vertices.size() * sizeof(Point));

//   acg::gui::get_vk_context().CopyHostToBuffer(line_vertices.data(), *wireframe_vertex_buffer_,
//                                               line_vertices.size() * sizeof(Point));
//   acg::gui::get_vk_context().CopyHostToBuffer(line_indices.data(), *wireframe_indice_buffer_,
//                                               line_indices.size() * sizeof(line_indices.front()));

//   auto cb = graphics_render_pass_->BeginRender();
//   mesh_ppl_->BeginPipeline(cb);
//   cb.bindVertexBuffers(0, vertex_buffer_->GetBuffer(), static_cast<vk::DeviceSize>(0));
//   cb.bindIndexBuffer(indice_buffer_->GetBuffer(), 0, vk::IndexType::eUint32);
//   cb.drawIndexed(indices.size(), 1, 0, 0, 0);

//   wireframe_pipeline_->BeginPipeline(cb);
//   cb.setLineWidth(1);
//   cb.bindVertexBuffers(0, wireframe_vertex_buffer_->GetBuffer(), static_cast<vk::DeviceSize>(0));
//   cb.bindIndexBuffer(wireframe_indice_buffer_->GetBuffer(), 0, vk::IndexType::eUint32);
//   cb.drawIndexed(line_indices.size(), 1, 0, 0, 0);
//   graphics_render_pass_->EndRender();
//   return {cb};
// }

// void Api::RefitBuffers() {
//   auto vs = scene_.GetRequiredVertexBufferSize();
//   auto is = scene_.GetRequiredIndexBufferSize();
//   decltype(vs) buffer_minimum_size = 1024;
//   vs = std::max(vs, buffer_minimum_size);
//   is = std::max(is, buffer_minimum_size);

//   std::unique_ptr<VkContext::BufMem> new_vb{nullptr}, new_ib{nullptr};

//   if (!vertex_buffer_ || vertex_buffer_->GetSize() < vs) {
//     vs = acg::utils::align_up(vs, buffer_minimum_size);
//     new_vb = get_vk_context().CreateBuffer(
//         vs, vk::BufferUsageFlagBits::eVertexBuffer,
//         vk::MemoryPropertyFlagBits::eHostCoherent | vk::MemoryPropertyFlagBits::eHostVisible);
//     ACG_INFO("Created a new vertex buffer. size = {}", vs);
//   }
//   if (!indice_buffer_ || indice_buffer_->GetSize() < is) {
//     is = acg::utils::align_up(is, buffer_minimum_size);
//     new_ib = get_vk_context().CreateBuffer(
//         is, vk::BufferUsageFlagBits::eIndexBuffer,
//         vk::MemoryPropertyFlagBits::eHostCoherent | vk::MemoryPropertyFlagBits::eHostVisible);
//     ACG_INFO("Created a new index buffer. size = {}", is);
//   }

//   if (new_vb || new_ib) {
//     get_vk_context().GetDevice().waitIdle();
//     vertex_buffer_.swap(new_vb);
//     indice_buffer_.swap(new_ib);
//   }
// }

// void Api::RecreateSwapchainCallback() {
//   graphics_render_pass_->RecreateSwapchain();
//   mesh_ppl_->Recreate(*graphics_render_pass_);
//   mesh_ppl_->SetCamera(camera_);
//   mesh_ppl_->UpdateUbo(true);
// }
