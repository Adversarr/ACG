#pragma once
#include "mesh.hpp"
#include "../math.hpp"

namespace acg::geometry {

using geometry::TriangleMesh;

/**
 * @brief Return a UV based sphere
 *
 * @param center
 * @param radius
 * @param n_slices number of vertical lines
 * @param n_stacks number of horizonal lines
 * @return TriangleMesh<f32>
 */
TriangleMesh<F32> sphere_uv(Vec3f center, F32 radius, Idx n_stacks = 6, Idx n_slices = 12);

/**
 * @brief Create an icosphere
 * 
 * @param center 
 * @param radius 
 * @return TriangleMesh<F32> 
 */
TriangleMesh<F32> sphere_20(Vec3f center, F32 radius);


}  // namespace acg::visualizer::models
