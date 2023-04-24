#pragma once

#include <acore/math/view.hpp>

namespace acg::physics::admm {

template <typename Scalar> struct SpringConstraint {
  Field<Scalar, 3> slack_;

  void Compute(const Field<Scalar, 3> &vert_position,
               const Field<Scalar, 3> &multiplier,
               const types::topology::LineList &constraints,
               const Field<Scalar> &original_length, 
               const Field<Scalar>& stiffness,
               const Field<Scalar> &multiplier_weight) noexcept;

  SpringConstraint(const Field<Scalar, 3> &vert_position,
                   const Field<Scalar, 3> &multiplier,
                   const types::topology::LineList &constraints,
                   const Field<Scalar> &original_length,
                   Field<Scalar> const &stiffness,
                   Field<Scalar> const &multiplier_weight) noexcept;

  SpringConstraint() = default;
};

template <typename Scalar>
SpringConstraint<Scalar>::SpringConstraint(
    const Field<Scalar, 3> &vert_position, const Field<Scalar, 3> &multiplier,
    const types::topology::LineList &constraints,
    const Field<Scalar> &original_length, Field<Scalar> const &stiffness,
    Field<Scalar> const &multiplier_weight) noexcept {
  Compute(vert_position, multiplier, constraints, original_length, stiffness,
          multiplier_weight);
}

template <typename Scalar>
void SpringConstraint<Scalar>::Compute(
    const Field<Scalar, 3> &vert_position, const Field<Scalar, 3> &multiplier,
    const types::topology::LineList &constraints,
    const Field<Scalar> &original_length, const Field<Scalar> &stiffness,
    const Field<Scalar> &multiplier_weight) noexcept {
  auto vp = view(vert_position);
  auto u_view = view(multiplier);
  slack_.resize(Eigen::NoChange, constraints.cols());
  auto s_view = view(slack_);

  for (auto [i, c] : enumerate(view(constraints))) {
    auto orig_len = original_length(i);
    auto dx = vp(c.x()) - vp(c.y());
    auto u = u_view(i);
    auto dx_u = (dx + u).eval();
    Scalar l_dx_u = dx_u.norm();
    Scalar lambda = (orig_len * stiffness(i) + multiplier_weight(i) * l_dx_u) /
                  (l_dx_u * (stiffness(i) + multiplier_weight(i)));
    s_view(i) = lambda * dx_u;
  }
}

} // namespace acg::physics::admm