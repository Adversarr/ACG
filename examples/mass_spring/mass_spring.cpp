#include "mass_spring.hpp"

geometry::SimpleMesh<F64> make_plane_xy(Idx n) {
  // z = 0, xy in [0, 1]
  // 3x3 => x = 0, 0.5, 1
  //        y = 0, 0.5, 1
  //        z = 0.
  Attr<F64, 3> vertices(3, n * n);
  geometry::SimpleMesh<F64>::FacesType faces(3, 2 * (n - 1) * (n - 1));
  for (Idx i = 0; i < n; ++i) {
    for (Idx j = 0; j < n; ++j) {
      Idx idx = i * n + j;
      vertices.col(idx)
          = Vec3d(static_cast<double>(i) / (n - 1), static_cast<double>(j) / (n - 1), 0);
    }
  }

  for (Idx i = 0; i < n - 1; ++i) {
    for (Idx j = 0; j < n - 1; ++j) {
      Idx idx = 2 * (i * (n - 1) + j);
      Idx lt = i * n + j;
      Idx rt = i * n + j + 1;
      Idx lb = (i + 1) * n + j;
      Idx rb = (i + 1) * n + j + 1;

      faces.col(idx) = Vec3Idx(lt, rt, lb);
      faces.col(idx + 1) = Vec3Idx(rt, rb, lb);
    }
  }
  return {vertices, faces};
}

MassSpring::MassSpring(Idx n) : n_(n), mesh_(make_plane_xy(n)) {}

int MassSpring::RunPhysicsImpl(F64 dt) {
  acceleration_.setZero();
  for (Idx i = 0; i < n_ * n_ ; ++i) {
    acceleration_.col(i) += Vec3d(0, 0, -1);
  }
  for (Idx i = 0; i < n_ - 1; ++i) {
    for (Idx j = 0; j < n_ - 1; ++j) {
      Idx lt = i * n_ + j;
      Idx rt = i * n_ + j + 1;
      Idx lb = (i + 1) * n_ + j;
      Idx rb = (i + 1) * n_ + j + 1;

      ApplyForce(lt, rt);
      ApplyForce(lt, lb);
      ApplyForce(rt, rb);
      ApplyForce(lb, rb);
      ApplyForce(lt, rb);
      ApplyForce(rt, lb);
    }
  }


  auto new_velocity = velocity_ + acceleration_ * dt;
  Attr<F64, 3> position = mesh_.GetVertices() + (velocity_ + new_velocity) * 0.5 * dt;
  position.col(0) = Vec3d::Zero();
  position.col(n_ - 1) = Vec3d(0, 1, 0);
  velocity_ = new_velocity * .9;

  mesh_ = Mesh(position, mesh_.GetFaces());

  RegenerateScene();
  return 0;
}

void MassSpring::RunUiImpl() {
  ImGui::Begin("Control Panel");
  {
    ImVec2 button_size(ImGui::GetFontSize() * 7.0f, 0.0f);
    ImGui::Checkbox("Physics Running", &is_physics_running_);
    ImGui::SliderFloat3("Camera Position", reinterpret_cast<float*>(&(camera_.GetPosition())),
                        -10.0, 10.0);
    ImGui::SliderFloat3("Camera Front", reinterpret_cast<float*>(&(camera_.GetFront())), -10.0,
                        10.0);

    ImGui::SliderFloat("K", &k_, 0, 10000);
    if (ImGui::Button("Update Camera!", button_size)) {
      update_camera_ = true;
    }
  }
  ImGui::End();
}

void MassSpring::PreRun() {
  color_ = Vec3f(.7, .7, .7);
  RegenerateScene();
  RefitBuffers();

  velocity_.resize(3, n_ * n_);
  velocity_.setZero();
  acceleration_.resize(3, n_ * n_);
  camera_.SetPosition(glm::vec3(1, 1, 1));
  camera_.SetFront(glm::vec3(-1, -1, -1));
  light_.light_position_ = Vec3f(3, 0, 0);
  light_.ambient_light_color_ = Vec3f(1, 1, 1);
  light_.ambient_light_density_ = 0.5;
  light_.light_color_ = Vec3f(.7, .7, .7);
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

void MassSpring::RegenerateScene() {
  scene_.Reset();
  // for (int i = 0; i < n_; ++i) {
  //   for (int j = 0; j < n_; ++j) {
  //     scene_.AddParticle(
  //         geometry::Particle<F32>(mesh_.GetVertices().col(i * n_ + j).cast<F32>(), 0.1), color_);
  //   }
  // }
  scene_.AddMesh(mesh_.Cast<F32>(), std::nullopt, color_);
}

void MassSpring::ApplyForce(Idx i, Idx j) {
  auto dij = mesh_.GetVertices().col(i) - mesh_.GetVertices().col(j);
  F64 original_length = 1.0 / (n_ - 1);

  F64 length = dij.norm();

  F64 force_length = k_ * (length - original_length);

  acceleration_.col(i) -= dij.normalized() * force_length;
  acceleration_.col(j) += dij.normalized() * force_length;
}
