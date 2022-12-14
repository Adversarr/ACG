#include "acg_core/geometry/common_models.hpp"
namespace acg::geometry {

Mesh<F32> sphere_uv(Vec3f center, F32 radius, Idx n_stacks, Idx n_slices) {
  auto n_faces = 2 * n_slices * n_stacks;
  auto n_vertices = 2 + n_slices * n_stacks;
  Mesh<F32>::StateType vertices(3, n_vertices);
  Mesh<F32>::FaceListType indices(3, n_faces);

  // 1. fill the vertices
  // north pole
  vertices.col(0) = center + Vec3f(0, 0, radius);
  // south pole
  vertices.col(n_vertices - 1) = center - Vec3f(0, 0, radius);
  // crossing points
  F32 d_theta = constants::pi<F32> / static_cast<F32>(n_stacks + 1);
  F32 d_phi = 2 * constants::pi<F32> / static_cast<F32>(n_slices);
  for (Idx i = 1; i <= n_stacks; ++i) {
    F32 theta = d_theta * i;
    for (Idx j = 0; j < n_slices; ++j) {
      F32 phi = d_phi * j;
      Idx idx = (i - 1) * n_slices + j + 1;
      Vec3f pos = {sin(theta) * cos(phi), sin(theta) * sin(phi), cos(theta)};
      vertices.col(idx) = pos * radius + center;
    }
  }

  // 2. fill the indices
  // north pole
  for (Idx i = 1; i <= n_slices; ++i) {
    Vec3Idx index_vector{0, i, i < n_slices ? i + 1 : 1};
    Idx idx = i - 1;
    indices.col(idx) = index_vector;
  }
  // south pole
  for (Idx i = 1; i <= n_slices; ++i) {
    auto base_idx = n_vertices - n_slices - 1;
    Vec3Idx index_vector{0, base_idx + i, base_idx + (i < n_slices ? i + 1 : 1)};
    Idx idx = n_faces - n_slices + i - 1;
    indices.col(idx) = index_vector;
  }
  Idx face_idx = n_slices;
  // crossing points:
  for (Idx i = 1; i < n_stacks; ++i) {
    for (Idx j = 1; j <= n_slices; ++j) {
      Idx left_top = (i - 1) * n_slices + j;
      Idx right_top = (j < n_slices) ? (left_top + 1) : (left_top + 1 - n_slices);
      Idx left_bottom = left_top + n_slices;
      Idx right_bottom = right_top + n_slices;
      indices.col(face_idx++) = Vec3Idx{left_top, left_bottom, right_top};
      indices.col(face_idx++) = Vec3Idx{left_bottom, right_bottom, right_top};
    }
  }
  return Mesh<F32>{vertices, indices};
}

Mesh<F32> sphere_20(Vec3f center, F32 radius) {
  Mesh<F32>::StateType vertices(3, 12);
  Mesh<F32>::FaceListType indices(3, 20);
  F32 x = .525731112119133606;
  F32 z = .850650808352039932;
  vertices = AttrVecTrans<F32, 3>{{-x, 0.0, z}, {x, 0.0, z},  {-x, 0.0, -z}, {x, 0.0, -z},
                                  {0.0, z, x},  {0.0, z, -x}, {0.0, -z, x},  {0.0, -z, -x},
                                  {z, x, 0.0},  {-z, x, 0.0}, {z, -x, 0.0},  {-z, -x, 0.0}}
                 .transpose();
  vertices = vertices * radius;
  vertices.colwise() += center;
  indices = AttrVecTrans<Idx, 3>{{1, 4, 0},  {4, 9, 0},  {4, 5, 9},  {8, 5, 4},  {1, 8, 4},
                                 {1, 10, 8}, {10, 3, 8}, {8, 3, 5},  {3, 2, 5},  {3, 7, 2},
                                 {3, 10, 7}, {10, 6, 7}, {6, 11, 7}, {6, 0, 11}, {6, 1, 0},
                                 {10, 1, 6}, {11, 0, 9}, {2, 11, 9}, {5, 2, 9},  {11, 2, 7}}
                .transpose();
  return {vertices, indices};
}
}  // namespace acg::geometry
