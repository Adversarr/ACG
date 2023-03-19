#pragma once
#include <Eigen/LU>
#include <acore/math/access.hpp>
#include <acore/math/kernels/bsplines.hpp>
#include <acore/math/utilities.hpp>
#include <aphysics/objects/fluid.hpp>
#include <autils/log.hpp>

namespace acg::physics::mpm {

/**
 * @brief Spline Kernel for APIC Transfer.
 *
 * @tparam Scalar
 * @tparam dim
 * @tparam Derived
 */
template <typename Scalar, int dim, typename Derived> struct MPMKernelBase {
  constexpr Scalar Sample(const Vec<Scalar, dim> &point) const noexcept {
    return static_cast<const Derived *>(this)->SampleImpl(point);
  }

  constexpr Scalar
  SampleImpl(const Vec<Scalar, dim> & /*point*/) const noexcept {
    static_assert(!std::is_same_v<Derived, Derived>,
                  "Call to unimplemented SampleImpl");
  }

  Mat<Scalar, dim, dim> GetMatrixD(Scalar dx) const noexcept {
    return static_cast<const Derived *>(this)->GetMatrixDImpl(dx);
  }
  // Given Grid [i, j, k], and particle location (x, y, z)
  static constexpr Index KernelSize() { return Derived::KernelSizeImpl(); }
};

template <typename Scalar, int dim>
struct CubicBSplineKernel
    : MPMKernelBase<Scalar, dim, CubicBSplineKernel<Scalar, dim>> {
  constexpr Scalar SampleImpl(const Vec<Scalar, dim> &point) const noexcept {
    return math::CubicBSpline<Scalar>{}(point);
  }

  Mat<Scalar, dim, dim> GetMatrixDImpl(Scalar dx) const noexcept {
    return math::constant<Scalar>(1) / math::constant<Scalar>(3) * dx * dx *
           Mat<Scalar, dim, dim>::Identity();
  }

  static constexpr Index KernelSizeImpl() { return 1; }
};

/**
 * @brief APIC transfer. for 3d sim.
 *
 * @tparam Scalar
 */
template <typename Scalar, typename Kern> class ApicRegular {
public:
  /**
   * @brief Forward velocity, density. and setup D.
   */
  void Forward();

  /**
   * @brief Backward velocity and density.
   */
  void Backward();

  /**
  * @brief Construct a new Apic Regular object
  * 
  * @param lag 
  * @param euler 
  */
  explicit ApicRegular(LagrangeFluid<Scalar, 3> &lag,
                       EulerFluidRegular<Scalar, 3> &euler);

  // HACK: private:
  void Foreach(std::function<void(Index pid, Vec<Index, 3> grid_id,
                                  Scalar weight, Vec3<Scalar> displacement)>
                   f) const;
  LagrangeFluid<Scalar, 3> &lagrange_;  // Standard Lagrange Fluid Model
  EulerFluidRegular<Scalar, 3> &euler_; // Standard Euler Fluid Model
  Kern interp_kernel_;                  // Kernel for P2G and G2P
  Field<Scalar, 9> matrix_b_;
  NdRangeIndexer<3> grid_idxer_;
};
} // namespace acg::physics::mpm


/****************************************
 * Implementation Details
 ****************************************/
