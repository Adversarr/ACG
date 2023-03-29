#include <doctest/doctest.h>
#include <fmt/format.h>

#include <Eigen/Eigenvalues>
#include <acore/math/utilities.hpp>
#include <aphysics/elastic/common.hpp>
#include <aphysics/elastic/neohookean.hpp>
#include <aphysics/elastic/shape_matching.hpp>
#include <aphysics/elastic/stvk.hpp>
#include <iostream>

using namespace acg;
TEST_CASE("Elastic Common") {
  Eigen::Matrix3f dg;
  dg.setZero();
  dg.diagonal().setConstant(0.9);
  acg::physics::elastic::SmithInvariants<float, 3> invariants(Eigen::Matrix3f::Identity());
  auto result = invariants.ComputeHessian();
  std::cout << result.inv_grad_result_.i1_ << std::endl;
  std::cout << result.inv_grad_result_.i2_ << std::endl;
  std::cout << result.inv_grad_result_.i3_ << std::endl;
  std::cout << result.inv_grad_result_.i1_grad_ << std::endl;
  std::cout << result.inv_grad_result_.i2_grad_ << std::endl;
  std::cout << result.inv_grad_result_.i3_grad_ << std::endl;
  std::cout << result.i1_hessian_ << std::endl;
  std::cout << result.i2_hessian_ << std::endl;
  std::cout << result.i3_hessian_ << std::endl;
}

TEST_CASE("Stvk") {
  float mu = 1;
  float lambda = 1;
  Mat3x3d dg;
  dg.setZero();
  dg.diagonal().setConstant(0.9);
  physics::elastic::StVK<double, 3> stvk(dg, lambda, mu);
  auto grad = stvk.ComputeGradient();
  std::cout << grad.energy_ << std::endl;
  std::cout << grad.grad_.reshaped(3, 3) << std::endl;
  {
    Mat3x3d dm_inv = Mat3x3d::Identity();
    Mat3x3d f = dg;
    // Green Strain
    Mat3x3d g = .5 * (f.transpose() * f - acg::Mat3x3d::Identity());

    Mat3x3d p = f * (2 * mu * g + lambda * g.trace() * acg::Mat3x3d::Identity());

    Mat3x3d forces = (-(1.0 / 6) / dm_inv.determinant() * p * dm_inv.transpose());
    auto w = lambda * 0.5 * g.trace() * g.trace() + mu * (g.array().square().sum());
    std::cout << "DmInv = " << std::endl << dm_inv << std::endl;
    std::cout << "g = " << std::endl << g << std::endl;
    std::cout << "p = " << std::endl << p << std::endl;
    std::cout << "f = " << std::endl << forces << std::endl;
    std::cout << "w = " << std::endl << w << std::endl;
  }
}

TEST_CASE("Shape Matching") {
  Field<float, 3> position{{1, 0, 0}, {0, 1, 0}, {0, 0, 0}};
  Field<float, 1> mass;

  mass.resize(Eigen::NoChange, 3);
  mass(0) = 1;
  mass(1) = 1;
  mass(2) = 10000;
  physics::elastic::ShapeMatching<float, 3> sm{position, mass};
  position *= 0.9;
  std::cout << sm.ComputeGoalPositions(position) << std::endl;
}

TEST_CASE("Neohookean") {
  float mu = 1;
  float lambda = 1;
  Mat3x3d dg;
  dg.setIdentity();
  // dg.setZero();
  // dg.diagonal().setConstant(0.9);
  physics::elastic::OgdenNeoHookean<double, 3> stvk(dg, lambda, mu);
  auto h = stvk.ComputeHessian();

  fmt::print("H = {}\n", h.hessian);
  fmt::print("g = {}\n", h.grad);
  fmt::print("Phi = {}\n", h.energy_);
}
