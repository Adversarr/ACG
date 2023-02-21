#include "./mass_spring.hpp"

#include <Eigen/SparseCholesky>
#include <Eigen/SparseQR>
#include <acore/geometry/common.hpp>
#include <acore/geometry/common_models.hpp>
#include <acore/math/access.hpp>
#include <acore/math/constants.hpp>

using namespace acg;

geometry::SimpleMesh<F64> make_plane_xy(Index n) {
  // z = 0, xy in [0, 1]
  // 3x3 => x = 0, 0.5, 1
  //        y = 0, 0.5, 1
  //        z = 0.
  Field<F64, 3> vertices(3, n * n);
  geometry::SimpleMesh<F64>::FacesType faces(3, 2 * (n - 1) * (n - 1));
  for (Index i = 0; i < n; ++i) {
    for (Index j = 0; j < n; ++j) {
      Index idx = i * n + j;
      vertices.col(idx)
          = Vec3d(static_cast<double>(i) / (n - 1), static_cast<double>(j) / (n - 1), 0);
    }
  }

  for (Index i = 0; i < n - 1; ++i) {
    for (Index j = 0; j < n - 1; ++j) {
      Index idx = 2 * (i * (n - 1) + j);
      Index lt = i * n + j;
      Index rt = i * n + j + 1;
      Index lb = (i + 1) * n + j;
      Index rb = (i + 1) * n + j + 1;

      faces.col(idx) = Vec3Index(lt, lb, rt);
      faces.col(idx + 1) = Vec3Index(rt, lb, rb);
    }
  }
  return {vertices, faces};
}

void App::Init() {
  auto mesh = make_plane_xy(n_grids_);
  position_ = mesh.GetVertices().cast<float>();
  n_vertices_ = n_grids_ * n_grids_;
  origin_position_ = position_;
  faces_ = mesh.GetFaces();
  velocity_.resizeLike(position_);
  velocity_.setZero();

  auto indexer = acg::MultiDimensionIndexer<2>(n_grids_, n_grids_);
  for (Index i = 0; i < n_grids_ - 1; ++i) {
    for (Index j = 0; j < n_grids_ - 1; ++j) {
      AddSpring(indexer(i, j), indexer(i, j + 1));
      AddSpring(indexer(i, j), indexer(i + 1, j));
      AddSpring(indexer(i, j), indexer(i + 1, j + 1));
      AddSpring(indexer(i + 1, j), indexer(i, j + 1));
      AddSpring(indexer(i, j + 1), indexer(i + 1, j + 1));
      AddSpring(indexer(i + 1, j), indexer(i + 1, j + 1));
    }
  }
  for (auto tri : access(faces_)) {
    AddSpring(tri.x(), tri.y());
    AddSpring(tri.y(), tri.z());
    AddSpring(tri.z(), tri.x());
  }

  using Trip = Eigen::Triplet<float>;
  std::vector<Trip> hessian_data;
  hessian_.resize(3 * n_vertices_, 3 * n_vertices_);
  for (Index i = 0; i < 3 * n_vertices_; ++i) {
    hessian_data.push_back(Trip(i, i, mass_));
  }
  for (auto [i, j] : springs_) {
    hessian_data.push_back(Trip(3 * i, 3 * i, k_ * dt_ * dt_));
    hessian_data.push_back(Trip(3 * i + 1, 3 * i + 1, k_ * dt_ * dt_));
    hessian_data.push_back(Trip(3 * i + 2, 3 * i + 2, k_ * dt_ * dt_));
    hessian_data.push_back(Trip(3 * j, 3 * j, k_ * dt_ * dt_));
    hessian_data.push_back(Trip(3 * j + 1, 3 * j + 1, k_ * dt_ * dt_));
    hessian_data.push_back(Trip(3 * j + 2, 3 * j + 2, k_ * dt_ * dt_));
    // Cross Part
    hessian_data.push_back(Trip(3 * i, 3 * j, -k_ * dt_ * dt_));
    hessian_data.push_back(Trip(3 * i + 1, 3 * j + 1, -k_ * dt_ * dt_));
    hessian_data.push_back(Trip(3 * i + 2, 3 * j + 2, -k_ * dt_ * dt_));
    hessian_data.push_back(Trip(3 * j, 3 * i, -k_ * dt_ * dt_));
    hessian_data.push_back(Trip(3 * j + 1, 3 * i + 1, -k_ * dt_ * dt_));
    hessian_data.push_back(Trip(3 * j + 2, 3 * i + 2, -k_ * dt_ * dt_));
  }
  hessian_.setFromTriplets(hessian_data.begin(), hessian_data.end());
  hessian_.makeCompressed();

  springs_vec_.assign(springs_.begin(), springs_.end());
  splu_.compute(hessian_);
}

