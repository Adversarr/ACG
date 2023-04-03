#include "app.hpp"

#include <acore/math/func.hpp>
#include <acore/parallel/common.hpp>
#include <aphysics/collision/barrier.hpp>
#include <aphysics/collision/detect.hpp>
#include <aphysics/collision/distance.hpp>
#include <aphysics/dynamics/inertia.hpp>
#include <aphysics/elastic/neohookean.hpp>
#include <set>

namespace acg::app {

void HybridApp::SubStep(bool verbose) {
  ComputeStepDirection();
  ComputeLinesearchTerminalThreshold();

  // determine largest step size.
  collisions_.clear();
  auto n_colls = collisions_.size();
  linesearch_alpha_ = 1;
  global_minimal_toi_ = 1;
  ComputeLinesearchPosition(linesearch_alpha_);
  if (DetectLinesearchCollision(true)) {
    while (n_colls != collisions_.size()) {
      ACG_INFO("Found new collision in step ({} / {}), alpha = {}. "
               "Recomputing direction...",
               n_colls, collisions_.size(), global_minimal_toi_);
      n_colls = collisions_.size();
      ComputeStepDirection();
      ComputeLinesearchPosition(1);
      DetectLinesearchCollision(true);
    }
    ACG_INFO("Determined. toi = {}", global_minimal_toi_);
  }

  // Compute goal at x_k.
  ComputeLinesearchTerminalThreshold();
  linesearch_alpha_ = 0;
  ComputeLinesearchPosition(0);
  ComputeLinesearchEnergy();
  auto initial_energy = linesearch_energy_;

  linesearch_alpha_ = global_minimal_toi_ * .99;
  ACG_INFO("Initial Energy = {}, Thre = {}, Alpha = {}, Step Size = {}",
           linesearch_energy_, linesearch_terminal_thre_, linesearch_alpha_,
           global_solve_direction_.norm());

  Scalar least_energy_alpha = std::numeric_limits<Scalar>::max();
  Scalar least_energy = std::numeric_limits<Scalar>::max();
  for (auto [ls_count] : NdRange(15)) {
    linesearch_alpha_ /= 2;
    ComputeLinesearchPosition(linesearch_alpha_);
    // Test if the linesearch step is valid.
    if (DetectLinesearchCollision()) {
      ACG_ERROR("Found Intersection in linesearch, does everything works?");
      DetectLinesearchCollision(true);
      throw std::runtime_error("IntersectionError");
    }

    ComputeLinesearchEnergy();
    if (verbose) {
      // ACG_INFO("Linesearch {}: Energy = {}, alpha = {}", ls_count,
      //          linesearch_energy_, linesearch_alpha_);
    }
    if (isinf(linesearch_energy_) || isnan(linesearch_energy_)) {
      // not a valid step, smaller alpha required.
      continue;
    }

    if (linesearch_energy_ < least_energy) {
      // ACG_DEBUG("Linesearch: Found a smaller energy alpha = {}",
      //           linesearch_alpha_);
      least_energy_alpha = linesearch_alpha_;
      least_energy = linesearch_energy_;
    }

    ls_count += 1;

    if (least_energy < initial_energy || ls_count > 15) {
      break;
    }

    if (linesearch_energy_ - initial_energy <
        linesearch_gamma_ * linesearch_alpha_ * linesearch_terminal_thre_) {
      // less than threshold, break
      break;
    }
  }
  if (least_energy_alpha != std::numeric_limits<Scalar>::max()) {
    linesearch_alpha_ = least_energy_alpha;
  } else {
    ACG_ERROR("Failed to find a good position for linesearch.");
  }
  // ACG_INFO("Linesearch done, with alpha = {}", linesearch_alpha_);
  CommitLinesearch();
}

void HybridApp::Step(bool verbose) {
  ComputeExtForces();
  // Linesearch:
  for (auto [i] : NdRange(steps_)) {
    ACG_INFO("Iteration {}", i);
    SubStep(verbose);
  }
  CopyIterateResult();
}

bool HybridApp::CommitLinesearch() {
  ComputeLinesearchPosition(linesearch_alpha_);
  auto result = DetectLinesearchCollision();
  if (result) {
    ACG_ERROR("Collision detected.");
  }

  for (auto &o : cloth_) {
    o.substep_position_ = o.linesearch_position_;
  }
  for (auto &o : softbody_) {
    o.substep_position_ = o.linesearch_position_;
  }
  fluid_.substep_position_ = fluid_.linesearch_position_;
  return true;
}

void HybridApp::EnforceConstraints() {
  // Apply Position Constraints.
  for (const auto &c : constraints_) {
    auto enforce_position = [c](auto &o) {
      o.linesearch_position_.col(c.object_.id_) = c.value_;
      o.substep_direction_.col(c.object_.id_).setZero();
    };

    if (c.object_.type_ == physics::PhysicsObjectType::kHyperElasticSoftbody) {
      auto &o = softbody_[c.object_.object_];
      enforce_position(o);
    } else if (c.object_.type_ == physics::PhysicsObjectType::kCloth) {
      auto &o = cloth_[c.object_.object_];
      enforce_position(o);
    }
  }

  // Apply Ground Constraints
  auto enforce_ground = [this](auto &v) {
    for (auto c : view(v)) {
      if (c.z() < ground_.z_value_) {
        c.z() = ground_.z_value_;
      }
    }
  };
  for (auto &o : softbody_) {
    enforce_ground(o.linesearch_position_);
  }

  for (auto &o : cloth_) {
    enforce_ground(o.linesearch_position_);
  }
}

void HybridApp::ComputeLinesearchTerminalThreshold() {
  linesearch_terminal_thre_ =
      (global_solve_grad_.array() * global_solve_direction_.array()).sum();
}

void HybridApp::Init() {
  ACG_DEBUG("Initialize Hybred App. global solve element count = {}, nnz = {}",
            global_solve_element_count_, global_hessian_dyn_data_.size());
  global_solve_grad_.resize(global_solve_element_count_);
  global_solve_direction_.resize(global_solve_element_count_);
  global_hessian_.resize(global_solve_element_count_,
                         global_solve_element_count_);
  global_hessian_.setFromTriplets(global_hessian_dyn_data_.begin(),
                                  global_hessian_dyn_data_.end());
  global_solver_.compute(global_hessian_);
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

void HybridApp::AddCloth(physics::Cloth<Scalar, 3> cloth) {
  Cloth cl;
  cl.data_ = std::move(cloth);
  cl.substep_position_.resizeLike(cl.data_.position_);
  cl.linesearch_position_.resizeLike(cl.data_.position_);
  cl.inertia_position_.resizeLike(cl.substep_position_);
  cl.substep_x_.resizeLike(cl.substep_position_);
  cl.grad_.resizeLike(cl.data_.position_);
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

  // Set index start point and increase global.
  cl.global_solve_index_start_ = global_solve_element_count_;
  global_solve_element_count_ += cl.grad_.size();
  AddHessian(cl.global_solve_index_start_, hdata);

  // Final, move into vector.
  cloth_.push_back(std::move(cl));
}

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
  s.linesearch_position_.resizeLike(s.data_.position_);
  s.substep_direction_.resizeLike(s.substep_position_);
  s.grad_.resizeLike(s.substep_position_);
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

  s.global_solve_index_start_ = global_solve_element_count_;
  global_solve_element_count_ += s.grad_.size();
  AddHessian(s.global_solve_index_start_, hdata);
  softbody_.push_back(std::move(s));
}

void HybridApp::ComputeExtForces() {
  physics::dynamics::InertiaPosition<Scalar, 3> ip_c{dt_};
  ip_c.grav_accel_ = gravity_;
  for (auto &o : cloth_) {
    o.inertia_position_ = ip_c(o.data_.position_, o.data_.velocity_);
  }

  for (auto &o : softbody_) {
    o.inertia_position_ = ip_c(o.data_.position_, o.data_.velocity_);
  }
  fluid_.inertia_position_ =
      ip_c(fluid_.data_.position_, fluid_.data_.velocity_);

  // TODO: Compute External forces.
  for (auto &o : cloth_) {
    o.substep_position_ = o.data_.position_;
  }
  for (auto &o : softbody_) {
    o.substep_position_ = o.data_.position_;
  }
  fluid_.substep_position_ = fluid_.data_.position_;
}

void HybridApp::ComputeStepDirection() {
  // 1. compute gradient.
  // 2. compute step direction.
  global_solve_grad_.resize(global_solve_element_count_);

  // Cloth
  for (auto &o : cloth_) {
    // Compute for cloth.
    auto &grad = o.grad_;
    grad.setZero();
    auto pacc = view(o.substep_position_);
    auto gacc = view(grad);
    for (auto [i, c] : enumerate(view(o.inertia_position_))) {
      // Target position Gradient = M / dt2 (x - \hat x)
      gacc(i) += (o.data_.mass_(i) / dt_ / dt_ * (pacc(i) - c));
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

    global_solve_grad_.block(o.global_solve_index_start_, 0, grad.size(), 1) =
        grad.reshaped();
  }

  // Softbody
  for (auto &o : softbody_) {
    o.grad_.setZero();
    auto pacc = view(o.substep_position_);
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
      auto result = physics::elastic::SNHNeoHookean<Scalar, 3>(
                        deform_grad, o.data_.lambda_, o.data_.mu_)
                        .ComputeGradient();
      ACG_CHECK(!isnan(result.energy_),
                "result.energy is nan, gradient may be inaccurate.");
      auto g = pfpx.transpose() * result.grad_;

      for (auto [p] : NdRange(4)) {
        gacc(tet(p)) += g.block<3, 1>(p * 3, 0);
      }
    }

    global_solve_grad_.middleRows(o.global_solve_index_start_, o.grad_.size()) =
        o.grad_.reshaped();
  }

  // Fluid
  fluid_.grad_.setZero();
  auto v = view(fluid_.grad_);
  auto iv = view(fluid_.inertia_position_);
  for (auto [i, p] : enumerate(view(fluid_.substep_position_))) {
    v(i) = (p - iv(i)) * fluid_.data_.mass_(i) / math::square(dt_);
  }
  global_solve_grad_.block(fluid_.global_solve_index_start_, 0,
                           fluid_.grad_.size(), 1) = fluid_.grad_.reshaped();

  // Collision Gradient
  collision_hessian_data_.clear();
  Mat<Scalar, 9, 12> gi;
  auto i = Mat3x3<Scalar>::Identity();
  auto o = Mat3x3<Scalar>::Zero();
  gi << -i, i, o, o, -i, o, i, o, -i, o, o, i;
  auto li = (gi.transpose() * gi).eval();
  li.setIdentity();
  for (auto c : collisions_) {
    // TODO: Here, assume that:
    //       c.obj1_.type_ == Cloth
    const auto &cl = cloth_[c.obj1_.object_];
    auto p = view(cloth_[c.obj1_.object_].substep_position_);
    auto t = view(cloth_[c.obj1_.object_].data_.face_)(c.obj1_.id_);
    auto f = view(fluid_.substep_position_)(c.obj0_.id_);
    auto t0 = p(t.x());
    auto t1 = p(t.y());
    auto t2 = p(t.z());
    physics::ccd::VertexTriangleNormalDistance<Scalar> vtnd(t0, t1, t2, f);
    Vec<Scalar, 12> g = -vtnd.Grad();
    g *= collision_stiffness_;
    ACG_INFO("g = {}", g.transpose());
    auto clstart = cloth_[c.obj1_.object_].global_solve_index_start_;
    global_solve_grad_.middleRows<3>(fluid_.global_solve_index_start_ +
                                     c.obj0_.id_ * 3) += g.middleRows<3>(9);
    global_solve_grad_.middleRows<3>(clstart + t.x() * 3) += g.middleRows<3>(0);
    global_solve_grad_.middleRows<3>(clstart + t.y() * 3) += g.middleRows<3>(3);
    global_solve_grad_.middleRows<3>(clstart + t.z() * 3) += g.middleRows<3>(6);
    Vec4<Index> v(t.x(), t.y(), t.z(), c.obj0_.id_);
    for (auto [i, j, di, dj] : NdRange<4>{{4, 4, 3, 3}}) {
      auto row = 3 * v(i) + di;
      if (i == 3) {
        row += fluid_.global_solve_index_start_;
      } else {
        row += cl.global_solve_index_start_;
      }
      auto col = 3 * v(j) + dj;
      if (j == 3) {
        col += fluid_.global_solve_index_start_;
      } else {
        col += cl.global_solve_index_start_;
      }
      auto t = T{row, col, li(3 * i + di, 3 * j + dj) * collision_stiffness_};
      collision_hessian_data_.push_back(t);
    }
  }

  ACG_INFO("Gradient is built. Statics: Max = {}; Norm = {}",
           global_solve_grad_.array().abs().maxCoeff(),
           global_solve_grad_.stableNorm());

  // 2. Solve the system
  Eigen::SimplicialCholesky<acg::SpMat<Scalar>> sc;
  if (collision_hessian_data_.empty()) {
    sc.compute(global_hessian_);
  } else {
    SpMat<Scalar> coll;
    coll.resize(global_solve_grad_.size(), global_solve_grad_.size());
    coll.setFromTriplets(collision_hessian_data_.begin(),
                         collision_hessian_data_.end());
    coll += global_hessian_;
    coll.makeCompressed();
    sc.compute(coll);
    // ACG_INFO("Hessian = {}", coll.toDense());
  }
  global_solve_direction_ = (-sc.solve(global_solve_grad_));
  // ACG_INFO("Gradient = {}", global_solve_grad_.transpose());
  // ACG_INFO("Direction = {}", global_solve_direction_.transpose());
  ACG_CHECK(sc.info() == Eigen::Success, "Hessian does not solve.");

  ACG_INFO("Direction is built. Statics: Max = {}; Norm = {}",
           global_solve_direction_.array().abs().maxCoeff(),
           global_solve_direction_.stableNorm());

  // 3. Copy back the update direction to local buffers.
  for (auto &o : cloth_) {
    o.substep_x_.reshaped() = global_solve_direction_.middleRows(
        o.global_solve_index_start_, o.substep_x_.size());
  }

  for (auto &o : softbody_) {
    o.substep_direction_.reshaped() = global_solve_direction_.middleRows(
        o.global_solve_index_start_, o.substep_direction_.size());
  }

  fluid_.substep_direction_.reshaped() =
      global_solve_direction_.bottomRows(fluid_.substep_direction_.size());

  EnforceConstraints();
}

void HybridApp::AddHessian(Index offset, const std::vector<T> &hessian) {
  for (const auto &v : hessian) {
    global_hessian_dyn_data_.push_back(
        T{v.row() + offset, v.col() + offset, v.value()});
  }
}

void HybridApp::ComputeLinesearchEnergy() {
  linesearch_energy_ = 0;
  if (!ComputeConstraintEnergy()) {
    return;
  }
  Scalar constraint_energy = linesearch_energy_;
  if (!ComputeInertiaEnergy()) {
    return;
  }
  Scalar inertia_energy = linesearch_energy_ - constraint_energy;
  // 3. collisions
  Scalar coll_energy = 0;
  for (const auto &c : collisions_) {
    // HACK: assert that obj1 is cloth.
    auto p = view(cloth_[c.obj1_.object_].linesearch_position_);
    auto t = view(cloth_[c.obj1_.object_].data_.face_)(c.obj1_.id_);
    auto f = view(fluid_.linesearch_position_)(c.obj0_.id_);
    physics::ccd::VertexTriangleNormalDistance<Scalar> b(p(t.x()), p(t.y()),
                                                         p(t.z()), f);
    coll_energy += collision_stiffness_ / (1e-4 + b.Value());
  }
  linesearch_energy_ += coll_energy;
  ACG_INFO(
      "Linesearch {}: Energy = {}, Collision Energy = {}, Inertia Energy = "
      "{}, Constraint Energy = {}",
      linesearch_alpha_, linesearch_energy_, coll_energy, inertia_energy,
      constraint_energy);
}

bool HybridApp::ComputeInertiaEnergy() {
  // 1. Cloth Energies
  for (auto &o : cloth_) {
    auto diff2 =
        (o.inertia_position_ - o.linesearch_position_).array().square();
    auto inertia_energy = .5 * (diff2.rowwise() * o.data_.mass_.array()).sum();
    ACG_INFO("Cloth inertia = {}", inertia_energy);
    linesearch_energy_ += inertia_energy / dt_ / dt_;
  }

  // 2. Softbody Energies.
  for (auto &o : softbody_) {
    auto diff2 =
        (o.inertia_position_ - o.linesearch_position_).array().square();
    auto inertia_energy = .5 * (diff2.rowwise() * o.data_.mass_.array()).sum();
    ACG_INFO("SB inertia = {}", inertia_energy);
    linesearch_energy_ += inertia_energy / dt_ / dt_;
  }

  auto d2 =
      (fluid_.linesearch_position_ - fluid_.inertia_position_).array().square();
  Scalar fluid_inertia =
      (d2.rowwise() * fluid_.data_.mass_.array()).sum() / math::square(dt_);
  ACG_INFO("Fluid inertia={}", fluid_inertia);
  linesearch_energy_ += fluid_inertia;
  return true;
}

void HybridApp::CopyIterateResult() {
  for (auto &o : cloth_) {
    auto d_pos = o.substep_position_ - o.data_.position_;
    o.data_.velocity_ = d_pos / dt_;
    o.data_.position_ = o.substep_position_;
  }

  for (auto &o : softbody_) {
    auto d_pos = o.substep_position_ - o.data_.position_;
    o.data_.velocity_ = d_pos / dt_;
    o.data_.position_ = o.substep_position_;
  }

  fluid_.data_.velocity_ =
      (fluid_.substep_position_ - fluid_.data_.position_) / dt_;
  fluid_.data_.position_ = fluid_.substep_position_;
}

bool HybridApp::ComputeConstraintEnergy() {
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
  }

