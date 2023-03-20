#pragma once
#include "kernels.hpp"
#include <Eigen/LU>
#include <aphysics/objects/fluid.hpp>
#include <autils/log.hpp>

namespace acg::physics::mpm {

/**
 * @brief APIC transfer. for 3d sim.
 *
 * @tparam Scalar
 */
template <typename Scalar, int dim, template <typename KScalar, int kdim> typename KernT =
                               CubicBSplineKernel>
class ApicRegular {
public:
  using Kern = KernT<Scalar, dim>;
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
  explicit ApicRegular(LagrangeFluid<Scalar, dim> &lag,
                       EulerFluidRegular<Scalar, dim> &euler);

  // HACK: private:
  void Foreach(std::function<void(Index pid, Vec<Index, dim> grid_id,
                                  Scalar weight, Vec<Scalar, dim> displacement)>
                   f) const;
  LagrangeFluid<Scalar, dim> &lagrange_;  // Standard Lagrange Fluid Model
  EulerFluidRegular<Scalar, dim> &euler_; // Standard Euler Fluid Model
  Kern interp_kernel_;                  // Kernel for P2G and G2P
  Field<Scalar, dim * dim> matrix_b_;
  NdRangeIndexer<dim> grid_idxer_;
};
} // namespace acg::physics::mpm

#include "details/transfer-inl.hpp" // IWYU pragma: export
