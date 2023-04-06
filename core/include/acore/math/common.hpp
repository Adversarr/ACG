#pragma once
#include <Eigen/Core>
#include <acore/common.hpp>

namespace acg {

template <typename PlainObjectType, int Options = 0,
          typename StrideType = typename Eigen::internal::conditional<
              PlainObjectType::IsVectorAtCompileTime, Eigen::InnerStride<1>,
              Eigen::OuterStride<>>::type>
using Ref = Eigen::Ref<PlainObjectType, Options, StrideType>;

/* Vector Decl */
template <typename T = acg::Float, int dim = Eigen::Dynamic>
using Vec = Eigen::Vector<T, dim>;
///> Type defines
template <typename T = acg::Float> using Vec2 = Eigen::Vector2<T>;

template <typename T = acg::Float> using Vec3 = Eigen::Vector3<T>;

template <typename T = acg::Float> using Vec4 = Eigen::Vector4<T>;

using Vec4f = Eigen::Vector4<Float32>;

using Vec4d = Eigen::Vector4<Float64>;

using Vec3f = Eigen::Vector3<Float32>;

using Vec3d = Eigen::Vector3<Float64>;

using Vec2f = Eigen::Vector2<Float32>;

using Vec2d = Eigen::Vector2<Float64>;

using Vec2Index = Eigen::Vector2<Index>;

using Vec3Index = Eigen::Vector3<Index>;

using Vec4Index = Eigen::Vector4<Index>;

// Matrix Alias for Eigen
template <typename Scalar = acg::Float, int Rows = Eigen::Dynamic,
          int Cols = Eigen::Dynamic,
          int Options =
              Eigen::AutoAlign |
              ((Rows == 1 && Cols != 1) ? Eigen::RowMajor : Eigen::ColMajor),
          int MaxRows = Rows, int MaxCols = Cols>
using Mat = Eigen::Matrix<Scalar, Rows, Cols, Options, MaxRows, MaxCols>;

// Full Dynamic Matrix.
template <typename Scalar = acg::Float, int Options = Eigen::ColMajor>
using MatXxX = Mat<Scalar, Eigen::Dynamic, Eigen::Dynamic, Options>;

template <typename Scalar, int Rows, int Options = Eigen::ColMajor>
using MatNxX = Mat<Scalar, Rows, Eigen::Dynamic, Options>;

template <typename Scalar, int Cols, int Options = Eigen::ColMajor>
using MatXxN = Mat<Scalar, Eigen::Dynamic, Cols, Options>;

template <typename Scalar, int Rows, int Cols, int Options = Eigen::ColMajor>
using MatNxM = Mat<Scalar, Rows, Cols, Options>;

template <int Options = Eigen::ColMajor>
using MatXxXf = MatXxX<Float32, Options>;

template <int Rows, int Options = Eigen::ColMajor>
using MatNxXf = MatNxX<Float32, Rows, Options>;

template <int Cols, int Options = Eigen::ColMajor>
using MatXxNf = MatXxN<Float32, Cols, Options>;

template <int Rows, int Cols, int Options = Eigen::ColMajor>
using MatNxMf = MatNxM<Float32, Rows, Cols, Options>;

template <int Options = Eigen::ColMajor>
using MatXxXd = MatXxX<Float64, Options>;

template <int Rows, int Options = Eigen::ColMajor>
using MatNxXd = MatNxX<Float64, Rows, Options>;

template <int Cols, int Options = Eigen::ColMajor>
using MatXxNd = MatXxN<Float64, Cols, Options>;

template <int Rows, int Cols, int Options = Eigen::ColMajor>
using MatNxMd = MatNxM<Float64, Rows, Cols, Options>;

template <typename Scalar = acg::Float> using Mat2x2 = Eigen::Matrix2<Scalar>;

template <typename Scalar = acg::Float> using Mat3x3 = Eigen::Matrix3<Scalar>;

template <typename Scalar = acg::Float> using Mat4x4 = Eigen::Matrix4<Scalar>;

/**
 * Shorthands for Squared Matrices.
 */
using Mat2x2f = Mat2x2<Float32>;
using Mat2x2d = Mat2x2<Float64>;
using Mat3x3f = Mat3x3<Float32>;
using Mat3x3d = Mat3x3<Float64>;
using Mat4x4f = Mat4x4<Float32>;
using Mat4x4d = Mat4x4<Float64>;

template <typename T, int n_attrib = 1>
using Field =
    Eigen::Matrix<T, n_attrib, Eigen::Dynamic,
                  Eigen::AutoAlign |
                      (n_attrib == 1 ? Eigen::RowMajor : Eigen::ColMajor)>;

template <typename T, int n_attrib>
using AttrTrans =
    Eigen::Matrix<T, Eigen::Dynamic, n_attrib,
                  Eigen::AutoAlign |
                      (n_attrib == 1 ? Eigen::RowMajor : Eigen::ColMajor)>;

template <typename T>
using TransposeType = Eigen::Matrix<typename T::Scalar, T::ColsAtCompileTime,
                                    T::RowsAtCompileTime, T::Options>;

namespace types {
template <typename Scalar = acg::Float, int dim = 3>
using Position = Vec3<Scalar>;
template <typename Scalar = acg::Float, int dim = 3>
using PositionField = Field<Scalar, dim>;

template <typename Scalar> using DynamicField = Field<Scalar, Eigen::Dynamic>;

using Rgb = Vec3f;
using RgbField = Field<Float32, 3>;

using Rgba = Vec4f;
using RgbaField = Field<Float32, 4>;

template <typename Scalar> using ScalarList = Field<Scalar, 1>;

namespace topology {
// Take from vulkan-topology:
// https://registry.khronos.org/vulkan/specs/1.3-extensions/html/vkspec.html#drawing-primitive-topologies

// @brief use Index *3 to represent a face.
using TriangleList = Field<Index, 3>;

// @brief number of triangles is #V - 2
using TriangleStrips = Field<Index, 1>;

using LineList = Field<Index, 2>;

// @brief p_i = {v[i], v[i+1]}
using LineStrips = Field<Index, 1>;

using PointList = Field<Index, 1>;

// @brief 3d-Tetra-Index List
using TetraList = Field<Index, 4>;

} // namespace topology
} // namespace types

// Helpers for std::array.
template <typename Scalar, int dim> using StlVec = std::array<Scalar, dim>;
template <int dim> using IndexVec = Vec<Index, dim>;
template <int dim> using StlIndexVec = StlVec<Index, dim>;

template <typename Scalar, int dim>
inline std::array<Scalar, dim> to_array(Vec<Scalar, dim> vec) {
  std::array<Scalar, dim> ret;
  for (Index i = 0; i < dim; ++i) {
    ret[i] = vec[i];
  }
  return ret;
}

template <typename Scalar, size_t dim>
inline auto to_vector(const std::array<Scalar, dim> &arr) {
  Vec<Scalar, dim> ret = Eigen::Map<const Vec<Scalar, dim>>(arr.data());
  return ret;
}

// Helpers for std::tuple
// I think Vec of max dim = 4 is enough. because your application is mostly
// 2-3d.
template <typename Scalar> Vec<Scalar, 1> to_vector(std::tuple<Scalar> tup) {
  return Vec<Scalar, 1>{std::get<0>(tup)};
}

template <typename Scalar>
Vec<Scalar, 2> to_vector(std::tuple<Scalar, Scalar> tup) {
  return {std::get<0>(tup), std::get<1>(tup)};
}

template <typename Scalar>
Vec<Scalar, 3> to_vector(std::tuple<Scalar, Scalar, Scalar> tup) {
  return {std::get<0>(tup), std::get<1>(tup), std::get<2>(tup)};
}

template <typename Scalar>
Vec<Scalar, 4> to_vector(std::tuple<Scalar, Scalar, Scalar, Scalar> tup) {
  return {std::get<0>(tup), std::get<1>(tup), std::get<2>(tup),
          std::get<3>(tup)};
}

template <typename Scalar> std::tuple<Scalar> to_tuple(Vec<Scalar, 1> vec) {
  return {vec.x()};
}

template <typename Scalar>
std::tuple<Scalar, Scalar> to_tuple(Vec<Scalar, 2> vec) {
  return {vec.x(), vec.y()};
}

template <typename Scalar>
std::tuple<Scalar, Scalar, Scalar> to_tuple(Vec<Scalar, 3> vec) {
  return {vec.x(), vec.y(), vec.z()};
}

template <typename Scalar>
std::tuple<Scalar, Scalar, Scalar, Scalar> to_tuple(Vec<Scalar, 4> vec) {
  return {vec.x(), vec.y(), vec.z(), vec.w()};
}

// Helper for make a vector directly.
template <typename Scalar> Vec<Scalar, 1> make_vector(Scalar s) {
  return Vec<Scalar, 1>{s};
}

template <typename Scalar> Vec<Scalar, 2> make_vector(Scalar x, Scalar y) {
  Vec<Scalar, 2> r({x, y});
  return r;
}

template <typename Scalar>
Vec<Scalar, 3> make_vector(Scalar x, Scalar y, Scalar z) {
  Vec<Scalar, 3> r({x, y, z});
  return r;
}

template <typename Scalar>
Vec<Scalar, 4> make_vector(Scalar x, Scalar y, Scalar z, Scalar w) {
  Vec<Scalar, 4> r({x, y, z, w});
  return r;
}

} // namespace acg