void App::AddSpring(acg::Index i, acg::Index j) {
  auto spring = std::make_pair(std::min(i, j), std::max(i, j));
  springs_.insert(spring);
}

void App::Step() {
  auto acceleration = acg::FieldBuilder<float, 3>(position_.cols()).Zeros();
  auto p_acc = access(position_);
  auto o_acc = access(origin_position_);
  auto a_acc = access(acceleration);
  for (auto sp : springs_) {
    auto [i, j] = sp;
    auto xij = (p_acc(i) - p_acc(j)).eval();
    auto origin_length = (o_acc(i) - o_acc(j)).norm();
    auto length = xij.norm();

    auto force_amp = length - origin_length;

    auto fij = -(force_amp * xij.normalized()).eval();

    a_acc(i) += fij;
    a_acc(j) -= fij;
  }

  acceleration.array() *= k_;
  acceleration.row(2).array() -= 9.8;
  velocity_ += acceleration * dt_ * 0.99;
  position_ += velocity_ * dt_;

  for (Index i = 0; i < n_grids_; ++i) {
    p_acc(i) = o_acc(i);
  }
}

void App::StepImplicit() {
  auto o_acc = access(origin_position_);
  auto d_acc = access(d_);
  d_.resize(Eigen::NoChange, springs_.size());
  auto p_acc = access(position_);

  Vec<float> x_tilde(n_vertices_ * 3);
  x_tilde.setZero();
  auto xn = position_.reshaped();
  auto vn = velocity_.reshaped();
  x_tilde += xn + dt_ * vn;
  x_tilde.reshaped(3, n_vertices_).array().row(2) -= 9.8 * dt_ * dt_;
  auto m_x_tilde = mass_ * x_tilde;
  auto current_solution = position_.eval();
  for (int i = 0; i < steps_; ++i) {
    /****************************************
      Step Local
    ****************************************/
    Index sp_id = 0;
    auto p_acc = access(current_solution);
    //
    tf::Taskflow tf;
    std::atomic_int sp_id_atom{0};
    tf.for_each_index(static_cast<int>(0), static_cast<int>(springs_vec_.size()), 1,
                      [&d_acc, &p_acc, &o_acc, this](int sp_id) {
                        auto [i, j] = springs_vec_[sp_id];
                        auto xij = (p_acc(i) - p_acc(j)).normalized();
                        auto origin_length = (o_acc(i) - o_acc(j)).norm();
                        d_acc(sp_id) = xij * origin_length;
                        sp_id += 1;
                      });
    executor_.run(tf).wait();
    /****************************************
      Step Global
    ****************************************/
    Field<float, 3> sdt = FieldBuilder<float, 3>(n_vertices_).Zeros();
    auto sdt_acc = access(sdt);
    sp_id = 0;
    for (auto [i, j] : springs_) {
      sdt_acc(i) += k_ * dt_ * dt_ * d_acc(sp_id);
      sdt_acc(j) -= k_ * dt_ * dt_ * d_acc(sp_id);
      sp_id += 1;
    }

    auto rhs = (m_x_tilde + sdt.reshaped());
    Vec<float> solution = splu_.solve(rhs).eval();
    ACG_CHECK(splu_.info() == Eigen::Success, "Splu Solver failed.");

    auto err = (current_solution.reshaped() - solution).norm();
    if (err < 0.01 / n_grids_) {
      current_solution = solution.reshaped(3, n_vertices_);
      ACG_INFO("Early Terminate because step size is small enough. #Iter = {} err = {}", i, err);
      break;
    }
    current_solution = solution.reshaped(3, n_vertices_);
    for (Index i = 0; i < n_grids_; ++i) {
      p_acc(i) = o_acc(i);
    }
  }
  /****************************************
    Step Update
  ****************************************/
  velocity_ = (current_solution.reshaped(3, n_vertices_) - position_) / dt_;
  position_ = current_solution.reshaped(3, n_vertices_);
  for (Index i = 0; i < n_grids_; ++i) {
    p_acc(i) = o_acc(i);
  }
}
