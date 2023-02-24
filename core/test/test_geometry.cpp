#include <doctest/doctest.h>

#include <acore/geometry/fv_transform.hpp>
#include <acore/geometry/normal.hpp>
#include <iostream>

TEST_CASE("FVTransform") {
  acg::geometry::topology::TriangleList triangle_list(3, 4);
  Eigen::MatrixX3<acg::Index> triangles_transpose(4, 3);
  triangles_transpose << 0, 1, 2, 1, 3, 2, 3, 0, 2, 0, 3, 1;
  triangle_list = triangles_transpose.transpose();
  acg::Index n = 4;
  acg::Field<float, 1> x(1, 4);
  x << 1, 2, 3, 4;
  acg::geometry::FVTransform transformer(triangle_list, n);
  auto result = transformer.FaceToVertex(x, x);
  CHECK_EQ(result(0), 3.25);
}

TEST_CASE("Normal") {
  acg::geometry::topology::TriangleList triangle_list(3, 4);
  Eigen::MatrixX3<acg::Index> triangles_transpose(4, 3);
  triangles_transpose << 0, 1, 2, 1, 3, 2, 3, 0, 2, 0, 3, 1;
  triangle_list = triangles_transpose.transpose();
  Eigen::MatrixX3f positions_transpose(4, 3);
  positions_transpose << 1, 0, 0, 0, 1, 0, 0, 0, 1, 0, 0, 0;
  acg::types::PositionField<float> position = positions_transpose.transpose();
  acg::geometry::Normal<acg::F32> normal(triangle_list, position);
  auto per_face = normal.PerFace();
  CHECK_EQ(per_face(0, 1), -1);
  auto per_vertex = normal.PerVertex();
  CHECK(fabs(per_vertex(0, 3) + 0.3333333) < 1e-3);
}
