#pragma once
#include "common.hpp"
#include "tensor_traits.hpp"

namespace acg {

namespace math {
// Support 4 kind of Kronecker Product:
// 1. Dense .* Desnse
// 2. Sparse .* Dense
// 3. Sparse .* Sparse
// 4. Dense .* Sparse

// Dense By Dense Kronecker Product Computing
template <typename Lhs, typename Rhs> class KroneckerDenseByDense {
  static constexpr int lhs_rows_ = details::TensorTrait<Lhs>::rows;
  static constexpr int rhs_rows_ = details::TensorTrait<Rhs>::rows;
  static constexpr int lhs_cols_ = details::TensorTrait<Lhs>::cols;
  static constexpr int rhs_cols_ = details::TensorTrait<Rhs>::cols;

  static constexpr int out_rows_ = (lhs_rows_ == Eigen::Dynamic || rhs_rows_ == Eigen::Dynamic)
                                       ? Eigen::Dynamic
                                       : lhs_rows_ * rhs_rows_;
  static constexpr int out_cols_ = (lhs_cols_ == Eigen::Dynamic || rhs_cols_ == Eigen::Dynamic)
                                       ? Eigen::Dynamic
                                       : lhs_cols_ * rhs_cols_;

  using Scalar = typename details::TensorTrait<Lhs>::Scalar;
  using OutType = Mat<Scalar, out_rows_, out_cols_>;
  const Lhs& lhs_;
  const Rhs& rhs_;

public:
  inline KroneckerDenseByDense(const Lhs& lhs, const Rhs& rhs) : lhs_(lhs), rhs_(rhs) {}

  inline OutType operator()() {
    static_assert(std::is_same_v<Scalar, typename details::TensorTrait<Rhs>::Scalar>,
                  "Lhs::Scalar and Rhs::Scalar does not match.");
    auto lhs_rows = lhs_.rows();
    auto lhs_cols = lhs_.cols();
    auto rhs_rows = rhs_.rows();
    auto rhs_cols = rhs_.cols();
    OutType output(lhs_rows * rhs_rows, lhs_cols * rhs_cols);

    for (Idx l_j = 0; l_j < lhs_cols; ++l_j) {
      for (Idx r_j = 0; r_j < rhs_cols; ++r_j) {
        for (Idx l_i = 0; l_i < lhs_rows; ++l_i) {
          for (Idx r_i = 0; r_i < rhs_rows; ++r_i) {
            output(l_i * rhs_rows + r_i, l_j * rhs_cols + r_j) = lhs_(l_i, l_j) * rhs_(r_i, r_j);
          }
        }
      }
    }
    return output;
  }
};

}  // namespace math

}  // namespace acg
