#pragma once

#include "acg_core/geometry/fv_transform.hpp"
#include "common.hpp"
#include "face_area.hpp"

namespace acg::geometry {

enum class NormalPerVertexMode { kArea, kAngle, kUniform };

// NOTE: Normal can only computed with Dimension == 3
//
// NOTE: Normal computing relies on the index sequence in tlist.
template <typename Scalar> class Normal {
private:
  const topology::TriangleList& triangle_list_;

  const types::PositionList<Scalar, 3>& positions_;

public:
  Field<Scalar, 3> PerVertex(NormalPerVertexMode mode = NormalPerVertexMode::kArea) const noexcept;

  Field<Scalar, 3> PerFace() const noexcept;

  Normal(const topology::TriangleList& triangle_list,
         const types::PositionList<Scalar, 3>& vertices_position)
      : triangle_list_(triangle_list), positions_(vertices_position) {}
};

template <typename Scalar> Field<Scalar, 3> Normal<Scalar>::PerFace() const noexcept {
  int n_face = triangle_list_.cols();
  Field<Scalar, 3> face_normal(3, n_face);
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
  types::ScalarList<Scalar> weight;
  switch (mode) {
    case NormalPerVertexMode::kUniform:
      weight.resize(1, triangle_list_.cols());
      weight.setOnes();
      break;

    case NormalPerVertexMode::kArea:
      weight = geometry::face_area(triangle_list_, positions_);
      break;

    case NormalPerVertexMode::kAngle:
      // TODO: Not Implemented Error.
      break;
  }
  FVTransform fvt(triangle_list_, positions_.cols());
  return fvt.template FaceToVertex<Scalar, 3>(face_normal, weight);
}

}  // namespace acg::geometry
