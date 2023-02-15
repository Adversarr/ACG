#include <acore/math/access.hpp>
#include <acore/math/constants.hpp>
#include <acore/geometry/common.hpp>
#include <acore/geometry/common_models.hpp>

#include "./mass_spring.hpp"
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
  origin_position_ = position_;
  faces_ = mesh.GetFaces();
  velocity_.resizeLike(position_);
  velocity_.setZero();

  for (auto tri: access(faces_)) {
    AddSpring(tri.x(), tri.y());
    AddSpring(tri.y(), tri.z());
    AddSpring(tri.z(), tri.x());
  }
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
  for (auto sp: springs_) {
    auto [i, j] = sp;
    auto xij = (p_acc(i) - p_acc(j)).eval();
    auto origin_length = (o_acc(i) - o_acc(j)).norm();
    auto length = xij.norm();

    auto force_amp = length - origin_length;

    auto fij = - (force_amp * xij.normalized()).eval();

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
