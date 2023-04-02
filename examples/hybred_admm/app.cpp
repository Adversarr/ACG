#include "app.hpp"
#include <Eigen/SparseCholesky>
#include <aphysics/dynamics/inertia.hpp>
#include <aphysics/solver/admm/spring.hpp>

#include <acore/parallel/common.hpp>

namespace acg::app {

void HybridApp::AddSoftbody(Field<Scalar, 3> position, Field<Index, 4> tetras,
                            Field<Scalar> mass, Scalar lambda, Scalar mu) {
  physics::HyperElasticSoftbody<Scalar, 3> sb{position, tetras, mass, lambda,
                                              mu};
  sb.InitAuxiliary();
  AddSoftbody(sb);
}

void HybridApp::AddSoftbody(physics::HyperElasticSoftbody<Scalar, 3> softbody) {
  Softbody s;
  s.data_ = std::move(softbody);
  s.substep_position_.resizeLike(s.data_.position_);
  s.inertia_position_.resizeLike(s.substep_position_);
  s.substep_solution_.resizeLike(s.substep_position_);
  using T = Eigen::Triplet<Scalar>;
  std::vector<T> hdata;

  Mat<Scalar, 9, 12> gi;
  auto i = Mat3x3<Scalar>::Identity();
  auto o = Mat3x3<Scalar>::Zero();
  gi << -i, i, o, o, -i, o, i, o, -i, o, o, i;
  auto coeff = (2 * s.data_.mu_ + s.data_.lambda_) * s.admm_weight_;
  auto li = (gi.transpose() * gi).eval();
  s.hessian_coefficient_ = coeff;
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

  s.global_solve_index_start_ =
      PutHessianDyn(s.substep_position_.size(), hdata);
  s.admm_compute_.iteration_count_ = 3;
  softbody_.push_back(std::move(s));
}

void HybridApp::AddCloth(physics::Cloth<Scalar, 3> cloth) {
  Cloth cl;
  cl.data_ = std::move(cloth);
  cl.inertia_position_.resizeLike(cl.substep_position_);
  cl.substep_position_.resizeLike(cl.data_.position_);
  cl.substep_x_.resizeLike(cl.substep_position_);
  cl.substep_solution_.resizeLike(cl.substep_x_);

  std::vector<Trip> hess;
  // W L = admm_weight * stiffness * Laplacian
  for (auto [i, c] : enumerate(view(cl.data_.constraints_))) {
    //[[I, -I]
    // [-I, I]
    Scalar k = cl.admm_weight_ * cl.data_.stiffness_(i);
    hess.push_back(Trip(3 * c.x(), 3 * c.x(), k));
    hess.push_back(Trip(3 * c.x() + 1, 3 * c.x() + 1, k));
    hess.push_back(Trip(3 * c.x() + 2, 3 * c.x() + 2, k));
    hess.push_back(Trip(3 * c.y(), 3 * c.y(), k));
    hess.push_back(Trip(3 * c.y() + 1, 3 * c.y() + 1, k));
    hess.push_back(Trip(3 * c.y() + 2, 3 * c.y() + 2, k));
    hess.push_back(Trip(3 * c.x(), 3 * c.y(), -k));
    hess.push_back(Trip(3 * c.x() + 1, 3 * c.y() + 1, -k));
    hess.push_back(Trip(3 * c.x() + 2, 3 * c.y() + 2, -k));
    hess.push_back(Trip(3 * c.y(), 3 * c.x(), -k));
    hess.push_back(Trip(3 * c.y() + 1, 3 * c.x() + 1, -k));
    hess.push_back(Trip(3 * c.y() + 2, 3 * c.x() + 2, -k));
  }
  // M / dt2
  for (auto [i, c] : enumerate(view(cl.data_.mass_))) {
    hess.push_back(Trip(3 * i, 3 * i, c / dt_ / dt_));
    hess.push_back(Trip(3 * i + 1, 3 * i + 1, c / dt_ / dt_));
    hess.push_back(Trip(3 * i + 2, 3 * i + 2, c / dt_ / dt_));
  }

  cl.global_solve_index_start_ = PutHessianDyn(cl.data_.position_.size(), hess);
  // Final, move into vector.
  cloth_.push_back(std::move(cl));
}

void HybridApp::AddCloth(Field<Scalar, 3> vert, Field<Index, 3> face,
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

void HybridApp::ComputeExtForce() {
  physics::dynamics::InertiaPosition<Scalar, 3> ip{dt_};
  for (auto &cl : cloth_) {
    cl.inertia_position_ = ip(cl.data_.position_, cl.data_.velocity_);
    cl.substep_position_ = cl.data_.position_;
    cl.constraint_admm_multiplier_.setZero(Eigen::NoChange,
                                           cl.data_.stiffness_.cols());
  }

  for (auto &sf : softbody_) {
    sf.inertia_position_ = ip(sf.data_.position_, sf.data_.velocity_);
    sf.substep_position_ = sf.data_.position_;
    sf.constraint_admm_multiplier_.setZero(Eigen::NoChange,
                                           sf.data_.tetras_.cols());
  }

  // TODO: Enforce constraints.
  EnforceConstraints();
}

void HybridApp::Step() {
  ComputeExtForce();

  for (auto [i] : NdRange(30)) {
    ACG_INFO("Iteration {}", i);
    ComputeLocal();
    ComputeGlobal();
    for (auto &cl : cloth_) {
      cl.substep_position_ = cl.substep_solution_;
    }

    for (auto &o : softbody_) {
      o.substep_position_ = o.substep_solution_;
    }
    EnforceConstraints();
    Scalar error = 0;
    for (auto &cl : cloth_) {
      auto lacc = view(cl.data_.constraints_);
      auto pacc = view(cl.substep_position_);
      for (auto [i, z] : enumerate(view(cl.admm_compute_.slack_))) {
        auto l = lacc(i).x();
        auto r = lacc(i).y();
        error += (pacc(l) - pacc(r) - z).norm() * cl.data_.stiffness_(i) * cl.admm_weight_;
      }
    }

    for (auto &o : softbody_) {
      auto tacc = view(o.data_.tetras_);
      auto pacc = view(o.substep_position_);
      auto aacc = view<DefaultIndexer, ReshapeTransform<3, 3>>(
          o.constraint_admm_multiplier_);
      auto racc = view(o.data_.rest_position_);
      auto sacc =
          view<DefaultIndexer, ReshapeTransform<3, 3>>(o.admm_compute_.slack_);
      for (auto [i, tet] : enumerate(tacc)) {
        Mat3x3<Scalar> rest;
        rest.col(0) = racc(tet.y()) - racc(tet.x());
        rest.col(1) = racc(tet.z()) - racc(tet.x());
        rest.col(2) = racc(tet.w()) - racc(tet.x());
        Mat3x3<Scalar> cur;
        cur.col(0) = pacc(tet.y()) - pacc(tet.x());
        cur.col(1) = pacc(tet.z()) - pacc(tet.x());
        cur.col(2) = pacc(tet.w()) - pacc(tet.x());
        error += (cur - rest * sacc(i)).norm() * o.hessian_coefficient_;
      }
      ACG_INFO("Error Estimate = {}", error);
    }
  }
  CommitStepResult();
}

void HybridApp::ComputeGlobal() {
  global_x_.setZero();
  for (auto &cl : cloth_) {
    Field<Scalar, 3> &rhs = cl.substep_solution_;
    rhs.setZero();
    for (auto [i, c] : enumerate(view(rhs))) {
      c += cl.inertia_position_.col(i) * cl.data_.mass_(i) / dt_ / dt_;
    }
    auto &z = cl.admm_compute_.slack_;
    auto &u = cl.constraint_admm_multiplier_;
    for (auto [i, c] : enumerate(view(cl.data_.constraints_))) {
      Vec3<Scalar> z_u = z.col(i) - u.col(i);
      rhs.col(c.x()) += z_u * cl.data_.stiffness_(i) * cl.admm_weight_;
      rhs.col(c.y()) -= z_u * cl.data_.stiffness_(i) * cl.admm_weight_;
    }

    global_x_.middleRows(cl.global_solve_index_start_, rhs.size()) =
        rhs.reshaped();
  }

  for (auto &o : softbody_) {
    o.substep_x_.setZero(Eigen::NoChange, o.data_.position_.cols());
    auto xacc = view(o.substep_x_);
    for (auto [i, c] : enumerate(xacc)) {
      c += o.inertia_position_.col(i) * o.data_.mass_(i) / dt_ / dt_;
    }

    auto tacc = view(o.data_.tetras_);
    auto pacc = view(o.substep_position_);
    auto aacc = view<DefaultIndexer, ReshapeTransform<3, 3>>(
        o.constraint_admm_multiplier_);
    auto racc = view(o.data_.rest_position_);
    auto sacc =
        view<DefaultIndexer, ReshapeTransform<3, 3>>(o.admm_compute_.slack_);
    Mat<Scalar, 9, 12> gi;
    auto i = Mat3x3<Scalar>::Identity();
    auto z = Mat3x3<Scalar>::Zero();
    gi << -i, i, z, z, -i, z, i, z, -i, z, z, i;
    for (auto [i, tet] : enumerate(tacc)) {
      // R F - U
      // Mat3x3<Scalar> rf_u =
      Mat3x3<Scalar> rest;
      rest.col(0) = racc(tet.y()) - racc(tet.x());
      rest.col(1) = racc(tet.z()) - racc(tet.x());
      rest.col(2) = racc(tet.w()) - racc(tet.x());
      Mat3x3<Scalar> cur;
      cur.col(0) = pacc(tet.y()) - pacc(tet.x());
      cur.col(1) = pacc(tet.z()) - pacc(tet.x());
      cur.col(2) = pacc(tet.w()) - pacc(tet.x());

      Vec<Scalar, 12> x_local =
          gi.transpose() * (rest * sacc(i) - aacc(i)).reshaped();
      x_local *= o.hessian_coefficient_;
      xacc(tet.x()) += x_local.middleRows<3>(0);
      xacc(tet.y()) += x_local.middleRows<3>(3);
      xacc(tet.z()) += x_local.middleRows<3>(6);
      xacc(tet.w()) += x_local.middleRows<3>(9);
    }

    global_x_.middleRows(o.global_solve_index_start_, o.substep_x_.size()) =
        o.substep_x_.reshaped();
  }

  // Solve the Linear System
  auto result = global_solver_.solve(global_x_);
  global_solution_ = result;

  // Put back.
  for (auto &cl : cloth_) {
    cl.substep_solution_.reshaped() = global_solution_.middleRows(
        cl.global_solve_index_start_, cl.substep_solution_.size());
  }

  for (auto &o : softbody_) {
    o.substep_solution_.reshaped() = global_solution_.middleRows(
        o.global_solve_index_start_, o.substep_solution_.size());
  }
}

void HybridApp::ComputeLocal() {
  for (auto &cl : cloth_) {
    cl.admm_compute_.Compute(
        cl.substep_position_, cl.constraint_admm_multiplier_,
        cl.data_.constraints_, cl.data_.original_length_, cl.data_.stiffness_,
        cl.admm_weight_ * cl.data_.stiffness_);

    auto lacc = view(cl.data_.constraints_);
    auto pacc = view(cl.substep_position_);
    Field<Scalar, 3> dx_z;
    dx_z.resizeLike(cl.constraint_admm_multiplier_);
    dx_z.setZero();
    for (auto [i, z] : enumerate(view(cl.admm_compute_.slack_))) {
      auto l = lacc(i).x();
      auto r = lacc(i).y();
      dx_z.col(i) += pacc(l) - pacc(r) - z;
    }

    cl.constraint_admm_multiplier_ += dx_z;
  }

  for (auto &o : softbody_) {
    o.admm_compute_.ComputeParallel(o.substep_position_, o.data_.rest_position_,
                            o.data_.tetra_rinvs_, o.data_.tetras_,
                            o.constraint_admm_multiplier_, o.data_.lambda_,
                            o.data_.mu_, o.hessian_coefficient_);

    auto tacc = view(o.data_.tetras_);
    auto pacc = view(o.substep_position_);
    auto aacc = view<DefaultIndexer, ReshapeTransform<3, 3>>(
        o.constraint_admm_multiplier_);
    auto racc = view(o.data_.rest_position_);
    auto sacc =
        view<DefaultIndexer, ReshapeTransform<3, 3>>(o.admm_compute_.slack_);
    for (auto [i, tet] : enumerate(tacc)) {
      Mat3x3<Scalar> rest;
      rest.col(0) = racc(tet.y()) - racc(tet.x());
      rest.col(1) = racc(tet.z()) - racc(tet.x());
      rest.col(2) = racc(tet.w()) - racc(tet.x());
      Mat3x3<Scalar> cur;
      cur.col(0) = pacc(tet.y()) - pacc(tet.x());
      cur.col(1) = pacc(tet.z()) - pacc(tet.x());
      cur.col(2) = pacc(tet.w()) - pacc(tet.x());
      aacc(i) += cur - rest * sacc(i);
    }
  }
}

void HybridApp::CommitStepResult() {
  for (auto &cl : cloth_) {
    cl.data_.velocity_ = (cl.substep_position_ - cl.data_.position_) / dt_;
    cl.data_.position_ = cl.substep_position_;
  }

  for (auto &sb : softbody_) {
    sb.data_.velocity_ = (sb.substep_position_ - sb.data_.position_) / dt_;
    sb.data_.position_ = sb.substep_position_;
  }
}

void HybridApp::Init() {
  hessian_dyn_.resize(global_variable_count_, global_variable_count_);
  hessian_dyn_.setFromTriplets(global_hessian_dyn_data_.begin(),
                               global_hessian_dyn_data_.end());
  hessian_dyn_.makeCompressed();
  global_solver_.compute(hessian_dyn_);
  global_x_.resize(global_variable_count_);
  ACG_CHECK(global_solver_.info() == Eigen::Success,
            "Failed to pre-factor hessian.");
}

Index HybridApp::PutHessianDyn(Index count, const std::vector<Trip> &hessian) {
  for (const auto &h : hessian) {
    global_hessian_dyn_data_.push_back({h.row() + global_variable_count_,
                                        h.col() + global_variable_count_,
                                        h.value()});
  }

  auto last_count = global_variable_count_;
  global_variable_count_ += count;
  return last_count;
}

void HybridApp::EnforceConstraints() {
  for (const auto &c : position_constraints_) {
    // assert c is about cloth.
    auto &cl = cloth_[c.object_.object_];
    auto i = c.object_.id_;
    cl.substep_position_.col(i) = c.value_;
    cl.inertia_position_.col(i) = c.value_;
    // ACG_INFO("Enforce position constraint obj {} id {}, {}",
    // c.object_.object_, i,
    //   c.value_.transpose());
  }

  for (auto &c : cloth_) {
    for (auto v : view(c.substep_position_)) {
      v.z() = std::max(v.z(), ground_constraints_.z_value_);
    }
    for (auto v : view(c.inertia_position_)) {
      v.z() = std::max(v.z(), ground_constraints_.z_value_);
    }
  }
  for (auto &c : softbody_) {
    for (auto v : view(c.substep_position_)) {
      v.z() = std::max(v.z(), ground_constraints_.z_value_);
    }
    for (auto v : view(c.inertia_position_)) {
      v.z() = std::max(v.z(), ground_constraints_.z_value_);
    }
  }
}

} // namespace acg::app