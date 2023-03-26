#include "app.hpp"

#include <acore/geometry/common_models.hpp>
#include <acore/parallel/common.hpp>
#include <set>

namespace acg::app {

void HybredApp::Step() {
  
}

void HybredApp::Init() {
  auto plane = acg::geometry::make_plane_xy(10);
  std::set<std::pair<Index, Index>> springs;
  auto add_spring = [&](Index i, Index j) {
    auto spring = std::make_pair(std::min(i, j), std::max(i, j));
    springs.insert(spring);
  };
  cloth_.face_ = plane.GetFaces();
  cloth_.mass_.resize(cloth_.position_.cols());
  cloth_.mass_.setOnes();
  for (auto tri : access(cloth_.face_)) {
    add_spring(tri.x(), tri.y());
    add_spring(tri.y(), tri.z());
    add_spring(tri.z(), tri.x());
  }
  cloth_.constraints_.resize(2, springs.size());
  Index i = 0;
  for (auto sp: springs) {
    cloth_.constraints_(0, i) = sp.first;
    cloth_.constraints_(1, i) = sp.second;
    i += 1;
  }
  cloth_.position_ = plane.GetVertices().cast<Scalar>();
  cloth_.stiffness_.resize(springs.size());
  cloth_.stiffness_.setConstant(cloth_k_);
  cloth_.InitAuxiliary();
}
}  // namespace acg::app
