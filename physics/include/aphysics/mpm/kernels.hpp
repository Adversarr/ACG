#pragma once

#include <acore/math/view.hpp>
#include <acore/math/kernels/bsplines.hpp>

namespace acg::physics::mpm {

/**
 * @brief Interpolate Kernel Base for APIC Transfer.
 *
 * @tparam Scalar
 * @tparam dim
 * @tparam Derived
 */
template <typename Scalar, int dim, typename Derived> struct MPMKernelBase {
  /**
   * @brief Sample the weight at given point. (no bias, var = 1)
   * 
   * @param point 
   * @return constexpr Scalar 
   */
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

/**
 * @brief Implement for MPM Kernel, Cubic BSpline.
 *
 * @tparam Scalar
 * @tparam dim
 */
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
} // namespace acg::physics::mpm