  // 2. Softbody Energies.
  for (auto &o : softbody_) {
    auto pacc = view(o.linesearch_position_);
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
        // The step is too large, and energy -> invalid.
        linesearch_energy_ = std::numeric_limits<Scalar>().infinity();
        return false;
      }
      linesearch_energy_ += result;
    }
  }

  return true;
}

void HybridApp::SetFluid(physics::LagrangeFluid<Scalar, 3> fluid) {
  fluid_.data_ = std::move(fluid);
  fluid_.substep_position_.resizeLike(fluid_.data_.position_);
  fluid_.substep_direction_.resizeLike(fluid_.data_.position_);
  fluid_.grad_.resizeLike(fluid_.data_.position_);
  fluid_.global_solve_index_start_ = global_solve_element_count_;
  global_solve_element_count_ += fluid_.substep_position_.size();
  std::vector<T> hdata;
  for (auto [i, c] : enumerate(view(fluid_.data_.position_))) {
    hdata.push_back(T{3 * i, 3 * i, fluid_.data_.mass_(i) / math::square(dt_)});
    hdata.push_back(
        T{3 * i + 1, 3 * i + 1, fluid_.data_.mass_(i) / math::square(dt_)});
    hdata.push_back(
        T{3 * i + 2, 3 * i + 2, fluid_.data_.mass_(i) / math::square(dt_)});
  }
  AddHessian(fluid_.global_solve_index_start_, hdata);
}

