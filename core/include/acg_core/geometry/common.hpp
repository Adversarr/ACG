#pragma once
#include "../math/common.hpp"

namespace acg::geometry {
namespace topology {
// Take from vulkan-topology:
// https://registry.khronos.org/vulkan/specs/1.3-extensions/html/vkspec.html#drawing-primitive-topologies

// @brief: use Idx*3 to represent a face.
using TriangleList = Field<Idx, 3>;

// @brief: number of triangles is #V - 2
using TriangleStrips = Field<Idx, 1>;

using LineList = Field<Idx, 2>;

// @brief: p_i = {v[i], v[i+1]}
using LineStrips = Field<Idx, 1>;

using PointList = Field<Idx, 1>;

}  // namespace topology

}  // namespace acg::geometry
