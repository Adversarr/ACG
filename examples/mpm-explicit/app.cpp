#include "app.hpp"

MpmExplictApp::MpmExplictApp() {
  dx_ = simulation_range_ / n_grid_;

  int grid_size = (n_grid_ + 1) * (n_grid_ + 1) * (n_grid_ + 1);
  grid_velocity_.resize(Eigen::NoChange, grid_size);
  grid_mass_.resize(Eigen::NoChange, grid_size);
  grid_mass_.setZero();

  // Particles.
  n_particles_ = grid_size;
  particle_C_.resize(Eigen::NoChange, n_particles_);
  particle_J_.resize(Eigen::NoChange, n_particles_);
  particle_position_.resize(Eigen::NoChange, n_particles_);
  particle_velocity_.resize(Eigen::NoChange, n_particles_);
}

void MpmExplictApp::Init() {
  particle_C_.setZero();
  particle_velocity_.setZero();
  for (Idx i = 0; i < n_particles_; ++i) {
    Vec3d dpos = Vec3d::Ones() * 0.2 + Vec3d::Random() * .4;
    particle_position_.col(i) = dpos;
    particle_J_(i) = 1.0;
  }
}

void MpmExplictApp::P2G() {
  // Foreach particle
  for (Idx i = 0; i < n_particles_; ++i) {
    Vec3d xp = (particle_position_.col(i) / dx_);
    Vec3Idx base = (xp.array() - .5).floor().matrix().cast<Idx>();
    Vec3d fx = xp - (xp.array() - 0.5).floor().matrix();
    Mat3x3d w;
    w.col(0) = (0.5 * (1.5 - fx.array()).square()).matrix();
    w.col(1) = (0.75 - (fx.array() - 1).square()).matrix();
    w.col(2) = (0.5 * (fx.array() - 0.5).square()).matrix();
    F64 stress = -dt_ * 4 * E_ * particle_vol_ * (particle_J_(i) - 1);
    Mat3x3d affine = Mat3x3d::Identity() * stress + 
      particle_mass_ * particle_C_.col(i).reshaped(3, 3);
    for (Idx di = -1; di <= 1; ++di) {
      for (Idx dj = -1; dj <= 1; ++dj) {
        for (Idx dk = -1; dk <= -1; ++dk) {
          Idx gid = GetGridIndex(base.x() + di, base.y() + dj, base.z() + dk);

          auto dpos = ()
        }
      }
    }
  }
}
