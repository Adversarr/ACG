#include "acg_vis/nbody.hpp"

#include <acg_core/geometry/common_models.hpp>
#include <acg_core/math.hpp>
#include <acg_utils/log.hpp>
#include <cmath>
namespace acg::visualizer::details {

NBodySim::NBodySim(int n) : n_(n) {}

NBodySim::~NBodySim() { CleanUp(); }

int NBodySim::RunPhysicsImpl(F64 dt) {
  // TODO: N-body simulation
  ACG_DEBUG_LOG("Run Simulation For dt={}", dt);
  acceleration_.setZero();
  for (int i = 0; i < n_; ++i) {
    for (int j = i + 1; j < n_; ++j) {
      auto force_direction = (particles_[i].GetCenter() - particles_[j].GetCenter());
      auto force_length = 0.1 * mass_[i] * mass_[j] * force_direction.cwiseAbs2().sum();
      auto force = force_direction.normalized() * force_length;

      acceleration_.col(i) -= force / mass_[i];
      acceleration_.col(j) += force / mass_[j];
    }
  }

  auto new_velocity = velocity_ + acceleration_ * dt;

  for (Idx i = 0; i < n_; ++i) {
    auto& p = particles_[i];
    p.SetCenter(dt * (velocity_.col(i) + new_velocity.col(i)) + p.GetCenter());
  }

  velocity_ = new_velocity * 0.9999;
  RegenerateScene();
  return 0;
}

void NBodySim::InitCallback() {
  mesh_ppl_ = MeshPipeline::Builder()
                  .SetCullMode(vk::CullModeFlagBits::eNone)
                  .SetPolygonMode(vk::PolygonMode::eFill)
                  .SetIsDstPresent(false)
                  .Build();
  ACG_DEBUG_LOG("Mesh pipeline!");
  ui_only_mode_ = false;
}

void NBodySim::CleanUp() {
  ACG_DEBUG_LOG("Mesh ppl Cleanup");
  get_vk_context().GetDevice().waitIdle();
  vertex_buffer_->Release();
  indice_buffer_->Release();
  mesh_ppl_.reset(nullptr);
}

void NBodySim::PreRun() {
  F64 r = 3;
  for (int i = 0; i < n_; ++i) {
    Vec3f center(0, r * sin(2.0 * i / n_ * acg::constants::pi<F32>),
                 r * cos(2.0 * i / n_ * acg::constants::pi<F32>));
    Vec3f color = 0.5f * (Vec3f::Random() + Vec3f::Ones());

    particles_.push_back(P64(center.cast<F64>(), .2));
    color_.push_back(color);
  }

  velocity_.resize(3, n_);
  acceleration_.resize(3, n_);
  velocity_.setRandom();
  velocity_*= 2;
  acceleration_.setZero();
  mass_.resize(n_);
  mass_.setOnes();

  RegenerateScene();
  vertex_buffer_ = get_vk_context().CreateBuffer(
      scene_.GetRequiredVertexBufferSize(), vk::BufferUsageFlagBits::eVertexBuffer,
      vk::MemoryPropertyFlagBits::eHostCoherent | vk::MemoryPropertyFlagBits::eHostVisible);
  indice_buffer_ = get_vk_context().CreateBuffer(
      scene_.GetRequiredIndexBufferSize(), vk::BufferUsageFlagBits::eIndexBuffer,
      vk::MemoryPropertyFlagBits::eHostCoherent | vk::MemoryPropertyFlagBits::eHostVisible);
  mesh_ppl_->SetUbo(&camera_, true);
}

void NBodySim::RecreateSwapchainCallback() {
  mesh_ppl_->RecreateSwapchain();
  update_camera_ = true;
}

std::vector<vk::CommandBuffer> NBodySim::DrawScene() {
  if (update_camera_) {
    mesh_ppl_->SetUbo(&camera_, true);
    update_camera_ = false;
  }
  const auto& vertices = scene_.GetVertices();
  const auto& indices = scene_.GetIndices();
  get_vk_context().GetDevice().waitIdle();
  get_vk_context().CopyHostToBuffer(vertices.data(), *vertex_buffer_,
                                    scene_.GetRequiredVertexBufferSize());
  get_vk_context().CopyHostToBuffer(indices.data(), *indice_buffer_,
                                    scene_.GetRequiredIndexBufferSize());

  auto cb = mesh_ppl_->BeginRender();
  cb.bindVertexBuffers(0, vertex_buffer_->GetBuffer(), static_cast<vk::DeviceSize>(0));
  cb.bindIndexBuffer(indice_buffer_->GetBuffer(), 0, vk::IndexType::eUint32);
  cb.drawIndexed(indices.size(), 1, 0, 0, 0);
  return {mesh_ppl_->EndRender()};
}

void NBodySim::RunUiImpl() {
  ImGui::Begin("Control Panel");
  {
    ImVec2 button_size(ImGui::GetFontSize() * 7.0f, 0.0f);
    ImGui::Checkbox("Physics Running", &is_physics_running_);
    ImGui::SliderFloat3("Camera Position", reinterpret_cast<float*>(&(camera_.GetPosition())),
                        -10.0, 10.0);
    ImGui::SliderFloat3("Camera Front", reinterpret_cast<float*>(&(camera_.GetFront())), -10.0,
                        10.0);
    if (ImGui::Button("Update Camera!", button_size)) {
      update_camera_ = true;
    }
    // Setup Camera:
  }
  ImGui::End();
}

void NBodySim::RegenerateScene() {
  scene_.Reset();
  for (Idx i = 0; i < n_; ++i) {
    scene_.AddMesh(
        geometry::sphere_20(particles_[i].GetCenter().cast<F32>(), particles_[i].GetRadius()),
        color_[i]);
  }
}

}  // namespace acg::visualizer::details
