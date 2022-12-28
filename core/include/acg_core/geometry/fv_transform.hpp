#pragma once

#include <iostream>

#include "common.hpp"

namespace acg::geometry {

struct FVTransform {
  const topology::TriangleList& triangle_list_;
  const Idx n_vertices_;

  inline FVTransform(const topology::TriangleList& triangle_list, Idx n_vertices)
      : triangle_list_(triangle_list), n_vertices_(n_vertices) {}

  template <typename Scalar0, int n_attr>
  Field<Scalar0, n_attr> FaceToVertex(const Field<Scalar0, n_attr>& x,
                                      const attr::ScalarList<Scalar0>& weight) const;
};

template <typename Scalar0, int n_attr>
Field<Scalar0, n_attr> FVTransform::FaceToVertex(const Field<Scalar0, n_attr>& x,
                                                 const attr::ScalarList<Scalar0>& weight) const {
  Field<Scalar0, n_attr> retval(n_attr, n_vertices_);
  attr::ScalarList<Scalar0> per_vertex_weight(1, n_vertices_);
  retval.setZero();
  per_vertex_weight.setZero();
  Idx idx = 0;
  for (const auto& triangle : triangle_list_.colwise()) {
    per_vertex_weight(triangle.x()) += weight(idx);
    per_vertex_weight(triangle.y()) += weight(idx);
    per_vertex_weight(triangle.z()) += weight(idx);
    retval.col(triangle.x()) += weight(idx) * x.col(idx).template cast<Scalar0>();
    retval.col(triangle.y()) += weight(idx) * x.col(idx).template cast<Scalar0>();
    retval.col(triangle.z()) += weight(idx) * x.col(idx).template cast<Scalar0>();
    idx += 1;
  }
  return (retval.array().rowwise() / per_vertex_weight.array()).matrix();
}

}  // namespace acg::geometry
