#include <Eigen/Sparse>

#include "common.hpp"
#include "traits.hpp"

namespace acg {
// Sparse Matrix
template <typename Scalar, int Options = 0> using SpMat
    = Eigen::SparseMatrix<Scalar, Options, acg::Index>;

// Sparse Vector
template <typename Scalar, int Options = 0> using SpVec
    = Eigen::SparseVector<Scalar, Options, acg::Index>;

namespace details {
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
}  // namespace details
}  // namespace acg
