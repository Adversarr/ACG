#include "nbody.hpp"

#include <co/random.h>

#include <acg_core/geometry/common_models.hpp>
#include <acg_core/math.hpp>
#include <acg_utils/log.hpp>
#include <cmath>
NBodySim::NBodySim(int n) : n_(n) {}


int NBodySim::RunPhysicsImpl(F64 dt) {
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

void NBodySim::PreRun() {
  F64 r = 3;
  // Initialzie the particles.
  for (int i = 0; i < n_; ++i) {
    Random rand(clock());  // Random number generator.

    // Center for Particle[i]
    Vec3f center((rand.next() % 1000) / 1000.0, r * sin(2.0 * i / n_ * acg::constants::pi<F32>),
                 r * cos(2.0 * i / n_ * acg::constants::pi<F32>));
    // NOTE: acg::constants::pi<F32> will returns 3.1415.... in float.
    //       therefore acg::constants::pi<F64> will returns 3.1415.... in double
    //  See: Variable Template (since C++ 14) / 变量模版

    // Color for Particle[i]
    Vec3f color = 0.5f * (Vec3f::Random() + Vec3f::Ones());

    // NOTE: particles_ is vector<Particle<F64>>, vector is a standard "container" in c++ stl. (列表类型)
    particles_.push_back(P64(center.cast<F64>(), .4));
    color_.push_back(color);
  }

  // Initialize velocity, each column (R^3) represents one particle's velocity
  velocity_.resize(3, n_);
  velocity_.setRandom();
  velocity_ *= 2;
  // Initialize accelerations.
  acceleration_.resize(3, n_);
  acceleration_.setZero();
  // Initialize particles' mass
  mass_.resize(n_);
  mass_.setOnes();

  // Generate the scene for rendering
  RegenerateScene();
  // Refit the Buffers according to the scene
  RefitBuffers();

  // Setup Camera
  camera_.SetPosition({-10, 0, 0});
  camera_.SetFront({1, 0, 0});
  // Setup Light
  light_.light_position_ = Vec3f(3, 0, 0);
  light_.ambient_light_color_ = Vec3f(1, 1, 1);
  light_.ambient_light_density_ = 0.5;
  light_.light_color_ = Vec3f(0.7, .7, .7);
  // Push camera and light data to the renderer(Mesh Pipeline)
  mesh_ppl_->SetUbo(&camera_, &light_, true);
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
  }
  ImGui::End();
}

void NBodySim::RegenerateScene() {
  // Clear previous scene
  scene_.Reset();
  // push the particles to the scene
  for (Idx i = 0; i < n_; ++i) {
    scene_.AddParticle(particles_[i].Cast<F32>(), color_[i]);
  }

}
