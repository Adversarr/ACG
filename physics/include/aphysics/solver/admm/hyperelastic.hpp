#pragma once
#include <Eigen/Core>
#include <acore/math/func.hpp>
#include <acore/math/view.hpp>
#include <acore/parallel/common.hpp>
#include <aphysics/elastic/common.hpp>

namespace acg::physics::admm {

template <typename Scalar, typename Model> struct HyperElasticConstraint {
  Field<Scalar, 9> slack_;
  Scalar ls_tau_{.5};
  Scalar ls_c_{.7};

  int iteration_count_{10};

  void ComputeParallel(const Field<Scalar, 3> &position,
                       const Field<Scalar, 3> &rest,
                       const Field<Scalar, 9> &rinv,
                       const types::topology::TetraList &tetras,
                       const Field<Scalar, 9> &multiplier, Scalar lambda,
                       Scalar mu, Scalar multiplier_weight) {
    tf::Executor executor;
    tf::Taskflow tf;
    slack_.resize(Eigen::NoChange, tetras.cols());
    tf.for_each_index(
        math::constant<int>(0), static_cast<int>(tetras.cols()), 1, [&](int i) {
          ComputeInternal(i, position, rest, rinv, tetras, multiplier, lambda,
                          mu, multiplier_weight);
        });
    executor.run(tf);
    executor.wait_for_all();
  }
  void ComputeInternal(Index i, const Field<Scalar, 3> &position,
                       const Field<Scalar, 3> &rest_p,
                       const Field<Scalar, 9> &rinv,
                       const types::topology::TetraList &tetras,
                       const Field<Scalar, 9> &multiplier, Scalar lambda,
                       Scalar mu, Scalar multiplier_weight) noexcept {
    auto racc = view(rest_p);
    auto rinv_acc =
        view<acg::DefaultIndexer, acg::ReshapeTransform<3, 3>>(rinv);
    auto pacc = view(position);
    auto m_acc =
        view<acg::DefaultIndexer, acg::ReshapeTransform<3, 3>>(multiplier);
    auto s_acc = view<acg::DefaultIndexer, acg::ReshapeTransform<3, 3>>(slack_);
    auto tet = tetras.col(i);

    Mat3x3<Scalar> rest;
    rest.col(0) = racc(tet.y()) - racc(tet.x());
    rest.col(1) = racc(tet.z()) - racc(tet.x());
    rest.col(2) = racc(tet.w()) - racc(tet.x());
    Mat3x3<Scalar> cur;
    cur.col(0) = pacc(tet.y()) - pacc(tet.x());
    cur.col(1) = pacc(tet.z()) - pacc(tet.x());
    cur.col(2) = pacc(tet.w()) - pacc(tet.x());

    auto weight = multiplier_weight;
    // D x + u
    Mat3x3<Scalar> dx_u = cur + m_acc(i);
    // bias = - R^T (Dx+u)
    Mat3x3<Scalar> bias = -weight * rest.transpose() * dx_u;
    Mat3x3<Scalar> hess = weight * rest.transpose() * rest;

    // Deformation gradient
    Mat3x3<Scalar> f = cur * rinv_acc(i);
    // Non Linear CG.
    Scalar goal;
    Mat3x3<Scalar> grad;
    Mat3x3<Scalar> direction;

    auto eval_energy = [&](Mat3x3<Scalar> f) {
      Scalar energy = Model{f, lambda, mu}.ComputeEnergy();
      if (std::isnan(energy) || std::isinf(energy)) {
        return std::numeric_limits<Scalar>::infinity();
      } else {
        Scalar dxu_f = (rest * f - dx_u).squaredNorm();
        Scalar goal = energy + dxu_f * weight * .5;
        return goal;
      }
    };

    auto eval_grad = [&]() -> void {
      auto model = Model{f, lambda, mu};
      auto g = model.ComputeGradient();
      auto dxu_f = (rest * f - dx_u).squaredNorm();
      auto energy = g.energy_;
      if (std::isnan(energy) || std::isinf(energy)) {
        goal = std::numeric_limits<Scalar>::infinity();
        grad.setZero();
      } else {
        goal = g.energy_ + dxu_f * weight * .5;
        grad = ReshapeTransform<3, 3>{}(g.grad_);
        grad += hess * f + bias;
      }
    };

    auto line_search = [&]() -> Scalar {
      Scalar cm = ls_c_ * (direction.array() * grad.array()).sum();
      Scalar energy = std::numeric_limits<Scalar>::infinity();
      Scalar alpha = 1;
      Scalar length = direction.norm();
      if (length > 1) {
        alpha /= length;
      }
      // Prepare for line search.
      while (std::isinf(energy) || energy - goal > .5 * cm * alpha) {
        alpha *= ls_tau_;
        energy = eval_energy(f + alpha * direction);
        if (alpha < std::numeric_limits<Scalar>::min() * 4) {
          return 0;
        }
      }

      // Perform line search.
      do {
        energy = eval_energy(f + alpha * direction);
        Scalar d_energy = goal - energy;
        if (d_energy > alpha * cm && !std::isinf(energy)) {
          break;
        }
        alpha *= ls_tau_;
      } while (alpha > std::numeric_limits<Scalar>::min() * 4);

      return alpha;
    };

    // Initialize Nonlinear CG
    eval_grad();
    direction = -grad;
    Scalar alpha = line_search();
    f += alpha * direction;
    direction *= alpha;
    for (int i = 0; i < iteration_count_; ++i) {
      Mat3x3<Scalar> last_grad = grad;
      eval_grad();
      if (grad.norm() < 1e-3 ||
          alpha < std::numeric_limits<Scalar>::min() * 10) {
        break;
      }

      Scalar beta = (grad.array() * (grad - last_grad).array()).sum() /
                    last_grad.squaredNorm();
      beta = std::max<Scalar>(0, beta);
      direction = (beta * direction - grad);
      if (direction.reshaped().dot(grad.reshaped()) >= 0) {
        // Make sure direction can become a descent direction.
        direction = -grad;
      }
      alpha = line_search();
      f += alpha * direction;
    }
    s_acc(i) = f;
  }

  void Compute(const Field<Scalar, 3> &position, const Field<Scalar, 3> &rest,
               const Field<Scalar, 9> &rinv,
               const types::topology::TetraList &tetras,
               const Field<Scalar, 9> &multiplier, Scalar lambda, Scalar mu,
               Scalar multiplier_weight) noexcept {

    slack_.resize(Eigen::NoChange, tetras.cols());
    for (auto [i, tet] : enumerate(view(tetras))) {
      ComputeInternal(i, position, rest, rinv, tetras, multiplier, lambda, mu,
                      multiplier_weight);
    }
  }
};

} // namespace acg::physics::admm