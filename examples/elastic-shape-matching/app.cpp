#include "app.hpp"
#include <acore/math/view.hpp>

void ShapeMatchingApp::Step() {
  auto sm_goal = shape_matching_.ComputeGoalPositions(position_);

  Field<Float, 3> f_ext = FieldBuilder<Float, 3>(position_.cols()).Zeros();
  // Gravity.
  f_ext.row(2).array() += -9.8;
  velocity_ += (alpha_ * (sm_goal - position_) / dt_ + f_ext * dt_) * 0.999;
  position_ += velocity_ * dt_;


  for (auto [i, pos]: enumerate(view(position_))) {
    if (pos.z() < 0) {
      pos.z() = 0;
      velocity_.col(i) = - 0.5 * velocity_.col(i);
    }
  }

}

void ShapeMatchingApp::Init() {
  auto num_points = position_.cols();
  velocity_.setZero(Eigen::NoChange, num_points);
  shape_matching_.Fit(position_);
}
