/***
 * @author Jerry Yang
 */
#pragma once

#include <Eigen/Core>

#include "core.hpp"

namespace acg {

template <typename T> using Vec2 = Eigen::Vector2<T>;

template <typename T> using Vec3 = Eigen::Vector3<T>;

template <typename T> using Vec4 = Eigen::Vector4<T>;

using Vec4f = Eigen::Vector4<F32>;

using Vec4d = Eigen::Vector4<F64>;

using Vec3f = Eigen::Vector3<F32>;

using Vec3d = Eigen::Vector3<F64>;

using Vec2f = Eigen::Vector2<F32>;

using Vec2d = Eigen::Vector2<F64>;

using Vec2Idx = Eigen::Vector2<Idx>;
using Vec3Idx = Eigen::Vector3<Idx>;
using Vec4Idx = Eigen::Vector4<Idx>;

template <typename T, int n_attrib> using AttrVec
    = Eigen::Matrix<T, n_attrib, Eigen::Dynamic, Eigen::AutoAlign | Eigen::ColMajor>;

template <typename T, int n_attrib> using AttrVecTrans
    = Eigen::Matrix<T, Eigen::Dynamic, n_attrib, Eigen::AutoAlign | Eigen::ColMajor>;

template <typename T> using TransposeType
    = Eigen::Matrix<typename T::Scalar, T::ColsAtCompileTime, T::RowsAtCompileTime, T::Options>;

template <typename T> struct TensorTrait {
  static constexpr int rows = T::RowsAtCompileTime;            // NOLINT
  static constexpr int cols = T::ColsAtCompileTime;            // NOLINT
  static constexpr bool is_vector = T::IsVectorAtCompileTime;  // NOLINT
  static constexpr int dim = rows * cols;                      // NOLINT
  static constexpr bool is_row_major = T::IsRowMajor;          // NOLINT
  static constexpr bool is_col_major = !is_row_major;          // NOLINT
  static constexpr bool is_scalar = false;                     // NOLINT
  static constexpr int opt = T::Options;
  using Scalar = typename T::Scalar;
  using type
      = Eigen::Matrix<T, rows, cols, T::Options, T::MaxRowsAtCompileTime, T::MaxColsAtCompileTime>;
  using transpose = Eigen::Matrix<T, cols, rows, opt ^ Eigen::RowMajor, T::MaxRowsAtCompileTime,
                                  T::MaxColsAtCompileTime>;
};
template <> struct TensorTrait<acg::F32> {
  static constexpr int rows = 1;                       // NOLINT
  static constexpr int cols = 1;                       // NOLINT
  static constexpr int dim = rows * cols;              // NOLINT
  static constexpr bool is_vector = false;             // NOLINT
  static constexpr bool is_row_major = false;          // NOLINT
  static constexpr bool is_col_major = !is_row_major;  // NOLINT
  static constexpr bool is_scalar = true;              // NOLINT
  using Scalar = acg::F32;
  using type = acg::F32;
  using transpose = type;
};
template <> struct TensorTrait<acg::F64> {
  static constexpr int rows = 1;                       // NOLINT
  static constexpr int cols = 1;                       // NOLINT
  static constexpr bool is_vector = false;             // NOLINT
  static constexpr int dim = rows * cols;              // NOLINT
  static constexpr bool is_row_major = false;          // NOLINT
  static constexpr bool is_col_major = !is_row_major;  // NOLINT
  static constexpr bool is_scalar = true;              // NOLINT
  using Scalar = acg::F64;
  using type = acg::F64;
  using transpose = type;
};

namespace constants {

template <typename F> constexpr F pi = 3.14159292035;

}

}  // namespace acg
