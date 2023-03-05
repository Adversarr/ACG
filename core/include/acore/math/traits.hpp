#pragma once

// NOLINTBEGIN(readability-identifier-naming)
#include "common.hpp"

namespace acg {
namespace details {

template <typename T> struct Trait {
  static constexpr int rows = T::RowsAtCompileTime;
  static constexpr int cols = T::ColsAtCompileTime;
  static constexpr bool is_vector = T::IsVectorAtCompileTime;
  static constexpr bool is_row_major = T::IsRowMajor;
  static constexpr bool is_col_major = !is_row_major;
  static constexpr bool is_scalar = false;
  using Scalar = typename T::Scalar;
  static constexpr bool is_dense = true;
};

template <> struct Trait<acg::Float32> {
  static constexpr int rows = 1;
  static constexpr int cols = 1;
  static constexpr int dim = rows * cols;
  static constexpr bool is_vector = false;
  static constexpr bool is_row_major = false;
  static constexpr bool is_col_major = !is_row_major;
  static constexpr bool is_scalar = true;
  static constexpr bool is_dense = true;
  using Scalar = acg::Float32;
};
template <> struct Trait<acg::Float64> {
  static constexpr int rows = 1;
  static constexpr int cols = 1;
  static constexpr bool is_vector = false;
  static constexpr int dim = rows * cols;
  static constexpr bool is_row_major = false;
  static constexpr bool is_col_major = !is_row_major;
  static constexpr bool is_scalar = true;
  static constexpr bool is_dense = true;
  using Scalar = acg::Float64;
};


}  // namespace details
///< Type Defines

// Tensor Trait Meta-Programming Helper
using details::Trait;


}  // namespace acg
// NOLINTEND(readability-identifier-naming)
