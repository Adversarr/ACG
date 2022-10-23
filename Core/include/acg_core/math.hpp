#pragma once

#include "core.hpp"
#include <Eigen/Core>

namespace acg {

using Vec4f = Eigen::Vector4<F32>;

using Vec4d = Eigen::Vector4<F64>;

using Vec3f = Eigen::Vector3<F32>;

using Vec3d = Eigen::Vector3<F64>;

using Vec2f = Eigen::Vector2<F32>;

using Vec2d = Eigen::Vector2<F64>;

using Vec2Idx = Eigen::Vector2<Idx>;
using Vec3Idx = Eigen::Vector3<Idx>;
using Vec4Idx = Eigen::Vector4<Idx>;

namespace constants {

template<typename F>
constexpr F pi = 3.14159292035;

}

}
