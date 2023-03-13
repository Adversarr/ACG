#pragma once
#include "../math/common.hpp"
#include "mesh.hpp"

namespace acg::geometry {

using geometry::SimpleMesh;
/**
 * @brief TODO: 
 * 
 * @param n 
 * @return SimpleMesh<Float64> 
 */
SimpleMesh<Float64> make_plane_xy(Index n);

/**
 * @brief Return a UV based sphere
 *
 * @param center
 * @param radius
 * @param n_slices number of vertical lines
 * @param n_stacks number of horizonal lines
 * @return TriangleMesh<f32>
 */
SimpleMesh<Float32> sphere_uv(Vec3f center, Float32 radius, Index n_stacks = 6,
                              Index n_slices = 12);

/**
 * @brief Create an icosphere
 *
 * @param center
 * @param radius
 * @return TriangleMesh<F32>
 */
SimpleMesh<Float32> sphere_20(Vec3f center, Float32 radius);

}  // namespace acg::geometry
