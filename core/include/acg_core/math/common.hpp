#pragma once
#include <Eigen/Eigen>

#include "../common.hpp"
namespace acg {

template <typename T, int dim> 
using Vec = Eigen::Vector<T, dim>;
///> Type defines
template <typename T> using Vec2 = Eigen::Vector2<T>;

template <typename T> using Vec3 = Eigen::Vector3<T>;

template <typename T> using Vec4 = Eigen::Vector4<T>;

using Vec4f = Eigen::Vector4<F32>;

using Vec4d = Eigen::Vector4<F64>;

using Vec3f = Eigen::Vector3<F32>;

using Vec3d = Eigen::Vector3<F64>;

using Vec2f = Eigen::Vector2<F32>;

using Vec2d = Eigen::Vector2<F64>;

using Vec2Index = Eigen::Vector2<Index>;

using Vec3Index = Eigen::Vector3<Index>;

using Vec4Index = Eigen::Vector4<Index>;

// Matrix Alias for Eigen
template <typename Scalar, int Rows, int Cols,
          int Options
          = Eigen::AutoAlign | ((Rows == 1 && Cols != 1) ? Eigen::RowMajor : Eigen::ColMajor),
          int MaxRows = Rows, int MaxCols = Cols>
using Mat = Eigen::Matrix<Scalar, Rows, Cols, Options, MaxRows, MaxCols>;

// Full Dynamic Matrix.
template <typename Scalar, int Options = Eigen::ColMajor> using MatXxX
    = Mat<Scalar, Eigen::Dynamic, Eigen::Dynamic, Options>;

template <typename Scalar, int Rows, int Options = Eigen::ColMajor> using MatNxX
    = Mat<Scalar, Rows, Eigen::Dynamic, Options>;

template <typename Scalar, int Cols, int Options = Eigen::ColMajor> using MatXxN
    = Mat<Scalar, Eigen::Dynamic, Cols, Options>;

template <typename Scalar, int Rows, int Cols, int Options = Eigen::ColMajor> using MatNxM
    = Mat<Scalar, Rows, Cols, Options>;

template <int Options = Eigen::ColMajor> using MatXxXf = MatXxX<F32, Options>;

template <int Rows, int Options = Eigen::ColMajor> using MatNxXf = MatNxX<F32, Rows, Options>;

template <int Cols, int Options = Eigen::ColMajor> using MatXxNf = MatXxN<F32, Cols, Options>;

template <int Rows, int Cols, int Options = Eigen::ColMajor> using MatNxMf
    = MatNxM<F32, Rows, Cols, Options>;

template <int Options = Eigen::ColMajor> using MatXxXd = MatXxX<F64, Options>;

template <int Rows, int Options = Eigen::ColMajor> using MatNxXd = MatNxX<F64, Rows, Options>;

template <int Cols, int Options = Eigen::ColMajor> using MatXxNd = MatXxN<F64, Cols, Options>;

template <int Rows, int Cols, int Options = Eigen::ColMajor> using MatNxMd
    = MatNxM<F64, Rows, Cols, Options>;

template <typename Scalar> using Mat2x2 = Eigen::Matrix2<Scalar>;

template <typename Scalar> using Mat3x3 = Eigen::Matrix3<Scalar>;

template <typename Scalar> using Mat4x4 = Eigen::Matrix4<Scalar>;

/**
 * Shorthands for Squared Matrices.
 */
using Mat2x2f = Mat2x2<F32>;
using Mat2x2d = Mat2x2<F64>;
using Mat3x3f = Mat3x3<F32>;
using Mat3x3d = Mat3x3<F64>;
using Mat4x4f = Mat4x4<F32>;
using Mat4x4d = Mat4x4<F64>;


// Sparse Matrix
template<typename Scalar, int Options = 0>
using SpMat = Eigen::SparseMatrix<Scalar, Options, acg::Index>;

// Sparse Vector
template<typename Scalar, int Options = 0>
using SpVec = Eigen::SparseVector<Scalar, Options, acg::Index>;


template <typename T, int n_attrib> using Field
    = Eigen::Matrix<T, n_attrib, Eigen::Dynamic,
                    Eigen::AutoAlign | (n_attrib == 1 ? Eigen::RowMajor : Eigen::ColMajor)>;

template <typename T, int n_attrib> using AttrTrans
    = Eigen::Matrix<T, Eigen::Dynamic, n_attrib,
                    Eigen::AutoAlign | (n_attrib == 1 ? Eigen::RowMajor : Eigen::ColMajor)>;

template <typename T> using TransposeType
    = Eigen::Matrix<typename T::Scalar, T::ColsAtCompileTime, T::RowsAtCompileTime, T::Options>;

namespace types {
template <typename Scalar, int dim = 3> using Position = Vec3<Scalar>;
template <typename Scalar, int dim = 3> using PositionField = Field<Scalar, dim>;

template <typename Scalar> using DynamicField = Field<Scalar, Eigen::Dynamic>;

using Rgb = Vec3f;
using RgbField = Field<F32, 3>;

using Rgba = Vec4f;
using RgbaField = Field<F32, 4>;

template <typename Scalar> using ScalarList = Field<Scalar, 1>;
}  // namespace types

}  // namespace acg
