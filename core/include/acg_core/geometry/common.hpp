#pragma once
#include "../math/common.hpp"

namespace acg::geometry {
namespace topology {
// Take from vulkan-topology:
// https://registry.khronos.org/vulkan/specs/1.3-extensions/html/vkspec.html#drawing-primitive-topologies

// @brief: use Index *3 to represent a face.
using TriangleList = Field<Index, 3>;

// @brief: number of triangles is #V - 2
using TriangleStrips = Field<Index, 1>;

using LineList = Field<Index, 2>;

// @brief: p_i = {v[i], v[i+1]}
using LineStrips = Field<Index, 1>;

using PointList = Field<Index, 1>;


// @brief: 3d-Tetra-Index List
using TetraList = Field<Index, 4>;

}  // namespace topology

}  // namespace acg::geometry
