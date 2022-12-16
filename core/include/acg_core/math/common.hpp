#pragma once
#include <Eigen/Eigen>

#include "../common.hpp"
namespace acg {

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

using Vec2Idx = Eigen::Vector2<Idx>;

using Vec3Idx = Eigen::Vector3<Idx>;

using Vec4Idx = Eigen::Vector4<Idx>;

template <typename T, int n_attrib> using Field
    = Eigen::Matrix<T, n_attrib, Eigen::Dynamic,
                    Eigen::AutoAlign | (n_attrib == 1 ? Eigen::RowMajor : Eigen::ColMajor)>;

template <typename T, int n_attrib> using AttrTrans
    = Eigen::Matrix<T, Eigen::Dynamic, n_attrib,
                    Eigen::AutoAlign | (n_attrib == 1 ? Eigen::RowMajor : Eigen::ColMajor)>;

template <typename T> using TransposeType
    = Eigen::Matrix<typename T::Scalar, T::ColsAtCompileTime, T::RowsAtCompileTime, T::Options>;

namespace attr {
template <typename Scalar, int dim = 3> using Position = Vec3<Scalar>;
template <typename Scalar, int dim = 3> using PositionList = Field<Scalar, dim>;
using Rgb = Vec3f;
using RgbList = Field<F32, 3>;
using Rgba = Vec4f;
using RgbaList = Field<F32, 4>;
template <typename Scalar> using ScalarList = Field<Scalar, 1>;
}  // namespace attribute
}  // namespace acg
