#pragma once

#include "../transfer.hpp"

#include <acore/math/utilities.hpp>
#include <autils/god/utilities.hpp>
/****************************************
 * Implementation Details
 ****************************************/
namespace acg::physics::mpm {

template <typename Scalar, int dim,
          template <typename KScalar, int kdim> typename KernT>
ApicRegular<Scalar, dim, KernT>::ApicRegular(
    LagrangeFluid<Scalar, dim> &lag, EulerFluidRegular<Scalar, dim> &euler)
    : lagrange_(lag), euler_(euler),
      grid_idxer_(std::make_from_tuple<NdRangeIndexer<dim>>(
          make_tuple_from_vector(euler.div_count_))) {
  matrix_b_.resize(Eigen::NoChange, lag.mass_.cols());
  matrix_b_.setZero();
}

template <typename Scalar, int dim,
          template <typename KScalar, int kdim> typename KernT>
void ApicRegular<Scalar, dim, KernT>::Forward() {
  auto mass = access(lagrange_.mass_);
  auto velo = access(lagrange_.velocity_);
  euler_.mass_.setZero();
  euler_.velocity_.setZero();
  auto grid_m = access(euler_.mass_, grid_idxer_);
  auto grid_v = access(euler_.velocity_, grid_idxer_);
  // Transfer mass.
  Foreach([&mass, &grid_m](Index p, Vec<Index, dim> g, Scalar weight,
                           Vec3<Scalar>) {
    auto gt = make_tuple_from_vector(g);
    std::apply(grid_m, gt) += weight * mass(p);
  });

  // Transfer momentum
  auto dp = interp_kernel_.GetMatrixD(euler_.grid_size_);
  Mat<Scalar, dim, dim> dp_inv = dp.inverse();
  auto bp = access<acg::DefaultIndexer, acg::ReshapeTransform<dim, dim>>(
      utils::as_const_arg(matrix_b_));
  Foreach([&](Index p, Vec<Index, dim> g, Scalar weight,
              Vec<Scalar, dim> displacement) -> void {
    auto gt = make_tuple_from_vector(g);
    Vec<Scalar, dim> momentum_inc =
        weight * mass(p) * (velo(p) + bp(p) * dp_inv * displacement);
    std::apply(grid_v, gt) += momentum_inc;
  });
  // compute velocity.
  auto acc = access(euler_.mass_);
  for (auto [i, v] : enumerate(access(euler_.velocity_))) {
    if (acc(i) != math::constant<Scalar>(0)) {
      v /= acc(i);
    }
  }
}

template <typename Scalar, int dim,
          template <typename KScalar, int kdim> typename KernT>
void ApicRegular<Scalar, dim, KernT>::Foreach(
    std::function<void(Index pid, Vec<Index, dim> grid_id, Scalar weight,
                       Vec<Scalar, dim>)>
        f) const {
  constexpr Index kern_size = Kern::KernelSize();
  auto posi = access(lagrange_.position_);
  auto cor = GridCoordConventer<Scalar, 3>(
      euler_.lower_bound_, euler_.upper_bound_, euler_.grid_size_);
  for (auto [p, po] : enumerate(posi)) {
    Vec<Scalar, dim> p_position_local = cor.World2LocalContinuous(po);
    Vec<Index, dim> most_close_grid = p_position_local.template cast<Index>();
    for (auto idx :
         NdRange<dim>(utils::god::duplicate<dim>(kern_size * 2 + 1))) {
      Vec<Index, dim> dijk{std::make_from_tuple<Vec<Index, dim>>(idx)};
      Vec<Index, dim> ijk = (most_close_grid + dijk).array() - kern_size;
      Vec<Scalar, dim> displacement =
          (ijk.template cast<Scalar>() - p_position_local).array() + 0.5;
      Scalar weight = interp_kernel_.Sample(displacement);
      f(p, ijk, weight, displacement * euler_.grid_size_);
    }
  }
}

template <typename Scalar, int dim,
          template <typename KScalar, int kdim> typename KernT>
void ApicRegular<Scalar, dim, KernT>::Backward() {
  auto particle_v = access(lagrange_.velocity_);
  auto grid_v = access(euler_.velocity_, grid_idxer_);
  auto bp =
      access<acg::DefaultIndexer, acg::ReshapeTransform<dim, dim>>(matrix_b_);
  matrix_b_.setZero();
  lagrange_.velocity_.setZero();

  Foreach([&](Index p, Vec3Index g, Scalar weight, Vec3<Scalar> displacement) {
    auto gt = make_tuple_from_vector(g);
    particle_v(p) += weight * std::apply(grid_v, gt);
    bp(p) += weight * std::apply(grid_v, gt) * displacement.transpose();
  });
}
} // namespace acg::physics::mpm