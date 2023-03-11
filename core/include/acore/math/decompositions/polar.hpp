#pragma once

#include <acore/math/common.hpp>
#include <acore/math/traits.hpp>

#include "svd.hpp"
namespace acg ::math {

template <typename Derived> class PolarDecomposition {
  using Scalar = typename Trait<Derived>::Scalar;
  static_assert(Trait<Derived>::cols == Trait<Derived>::rows,
                "Polar Decomposition requires rows == cols");

  static constexpr Index dim_ = Trait<Derived>::cols;

public:
  explicit PolarDecomposition(const Eigen::MatrixBase<Derived>& matrix);

  Svd<Derived> svd_;
  Mat<Scalar, dim_, dim_> rot_;
  Mat<Scalar, dim_, dim_> symm_;
};

template <typename Derived> class PolarDecompositionRv {
  using Scalar = typename Trait<Derived>::Scalar;
  static_assert(Trait<Derived>::cols <= 3, "Expect Cols <= 3.");
  static_assert(Trait<Derived>::cols >= 2, "Expect Cols >= 2.");
  static_assert(Trait<Derived>::rows >= 2, "Expect Rows >= 2.");
  static_assert(Trait<Derived>::rows <= 3, "Expect Rows <= 3.");

public:
  explicit PolarDecompositionRv(const Eigen::MatrixBase<Derived>& matrix);

  SvdRv<Derived> svd_;
  Mat<Scalar, 3, 3> rot_;
  Mat<Scalar, 3, 3> symm_;
};
}  // namespace acg::math
#include "./details/polar-inl.hpp"
