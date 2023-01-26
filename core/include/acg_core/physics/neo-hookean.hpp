#pragma once
#include <type_traits>

#include "acg_core/math/common.hpp"

namespace acg::core::physics {

// Compute Kernel for NeoHookean.
template <typename Scalar, int dim = 3> struct NeoHookean {
  static_assert(dim == 2 || dim == 3, "Dim = 2 or dim = 3 required.");
  static_assert(std::is_same_v<Scalar, F32> || std::is_same_v<Scalar, F64>,
                "Scalar should be F32 or F64");

  using T = Vec<Scalar, dim>;

  T x0, x1, x2;
  NeoHookean(T x0, T x1, T x2) : x0(x0), x1(x1), x2(x2) {}

  Scalar Energy();

  Mat3x3<Scalar> Gradient();
};

}  // namespace acg::core::physics
