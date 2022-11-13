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

namespace constants {

template <typename F> constexpr F pi = 3.14159292035;

}

}  // namespace acg
