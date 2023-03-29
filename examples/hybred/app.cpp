#include "app.hpp"

#include <acore/geometry/common_models.hpp>
#include <acore/math/func.hpp>
#include <acore/parallel/common.hpp>
#include <aphysics/elastic/neohookean.hpp>
#include <aphysics/elastic/stvk.hpp>
#include <set>

namespace acg::app {

void HybredApp::Step(bool verbose) {
  ComputeExtForces();
  // Linesearch:
  for (auto [i] : NdRange(steps_)) {
    ACG_INFO("Iteration {}", i);
    ComputeStepDirection();
    linesearch_alpha_ = 0;
    ComputeEnergy();
    ComputeLinesearchTerminalThreshold();
    linesearch_alpha_ = 2.0;
    auto initial_energy = linesearch_energy_;
    ACG_INFO("Initial Energy = {}, Thre = {}", linesearch_energy_,
             linesearch_terminal_thre_);
    Scalar least_energy_alpha = 0;
    Scalar least_energy = initial_energy;
    for (auto [ls_count] : NdRange(15)) {
      linesearch_alpha_ /= 2;
      ComputeEnergy();
      if (linesearch_energy_ < least_energy) {
        least_energy_alpha = linesearch_alpha_;
        least_energy = linesearch_energy_;
      }
      ls_count += 1;
      if (verbose) {
        ACG_INFO("Linesearch {}: Energy = {}, IE = {}, alpha = {}", ls_count,
                 linesearch_energy_, initial_energy, linesearch_alpha_);
      }
      if (isinf(linesearch_energy_)) {
        continue;
      }
      if (linesearch_energy_ - initial_energy <
          linesearch_gamma_ * linesearch_alpha_ * linesearch_terminal_thre_) {
        // less than threshold, break
        break;
      }
    }
    linesearch_alpha_ = least_energy_alpha;
    ACG_INFO("Alpha = {}", linesearch_alpha_);
    for (auto &o : cloth_) {
      o.update_position_ += linesearch_alpha_ * o.update_direction_;
    }
    for (auto &o : softbody_) {
      o.update_position_ += linesearch_alpha_ * o.update_direction_;
    }
    EnforceConstraints();
  }

  CopyIterateResult();
}

void HybredApp::EnforceConstraints() {
  for (const auto &c : constraints_) {
    if (c.object_.type_ == physics::PhysicsObjectType::kHyperElasticSoftbody) {
      auto &o = softbody_[c.object_.object_];
      view(o.update_position_)(c.object_.id_) = c.value_;
      view(o.update_direction_)(c.object_.id_).setZero();
    } else if (c.object_.type_ == physics::PhysicsObjectType::kCloth) {
      auto &o = cloth_[c.object_.object_];
      view(o.update_position_)(c.object_.id_) = c.value_;
      view(o.update_direction_)(c.object_.id_).setZero();
    }
  }

  for (auto &o : softbody_) {
    for (auto [i, c] : enumerate(view(o.update_position_))) {
      if (c.z() < 0) {
        c.z() = 0;
      }
    }
  }
}

void HybredApp::ComputeLinesearchTerminalThreshold() {
  // tr[ Grad  Dire]
  linesearch_terminal_thre_ = 0;
  for (auto &o : cloth_) {
    linesearch_terminal_thre_ +=
        (o.grad_.array() * o.update_direction_.array()).sum();
  }
  for (auto &o : softbody_) {
    linesearch_terminal_thre_ +=
        (o.grad_.array() * o.update_direction_.array()).sum();
  }
}

void HybredApp::Init() {}

void HybredApp::AddCloth(Field<Scalar, 3> vert, Field<Index, 3> face,
                         Field<Scalar> mass, Scalar stiffness) {
  physics::Cloth<Scalar, 3> cl;
  cl.position_ = vert;
  cl.face_ = face;
  cl.mass_ = mass;
  std::set<std::pair<Index, Index>> springs;
  auto add_sp = [&springs](Index i, Index j) {
    if (i > j) {
      std::swap(i, j);
    }
    springs.emplace(i, j);
  };

  for (auto p : view(face)) {
    add_sp(p.x(), p.y());
    add_sp(p.z(), p.y());
    add_sp(p.x(), p.z());
  }
  cl.constraints_.resize(2, springs.size());
  cl.stiffness_.resize(Eigen::NoChange, springs.size());
  cl.stiffness_.setConstant(stiffness);
  Index id = 0;
  for (auto [i, j] : springs) {
    cl.constraints_(0, id) = i;
    cl.constraints_(1, id) = j;
    id += 1;
  }
  cl.InitAuxiliary();
  AddCloth(cl);
}

void HybredApp::AddCloth(physics::Cloth<Scalar, 3> cloth) {
  Cloth cl;
  cl.data_ = std::move(cloth);
  cl.update_position_.resizeLike(cl.data_.position_);
  cl.inertia_position_.resizeLike(cl.update_position_);
  cl.update_direction_.resizeLike(cl.update_position_);
  cl.grad_.resizeLike(cl.data_.position_);
  // Fill hessian
  cl.hessian_.resize(cl.update_position_.size(), cl.update_position_.size());
  using T = Eigen::Triplet<Scalar>;
  std::vector<T> hdata;
  for (auto [i, c] : enumerate(view(cl.data_.constraints_))) {
    //[[I, -I]
    // [-I, I]
    hdata.push_back(T(3 * c.x(), 3 * c.x(), cl.data_.stiffness_(i)));
    hdata.push_back(T(3 * c.x() + 1, 3 * c.x() + 1, cl.data_.stiffness_(i)));
    hdata.push_back(T(3 * c.x() + 2, 3 * c.x() + 2, cl.data_.stiffness_(i)));
    hdata.push_back(T(3 * c.y(), 3 * c.y(), cl.data_.stiffness_(i)));
    hdata.push_back(T(3 * c.y() + 1, 3 * c.y() + 1, cl.data_.stiffness_(i)));
    hdata.push_back(T(3 * c.y() + 2, 3 * c.y() + 2, cl.data_.stiffness_(i)));
    hdata.push_back(T(3 * c.x(), 3 * c.y(), -cl.data_.stiffness_(i)));
    hdata.push_back(T(3 * c.x() + 1, 3 * c.y() + 1, -cl.data_.stiffness_(i)));
    hdata.push_back(T(3 * c.x() + 2, 3 * c.y() + 2, -cl.data_.stiffness_(i)));
    hdata.push_back(T(3 * c.y(), 3 * c.x(), -cl.data_.stiffness_(i)));
    hdata.push_back(T(3 * c.y() + 1, 3 * c.x() + 1, -cl.data_.stiffness_(i)));
    hdata.push_back(T(3 * c.y() + 2, 3 * c.x() + 2, -cl.data_.stiffness_(i)));
  }
  for (auto [i, c] : enumerate(view(cl.data_.mass_))) {
    hdata.push_back(T(3 * i, 3 * i, c / dt_ / dt_));
    hdata.push_back(T(3 * i + 1, 3 * i + 1, c / dt_ / dt_));
    hdata.push_back(T(3 * i + 2, 3 * i + 2, c / dt_ / dt_));
  }

  cl.hessian_.setFromTriplets(hdata.begin(), hdata.end());
  cl.hessian_.makeCompressed();
  cl.solver_ =
      std::make_unique<Eigen::SimplicialCholesky<acg::SpMat<Scalar>>>();
  cl.solver_->compute(cl.hessian_);
  cloth_.push_back(std::move(cl));
}

void HybredApp::AddSoftbody(Field<Scalar, 3> position, Field<Index, 4> tetras,
                            Field<Scalar> mass, Scalar lambda, Scalar mu) {
  physics::HyperElasticSoftbody<Scalar, 3> sb{position, tetras, mass, lambda,
                                              mu};
  sb.InitAuxiliary();
  AddSoftbody(sb);
}

void HybredApp::AddSoftbody(physics::HyperElasticSoftbody<Scalar, 3> softbody) {
  Softbody s;
  s.data_ = std::move(softbody);
  s.update_position_.resizeLike(s.data_.position_);
  s.inertia_position_.resizeLike(s.update_position_);
  s.update_direction_.resizeLike(s.update_position_);
  s.grad_.resizeLike(s.update_position_);
  s.hessian_.resize(s.data_.position_.size(), s.data_.position_.size());
  using T = Eigen::Triplet<Scalar>;
  std::vector<T> hdata;

  Mat<Scalar, 9, 12> gi;
  auto i = Mat3x3<Scalar>::Identity();
  auto o = Mat3x3<Scalar>::Zero();
  gi << -i, i, o, o, -i, o, i, o, -i, o, o, i;
  auto coeff = 2 * s.data_.mu_ + s.data_.lambda_;
  auto li = (gi.transpose() * gi).eval();
  for (auto v : view(s.data_.tetras_)) {
    for (auto [i, j, di, dj] : NdRange<4>{{4, 4, 3, 3}}) {
      auto t =
          T{3 * v(i) + di, 3 * v(j) + dj, li(3 * i + di, 3 * j + dj) * coeff};
      hdata.push_back(t);
    }
  }
  for (auto [i, v] : enumerate(view(s.data_.mass_))) {
    hdata.push_back(T{3 * i, 3 * i, v / math::square(dt_)});
    hdata.push_back(T{3 * i + 1, 3 * i + 1, v / math::square(dt_)});
    hdata.push_back(T{3 * i + 2, 3 * i + 2, v / math::square(dt_)});
  }
  s.hessian_.setFromTriplets(hdata.begin(), hdata.end());
  s.hessian_.makeCompressed();
  s.solver_ = std::make_unique<Eigen::SimplicialCholesky<acg::SpMat<Scalar>>>();
  s.solver_->compute(s.hessian_);
  softbody_.push_back(std::move(s));
}

void HybredApp::ComputeExtForces() {
  for (auto &o : cloth_) {
    // Dt * force = Delta Delta X.
    o.inertia_position_ = o.data_.position_ + dt_ * o.data_.velocity_;
    o.inertia_position_.colwise() += .5 * dt_ * dt_ * gravity_;
  }

  for (auto &o : softbody_) {
    o.inertia_position_ = o.data_.position_ + dt_ * o.data_.velocity_;
    o.inertia_position_.colwise() += .5 * dt_ * dt_ * gravity_;
  }

  // TODO: Compute force for fluids.
  for (auto &e : external_forces_) {
    if (e.object_.type_ == physics::PhysicsObjectType::kCloth) {
      cloth_[e.object_.object_].inertia_position_.col(e.object_.id_) +=
          e.value_ * dt_ * dt_ * .5 /
          cloth_[e.object_.object_].data_.mass_(e.object_.id_);
    } else if (e.object_.type_ ==
               physics ::PhysicsObjectType::kHyperElasticSoftbody) {
      softbody_[e.object_.object_].inertia_position_.col(e.object_.id_) +=
          e.value_ * dt_ * dt_ * .5 /
          softbody_[e.object_.object_].data_.mass_(e.object_.id_);
    }
  }

  for (auto &o : cloth_) {
    o.update_position_ = o.inertia_position_;
  }
  for (auto &o : softbody_) {
    o.update_position_ = o.inertia_position_;
  }
  EnforceConstraints();
  for (auto &o : cloth_) {
    o.inertia_position_ = o.update_position_;
  }
  for (auto &o : softbody_) {
    o.inertia_position_ = o.update_position_;
  }
}

void HybredApp::ComputeStepDirection() {
  // 1. compute gradient.
  // 2. compute step direction.
  for (auto &o : cloth_) {
    // Compute for cloth.
    auto &grad = o.grad_;
    grad.setZero();
    auto pacc = view(o.update_position_);
    auto gacc = view(grad);
    for (auto [i, c] : enumerate(view(o.inertia_position_))) {
      // Target position Gradient = M / dt2 (x - \hat x)
      gacc(i) += o.data_.mass_(i) / dt_ / dt_ * (pacc(i) - c);
    }

    for (auto [i, c] : enumerate(view(o.data_.constraints_))) {
      auto k = o.data_.stiffness_(i);
      auto ol = o.data_.original_length_(i);
      auto d = pacc(c.x()) - pacc(c.y());
      auto l = d.norm();
      auto force = k * d.normalized() * (l - ol);
      gacc(c.x()) += force;
      gacc(c.y()) -= force;
    }

    auto result = o.solver_->solve(grad.reshaped()).eval();
    o.update_direction_ = -result.reshaped(3, o.update_position_.cols());
  }

  for (auto &o : softbody_) {
    o.grad_.setZero();
    auto pacc = view(o.update_position_);
    auto gacc = view(o.grad_);

    for (auto [i, c] : enumerate(view(o.inertia_position_))) {
      gacc(i) += o.data_.mass_(i) / dt_ / dt_ * (pacc(i) - c);
    }

    for (auto [i, tet] : enumerate(view(o.data_.tetras_))) {
      Mat3x3<Scalar> cur;
      cur.col(0) = pacc(tet.y()) - pacc(tet.x());
      cur.col(1) = pacc(tet.z()) - pacc(tet.x());
      cur.col(2) = pacc(tet.w()) - pacc(tet.x());
      auto rinv = ReshapeTransform<3, 3>{}(o.data_.tetra_rinvs_.col(i));
      Mat3x3<Scalar> deform_grad = cur * rinv;

      auto pfpx = physics::elastic::compose_pfpx(rinv);
      auto result = physics::elastic::OgdenNeoHookean<Scalar, 3>(
                        deform_grad, o.data_.lambda_, o.data_.mu_)
                        .ComputeGradient();
      auto g = pfpx.transpose() * result.grad_;

      for (auto [p] : NdRange(4)) {
        gacc(tet(p)) += g.block<3, 1>(p * 3, 0);
      }
    }

    auto result = o.solver_->solve(o.grad_.reshaped()).eval();
    o.update_direction_ = -result.reshaped(3, o.update_position_.cols());
  }
}

void HybredApp::ComputeEnergy() {
  linesearch_energy_ = 0;
  for (auto &o : cloth_) {
    o.linesearch_position_ =
        o.update_position_ + o.update_direction_ * linesearch_alpha_;
  }
  for (auto &o : softbody_) {
    o.linesearch_position_ =
        o.update_position_ + o.update_direction_ * linesearch_alpha_;
  }

  if (!ComputeInertiaEnergy()) {
    return;
  }
  if (!ComputeConstraintEnergy()) {
    return;
  }
}

bool HybredApp::ComputeInertiaEnergy() {
  // 1. Cloth Energies
  for (auto &o : cloth_) {
    auto pacc = view(o.linesearch_position_);
    Scalar inertia_energy = 0;
    for (auto [i, c] : enumerate(view(o.inertia_position_))) {
      inertia_energy += .5 * o.data_.mass_(i) * (c - pacc(i)).squaredNorm();
    }
    linesearch_energy_ += inertia_energy / dt_ / dt_;
  }

  // 2. Softbody Energies.
  for (auto &o : softbody_) {
    auto pacc = view(o.linesearch_position_);
    Scalar inertia_energy = 0;
    for (auto [i, c] : enumerate(view(o.inertia_position_))) {
      // M/(2 h^2) | x - xhat |^2
      inertia_energy += o.data_.mass_(i) * (pacc(i) - c).squaredNorm();
    }
    linesearch_energy_ += inertia_energy / dt_ / dt_;
  }
  return true;
}

void HybredApp::CopyIterateResult() {
  for (auto &o : cloth_) {
    auto d_pos = o.update_position_ - o.data_.position_;
    o.data_.velocity_ = d_pos / dt_;
    o.data_.position_ = o.update_position_;
  }

  for (auto &o : softbody_) {
    auto d_pos = o.update_position_ - o.data_.position_;
    o.data_.velocity_ = d_pos / dt_;
    o.data_.position_ = o.update_position_;
  }
}

bool HybredApp::ComputeConstraintEnergy() {
  // 1. Cloth Energies
  for (auto &o : cloth_) {
    auto pacc = view(o.linesearch_position_);
    for (auto [i, c] : enumerate(view(o.data_.constraints_))) {
      auto k = o.data_.stiffness_(i);
      auto ol = o.data_.original_length_(i);
      auto d = pacc(c.x()) - pacc(c.y());
      auto l = d.norm();
      linesearch_energy_ += .5 * k * math::square(l - ol);
    }
    Scalar inertia_energy = 0;
    for (auto [i, c] : enumerate(view(o.inertia_position_))) {
      inertia_energy += .5 * o.data_.mass_(i) * (c - pacc(i)).squaredNorm();
    }
    linesearch_energy_ += inertia_energy / dt_ / dt_;
  }

  // 2. Softbody Energies.
  for (auto &o : softbody_) {
    auto pacc = view(o.linesearch_position_);
    Scalar inertia_energy = 0;
    for (auto [i, c] : enumerate(view(o.inertia_position_))) {
      inertia_energy += o.data_.mass_(i) * (pacc(i) - c).squaredNorm();
    }
    linesearch_energy_ += inertia_energy / dt_ / dt_;

    Scalar elastic_energy = 0;

    for (auto [i, tet] : enumerate(view(o.data_.tetras_))) {
      Mat3x3<Scalar> cur;
      cur.col(0) = pacc(tet.y()) - pacc(tet.x());
      cur.col(1) = pacc(tet.z()) - pacc(tet.x());
      cur.col(2) = pacc(tet.w()) - pacc(tet.x());
      auto rinv = ReshapeTransform<3, 3>()(o.data_.tetra_rinvs_.col(i));
      Mat3x3<Scalar> deform_grad = cur * rinv;
      auto result = physics::elastic::OgdenNeoHookean<Scalar, 3>(
                        deform_grad, o.data_.lambda_, o.data_.mu_)
                        .ComputeEnergy();
      if (isnan(result)) {
        linesearch_energy_ = std::numeric_limits<Scalar>().infinity();
        return false;
      }
      elastic_energy += result;
    }

    linesearch_energy_ += elastic_energy;
  }
}

} // namespace acg::app