void HybridApp::ComputeLinesearchPosition(Scalar alpha) {
  for (auto &o : cloth_) {
    o.linesearch_position_ = o.substep_position_ + o.substep_x_ * alpha;
  }
  for (auto &o : softbody_) {
    o.linesearch_position_ = o.substep_position_ + o.substep_direction_ * alpha;
  }

  fluid_.linesearch_position_ =
      fluid_.substep_position_ + fluid_.substep_direction_ * alpha;
  EnforceConstraints();
}

bool HybridApp::DetectLinesearchCollision(bool verbose) {
  Scalar scale = 1;
  bool has_collision = false;
  for (auto [i, p] : enumerate(view(fluid_.substep_position_))) {
    Vec3<Scalar> p_dest = fluid_.linesearch_position_.col(i);
    for (size_t cid = 0; cid < cloth_.size(); ++cid) {
      const auto &cl = cloth_[cid];
      auto c = view(cl.substep_position_);
      auto d = view(cl.linesearch_position_);
      for (auto [j, face] : enumerate(view(cl.data_.face_))) {
        physics::ccd::VertexTriangleDynamic<Scalar> vf;
        if (vf(p, c(face.x()), c(face.y()), c(face.z()), p_dest, d(face.x()),
               d(face.y()), d(face.z()))) {
          // Found the collision.
          scale = std::min(vf.toi_, scale);
          physics::CollisionConstraint cc;
          cc.type_ = physics::CollisionType::kVertexFace;
          cc.obj0_.type_ = physics::PhysicsObjectType::kLaglangianFluid;
          cc.obj0_.id_ = i;
          cc.obj1_.type_ = physics::PhysicsObjectType::kCloth;
          cc.obj1_.object_ = cid;
          cc.obj1_.id_ = j;
          if (verbose) {
            ACG_DEBUG("TOI = {}, Fluid {}, Face {}", vf.toi_, i, cid);
            ACG_DEBUG("Particle path = {} -> {}", p.transpose(),
                      p_dest.transpose());
            ACG_DEBUG("Vert 1 path = {} -> {}", c(face.x()).transpose(),
                      d(face.x()).transpose());
            ACG_DEBUG("Vert 2 path = {} -> {}", c(face.y()).transpose(),
                      d(face.y()).transpose());
            ACG_DEBUG("Vert 3 path = {} -> {}", c(face.z()).transpose(),
                      d(face.z()).transpose());
          }
          AddCollision(cc);
          has_collision = true;
        }
      }
    }
    // TODO: Detect the collision between fluid and softbody.
  }

  if (has_collision) {
    ACG_WARN("Found {} collisions, minimal toi = {}", collisions_.size(),
             scale);
  }

  global_minimal_toi_ = scale;
  return has_collision;
}

bool HybridApp::AddCollision(physics::CollisionConstraint cc) {
  if (collisions_.find(cc) != collisions_.end()) {
    return false;
  }
  ACG_INFO("Found the collision! fluid {}, object {}, id {}", cc.obj0_.id_,
           cc.obj1_.object_, cc.obj1_.id_);
  collisions_.insert(cc);
  return true;
}

} // namespace acg::app
