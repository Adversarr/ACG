#pragma once
#include <acore/math/common.hpp>
#include <acore/math/traits.hpp>
namespace acg::math {

template <typename Derived> class Svd {
  using Scalar = typename Trait<Derived>::Scalar;
  static constexpr Index dim_ = Trait<Derived>::cols;

  static_assert(Trait<Derived>::cols == Trait<Derived>::rows,
                "Expect Rows = Cols.");

public:
  explicit Svd(const Eigen::MatrixBase<Derived> &x);
  Mat<Scalar, dim_, dim_> u_;
  Mat<Scalar, dim_, dim_> v_;
  Vec<Scalar, dim_> sigma_;
};

template <typename Derived> class SvdRv {
  using Scalar = typename Trait<Derived>::Scalar;
  static_assert(Trait<Derived>::cols == 3, "Expect Rows = 3.");
  static_assert(Trait<Derived>::rows == 3, "Expect Cols = 3.");

public:
  explicit SvdRv(const Eigen::MatrixBase<Derived> &x);
  Mat<Scalar, 3, 3> u_;
  Mat<Scalar, 3, 3> v_;
  Vec<Scalar, 3> sigma_;
};
} // namespace acg::math

#include "details/svd-inl.hpp" // IWYU pragma: export
