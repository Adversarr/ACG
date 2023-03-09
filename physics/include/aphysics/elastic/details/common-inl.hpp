#pragma once
#include <acore/math/traits.hpp>

#include "../common.hpp"
namespace acg::physics::elastic {

template <typename Derived> auto compose_pfpx(const Eigen::MatrixBase<Derived>& rinv) {
  using Scalar = typename Trait<Derived>::Scalar;
  constexpr Index dim = Trait<Derived>::rows;
  static_assert(dim == 3, "Only dim = 3 is allowed.");
  // Page 224
  Mat<Scalar, 9, 12> pfpx = Mat<Scalar, 9, 12>::Zero();
  const Scalar m = rinv(0, 0);
  const Scalar n = rinv(0, 1);
  const Scalar o = rinv(0, 2);
  const Scalar p = rinv(1, 0);
  const Scalar q = rinv(1, 1);
  const Scalar r = rinv(1, 2);
  const Scalar s = rinv(2, 0);
  const Scalar t = rinv(2, 1);
  const Scalar u = rinv(2, 2);
  const Scalar t1 = -m - p - s;
  const Scalar t2 = -n - q - t;
  const Scalar t3 = -o - r - u;
  pfpx(0,0) = t1;
  pfpx(0, 3) = m;
  pfpx(0, 6) = p;
  pfpx(0, 9) = s;
  pfpx(1, 1) = t1;
  pfpx(1, 4) = m;
  pfpx(1, 7) = p;
  pfpx(1, 10) = s;
  pfpx(2, 2) = t1;
  pfpx(2, 5) = m;
  pfpx(2, 8) = p;
  pfpx(2, 11) = s;
  pfpx(3, 0) = t2;
  pfpx(3, 3) = n;
  pfpx(3, 6) = q;
  pfpx(3, 9) = t;
  pfpx(4, 1) = t2;
  pfpx(4, 4) = n;
  pfpx(4, 7) = q;
  pfpx(4, 10) = t;
  pfpx(5, 2) = t2;
  pfpx(5, 5) = n;
  pfpx(5, 7) = q;
  pfpx(5, 11) = t;
  pfpx(6, 0) = t3;
  pfpx(6, 3) = o;
  pfpx(6, 6) = r;
  pfpx(6, 9) = u;
  pfpx(7, 1) = t3;
  pfpx(7, 4) = o;
  pfpx(7, 7) = r;
  pfpx(7, 10) = u;
  pfpx(8, 2) = t3;
  pfpx(8, 5) = o;
  pfpx(8, 8) = r;
  pfpx(8, 11) = u;

  return pfpx;
}
}  // namespace acg::physics::elastic
