#include "app.hpp"

#include <acg_utils/log.hpp>
MpmExplictApp::MpmExplictApp() = default;

void MpmExplictApp::Init() {
  dx_ = simulation_range_ / n_grid_;

  int grid_size = (n_grid_ + 1) * (n_grid_ + 1) * (n_grid_ + 1);
  grid_velocity_.resize(Eigen::NoChange, grid_size);
  grid_mass_.resize(Eigen::NoChange, grid_size);
  grid_mass_.setZero();
  grid_size_ = grid_size;

  // Particles.
  n_particles_ = grid_size * 2;
  particle_C_.resize(Eigen::NoChange, n_particles_);
  particle_J_.resize(Eigen::NoChange, n_particles_);
  particle_position_.resize(Eigen::NoChange, n_particles_);
  particle_velocity_.resize(Eigen::NoChange, n_particles_);
  particle_C_.setZero();
  particle_velocity_.setZero();
  for (Index i = 0; i < n_particles_; ++i) {
    Vec3d dpos = Vec3d::Ones() * 0.5 + Vec3d::Random() * .4;
    particle_position_.col(i) = dpos;
    particle_J_(i) = 1.0;
  }
  particle_vol_ = std::pow(dx_, 3) * 0.3;
  particle_mass_ = rho_ * particle_vol_;
}

void MpmExplictApp::P2G() {
  grid_mass_.setZero();
  grid_velocity_.setZero();
  // Foreach particle
  // base --- *
  //  |  [p]  |
  //  * ----- *
  for (Index i = 0; i < n_particles_; ++i) {
    Vec3d xp = (particle_position_.col(i) / dx_);
    Vec3d base_f = xp.array().floor().matrix();
    Vec3Index base = base_f.cast<Index >();
    // std::cout << base << std::endl;
    Vec3d fx = xp - base_f;
    // F64 stress = -dt_ * 4 * E_ * particle_vol_ * (particle_J_(i) - 1);
    // Mat3x3d affine
    //     = Mat3x3d::Identity() * stress + particle_mass_ * particle_C_.col(i).reshaped(3, 3);
    for (Index di = 0; di <= 1; ++di) {
      for (Index dj = 0; dj <= 1; ++dj) {
        for (Index dk = 0; dk <= 1; ++dk) {
          Index gid = GetGridIndex(base.x() + di, base.y() + dj, base.z() + dk);
          assert(gid < grid_size_);
          // compute the weight.
          auto weight_d = (Vec3d(1 - di, 1 - dj, 1 - dk) - fx);
          F64 weight = abs(weight_d.x() * weight_d.y() * weight_d.z());
          // TODO: Affine Transform.
          // ACG_DEBUG_LOG("Gid = {}", gid);
          grid_mass_(gid) += weight;
          grid_velocity_.col(gid) += weight * (particle_velocity_.col(i));
        }
      }
    }
  }
}

void MpmExplictApp::Projection() {
  for (Index i = 0; i <= n_grid_; ++i) {
    for (Index j = 0; j <= n_grid_; ++j) {
      for (Index k = 0; k <= n_grid_; ++k) {
        Index gid = GetGridIndex(i, j, k);
        if (grid_mass_(gid) > 0) {
          grid_velocity_.col(gid) /= grid_mass_(gid);
        } else {
          grid_velocity_.col(gid).setZero();
        }
        grid_velocity_.col(gid) += dt_ * grav_.cast<F64>();
      }
    }
  }
  for (Index i = 1; i < n_grid_; ++i) {
    for (Index j = 1; j < n_grid_; ++j) {
      for (Index k = 1; k < n_grid_; ++k) {
        Index g_dx0 = GetGridIndex(i - 1, j, k);
        Index g_dx1 = GetGridIndex(i + 1, j, k);
        Index g_dy0 = GetGridIndex(i, j - 1, k);
        Index g_dy1 = GetGridIndex(i, j + 1, k);
        Index g_dz0 = GetGridIndex(i, j, k - 1);
        Index g_dz1 = GetGridIndex(i, j, k + 1);
        Index g_this = GetGridIndex(i, j, k);
        F64 m_dx = grid_mass_(g_dx1) - grid_mass_(g_dx0);
        F64 m_dy = grid_mass_(g_dy1) - grid_mass_(g_dy0);
        F64 m_dz = grid_mass_(g_dz1) - grid_mass_(g_dz0);
        grid_velocity_.col(g_this) -= 10 * dt_ * Vec3d(m_dx, m_dy, m_dz) / 2;
      }
    }
  }

  for (Index i = 0; i <= n_grid_; ++i) {
    for (Index j = 0; j <= n_grid_; ++j) {
      for (Index k = 0; k <= n_grid_; ++k) {
        auto blk = grid_velocity_.col(GetGridIndex(i, j, k));
        if (i == 0) {
          blk.x() = abs(blk.x());
        } else if (i == n_grid_) {
          blk.x() = -abs(blk.x());
        }
        if (j == 0) {
          blk.y() = abs(blk.y());
        } else if (j == n_grid_) {
          blk.y() = -abs(blk.y());
        }
        if (k == 0) {
          blk.z() = abs(blk.z());
        } else if (k == n_grid_) {
          blk.z() = -abs(blk.z());
        }
      }
    }
  }
}

void MpmExplictApp::G2P() {
  Field<F64, 3> new_vel;
  new_vel.resizeLike(particle_velocity_);
  new_vel.setZero();
  weight_sum = 0;
  for (Index i = 0; i < n_particles_; ++i) {
    Vec3d xp = (particle_position_.col(i) / dx_);
    Vec3d base_f = xp.array().floor().matrix();
    Vec3Index base = base_f.cast<Index >();
    Vec3d fx = xp - base_f;
    F64 weight_sum_local = 0;
    auto blk = new_vel.col(i);
    for (Index di = 0; di <= 1; ++di) {
      for (Index dj = 0; dj <= 1; ++dj) {
        for (Index dk = 0; dk <= 1; ++dk) {
          Index gid = GetGridIndex(base.x() + di, base.y() + dj, base.z() + dk);
          // compute the weight.
          auto weight_d = (Vec3d(1 - di, 1 - dj, 1 - dk) - fx);
          F64 weight = abs(weight_d.x() * weight_d.y() * weight_d.z());
          assert(weight < 1);
          weight_sum_local += weight;

          // TODO: Affine Transform.
          blk += weight * grid_velocity_.col(gid);
        }
      }
    }
    weight_sum += weight_sum_local;
  }
  weight_sum /= n_particles_;

  auto new_position = (particle_position_ + (particle_velocity_ + new_vel) * dt_ * .5).eval();
  for (auto blk : new_position.colwise()) {
    blk.x() = std::clamp(blk.x(), 0.01, 0.99);
    blk.y() = std::clamp(blk.y(), 0.01, .99);
    blk.z() = std::clamp(blk.z(), 0.01, .99);
  }
  particle_velocity_ = (new_position - particle_position_) / dt_;
  for (auto blk : particle_velocity_.colwise()) {
    // blk.x() = std::clamp(blk.x(), 0.01, 0.99);
    // blk.y() = std::clamp(blk.y(), 0.01, .99);
    // blk.z() = std::clamp(blk.z(), 0.01, .99);
  }
  particle_position_ = new_position;
}

void MpmExplictApp::Run() {
  P2G();
  Projection();
  G2P();
}
