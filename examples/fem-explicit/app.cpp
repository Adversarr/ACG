#include "app.hpp"

#include <Eigen/Eigenvalues>
#include <iostream>

using namespace acg;

void FemExplicitApp::Init() {
  num_vert_ = position_.cols();
  num_tetra_ = tetra_.cols();
  x_reference_ = position_;
  force_.resize(3, num_vert_);
  force_.setZero();
  velocity_.resize(3, num_vert_);
  velocity_.setZero();

  dm_inv_.resize(Eigen::NoChange, num_tetra_);

  auto tetra_accessor = acg::access(tetra_);
  auto position_accessor = acg::access(position_);
  auto dm_inv_accessor = acg::access(dm_inv_);
  for (Index i = 0; i < num_tetra_; ++i) {
    auto indices = tetra_accessor(i);
    auto x10 = position_accessor(indices.y()) - position_accessor(indices.x());
    auto x20 = position_accessor(indices.z()) - position_accessor(indices.x());
    auto x30 = position_accessor(indices.w()) - position_accessor(indices.x());

    acg::Mat3x3d d;
    d << x10, x20, x30;
    dm_inv_accessor(i) = d.inverse().reshaped();
  }
}

void FemExplicitApp::Step() {
  auto tetra_accessor = acg::access(tetra_);
  auto position_accessor = acg::access(position_);
  auto dm_inv_accessor = acg::access<acg::DefaultGetter, acg::ReshapeTransform<3, 3>>(dm_inv_);

  auto acceleration = FieldBuilder<acg::F64, 3>(num_vert_).Zeros();
  auto acc_acc = acg::access(acceleration);
  for (Index i = 0; i < num_tetra_; ++i) {
    auto indices = tetra_accessor(i);
    auto x10 = position_accessor(indices.y()) - position_accessor(indices.x());
    auto x20 = position_accessor(indices.z()) - position_accessor(indices.x());
    auto x30 = position_accessor(indices.w()) - position_accessor(indices.x());

    acg::Mat3x3d d;
    auto dm_inv = dm_inv_accessor(i);
    d << x10, x20, x30;

    Mat3x3d f = (d * dm_inv);

    // Green Strain
    Mat3x3d g = .5 * (f.transpose() * f - acg::Mat3x3d::Identity());

    Mat3x3d p = f * (2 * mu_ * g + lambda_ * g.trace() * acg::Mat3x3d::Identity());

    Mat3x3d forces = (-(1.0 / 6) / dm_inv.determinant() * p * dm_inv.transpose());
    auto f1 = forces.col(0);
    auto f2 = forces.col(1);
    auto f3 = forces.col(2);
    auto f0 = -(f1 + f2 + f3);
    auto w = lambda_ * 0.5 * g.trace() * g.trace() + mu_ * (g.array().square().sum());
    auto eigen = f.eigenvalues().eval();
    std::cout << "I = " << i << std::endl;
    std::cout << "Eig = " << std::endl << eigen << std::endl;
    std::cout << "DmInv = " << std::endl << dm_inv << std::endl;
    std::cout << "F = " << std::endl << f << std::endl;
    std::cout << "g = " << std::endl << g << std::endl;
    std::cout << "p = " << std::endl << p << std::endl;
    std::cout << "f = " << std::endl << forces << std::endl;
    std::cout << "w = " << std::endl << w << std::endl;
    acc_acc(indices.x()) += f0;
    acc_acc(indices.y()) += f1;
    acc_acc(indices.z()) += f2;
    acc_acc(indices.w()) += f3;
  }

  std::cout << "Acceleration = " << std::endl;
  std::cout << acceleration << std::endl;
  acceleration.row(2).array() += -9.8;

  auto new_vel = velocity_ + acceleration * dt_;

  position_ += (velocity_ + new_vel) * dt_;

  velocity_ = new_vel * 0.95;

  auto acc = acg::access(position_);
  position_.block(0, 4, 3, 4) = x_reference_.block(0, 4, 3, 4);

  std::cout << "Position = " << std::endl;
  std::cout << position_ << std::endl;
  std::cout << "Velocity = " << std::endl;
  std::cout << velocity_ << std::endl;
}
