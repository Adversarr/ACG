#pragma once

#include "./common.hpp"
namespace acg::physics::elastic {

template <typename Scalar, int dim> class ShapeMatching {
public:
  ShapeMatching() = default;
  ShapeMatching(const Field<Scalar, dim>& rest_pose, const Field<Scalar, 1>& mass);
  explicit ShapeMatching(const Field<Scalar, dim>& rest_pose);
  Field<Scalar, dim> ComputeGoalPositions(const Field<Scalar, dim>& current_pose) const noexcept;

  void Fit(const Field<Scalar, dim>& rest_pose, const Field<Scalar, 1>& mass);

  void Fit(const Field<Scalar, dim>& rest_pose);

  // TODO: Move to private.
  Field<Scalar, dim> rest_pose_local_;

  Vec<Scalar, dim> rest_pose_mass_center_;

  Field<Scalar, 1> mass_;

  Scalar mass_sum_;
};
}  // namespace acg::physics::elastic
#include "./details/shape_matching-inl.hpp"
