#pragma once
#include "../math/common.hpp"
#include "common.hpp"
#include <Eigen/Dense>

namespace acg {

namespace geometry {
template <typename Scalar, int dim = 3>
types::ScalarList<Scalar> face_area(const types::topology::TriangleList& triangles,
                                   const types::PositionField<Scalar, dim>& positions) {
  static_assert(dim == 2 || dim == 3, "Face area computing requires dim = 2 or 3.");
  types::ScalarList<Scalar> areas;
  areas.resize(1, triangles.cols());
  for (Index i = 0; i < triangles.cols(); ++i) {
    auto x = triangles(0, i);
    auto y = triangles(1, i);
    auto z = triangles(2, i);
    auto d0 = positions.col(x) - positions.col(z);
    auto d1 = positions.col(y) - positions.col(z);
    areas(i) = d0.cross(d1).norm();
  }
  areas *= 0.5;
  return areas;
}
}  // namespace geometry
}  // namespace acg
