#include "app.hpp"
#include "aphysics/collision/distance.hpp"
#include <Eigen/SparseCholesky>
#include <acore/parallel/common.hpp>
#include <aphysics/collision/detect.hpp>
#include <aphysics/collision/tidetect.hpp>
#include <aphysics/dynamics/inertia.hpp>
#include <aphysics/solver/admm/spring.hpp>

namespace acg::app {

void HybridAdmmApp::AddSoftbody(Field<Scalar, 3> position,
                                Field<Index, 4> tetras, Field<Scalar> mass,
                                Scalar lambda, Scalar mu) {
  physics::HyperElasticSoftbody<Scalar, 3> sb{position, tetras, mass, lambda,
                                              mu};
  sb.InitAuxiliary();
  AddSoftbody(sb);
}

void HybridAdmmApp::AddSoftbody(
    physics::HyperElasticSoftbody<Scalar, 3> softbody) {
  Softbody s;
  s.data_ = std::move(softbody);
  s.substep_position_.resizeLike(s.data_.position_);
  s.inertia_position_.resizeLike(s.substep_position_);
  s.substep_solution_.resizeLike(s.substep_position_);
  using T = Eigen::Triplet<Scalar>;
  std::vector<T> hdata;

  Mat<Scalar, 9, 12> gi;
  s.admm_compute_.iteration_count_ = 3;
  auto i = Mat3x3<Scalar>::Identity();
  auto o = Mat3x3<Scalar>::Zero();
  gi << -i, i, o, o, -i, o, i, o, -i, o, o, i;
  auto coeff = (2 * s.data_.mu_ + s.data_.lambda_) * s.admm_weight_;
  auto li = (gi.transpose() * gi).eval();
  s.hessian_coefficient_ = coeff;
  for (auto v : view(s.data_.tetras_)) {
    for (auto [i, j, di, dj] : NdRange<4>{{4, 4, 3, 3}}) {
      auto t =
          T(3 * v(i) + di, 3 * v(j) + dj, li(3 * i + di, 3 * j + dj) * coeff);
      hdata.push_back(t);
    }
  }

  for (auto [i, v] : enumerate(view(s.data_.mass_))) {
    hdata.push_back(Trip(3 * i, 3 * i, v / math::square(dt_)));
    hdata.push_back(Trip(3 * i + 1, 3 * i + 1, v / math::square(dt_)));
    hdata.push_back(Trip(3 * i + 2, 3 * i + 2, v / math::square(dt_)));
  }

  // Prefer a local solver
  SpMat<Scalar> hessian;
  hessian.resize(s.substep_position_.size(), s.substep_position_.size());
  hessian.setFromTriplets(hdata.begin(), hdata.end());
  s.solver_ = std::make_unique<SolverType>(hessian);
  ACG_CHECK(s.solver_->info() == Eigen::Success, "Failed to prefactor hessian");

  // s.global_solve_index_start_ =
  //     PutHessianDyn(s.substep_position_.size(), hdata);
  softbody_.push_back(std::move(s));
}

void HybridAdmmApp::AddCloth(physics::Cloth<Scalar, 3> cloth) {
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

  // Prefer a local solver
  SpMat<Scalar> hessian;
  hessian.resize(cl.substep_position_.size(), cl.substep_position_.size());
  hessian.setFromTriplets(hess.begin(), hess.end());
  cl.solver_ = std::make_unique<SolverType>(hessian);
  ACG_CHECK(cl.solver_->info() == Eigen::Success,
            "Failed to prefactor hessian");

  // cl.global_solve_index_start_ = PutHessianDyn(cl.data_.position_.size(),
  // hess); Final, move into vector.
  cloth_.push_back(std::move(cl));
}

void HybridAdmmApp::AddCloth(Field<Scalar, 3> vert, Field<Index, 3> face,
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

void HybridAdmmApp::ComputeExtForce() {
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

  // if (fluid_) {
  //   fluid_->inertia_position_ =
  //       ip(fluid_->data_.position_, fluid_->data_.velocity_);
  //   fluid_->substep_position_ = fluid_->data_.position_;
  // }
  EnforceConstraints();
}

void HybridAdmmApp::CcdPrologue() {
  EnforceConstraints();
  // TODO: Also force substep solution to satisfy these constraints.
  size_t collision_count = 0;
  bool force_break = false;
  do {
    // Setup ccd position buffer and run ccd.
    for (auto &cl : cloth_) {
      cl.ccd_dst_position_ = cl.substep_solution_;
    }
    if (fluid_) {
      fluid_->ccd_dst_position_ = fluid_->substep_solution_;
    }
    DetectCollisions();
    for (const auto &c : collisions_) {
      auto tri = cloth_[c.obj0_.object_].data_.face_.col(c.obj0_.id_);
      const auto p = view(cloth_[c.obj0_.object_].substep_position_);
      auto fi = c.obj1_.id_;
      auto pp = view(cloth_[c.obj0_.object_].substep_solution_);

      // TODO: Compute L, and apply penalty.
      Vec<Scalar, 12> x, xp;
      x.middleRows<3>(0) = p(tri.x());
      x.middleRows<3>(3) = p(tri.y());
      x.middleRows<3>(6) = p(tri.z());
      x.middleRows<3>(9) = fluid_->substep_position_.col(fi);
      xp.middleRows<3>(0) = pp(tri.x());
      xp.middleRows<3>(3) = pp(tri.y());
      xp.middleRows<3>(6) = pp(tri.z());
      xp.middleRows<3>(9) = fluid_->substep_solution_.col(fi);
      Vec3<Scalar> v0 = p(tri.x()), v1 = p(tri.y()), v2 = p(tri.z()),
                   v = fluid_->substep_position_.col(fi);

      // auto normal_compute = [](Vec3<Scalar> e0, Vec3<Scalar> e1,
      //                          Vec3<Scalar> ev) -> Vec3<Scalar> {
      //   Vec3<Scalar> n = e0.cross(e1).normalized();
      //   if (n.dot(ev) >= 0) {
      //     return n;
      //   } else {
      //     return -n;
      //   }
      // };

      Vec<Scalar, 12> g;
      // g.middleRows<3>(0) = normal_compute(v1 - v, v2 - v, v0 - v);
      // g.middleRows<3>(3) = normal_compute(v0 - v, v2 - v, v1 - v);
      // g.middleRows<3>(6) = normal_compute(v1 - v, v0 - v, v2 - v);
      // g.middleRows<3>(9) = normal_compute(v2 - v0, v1 - v0, v - v0);
      g = physics::ccd::VertexTriangleNormalDistance<Scalar>(v0, v1, v2, v)
              .Grad();
      g.normalize();

      Scalar len = (x - xp).dot(g);
      if (len < min_distance_) {
        len = min_distance_;
      }
      // ACG_INFO("Length = {}", len);
      ACG_CHECK(!std::isnan(len), "Length is nan");
      pp(tri.x()) += g.middleRows<3>(0) * len;
      pp(tri.y()) += g.middleRows<3>(3) * len;
      pp(tri.z()) += g.middleRows<3>(6) * len;
      fluid_->substep_solution_.col(fi) += g.middleRows<3>(9) * len;
    }

    if (collision_count == collisions_.size() && collision_count != 0) {
      ACG_WARN("Cannot solve.");
      break;
    }
    collision_count = collisions_.size();
  } while (!collisions_.empty() && !force_break);
}

void HybridAdmmApp::CcdEpilogue() {
  do {
    DetectCollisions(true);
    if (collisions_.empty()) {
      return;
    }
    Scalar dx = 0;
    for (auto &o : cloth_) {
      dx += (o.ccd_dst_position_ - o.substep_position_).squaredNorm();
    }
    if (fluid_) {
      dx +=
          (fluid_->ccd_dst_position_ - fluid_->substep_position_).squaredNorm();
    }
    // Avoid Extreme Error.
    minimum_toi_ *= 0.99;
    ACG_WARN("Collision set not empty after constraint process. set toi to {}, "
             "dx = {}",
             minimum_toi_, dx);
    for (auto &o : cloth_) {
      o.ccd_dst_position_ = minimum_toi_ * o.ccd_dst_position_ +
                            (1 - minimum_toi_) * o.substep_position_;
    }
    for (auto &o : softbody_) {
      o.ccd_dst_position_ = minimum_toi_ * o.ccd_dst_position_ +
                            (1 - minimum_toi_) * o.substep_position_;
    }
    if (fluid_) {
      fluid_->ccd_dst_position_ =
          minimum_toi_ * fluid_->ccd_dst_position_ +
          (1 - minimum_toi_) * fluid_->substep_position_;
    }
    if (dx < 1e-10) {
      break;
    }
  } while (!collisions_.empty());
}

void HybridAdmmApp::SubStepCollisionFree() {
  CcdPrologue();

  for (auto &cl : cloth_) {
    cl.ccd_dst_position_ = cl.substep_solution_;
  }
  if (fluid_) {
    fluid_->ccd_dst_position_ = fluid_->substep_solution_;
  }
  DetectCollisions();
  if (!collisions_.empty()) {
    CcdEpilogue();
  }
  if (enable_result_check_) {
    DetectCollisions(true);
    ACG_CHECK(minimum_toi_ > 0.99,
              "Collisions found from substep to next step. TOI = {} count = {}",
              minimum_toi_, collisions_.size());
  }
  // after check:
  for (auto &o : cloth_) {
    o.substep_position_ = o.ccd_dst_position_;
  }
  if (fluid_) {
    fluid_->substep_position_ = fluid_->ccd_dst_position_;
  }
}

void HybridAdmmApp::Step() {
  ComputeExtForce();

  SolveFluid();
  for (auto i = 0; i < max_iteration_; ++i) {
    // ACG_INFO("Iteration {}", i);
    ComputeLocal();
    ComputeGlobal();
    SubStepCollisionFree();
  }
  Scalar error = 0;
  for (auto &cl : cloth_) {
    auto lacc = view(cl.data_.constraints_);
    auto pacc = view(cl.substep_position_);
    for (auto [i, z] : enumerate(view(cl.admm_compute_.slack_))) {
      auto l = lacc(i).x();
      auto r = lacc(i).y();
      error += (pacc(l) - pacc(r) - z).norm() * cl.data_.stiffness_(i) *
               cl.admm_weight_;
    }
  }

  for (auto &o : softbody_) {
    auto tacc = view(o.data_.tetras_);
    auto pacc = view(o.substep_position_);
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
  }
  ACG_INFO("Error Estimate = {}", error);
  CommitStepResult();
}

void HybridAdmmApp::ComputeGlobal() {
  for (auto &cl : cloth_) {
    auto &rhs = cl.substep_x_;
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

    cl.substep_solution_.reshaped() =
        cl.solver_->solve(cl.substep_x_.reshaped());
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

    o.substep_solution_.reshaped() = o.solver_->solve(o.substep_x_.reshaped());
  }

  // Put back.
  // for (auto &cl : cloth_) {
  //   cl.substep_solution_.reshaped() = global_solution_.middleRows(
  //       cl.global_solve_index_start_, cl.substep_solution_.size());
  // }

  // for (auto &o : softbody_) {
  //   o.substep_solution_.reshaped() = global_solution_.middleRows(
  //       o.global_solve_index_start_, o.substep_solution_.size());
  // }
}

void HybridAdmmApp::ComputeLocal() {
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
    o.admm_compute_.ComputeParallel(
        o.substep_position_, o.data_.rest_position_, o.data_.tetra_rinvs_,
        o.data_.tetras_, o.constraint_admm_multiplier_, o.data_.lambda_,
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

void HybridAdmmApp::CommitStepResult() {
  for (auto &cl : cloth_) {
    cl.data_.velocity_ +=
        (cl.substep_position_ - cl.data_.position_) / dt_ * velocity_damping_;
    cl.data_.velocity_ /= 2;
    cl.data_.position_ = cl.substep_position_;
  }

  for (auto &sb : softbody_) {
    sb.data_.velocity_ +=
        (sb.substep_position_ - sb.data_.position_) / dt_ * velocity_damping_;
    sb.data_.velocity_ /= 2;
    sb.data_.position_ = sb.substep_position_;
  }

  if (fluid_) {
    fluid_->data_.velocity_ +=
        (fluid_->substep_position_ - fluid_->data_.position_) / dt_ *
        velocity_damping_;
    fluid_->data_.velocity_ /= 2;
    fluid_->data_.position_ = fluid_->substep_position_;
  }
}

void HybridAdmmApp::Init() {}

void HybridAdmmApp::EnforceConstraints() {
  for (const auto &c : position_constraints_) {
    auto &cl = cloth_[c.object_.object_];
    auto i = c.object_.id_;
    cl.substep_solution_.col(i) = c.value_;
    cl.inertia_position_.col(i) = c.value_;
  }

  for (auto &c : cloth_) {
    for (auto v : view(c.substep_solution_)) {
      v.z() = std::max(v.z(), ground_constraints_.z_value_);
    }
    for (auto v : view(c.inertia_position_)) {
      v.z() = std::max(v.z(), ground_constraints_.z_value_);
    }
  }
  for (auto &c : softbody_) {
    for (auto v : view(c.substep_solution_)) {
      v.z() = std::max(v.z(), ground_constraints_.z_value_);
    }
    for (auto v : view(c.inertia_position_)) {
      v.z() = std::max(v.z(), ground_constraints_.z_value_);
    }
  }

  if (fluid_) {
    for (auto v : view(fluid_->substep_solution_)) {
      v.z() = std::max(v.z(), ground_constraints_.z_value_);
      v.x() = std::clamp(v.x(), wall_x_low_.value_, wall_x_high_.value_);
      v.y() = std::clamp(v.y(), wall_y_low_.value_, wall_y_high_.value_);
    }
  }
}

void HybridAdmmApp::DetectCollisions(bool verbose) {
  // Detect collisions between `position` and `ccd`
  collisions_.clear();
  minimum_toi_ = 1;
  if (!fluid_) {
    return;
  }

  if (enable_subd_) {
    using AABB = spatial::AABB<Scalar, 3>;
    Vec3<Scalar> epsi = Vec3<Scalar>::Constant(min_distance_);
    spatial::SubDivisionAABB<Scalar, Index, 3, 4, 8> sd;
    auto cpacc = view(fluid_->ccd_dst_position_);
    for (auto [i, p] : enumerate(view(fluid_->substep_position_))) {
      AABB aabbl(p - epsi, p + epsi);
      AABB aabbr(cpacc(i) - epsi, cpacc(i) + epsi);
      sd.Insert({aabbl.Merge(aabbr), i});
    }

    for (size_t i = 0; i < cloth_.size(); ++i) {
      auto cur = view(cloth_[i].substep_position_);
      auto dst = view(cloth_[i].ccd_dst_position_);
      for (auto [j, tri] : enumerate(view(cloth_[i].data_.face_))) {
        AABB c00(cur(tri.x()) - epsi, cur(tri.x()) + epsi);
        AABB c01(cur(tri.y()) - epsi, cur(tri.y()) + epsi);
        AABB c02(cur(tri.z()) - epsi, cur(tri.z()) + epsi);
        AABB d00(dst(tri.x()) - epsi, dst(tri.x()) + epsi);
        AABB d01(dst(tri.y()) - epsi, dst(tri.y()) + epsi);
        AABB d02(dst(tri.z()) - epsi, dst(tri.z()) + epsi);
        auto result =
            c00.Merge(c01).Merge(c02).Merge(d00).Merge(d01).Merge(d02);
        auto query = sd.Query(result);
        for (auto sdi : query) {
          auto fi = sd.Get(sdi).second;
          ACG_INFO("SDI = {}, fi = {}", sdi, fi);
          ACG_CHECK(fi < fluid_->data_.position_.cols(), "Invalid fluid id!");
          auto p = fluid_->substep_position_.col(fi);
          auto fdst = fluid_->ccd_dst_position_.col(fi);
          physics::ccd::VertexTriangleDynamic<Scalar> vt;
          vt.min_distance_ = min_distance_;
          vt(p, cur(tri.x()), cur(tri.y()), cur(tri.z()), fdst, dst(tri.x()),
             dst(tri.y()), dst(tri.z()));

          if (vt.valid_) {
            if (verbose) {
              ACG_INFO("Found Collision! Object {}, Id {}, Fluid {}, toi {}", i,
                       j, fi, vt.toi_);
            }
            Collision c;
            c.type_ = physics::kVertexFace;
            c.obj0_.type_ = physics::kCloth;
            c.obj0_.object_ = i;
            c.obj0_.id_ = j;
            c.obj1_.id_ = fi;
            collisions_.push_back(c);
            minimum_toi_ = std::min(vt.toi_, minimum_toi_);
            continue;
          }

          // Assert: vt at cur does not have a small distance.
          physics::ccd::VertexTriangleStatic<Scalar> vts2;
          vts2.min_distance_ = min_distance_;
          vts2(dst(tri.x()), dst(tri.y()), dst(tri.z()), fdst);
          if (vts2.valid_) {
            if (verbose) {
              ACG_INFO("Found Collision! Object {}, Id {}, Fluid {}, no toi", i,
                       j, fi);
            }
            Collision c;
            minimum_toi_ = std::min(0.99, minimum_toi_);
            c.type_ = physics::kVertexFace;
            c.obj0_.type_ = physics::kCloth;
            c.obj0_.object_ = i;
            c.obj0_.id_ = j;
            c.obj1_.id_ = fi;
            collisions_.push_back(c);
          }
        }
      }
    }
  } else {

    for (size_t i = 0; i < cloth_.size(); ++i) {
      auto cur = view(cloth_[i].substep_position_);
      auto dst = view(cloth_[i].ccd_dst_position_);
      auto fluid_dst = view(fluid_->ccd_dst_position_);
      // Parallel run:
      for (auto [fi, p] : enumerate(view(fluid_->substep_position_))) {
        auto fdst = fluid_dst(fi);
        for (auto [j, tri] : enumerate(view(cloth_[i].data_.face_))) {
          physics::ccd::VertexTriangleDynamic<Scalar> vt;
          vt.min_distance_ = min_distance_;
          vt(p, cur(tri.x()), cur(tri.y()), cur(tri.z()), fdst, dst(tri.x()),
             dst(tri.y()), dst(tri.z()));

          if (vt.valid_) {
            if (verbose) {
              ACG_INFO("Found Collision! Object {}, Id {}, Fluid {}, toi {}", i,
                       j, fi, vt.toi_);
            }
            Collision c;
            c.type_ = physics::kVertexFace;
            c.obj0_.type_ = physics::kCloth;
            c.obj0_.object_ = i;
            c.obj0_.id_ = j;
            c.obj1_.id_ = fi;
            collisions_.push_back(c);
            minimum_toi_ = std::min(vt.toi_, minimum_toi_);
            continue;
          }

          // Assert: vt at cur does not have a small distance.
          physics::ccd::VertexTriangleStatic<Scalar> vts2;
          vts2.min_distance_ = min_distance_;
          vts2(dst(tri.x()), dst(tri.y()), dst(tri.z()), fdst);
          if (vts2.valid_) {
            if (verbose) {
              ACG_INFO("Found Collision! Object {}, Id {}, Fluid {}, no toi", i,
                       j, fi);
            }
            Collision c;
            minimum_toi_ = std::min(0.99, minimum_toi_);
            c.type_ = physics::kVertexFace;
            c.obj0_.type_ = physics::kCloth;
            c.obj0_.object_ = i;
            c.obj0_.id_ = j;
            c.obj1_.id_ = fi;
            collisions_.push_back(c);
          }
        }
      }
    }
  }
}

void HybridAdmmApp::SetFluid(physics::LagrangeFluid<Scalar, 3> fluid) {
  fluid_ = std::make_unique<Fluid>();
  fluid_->data_ = std::move(fluid);
  fluid_->substep_position_.resizeLike(fluid_->data_.position_);
  fluid_->substep_solution_.resizeLike(fluid_->data_.position_);
  fluid_->grad_.resizeLike(fluid_->data_.position_);
}

void HybridAdmmApp::SolveFluid() {
  if (!fluid_) {
    return;
  }

  auto sph_kernel_sample = [&](Scalar v) -> Scalar { return exp(-v / 2); };

  auto sph_kernel_diff = [&](Scalar v) -> Scalar {
    return -sph_kernel_sample(v) / 2;
  };

  std::vector<std::vector<Index>> neighbours;
  neighbours.resize(fluid_->data_.position_.cols());
  Field<Scalar> rho = Field<Scalar>::Zero(fluid_->data_.position_.cols());
  auto vacc = view(fluid_->data_.velocity_);
  auto pacc = view(fluid_->data_.position_);
  // SPH Solve.
  // 1. Neighbour finding.
  auto &sd = fluid_->sd_;
  sd.Clear();
  for (auto [i, p] : enumerate(view(fluid_->data_.position_))) {
    spatial::AABB<Scalar, 3> aabb(
        p - Vec3<Scalar>::Constant(fluid_->sph_kern_size_),
        p + Vec3<Scalar>::Constant(fluid_->sph_kern_size_));
    sd.Insert({aabb, i});
  }

  // Maybe parallel run.
  for (auto [i, p] : enumerate(view(fluid_->data_.position_))) {
    spatial::AABB<Scalar, 3> aabb(
        p - Vec3<Scalar>::Constant(fluid_->sph_kern_size_),
        p + Vec3<Scalar>::Constant(fluid_->sph_kern_size_));
    auto result = sd.Query(aabb);

    // setup rho:
    rho(i) = fluid_->data_.mass_(i);
    for (auto r : result) {
      auto j = sd.Get(r).second;
      auto q = pacc(j);

      // normalized (p - q)
      auto dpq_n = (p - q) / fluid_->sph_kern_size_;
      if (j != i && dpq_n.norm() <= 1) {
        neighbours[i].push_back(j);
        auto weight = sph_kernel_sample(dpq_n.norm());
        // ACG_INFO("Neighbour for {} <- {}, weight := {}", i, j, weight);
        rho(i) += fluid_->data_.mass_(j) * weight;
      }
    }
  }

  Field<Scalar> pressure = rho;
  pressure.array() -= fluid_->data_.rho_;

  // Compute force.
  for (auto [i, p] : enumerate(view(fluid_->data_.position_))) {
    Vec3<Scalar> a = Vec3<Scalar>::UnitZ() * -9.8;
    Vec3<Scalar> ap = Vec3<Scalar>::Zero();
    // Foreach neighbour.
    for (auto j : neighbours[i]) {
      auto m = fluid_->data_.mass_(j);
      auto q = pacc(j);
      auto dpq_n = (p - q) / fluid_->sph_kern_size_;
      ap += (pressure(j) - pressure(i)) * m * sph_kernel_diff(dpq_n.norm()) *
            dpq_n.normalized();
    }
    ap /= rho(i) * rho(i);

    // ACG_INFO("A_p length = {}", ap.norm());
    a -= fluid_->pressure_scale_ * ap;
    fluid_->substep_solution_.col(i) =
        fluid_->data_.position_.col(i) + vacc(i) * dt_ + .5 * a * dt_ * dt_;
  }

  fluid_->substep_position_ = fluid_->data_.position_;
}

} // namespace acg::app
