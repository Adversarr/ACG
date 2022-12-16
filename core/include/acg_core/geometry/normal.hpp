#pragma once

#include "common.hpp"

namespace acg::geometry {

enum class NormalPerVertexMode { kArea, kAngle, kUniform };

// NOTE: Normal can only computed with Dimension == 3
//
// NOTE: Normal computing relies on the index sequence in tlist.
template <typename Scalar> class Normal {
private:
  const topology::TriangleList& triangle_list_;

  const attr::PositionList<Scalar, 3>& positions_;

public:
  Field<Scalar, 3> PerVertex(NormalPerVertexMode mode = NormalPerVertexMode::kArea) const noexcept;

  Field<Scalar, 3> PerFace() const noexcept;

  Normal(const topology::TriangleList& triangle_list,
         const attr::PositionList<Scalar, 3>& vertices_position)
      : triangle_list_(triangle_list), positions_(vertices_position) {}
};

template <typename Scalar> Field<Scalar, 3> Normal<Scalar>::PerFace() const noexcept {
  Field<Scalar, 3> face_normal;
  int n_face = triangle_list_.cols();
  for (Idx i = 0; i < n_face; ++i) {
    const auto& face = triangle_list_.col(i);
    auto d0 = positions_.col(face.y()) - positions_.col(face.x());
    auto d1 = positions_.col(face.z()) - positions_.col(face.x());
    face_normal.col(i) = d0.cross(d1).normalized();
  }
  return face_normal;
}

template <typename Scalar>
Field<Scalar, 3> Normal<Scalar>::PerVertex(NormalPerVertexMode mode) const noexcept {
  Field<Scalar, 3> face_normal = PerFace();
  Field<Scalar, 3> vertex_normal;
  vertex_normal.resizeLike(positions_);
  attr::ScalarList<Scalar> weight;
  weight.resize(1, positions_.cols());
  switch (mode) {
    case NormalPerVertexMode::kUniform:
      break;

    case NormalPerVertexMode::kArea:
      // TODO: do compute
      break;

    case NormalPerVertexMode::kAngle:
      // TODO: do compute
      break;
  }

  return vertex_normal;
}

}  // namespace acg::geometry
