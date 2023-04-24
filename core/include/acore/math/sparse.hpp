/****************************************
 *
 * @brief Sparse Matrix Support
 *
 ****************************************/

#pragma once

#include <Eigen/SparseCore>
#include <acore/math/common.hpp> // IWYU pragma: export
#include <acore/math/traits.hpp> // IWYU pragma: export

// NOLINTBEGIN(readability-identifier-naming)
namespace acg {
// Sparse Matrix
template <typename Scalar, int Options = 0>
using SpMat = Eigen::SparseMatrix<Scalar, Options, acg::Index>;

// Sparse Vector
template <typename Scalar, int Options = 0>
using SpVec = Eigen::SparseVector<Scalar, Options, acg::Index>;

template <typename Sca, int Options> struct Trait<SpMat<Sca, Options>> {
  static constexpr int rows = Eigen::Dynamic;
  static constexpr int cols = Eigen::Dynamic;
  static constexpr bool is_vector = false;
  static constexpr bool is_row_major = SpMat<Sca, Options>::IsRowMajor;
  static constexpr bool is_col_major = !is_row_major;
  static constexpr bool is_scalar = false;
  static constexpr bool is_dense = false;
  using Scalar = Sca;
  using type = SpMat<Sca, Options>;
  using transpose = type;
};
} // namespace acg
// NOLINTEND(readability-identifier-naming)
