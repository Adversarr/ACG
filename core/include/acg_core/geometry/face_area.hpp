#pragma once
#include "../math/common.hpp"
#include "common.hpp"

namespace acg {

namespace geometry {
template <typename Scalar, int dim = 3> attr::ScalarList<Scalar> compute_face_area(
    const topology::TriangleList& triangles,
    const attr::PositionList<Scalar, dim>& positions) {
  attr::ScalarList<Scalar> areas;
  areas.resize(1, triangles.cols());
  for (Idx i = 0; i < triangles.cols(); ++i) {
    int x = triangles(0, i);
    int y = triangles(1, i);
    int z = triangles(2, i);
    auto d0 = positions.col(x) - positions.col(z);
    auto d1 = positions.col(y) - positions.col(z);
    areas.col(i) = d0.cross(d1).norm();
  }
  areas *= 0.5;
  return areas;
}
}  // namespace geometry
}  // namespace acg
