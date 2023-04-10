#include "app.hpp"
#include "acore/math/sparse.hpp"
#include <acore/math/utilities.hpp>

#include <autils/log.hpp>
MpmExplictApp::MpmExplictApp() = default;

void MpmExplictApp::Init() {
  dx_ = simulation_range_ / n_grid_;

  Index grid_size = (n_grid_ + 1) * (n_grid_ + 1) * (n_grid_ + 1);
  grid_velocity_.resize(Eigen::NoChange, grid_size);
  grid_mass_.resize(Eigen::NoChange, grid_size);
  grid_mass_.setZero();
  grid_size_ = grid_size;

  // Particles.
  n_particles_ = grid_size;
  particle_C_.resize(Eigen::NoChange, n_particles_);
  particle_J_.resize(Eigen::NoChange, n_particles_);
  particle_position_.resize(Eigen::NoChange, n_particles_);
  particle_velocity_.resize(Eigen::NoChange, n_particles_);
  particle_C_.setZero();
  particle_velocity_.setZero();
  for (Index i = 0; i < n_particles_; ++i) {
    Vec3d dpos = Vec3d::Ones() * 0.5 + Vec3d::Random() * 0.499;
    particle_position_.col(i) = dpos;
    particle_J_(i) = 1.0;
  }
  particle_vol_ = std::pow(dx_, 3) * 0.3;
  particle_mass_ = rho_ * particle_vol_;

  lag_.mass_.setConstant(1, n_particles_, particle_mass_);
  lag_.velocity_ = particle_velocity_;
  lag_.position_ = particle_position_;

  euler_.grid_size_ = dx_;
  euler_.div_count_ = Vec3Index::Constant(n_grid_);
  euler_.lower_bound_.setZero();
  euler_.upper_bound_.setOnes();
  euler_.mass_.resize(1, math::pow<3>(n_grid_));
  euler_.velocity_.resize(3, math::pow<3>(n_grid_));
  euler_.velocity_.setZero();
  using APIC = physics::mpm::ApicRegular<Float64, 3>;
  apic_ = std::make_unique<APIC>(lag_, euler_);

  using Trip = Eigen::Triplet<double>;
  std::vector<Trip> hessian;
  auto dvc = euler_.div_count_;
  auto idxer = DiscreteStorageSequentialTransform<3>({dvc.x(), dvc.y(), dvc.z()});
  for (auto [i, j, k] : NdRange<3>(make_tuple_from_vector(euler_.div_count_))) {
    auto row = idxer(i, j, k);
    for (Index dim = 0; dim < 3; ++dim) {
      if (i == 0) {
        hessian.push_back(Trip(row, dim + 3 * idxer(i, j, k), -1));
        hessian.push_back(Trip(row, dim + 3 * idxer(i + 1, j, k), 1));
      } else if (i == dvc.x() - 1) {
        hessian.push_back(Trip(row, dim + 3 * idxer(i - 1, j, k), -1));
        hessian.push_back(Trip(row, dim + 3 * idxer(i, j, k), 1));
      } else {
        hessian.push_back(Trip(row, dim + 3 * idxer(i - 1, j, k), -0.5 * 1));
        hessian.push_back(Trip(row, dim + 3 * idxer(i + 1, j, k), 0.5 * 1));
      }

      if (j == 0) {
        hessian.push_back(Trip(row, dim + 3 * idxer(i, j, k), -1));
        hessian.push_back(Trip(row, dim + 3 * idxer(i, j + 1, k), 1));
      } else if (j == dvc.y() - 1) {
        hessian.push_back(Trip(row, dim + 3 * idxer(i, j - 1, k), -1));
        hessian.push_back(Trip(row, dim + 3 * idxer(i, j, k), 1));
      } else {
        hessian.push_back(Trip(row, dim + 3 * idxer(i, j - 1, k), -0.5 * 1));
        hessian.push_back(Trip(row, dim + 3 * idxer(i, j + 1, k), 0.5 * 1));
      }

      if (k == 0) {
        hessian.push_back(Trip(row, dim + 3 * idxer(i, j, k), -1));
        hessian.push_back(Trip(row, dim + 3 * idxer(i, j, k + 1), 1));
      } else if (k == dvc.z() - 1) {
        hessian.push_back(Trip(row, dim + 3 * idxer(i, j, k - 1), -1));
        hessian.push_back(Trip(row, dim + 3 * idxer(i, j, k), 1));
      } else {
        hessian.push_back(Trip(row, dim + 3 * idxer(i, j, k - 1), -0.5 * 1));
        hessian.push_back(Trip(row, dim + 3 * idxer(i, j, k + 1), 0.5 * 1));
      }
    }
  }

  for (auto t : hessian) {
    ACG_CHECK(t.row() < dvc.prod(), "T.row > prod.");
    ACG_CHECK(t.col() < 3 * dvc.prod(), "T.col > 3 * prod.");
  }

  nabla_.resize(dvc.prod(), 3 * dvc.prod());
  nabla_.setFromTriplets(hessian.begin(), hessian.end());
  nabla_.makeCompressed();
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
    Vec3Index base = base_f.cast<Index>();
    // std::cout << base << std::endl;
    Vec3d fx = xp - base_f;
    // F64 stress = -dt_ * 4 * E_ * particle_vol_ * (particle_J_(i) - 1);
    // Mat3x3d affine
    //     = Mat3x3d::Identity() * stress + particle_mass_ *
    //     particle_C_.col(i).reshaped(3, 3);
    for (Index di = 0; di <= 1; ++di) {
      for (Index dj = 0; dj <= 1; ++dj) {
        for (Index dk = 0; dk <= 1; ++dk) {
          Index gid = GetGridIndex(base.x() + di, base.y() + dj, base.z() + dk);
          assert(gid < grid_size_);
          // compute the weight.
          auto weight_d = (Vec3d(1 - di, 1 - dj, 1 - dk) - fx);
          Float64 weight = abs(weight_d.x() * weight_d.y() * weight_d.z());
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
        grid_velocity_.col(gid) += dt_ * grav_.cast<Float64>();
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
        Float64 m_dx = grid_mass_(g_dx1) - grid_mass_(g_dx0);
        Float64 m_dy = grid_mass_(g_dy1) - grid_mass_(g_dy0);
        Float64 m_dz = grid_mass_(g_dz1) - grid_mass_(g_dz0);
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
  Field<Float64, 3> new_vel;
  new_vel.resizeLike(particle_velocity_);
  new_vel.setZero();
  weight_sum_ = 0;
  for (Index i = 0; i < n_particles_; ++i) {
    Vec3d xp = (particle_position_.col(i) / dx_);
    Vec3d base_f = xp.array().floor().matrix();
    Vec3Index base = base_f.cast<Index>();
    Vec3d fx = xp - base_f;
    Float64 weight_sum_local = 0;
    auto blk = new_vel.col(i);
    for (Index di = 0; di <= 1; ++di) {
      for (Index dj = 0; dj <= 1; ++dj) {
        for (Index dk = 0; dk <= 1; ++dk) {
          Index gid = GetGridIndex(base.x() + di, base.y() + dj, base.z() + dk);
          // compute the weight.
          auto weight_d = (Vec3d(1 - di, 1 - dj, 1 - dk) - fx);
          Float64 weight = abs(weight_d.x() * weight_d.y() * weight_d.z());
          assert(weight < 1);
          weight_sum_local += weight;

          blk += weight * grid_velocity_.col(gid);
        }
      }
    }
    weight_sum_ += weight_sum_local;
  }
  weight_sum_ /= n_particles_;

  auto new_position =
      (particle_position_ + (particle_velocity_ + new_vel) * dt_ * .5).eval();
  for (auto blk : new_position.colwise()) {
    blk.x() = std::clamp(blk.x(), 0.01, 0.99);
    blk.y() = std::clamp(blk.y(), 0.01, .99);
    blk.z() = std::clamp(blk.z(), 0.01, .99);
  }
  particle_velocity_ = (new_position - particle_position_) / dt_;
  particle_position_ = new_position;
}

void MpmExplictApp::Run() {
  P2G();
  Projection();
  G2P();
}

void MpmExplictApp::Step() {
  auto new_pos = (lag_.position_ + lag_.velocity_ * dt_).eval();
  for (auto [i, blk] : enumerate(view(new_pos))) {
    blk.x() = std::clamp(blk.x(), 0.01, .99);
    blk.y() = std::clamp(blk.y(), 0.01, .99);
    blk.z() = std::clamp(blk.z(), 0.01, .99);
  }
  lag_.velocity_ = (new_pos - lag_.position_) / dt_;
  lag_.position_ = new_pos;
  apic_->Forward();

  // cg.
  Vec<double> x = euler_.velocity_.reshaped();
  Index iter = 0;
  SpMat<double> a = nabla_.transpose() * nabla_;
  Vec<double> r = -a * x;
  Vec<double> d = r;
  double delta_new = r.squaredNorm();
  double delta_0 = delta_new;
  auto vel = view(euler_.velocity_, apic_->grid_idxer_);
  auto rho = view(euler_.mass_, apic_->grid_idxer_);
  auto sample_diff = [&](Index i, Index j, Index k) {
    Vec3<double> result;
    if (i == 0) {
      result.x() = rho(1, j, k) - rho(0, j, k);
    } else if (i == n_grid_ - 1) {
      result.x() = rho(n_grid_ - 1, j, k) - rho(n_grid_ - 2, j, k);
    } else {
      result.x() = .5 * (rho(i + 1, j, k) - rho(i - 1, j, k));
    }
    if (j == 0) {
      result.y() = rho(i, 1, k) - rho(i, 0, k);
    } else if (j == n_grid_ - 1) {
      result.y() = rho(i, j, k) - rho(i, j - 1, k);
    } else {
      result.y() = .5 * (rho(i, j + 1, k) - rho(i, j - 1, k));
    }
    if (k == 0) {
      result.z() = rho(i, j, k + 1) - rho(i, j, k);
    } else if (k == n_grid_ - 1) {
      result.z() = rho(i, j, k) - rho(i, j, k - 1);
    } else {
      result.z() = .5 * (rho(i, j, k + 1) - rho(i, j, k - 1));
    }
    return result;
  };
  for (auto [i, j, k] : apic_->grid_idxer_.Iterate()) {
    vel(i, j, k) -= sample_diff(i, j, k) / dx_ * E_;
  }
  // while (iter < 100 && delta_new > 0.01 * delta_0) {
  //   ACG_INFO("nabla shape {} x {}, d shape {}", nabla_.rows(), nabla_.cols(),
  //            d.size());
  //   Vec<double> q = a * d;
  //   double alpha = delta_new / (d.dot(q));
  //   x += alpha * d;
  //   if (iter % 5 == 0) {
  //     r = -a * x;
  //   } else {
  //     r = r - alpha * q;
  //   }

  //   double delta_old = delta_new;
  //   delta_new = r.squaredNorm();
  //   double beta = delta_new / delta_old;
  //   d = r + beta * d;
  //   iter += 1;
  //   ACG_INFO("Conjugate Gradient Iter {}: delta = {} divergence = {}", iter,
  //            delta_old, (nabla_ * x).squaredNorm());
  // }
  // euler_.velocity_.reshaped() = x;
  euler_.velocity_.array().row(2) -= 9.8 * dt_;
  apic_->Backward();
  // apic_->matrix_b_.setZero();
}
