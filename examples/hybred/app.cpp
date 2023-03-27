#include "app.hpp"

#include <acore/geometry/common_models.hpp>
#include <acore/math/func.hpp>
#include <acore/parallel/common.hpp>
#include <set>

namespace acg::app {

void HybredApp::Step() {
  ComputeExtForces();
  // Linesearch:
  for (auto [i] : NdRange(steps_)) {
    ACG_INFO("Iteration {}", i);
    ComputeStepDirection();
    linesearch_alpha_ = 0;
    ComputeEnergy();
    ComputeLinesearchTerminalThreshold();
    linesearch_alpha_ = 2.0;
    linesearch_energy_ = std::numeric_limits<Scalar>::max();
    auto initial_energy = linesearch_energy_;

    int ls_count = 0;
    do {
      linesearch_alpha_ /= 2;
      ComputeEnergy();
      ls_count += 1;
      ACG_INFO("Linesearch {}: Energy = {}, IE = {}", ls_count,
               linesearch_energy_, initial_energy);
    } while (linesearch_energy_ - initial_energy < linesearch_terminal_thre_);

    for (auto& o : cloth_) {
      o.update_position_ += linesearch_alpha_ * o.update_direction_;
    }
  }

  CopyIterateResult();
}

void HybredApp::ComputeLinesearchTerminalThreshold() {
  // tr[ Grad  Dire]
  linesearch_terminal_thre_ = 0;
  for (auto& o : cloth_) {
    linesearch_terminal_thre_
        += (o.grad_.array() * o.update_direction_.array()).sum();
  }

  ACG_INFO("Linesearch Terminal Thre = {}", linesearch_terminal_thre_);
}

void HybredApp::Init() {}

void HybredApp::AddCloth(Field<Scalar, 3> vert, Field<Index, 3> face,
                         Scalar stiffness) {
  physics::ClothData<Scalar, 3> cl;
  cl.position_ = vert;
  cl.face_ = face;
  std::set<std::pair<Index, Index>> springs;
  auto add_sp = [&springs](Index i, Index j) {
    if (i > j) {
      std::swap(i, j);
    }
    springs.emplace(i, j);
  };

  for (auto p : access(face)) {
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

void HybredApp::AddCloth(physics::ClothData<Scalar, 3> cloth) {
  Cloth cl;
  cl.data_ = std::move(cloth);
  cl.update_position_.resizeLike(cl.data_.position_);
  cl.inertia_position_.resizeLike(cl.update_position_);
  cl.update_direction_.resizeLike(cl.update_position_);
  // Fill hessian
  cl.hessian_.resize(cl.update_position_.size(), cl.update_position_.size());
  using T = Eigen::Triplet<Scalar>;
  std::vector<T> hdata;
  for (auto [i, c] : enumerate(access(cl.data_.constraints_))) {
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
  for (auto [i, c] : enumerate(access(cl.data_.mass_))) {
    hdata.push_back(T(3 * i, 3 * i, c / dt_ / dt_));
    hdata.push_back(T(3 * i + 1, 3 * i + 1, c / dt_ / dt_));
    hdata.push_back(T(3 * i + 2, 3 * i + 2, c / dt_ / dt_));
  }

  cl.hessian_.setFromTriplets(hdata.begin(), hdata.end());
  cl.hessian_.makeCompressed();
  cl.solver_
      = std::make_unique<Eigen::SimplicialCholesky<acg::SpMat<Scalar>>>();
  cl.solver_->compute(cl.hessian_);
  cloth_.push_back(std::move(cl));
}

void HybredApp::ComputeExtForces() {
  for (auto& o : cloth_) {
    // Dt * force = Delta Delta X.
    o.inertia_position_ += dt_ * o.data_.velocity_;
    o.inertia_position_.colwise() += .5 * dt_ * dt_ * gravity_;
    ACG_INFO("Cloth initial position = {}", o.inertia_position_);
  }

  for (auto& o : softbody_) {
    o.update_direction_ += dt_ * o.data_.velocity_;
    o.inertia_position_.colwise() += .5 * dt_ * dt_ * gravity_;
  }
  // TODO: Compute force for fluids.

  for (auto& e : external_forces_) {
    if (e.object_.type_ == physics::PhysicsObjectType::kCloth) {
      cloth_[e.object_.object_].inertia_position_.col(e.object_.id_)
          += e.value_ * dt_ * dt_ * .5
             / cloth_[e.object_.object_].data_.mass_(e.object_.id_);
    } else if (e.object_.type_
               == physics ::PhysicsObjectType::kHyperElasticSoftbody) {
      softbody_[e.object_.object_].inertia_position_.col(e.object_.id_)
          += e.value_ * dt_ * dt_ * .5
             / softbody_[e.object_.object_].data_.mass_(e.object_.id_);
    }
  }

  for (auto& o : cloth_) {
    o.update_position_ = o.inertia_position_;
  }
  for (auto& o : softbody_) {
    o.update_position_ = o.inertia_position_;
  }
}

void HybredApp::ComputeStepDirection() {
  for (auto& o : cloth_) {
    // Compute for cloth.
    // 1. compute gradient.
    // 2. compute step direction.
    auto& grad = o.grad_;
    grad.resizeLike(o.data_.position_);
    grad.setZero();
    auto pacc = access(o.update_position_);
    auto gacc = access(grad);
    for (auto [i, c] : enumerate(access(o.inertia_position_))) {
      // Target position Gradient = M / dt2 (x - \hat x)
      gacc(i) += o.data_.mass_(i) / dt_ / dt_ * (pacc(i) - c);
    }

    for (auto [i, c] : enumerate(access(o.data_.constraints_))) {
      auto k = o.data_.stiffness_(i);
      auto ol = o.data_.original_length_(i);
      auto d = pacc(c.x()) - pacc(c.y());
      auto l = d.norm();
      auto force = k * d.normalized() * (l - ol);
      gacc(c.x()) -= force;
      gacc(c.y()) += force;
    }

    auto result = o.solver_->solve(grad.reshaped());
    o.update_direction_.reshaped() = result;
    ACG_INFO("Cloth Update Direction = {}", o.update_direction_);
    ACG_INFO("Cloth Update Gradient = {}", o.grad_);
  }
}

void HybredApp::ComputeEnergy() {
  linesearch_energy_ = 0;
  for (auto& o : cloth_) {
    auto pacc = access(o.update_position_);
    for (auto [i, c] : enumerate(access(o.data_.constraints_))) {
      auto k = o.data_.stiffness_(i);
      auto ol = o.data_.original_length_(i);
      auto d = pacc(c.x()) - pacc(c.y());
      auto l = d.norm();
      linesearch_energy_ += .5 * k * math::square(l - ol);
    }

    Scalar inertia_energy = 0;
    for (auto [i, c] : enumerate(access(o.inertia_position_))) {
      inertia_energy += o.data_.mass_(i) * (c - pacc(i)).squaredNorm();
    }
    linesearch_energy_ += inertia_energy / dt_ / dt_;
  }
}

void HybredApp::CopyIterateResult() {
  for (auto& o : cloth_) {
    auto d_pos = o.update_position_ - o.data_.position_;
    o.data_.velocity_ = d_pos / dt_;
    o.data_.position_ = o.update_position_;
  }
}

}  // namespace acg::app
