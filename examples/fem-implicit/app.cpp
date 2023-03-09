#include "app.hpp"

#include <Eigen/Sparse>
#include <acore/math/sparse.hpp>
#include <acore/math/utilities.hpp>
#include <aphysics/elastic/neohookean.hpp>
#include <aphysics/elastic/stvk.hpp>
#include <autils/log.hpp>
using namespace app;

/**
 * NOTE: What is a kernel?
 * 1. have zero, one, or multiple `Input`
 * 2. have one, or multiple `Output`
 * 3. have zero, one, or multiple `Constants`
 * 4. The computation should not change the input buffer
 */

void FemImplicitApp::Step() {
  using Tri = Eigen::Triplet<Float32>;
  // M / dt2 = 1 / (dt * dt)
  // .5 * (1/dt)^2 |x - x^|_M^2 + Phi(x)
  // Hes = M + Hessian
  // Grad = M (x - x^)
  Field<Float32, 3> acceleration;
  auto racc = access(rest_position_);
  auto pacc = access(position_);
  auto aacc = access(acceleration);
  acceleration.setZero(3, position_.cols());
  if (explicit_) {
    // Vec<Float32> rhs
    //     = ((position_ + dt_ * velocity_).colwise() - 9.8 * Vec3f::UnitZ() * dt_ *
    //     dt_).reshaped();

    for (auto [i, tet] : enumerate(access(tetras_))) {
      // rest:
      Mat3x3f rest;
      rest.col(0) = racc(tet.y()) - racc(tet.x());
      rest.col(1) = racc(tet.z()) - racc(tet.x());
      rest.col(2) = racc(tet.w()) - racc(tet.x());
      // cur:
      Mat3x3f cur;
      cur.col(0) = pacc(tet.y()) - pacc(tet.x());
      cur.col(1) = pacc(tet.z()) - pacc(tet.x());
      cur.col(2) = pacc(tet.w()) - pacc(tet.x());

      Mat3x3f rinv = rest.inverse();
      Mat3x3f deform_grad = cur * rinv;

      auto pfpx = physics::elastic::compose_pfpx(rinv);
      if (use_stvk_) {
        auto hes = physics::elastic::OgdenNeoHookean<Float32, 3>(deform_grad, lambda_, mu_)
                       .ComputeHessian();
        auto grad = pfpx.transpose() * hes.grad;
        for (Index i = 0; i < 4; ++i) {
          aacc(tet(i)) -= grad.block<3, 1>(3 * i, 0);
        }
      } else {
        auto hes = physics::elastic::StVK<Float32, 3>(deform_grad, lambda_, mu_).ComputeGradient();
        auto grad = pfpx.transpose() * hes.grad;
        for (Index i = 0; i < 4; ++i) {
          aacc(tet(i)) -= grad.block<3, 1>(3 * i, 0);
        }
      }
    }
    if (apply_force_) {
      position_ += velocity_ * dt_;
      velocity_ += acceleration * dt_;
      velocity_ *= 0.993;
    }
  } else {
    Vec<Float32> rhs;
    rhs.resizeLike(position_.reshaped());
    rhs.setZero();
    auto x_tilde = (position_ + dt_ * velocity_).eval();
    x_tilde.row(2).array() += dt_ * dt_ * -9.8;
    Vec<Float32> x_tilde_vec = x_tilde.reshaped();
    auto current_solution = position_;
    auto pacc = access(position_);
    for (auto [iter] : NdRange<1>({steps_})) {
      rhs = (current_solution.reshaped() - x_tilde_vec) * mass_ / (dt_ * dt_);
      std::vector<Tri> hessian_data;
      // ACG_INFO("Iteration {}", iter);
      // Put diagnal, i.e. Inertia Part.
      for (auto [i, p] : enumerate(access(position_))) {
        // mass = 1
        hessian_data.push_back(Tri{3 * i, 3 * i, mass_ / dt_ / dt_});
        hessian_data.push_back(Tri{3 * i + 1, 3 * i + 1, mass_ / dt_ / dt_});
        hessian_data.push_back(Tri{3 * i + 2, 3 * i + 2, mass_ / dt_ / dt_});
      }
      // Put crossings, i.e. Potential Energy Part.
      for (auto [i, tet] : enumerate(access(tetras_))) {
        // rest:
        Mat3x3f rest;
        rest.col(0) = racc(tet.y()) - racc(tet.x());
        rest.col(1) = racc(tet.z()) - racc(tet.x());
        rest.col(2) = racc(tet.w()) - racc(tet.x());
        // cur:
        Mat3x3f cur;
        cur.col(0) = pacc(tet.y()) - pacc(tet.x());
        cur.col(1) = pacc(tet.z()) - pacc(tet.x());
        cur.col(2) = pacc(tet.w()) - pacc(tet.x());

        Mat3x3f rinv = rest.inverse();
        Mat3x3f deform_grad = cur * rinv;

        auto pfpx = physics::elastic::compose_pfpx(rinv);
        auto hes = physics::elastic::OgdenNeoHookean<Float32, 3>(deform_grad, lambda_, mu_)
                       .ComputeHessian();
        Mat<Float32, 12, 12> hessian = pfpx.transpose() * hes.hessian * pfpx;
        auto grad = pfpx.transpose() * hes.grad;

        // Spread hessian to sparse data.
        for (Index i = 0; i < 4; ++i) {
          for (Index j = 0; j < 4; ++j) {
            for (Index di = 0; di < 3; ++di) {
              for (Index dj = 0; dj < 3; ++dj) {
                hessian_data.push_back(
                    Tri{3 * tet(i) + di, 3 * tet(j) + dj, hessian(3 * i + di, 3 * j + dj)});
              }
            }
          }

          for (Index di = 0; di < 3; ++di) {
            rhs(3 * tet(i) + di) += grad(3 * i + di);
          }
        }
      }
      SpMat<Float32> sp(3 * position_.cols(), 3 * position_.cols());
      sp.setFromTriplets(hessian_data.begin(), hessian_data.end());
      Eigen::SparseLU splu(sp);
      auto result = splu.solve(rhs);
      current_solution -= result.reshaped(3, position_.cols());
      // ACG_INFO("Hessian = {}, Position = {}", sp.toDense(), current_solution);
      current_solution.col(0) = rest_position_.col(0);
      current_solution.col(1) = rest_position_.col(1);
    }

    velocity_ = (current_solution - position_) / dt_;
    position_ = current_solution;
  }
}

void FemImplicitApp::Init() {
  velocity_.resizeLike(position_);
  velocity_.setZero();
}
