#pragma once

#include "common.hpp"

namespace acg {
namespace details {
template <typename T> struct Trait {
  static constexpr int rows = T::RowsAtCompileTime;            // NOLINT
  static constexpr int cols = T::ColsAtCompileTime;            // NOLINT
  static constexpr bool is_vector = T::IsVectorAtCompileTime;  // NOLINT
  static constexpr bool is_row_major = T::IsRowMajor;          // NOLINT
  static constexpr bool is_col_major = !is_row_major;          // NOLINT
  static constexpr bool is_scalar = false;                     // NOLINT
  static constexpr int opt = T::Options;                       // NOLINT
  using Scalar = typename T::Scalar;
  using type
      = Eigen::Matrix<T, rows, cols, T::Options, T::MaxRowsAtCompileTime, T::MaxColsAtCompileTime>;
  using transpose = Eigen::Matrix<T, cols, rows, opt ^ Eigen::RowMajor, T::MaxRowsAtCompileTime,
                                  T::MaxColsAtCompileTime>;
  static constexpr bool is_dense = true;  // NOLINT
};

template <typename Sca, int Options> struct Trait<SpMat<Sca, Options>> {
  static constexpr int rows = Eigen::Dynamic;                            // NOLINT
  static constexpr int cols = Eigen::Dynamic;                            // NOLINT
  static constexpr bool is_vector = false;                               // NOLINT
  static constexpr bool is_row_major = SpMat<Sca, Options>::IsRowMajor;  // NOLINT
  static constexpr bool is_col_major = !is_row_major;                    // NOLINT
  static constexpr bool is_scalar = false;                               // NOLINT
  static constexpr bool is_dense = false;                                // NOLINT
  using Scalar = Sca;
  using type = SpMat<Sca, Options>;
  using transpose = type;
};

template <> struct Trait<acg::F32> {
  static constexpr int rows = 1;                       // NOLINT
  static constexpr int cols = 1;                       // NOLINT
  static constexpr int dim = rows * cols;              // NOLINT
  static constexpr bool is_vector = false;             // NOLINT
  static constexpr bool is_row_major = false;          // NOLINT
  static constexpr bool is_col_major = !is_row_major;  // NOLINT
  static constexpr bool is_scalar = true;              // NOLINT
  static constexpr bool is_dense = true;               // NOLINT
  using Scalar = acg::F32;
  using type = acg::F32;
  using transpose = type;
};
template <> struct Trait<acg::F64> {
  static constexpr int rows = 1;                       // NOLINT
  static constexpr int cols = 1;                       // NOLINT
  static constexpr bool is_vector = false;             // NOLINT
  static constexpr int dim = rows * cols;              // NOLINT
  static constexpr bool is_row_major = false;          // NOLINT
  static constexpr bool is_col_major = !is_row_major;  // NOLINT
  static constexpr bool is_scalar = true;              // NOLINT
  static constexpr bool is_dense = true;               // NOLINT
  using Scalar = acg::F64;
  using type = acg::F64;
  using transpose = type;
};

}  // namespace details
///< Type Defines


// Tensor Trati Meta-Programming Helper
using details::Trait; // NOLINT
}  // namespace acg
