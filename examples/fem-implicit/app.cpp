#include "app.hpp"

#include <Eigen/Sparse>
#include <acore/math/sparse.hpp>
#include <acore/math/utilities.hpp>
#include <aphysics/elastic/neohookean.hpp>
#include <aphysics/elastic/stvk.hpp>
#include <autils/log.hpp>
using namespace app;

void FemImplicitApp::Step() {
  using Tri = Eigen::Triplet<Scalar>;
  // M / dt2 = 1 / (dt * dt)
  // .5 * (1/dt)^2 |x - x^|_M^2 + Phi(x)
  // Hes = M + Hessian
  // Grad = M (x - x^)
  Field<Scalar, 3> acceleration;
  auto racc = view(rest_position_);
  auto pacc = view(position_);
  auto aacc = view(acceleration);
  acceleration.setZero(3, position_.cols());
  if (explicit_) {
    // Vec<Scalar> rhs
    //     = ((position_ + dt_ * velocity_).colwise() - 9.8 * Vec3f::UnitZ() * dt_ *
    //     dt_).reshaped();

    for (auto [i, tet] : enumerate(view(tetras_))) {
      // rest:
      Mat3x3<Scalar> rest;
      rest.col(0) = racc(tet.y()) - racc(tet.x());
      rest.col(1) = racc(tet.z()) - racc(tet.x());
      rest.col(2) = racc(tet.w()) - racc(tet.x());
      // cur:
      Mat3x3<Scalar> cur;
      cur.col(0) = pacc(tet.y()) - pacc(tet.x());
      cur.col(1) = pacc(tet.z()) - pacc(tet.x());
      cur.col(2) = pacc(tet.w()) - pacc(tet.x());

      Mat3x3<Scalar> rinv = rest.inverse();
      Mat3x3<Scalar> deform_grad = cur * rinv;

      auto pfpx = physics::elastic::compose_pfpx(rinv);
      if (use_stvk_) {
        auto hes = physics::elastic::OgdenNeoHookean<Scalar, 3>(deform_grad, lambda_, mu_)
                       .ComputeHessian();
        auto grad = pfpx.transpose() * hes.grad;
        for (Index i = 0; i < 4; ++i) {
          aacc(tet(i)) -= grad.block<3, 1>(3 * i, 0);
        }
      } else {
        auto hes = physics::elastic::StVK<Scalar, 3>(deform_grad, lambda_, mu_).ComputeGradient();
        auto grad = pfpx.transpose() * hes.grad_;
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
    Vec<Scalar> rhs;
    rhs.resizeLike(position_.reshaped());
    rhs.setZero();
    auto x_tilde = (position_ + dt_ * velocity_).eval();
    x_tilde.row(2).array() += dt_ * dt_ * -9.8;
    Vec<Scalar> x_tilde_vec = x_tilde.reshaped();
    auto current_solution = position_;
    auto pacc = view(position_);
    for (auto [iter] : NdRange<1>({steps_})) {
      rhs = (current_solution.reshaped() - x_tilde_vec) * mass_ / (dt_ * dt_);
      std::vector<Tri> hessian_data;
      // Put diagnal, i.e. Inertia Part.
      for (auto [i, p] : enumerate(view(position_))) {
        // mass = 1
        hessian_data.push_back(Tri{3 * i, 3 * i, mass_ / dt_ / dt_});
        hessian_data.push_back(Tri{3 * i + 1, 3 * i + 1, mass_ / dt_ / dt_});
        hessian_data.push_back(Tri{3 * i + 2, 3 * i + 2, mass_ / dt_ / dt_});
      }
      // Put crossings, i.e. Potential Energy Part.
      for (auto [i, tet] : enumerate(view(tetras_))) {
        // rest:
        Mat3x3<Scalar> rest;
        rest.col(0) = racc(tet.y()) - racc(tet.x());
        rest.col(1) = racc(tet.z()) - racc(tet.x());
        rest.col(2) = racc(tet.w()) - racc(tet.x());
        // cur:
        Mat3x3<Scalar> cur;
        cur.col(0) = pacc(tet.y()) - pacc(tet.x());
        cur.col(1) = pacc(tet.z()) - pacc(tet.x());
        cur.col(2) = pacc(tet.w()) - pacc(tet.x());

        Mat3x3<Scalar> rinv = rest.inverse();
        Mat3x3<Scalar> deform_grad = cur * rinv;

        auto pfpx = physics::elastic::compose_pfpx(rinv);
        auto hes = physics::elastic::OgdenNeoHookean<Scalar, 3>(deform_grad, lambda_, mu_)
                       .ComputeHessian();
        Mat<Scalar, 12, 12> hessian = pfpx.transpose() * hes.hessian * pfpx;
        auto grad = pfpx.transpose() * hes.grad;

        // Spread hessian to sparse data.
        for (Index i = 0; i < 4; ++i) {
          for (Index di = 0; di < 3; ++di) {
            for (Index j = 0; j < 4; ++j) {
              for (Index dj = 0; dj < 3; ++dj) {
                hessian_data.push_back(
                    Tri{3 * tet(i) + di, 3 * tet(j) + dj, hessian(3 * i + di, 3 * j + dj)});
              }
            }
            rhs(3 * tet(i) + di) += grad(3 * i + di);
          }
        }
      }
      SpMat<Scalar> sp(3 * position_.cols(), 3 * position_.cols());
      sp.setFromTriplets(hessian_data.begin(), hessian_data.end());
      Eigen::SparseLU splu(sp);
      auto result = splu.solve(rhs);
      current_solution -= result.reshaped(3, position_.cols());

      ACG_INFO("Iteration {}, delta={}", iter, result.norm());

      for (auto pos : view(current_solution)) {
        if (pos.z() < 0) {
          pos.z() = 0;
        }
      }
      if (result.norm() < eps_) {
        break;
      }
    }

    velocity_ = 0.997 * (current_solution - position_) / dt_;
    position_ = current_solution;
  }
}

void FemImplicitApp::Init() {
  velocity_.resizeLike(position_);
  velocity_.setZero();
  using Trip = Eigen::Triplet<Scalar>;
  std::vector<Trip> laplacian_data;
  auto coeff = 2 * mu_ + lambda_;
  Mat<Scalar, 9, 12> gi;
  auto i = Mat3x3<Scalar>::Identity();
  auto o = Mat3x3<Scalar>::Zero();
  gi << -i, i, o, o, -i, o, i, o, -i, o, o, i;
  auto li = (gi.transpose() * gi).eval();
  for (auto v : view(tetras_)) {
    for (auto [i, j, di, dj] : NdRange<4>{{4, 4, 3, 3}}) {
      auto t = Trip{3 * v(i) + di, 3 * v(j) + dj, li(3 * i + di, 3 * j + dj) * coeff};
      laplacian_data.push_back(t);
    }
  }

  pc_hes_.resize(position_.size(), position_.size());
  pc_hes_.setFromTriplets(laplacian_data.begin(), laplacian_data.end());
  pc_hes_.diagonal().array() += mass_ / math::square(dt_);
  pc_hes_.makeCompressed();
  solver_.compute(pc_hes_);
}

void FemImplicitApp::StepMF() {
  Field<Scalar, 3> x_tilde = position_ + velocity_ * dt_;

  // Ext forces:
  x_tilde.array().row(2) -= 9.8 * dt_ * dt_;

  auto racc = view(rest_position_);
  Field<Scalar, 3> current_solution = x_tilde;
  auto pacc = view(current_solution);

  auto mdd = mass_ / dt_ / dt_;
  for (auto [iter] : NdRange<1>(steps_)) {
    Field<Scalar, 9> weights = FieldBuilder<Scalar, 9>(position_.cols()).Zeros();
    // Inertia:
    auto wacc = view<DefaultIndexer, ReshapeTransform<3, 3>>(weights);
    auto rhs = (mdd * (x_tilde - current_solution)).eval();

    auto rhsacc = view(rhs);
    for (auto w : wacc) {
      w.setZero();
      w.reshaped(3, 3).diagonal().setConstant(mdd);
    }
    for (auto [i, tet] : enumerate(view(tetras_))) {
      // Foreach tetra. gather the weights.
      Mat3x3<Scalar> rest;
      rest.col(0) = racc(tet.y()) - racc(tet.x());
      rest.col(1) = racc(tet.z()) - racc(tet.x());
      rest.col(2) = racc(tet.w()) - racc(tet.x());
      // cur:
      Mat3x3<Scalar> cur;
      cur.col(0) = pacc(tet.y()) - pacc(tet.x());
      cur.col(1) = pacc(tet.z()) - pacc(tet.x());
      cur.col(2) = pacc(tet.w()) - pacc(tet.x());

      Mat3x3<Scalar> rinv = rest.inverse();
      Mat3x3<Scalar> deform_grad = cur * rinv;

      auto pfpx = physics::elastic::compose_pfpx(rinv);
      auto hes = physics::elastic::OgdenNeoHookean<Scalar, 3>(deform_grad, lambda_, mu_)
                     .ComputeHessian();
      Mat<Scalar, 12, 12> hessian = pfpx.transpose() * hes.hessian * pfpx;
      auto grad = pfpx.transpose() * hes.grad;

      wacc(tet.x()) += Mat3x3<Scalar>::Identity();
      wacc(tet.y()) += Mat3x3<Scalar>::Identity();
      wacc(tet.z()) += Mat3x3<Scalar>::Identity();
      wacc(tet.w()) += Mat3x3<Scalar>::Identity();

      rhsacc(tet.x()) -= grad.block<3, 1>(0, 0);
      rhsacc(tet.y()) -= grad.block<3, 1>(3, 0);
      rhsacc(tet.z()) -= grad.block<3, 1>(6, 0);
      rhsacc(tet.w()) -= grad.block<3, 1>(9, 0);
    }

    for (auto [i, p] : enumerate(pacc)) {
      auto delta = wacc(i).reshaped(3, 3).inverse() * rhsacc(i);
      p += delta * 0.7;
    }
    for (auto pos : view(current_solution)) {
      if (pos.z() < 0) {
        pos.z() = 0;
      }
    }
  }

  velocity_ = (current_solution - position_) / dt_;
  position_ = current_solution;
}

void FemImplicitApp::StepQuasi() {
  Field<Scalar, 3> x_tilde = position_ + velocity_ * dt_;

  // Ext forces:
  x_tilde.array().row(2) -= 9.8 * dt_ * dt_;

  auto racc = view(rest_position_);
  Field<Scalar, 3> current_solution = x_tilde;
  auto pacc = view(current_solution);
  auto eval_object = [&]() -> Scalar {
    Scalar o = 0;
    for (auto [i, tet] : enumerate(view(tetras_))) {
      // Foreach tetra. gather the weights.
      Mat3x3<Scalar> rest;
      rest.col(0) = racc(tet.y()) - racc(tet.x());
      rest.col(1) = racc(tet.z()) - racc(tet.x());
      rest.col(2) = racc(tet.w()) - racc(tet.x());
      // cur:
      Mat3x3<Scalar> cur;
      cur.col(0) = pacc(tet.y()) - pacc(tet.x());
      cur.col(1) = pacc(tet.z()) - pacc(tet.x());
      cur.col(2) = pacc(tet.w()) - pacc(tet.x());

      Mat3x3<Scalar> rinv = rest.inverse();
      Mat3x3<Scalar> deform_grad = cur * rinv;
      o += physics::elastic::SNHNeoHookean<Scalar, 3>(deform_grad, lambda_, mu_).ComputeEnergy();
    }
    return o;
  };

  auto eval_grad = [&]() {
    Vec<Scalar> g = Vec<Scalar>::Zero(position_.size());
    for (auto [i, tet] : enumerate(view(tetras_))) {
      // Foreach tetra. gather the weights.
      Mat3x3<Scalar> rest;
      rest.col(0) = racc(tet.y()) - racc(tet.x());
      rest.col(1) = racc(tet.z()) - racc(tet.x());
      rest.col(2) = racc(tet.w()) - racc(tet.x());
      // cur:
      Mat3x3<Scalar> cur;
      cur.col(0) = pacc(tet.y()) - pacc(tet.x());
      cur.col(1) = pacc(tet.z()) - pacc(tet.x());
      cur.col(2) = pacc(tet.w()) - pacc(tet.x());

      Mat3x3<Scalar> rinv = rest.inverse();
      Mat3x3<Scalar> deform_grad = cur * rinv;

      auto pfpx = physics::elastic::compose_pfpx(rinv);
      auto hes = physics::elastic::OgdenNeoHookean<Scalar, 3>(deform_grad, lambda_, mu_)
                     .ComputeGradient();
      auto grad = pfpx.transpose() * hes.grad_;

      for (auto [p] : NdRange<1>{4}) {
        g.block<3, 1>(tet(p) * 3, 0) += grad.block<3, 1>(p * 3, 0);
      }
    }

    return g;
  };
  for (auto [i] : NdRange<1>{steps_}) {
    auto grad = eval_grad();
    auto d = (-solver_.solve(grad)).eval();

    auto thre = (grad.array() * d.array()).sum();
    Scalar alpha = 2.0;
    bool converged = false;
    auto g = eval_object();

    auto backup_sol = current_solution.eval();

    ACG_INFO("Step {}", i);
    do {
      alpha /= 2;
      ACG_INFO("Linesearch... alpha = {} g = {}, eps = {}", alpha, g, eps_);
      current_solution = backup_sol + alpha * d.reshaped(3, position_.cols());
      auto gnew = eval_object();
      if (gnew - g < 0.3 * alpha * thre) {
        converged = true;
        ACG_INFO("Converged! delta g = {}", gnew - g);
      }
    } while (!converged && alpha > eps_);

    for (auto pos : view(current_solution)) {
      if (pos.z() < 0) {
        pos.z() = 0;
      }
    }
    if (d.norm() < eps_) {
      break;
    }
  }

  velocity_ = (current_solution - position_) / dt_;
  position_ = current_solution;
}
