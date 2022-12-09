#include "ff.hpp"

#include <acg_core/geometry/common_models.hpp>
#include <acg_core/math/common.hpp>
#include <acg_core/math/constants.hpp>
#include <acg_utils/log.hpp>
#include <cmath>

#include "co/random.h"

#define GLFW_INCLUDE_VULKAN
#include "GLFW/glfw3.h"

FreeFall::FreeFall(int n) : n_(n) {}

FreeFall::~FreeFall() = default;

int FreeFall::RunPhysicsImpl(F64 dt) {
  ACG_DEBUG_LOG("Run Simulation For dt={}", dt);
  acceleration_.setZero();
  for (int i = 0; i < n_; ++i) {
    // for (int j = i + 1; j < n_; ++j) {
    //   auto force_direction = (particles_[i].GetCenter() - particles_[j].GetCenter());
    //   auto force_length = 0.1 * mass_[i] * mass_[j] * force_direction.cwiseAbs2().sum();
    //   auto force = force_direction.normalized() * force_length;

    //   acceleration_.col(i) -= force / mass_[i];
    //   acceleration_.col(j) += force / mass_[j];
    // }
    acceleration_.col(i) = grav.cast<F64>();
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

void FreeFall::InitCallback() {
  mesh_ppl_ = MeshPipeline::Builder()
                  .SetCullMode(vk::CullModeFlagBits::eNone)
                  .SetPolygonMode(vk::PolygonMode::eFill)
                  .SetIsDstPresent(false)
                  .Build();
  ACG_DEBUG_LOG("Mesh pipeline!");
  ui_only_mode_ = false;
}

void FreeFall::CleanUpCallback() {
  ACG_DEBUG_LOG("Mesh ppl Cleanup");
  acg::gui::get_vk_context().GetDevice().waitIdle();
  vertex_buffer_->Release();
  indice_buffer_->Release();
  mesh_ppl_.reset(nullptr);
}

void FreeFall::PreRun() {
  F64 r = 3;
  for (int i = 0; i < n_; ++i) {
    Random rand;
    Vec3f center((rand.next() % 1000) / 1000.0, r * sin(2.0 * i / n_ * acg::constants::pi<F32>),
                 r * cos(2.0 * i / n_ * acg::constants::pi<F32>));
    Vec3f color = 0.5f * (Vec3f::Random() + Vec3f::Ones());

    particles_.push_back(P64(center.cast<F64>(), .4));
    color_.push_back(color);
  }

  velocity_.resize(3, n_);
  acceleration_.resize(3, n_);
  velocity_.setZero();
  velocity_ *= 2;
  acceleration_.setZero();
  mass_.resize(n_);
  mass_.setOnes();

  RegenerateScene();
  spdlog::info("buffersize = {} + {}", scene_.GetRequiredVertexBufferSize(),
               scene_.GetRequiredIndexBufferSize());
  vertex_buffer_ = acg::gui::get_vk_context().CreateBuffer(
      scene_.GetRequiredVertexBufferSize(), vk::BufferUsageFlagBits::eVertexBuffer,
      vk::MemoryPropertyFlagBits::eHostCoherent | vk::MemoryPropertyFlagBits::eHostVisible);
  indice_buffer_ = acg::gui::get_vk_context().CreateBuffer(
      scene_.GetRequiredIndexBufferSize(), vk::BufferUsageFlagBits::eIndexBuffer,
      vk::MemoryPropertyFlagBits::eHostCoherent | vk::MemoryPropertyFlagBits::eHostVisible);

  camera_.SetPosition({-10, 0, 0});
  camera_.SetFront({1, 0, 0});
  light_.light_position_ = Vec3f(3, 0, 0);
  light_.ambient_light_color_ = Vec3f(1, 1, 1);
  light_.ambient_light_density_ = 0.5;
  light_.light_color_ = Vec3f(0.7, .7, .7);
  mesh_ppl_->SetUbo(&camera_, &light_, true);

  keyboard_callbacks_.insert({GLFW_KEY_W, [this]() {
                                this->camera_.Move(this->camera_.GetFront(),
                                                   1.0 / this->fps_limit_);
                                this->update_camera_ = true;
                                return true;
                              }});
  keyboard_callbacks_.insert({GLFW_KEY_S, [this]() {
                                this->camera_.Move(-this->camera_.GetFront(),
                                                   1.0 / this->fps_limit_);
                                this->update_camera_ = true;
                                return true;
                              }});

  keyboard_callbacks_.insert({GLFW_KEY_A, [this]() {
                                this->camera_.Move(
                                    -this->camera_.GetFront().cross(this->camera_.GetUp()),
                                    1.0 / this->fps_limit_);
                                this->update_camera_ = true;
                                return true;
                              }});
  keyboard_callbacks_.insert({GLFW_KEY_D, [this]() {
                                this->camera_.Move(
                                    this->camera_.GetFront().cross(this->camera_.GetUp()),
                                    1.0 / this->fps_limit_);
                                this->update_camera_ = true;
                                return true;
                              }});
}

void FreeFall::RecreateSwapchainCallback() {
  mesh_ppl_->RecreateSwapchain();
  update_camera_ = true;
}

std::vector<vk::CommandBuffer> FreeFall::DrawScene() {
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

void FreeFall::RunUiImpl() {
  ImGui::Begin("Control Panel");
  {
    ImVec2 button_size(ImGui::GetFontSize() * 7.0f, 0.0f);
    ImGui::Checkbox("Physics Running", &is_physics_running_);
    ImGui::SliderFloat3("Camera Position", reinterpret_cast<float*>(&(camera_.GetPosition())),
                        -10.0, 10.0);
    ImGui::SliderFloat3("Camera Front", reinterpret_cast<float*>(&(camera_.GetFront())), -10.0,
                        10.0);
    ImGui::SliderFloat3("Gravity", reinterpret_cast<float*>(grav.data()), -1.0, 1.0);
    if (ImGui::Button("Update Camera!", button_size)) {
      update_camera_ = true;
    }
  }
  ImGui::End();
}

void FreeFall::RegenerateScene() {
  scene_.Reset();
  for (Idx i = 0; i < n_; ++i) {
    scene_.AddParticle(particles_[i].Cast<F32>(), color_[i]);
  }
}
