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
  Mesh<F32>:: StateType vertices(3, 12);
  Mesh<F32>::FaceListType indices(3, 20);
  float t = 0.5 * (1 + sqrt(5.0));
  int i = 0;
  vertices.col(i++) = Vec3f(-1, t, 0);
  vertices.col(i++) = Vec3f(1, t, 0);
  vertices.col(i++) = Vec3f(-1, -t, 0);
  vertices.col(i++) = Vec3f(1, -t, 0);
  vertices.col(i++) = Vec3f(0, -1, t);
  vertices.col(i++) = Vec3f(0, 1, t);
  vertices.col(i++) = Vec3f(0, -1, -t);
  vertices.col(i++) = Vec3f(0, 1, -t);
  vertices.col(i++) = Vec3f(t, 0, -1);
  vertices.col(i++) = Vec3f(t, 0, 1);
  vertices.col(i++) = Vec3f(-t, 0, -1);
  vertices.col(i++) = Vec3f(-t, 0, 1);
  vertices *= radius / sqrt(1.0 + t * t);
  vertices.colwise() += center;
  int ni = 0;
  indices.col(ni++) = Vec3Idx(0, 11, 5);
  indices.col(ni++) = Vec3Idx(0, 5, 1);
  indices.col(ni++) = Vec3Idx(0, 1, 7);
  indices.col(ni++) = Vec3Idx(0, 7, 10);
  indices.col(ni++) = Vec3Idx(1, 5, 9);
  indices.col(ni++) = Vec3Idx(5, 11, 4);
  indices.col(ni++) = Vec3Idx(11, 10, 2);
  indices.col(ni++) = Vec3Idx(10, 7, 6);
  indices.col(ni++) = Vec3Idx(7, 1, 8);
  indices.col(ni++) = Vec3Idx(3, 9, 4);
  indices.col(ni++) = Vec3Idx(3, 4, 2);
  indices.col(ni++) = Vec3Idx(3, 2, 6);
  indices.col(ni++) = Vec3Idx(3, 6, 8);
  indices.col(ni++) = Vec3Idx(3, 8, 9);
  indices.col(ni++) = Vec3Idx(4, 9, 5);
  indices.col(ni++) = Vec3Idx(2, 4, 11);
  indices.col(ni++) = Vec3Idx(0, 1, 7);
  indices.col(ni++) = Vec3Idx(8, 6, 7);
  indices.col(ni++) = Vec3Idx(9, 8, 1);
  return {vertices, indices};
}
}
