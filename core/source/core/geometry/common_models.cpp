#include "acg_core/geometry/common_models.hpp"
#include "acg_core/math/constants.hpp"
namespace acg::geometry {

SimpleMesh<F32> sphere_uv(Vec3f center, F32 radius, Index n_stacks, Index n_slices) {
  auto n_faces = 2 * n_slices * n_stacks;
  auto n_vertices = 2 + n_slices * n_stacks;
  SimpleMesh<F32>::VerticesType vertices(3, n_vertices);
  SimpleMesh<F32>::FacesType indices(3, n_faces);

  // 1. fill the vertices
  // north pole
  vertices.col(0) = center + Vec3f(0, 0, radius);
  // south pole
  vertices.col(n_vertices - 1) = center - Vec3f(0, 0, radius);
  // crossing points
  F32 d_theta = constants::pi<F32> / static_cast<F32>(n_stacks + 1);
  F32 d_phi = 2 * constants::pi<F32> / static_cast<F32>(n_slices);
  for (Index i = 1; i <= n_stacks; ++i) {
    F32 theta = d_theta * i;
    for (Index j = 0; j < n_slices; ++j) {
      F32 phi = d_phi * j;
      Index idx = (i - 1) * n_slices + j + 1;
      Vec3f pos = {sin(theta) * cos(phi), sin(theta) * sin(phi), cos(theta)};
      vertices.col(idx) = pos * radius + center;
    }
  }

  // 2. fill the indices
  // north pole
  for (Index i = 1; i <= n_slices; ++i) {
    Vec3Index index_vector{0, i, i < n_slices ? i + 1 : 1};
    Index idx = i - 1;
    indices.col(idx) = index_vector;
  }
  // south pole
  for (Index i = 1; i <= n_slices; ++i) {
    auto base_idx = n_vertices - n_slices - 1;
    Vec3Index index_vector{0, base_idx + i, base_idx + (i < n_slices ? i + 1 : 1)};
    Index idx = n_faces - n_slices + i - 1;
    indices.col(idx) = index_vector;
  }
  Index face_idx = n_slices;
  // crossing points:
  for (Index i = 1; i < n_stacks; ++i) {
    for (Index j = 1; j <= n_slices; ++j) {
      Index left_top = (i - 1) * n_slices + j;
      Index right_top = (j < n_slices) ? (left_top + 1) : (left_top + 1 - n_slices);
      Index left_bottom = left_top + n_slices;
      Index right_bottom = right_top + n_slices;
      indices.col(face_idx++) = Vec3Index {left_top, left_bottom, right_top};
      indices.col(face_idx++) = Vec3Index {left_bottom, right_bottom, right_top};
    }
  }
  return SimpleMesh<F32>{vertices, indices};
}

SimpleMesh<F32> sphere_20(Vec3f center, F32 radius) {
  SimpleMesh<F32>::VerticesType vertices(3, 12);
  SimpleMesh<F32>::FacesType indices(3, 20);
  F32 x = .525731112119133606;
  F32 z = .850650808352039932;
  vertices = AttrTrans<F32, 3>{{-x, 0.0, z}, {x, 0.0, z},  {-x, 0.0, -z}, {x, 0.0, -z},
                                  {0.0, z, x},  {0.0, z, -x}, {0.0, -z, x},  {0.0, -z, -x},
                                  {z, x, 0.0},  {-z, x, 0.0}, {z, -x, 0.0},  {-z, -x, 0.0}}
                 .transpose();
  vertices = vertices * radius;
  vertices.colwise() += center;
  indices = AttrTrans<Index, 3>{{1, 4, 0},  {4, 9, 0},  {4, 5, 9},  {8, 5, 4},  {1, 8, 4},
                                 {1, 10, 8}, {10, 3, 8}, {8, 3, 5},  {3, 2, 5},  {3, 7, 2},
                                 {3, 10, 7}, {10, 6, 7}, {6, 11, 7}, {6, 0, 11}, {6, 1, 0},
                                 {10, 1, 6}, {11, 0, 9}, {2, 11, 9}, {5, 2, 9},  {11, 2, 7}}
                .transpose();
  return {vertices, indices};
}
}  // namespace acg::geometry