namespace acg::physics::mpm {
template <typename Scalar, typename Kern>
void ApicRegular<Scalar, Kern>::Forward() {
  auto mass = access(lagrange_.mass_);
  auto velo = access(lagrange_.velocity_);
  euler_.mass_.setZero();
  euler_.velocity_.setZero();
  auto grid_m = access(euler_.mass_, grid_idxer_);
  auto grid_v = access(euler_.velocity_, grid_idxer_);
  // Transfer mass.
  Foreach([this, &mass, &grid_m](Index p, Vec3Index g, Scalar weight,
                                 Vec3<Scalar>) {
    grid_m(g.x(), g.y(), g.z()) += weight * mass(p);
  });

  // Transfer momentum
  auto dp = interp_kernel_.GetMatrixD(euler_.grid_size_);
  auto dp_inv = dp.inverse();
  auto bp = access<acg::DefaultIndexer, acg::ReshapeTransform<3, 3>>(matrix_b_);
  Foreach([&](Index p, Vec3Index g, Scalar weight,
              Vec3<Scalar> displacement) -> void {
    Vec3<Scalar> momentum_inc =
        weight * mass(p) * (velo(p) + bp(p) * dp_inv * displacement);
    // fmt::print("Grid Velocity at [{}] += {}\n", g.transpose(),
    //            momentum_inc.transpose());
    grid_v(g.x(), g.y(), g.z()) += momentum_inc;
  });

  // compute velocity.
  auto acc = access(euler_.mass_);
  for (auto [i, v] : enumerate(access(euler_.velocity_))) {
    if (acc(i) != math::constant<Scalar>(0)) {
      v /= acc(i);
    }
  }
}

template <typename Scalar, typename Kern>
void ApicRegular<Scalar, Kern>::Foreach(
    std::function<void(Index pid, Vec<Index, 3> grid_id, Scalar weight,
                       Vec<Scalar, 3>)>
        f) const {
  constexpr Index kern_size = Kern::KernelSize();
  auto posi = access(lagrange_.position_);
  auto cor = GridCoordConventer<Scalar, 3>(
      euler_.lower_bound_, euler_.upper_bound_, euler_.grid_size_);
  for (auto [p, po] : enumerate(posi)) {
    auto p_position_local = cor.World2LocalContinuous(po);
    auto most_close_grid = p_position_local.template cast<Index>();
    for (Index di = -kern_size; di <= kern_size; ++di) {
      for (Index dj = -kern_size; dj <= kern_size; ++dj) {
        for (Index dk = -kern_size; dk <= kern_size; ++dk) {
          // Grid [i, j, k]
          Index i = most_close_grid.x() + di;
          Index j = most_close_grid.y() + dj;
          Index k = most_close_grid.z() + dk;
          Vec<Index, 3> ijk{i, j, k};
          ACG_DEBUG_CHECK((ijk.array() >= 0).all(), "ijk invalid = {}, p = {}",
                          ijk.transpose(), po.transpose());
          Vec<Scalar, 3> displacement =
              (ijk.template cast<Scalar>() - p_position_local).array() + 0.5;
          auto weight = interp_kernel_.Sample(displacement);
          f(p, {i, j, k}, weight, displacement * euler_.grid_size_);
        }
      }
    }
  }
}

template <typename Scalar, typename Kern>
ApicRegular<Scalar, Kern>::ApicRegular(LagrangeFluid<Scalar, 3> &lag,
                                       EulerFluidRegular<Scalar, 3> &euler)
    : lagrange_(lag), euler_(euler),
      grid_idxer_(std::make_from_tuple<NdRangeIndexer<3>>(
          make_tuple_from_vector(euler.div_count_))) {
  matrix_b_.resize(9, lag.mass_.cols());
  matrix_b_.setZero();
}

template <typename Scalar, typename Kern>
void ApicRegular<Scalar, Kern>::Backward() {
  auto particle_v = access(lagrange_.velocity_);
  auto grid_v = access(euler_.velocity_, grid_idxer_);
  auto bp = access<acg::DefaultIndexer, acg::ReshapeTransform<3, 3>>(matrix_b_);
  matrix_b_.setZero();
  lagrange_.velocity_.setZero();

  Foreach([&](Index p, Vec3Index g, Scalar weight, Vec3<Scalar> displacement) {
    particle_v(p) += weight * grid_v(g.x(), g.y(), g.z());
    bp(p) += weight * grid_v(g.x(), g.y(), g.z()) * displacement.transpose();
  });
}

} // namespace acg::physics::mpm
