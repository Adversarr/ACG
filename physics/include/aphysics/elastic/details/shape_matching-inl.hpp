#pragma once
#include <acore/math/access.hpp>
#include <acore/math/decompositions/polar.hpp>
#include <acore/math/ops/average.hpp>
#include <aphysics/elastic/shape_matching.hpp>
#include <iostream>

namespace acg::physics::elastic {

template <typename Scalar, int dim>
ShapeMatching<Scalar, dim>::ShapeMatching(const Field<Scalar, dim>& rest_pose,
                                          const Field<Scalar, 1>& mass) {
  Fit(rest_pose, mass);
}

template <typename Scalar, int dim>
ShapeMatching<Scalar, dim>::ShapeMatching(const Field<Scalar, dim>& rest_pose) {
  Fit(rest_pose, mass_);
}

template <typename Scalar, int dim>
Field<Scalar, dim> ShapeMatching<Scalar, dim>::ComputeGoalPositions(
    const Field<Scalar, dim>& current_pose) const noexcept {
  Vec<Scalar, dim> current_pose_center = acg::math::WeightedAverage{}(current_pose, mass_);
  Field<Scalar, dim> p = (current_pose.colwise() - current_pose_center);
  Mat<Scalar, dim, dim> apq = Mat<Scalar, dim, dim>::Zero();
  for (auto [i] : NdRange<1>(rest_pose_local_.cols())) {
    apq += mass_(0, i) * rest_pose_local_.col(i) * p.col(i).transpose();
  }

  math::PolarDecomposition polar{apq};
  for (auto [i, col] : enumerate(access(p))) {
    col = polar.rot_ * rest_pose_local_.col(i) + current_pose_center;
  }
  return p;
}
template <typename Scalar, int dim>
void ShapeMatching<Scalar, dim>::Fit(const Field<Scalar, dim>& rest_pose) {
  rest_pose_mass_center_ = rest_pose.rowwise().mean();
  rest_pose_local_ = rest_pose.colwise() - rest_pose_mass_center_;
  mass_.setOnes(1, rest_pose.cols());
  mass_sum_ = mass_.sum();
}
template <typename Scalar, int dim>
void ShapeMatching<Scalar, dim>::Fit(const Field<Scalar, dim>& rest_pose,
                                     const Field<Scalar, 1>& mass) {
  rest_pose_mass_center_ = acg::math::WeightedAverage{}(rest_pose, mass);
  rest_pose_local_ = rest_pose.colwise() - rest_pose_mass_center_;
  mass_ = mass;
  mass_sum_ = mass.sum();
}
}  // namespace acg::physics::elastic
