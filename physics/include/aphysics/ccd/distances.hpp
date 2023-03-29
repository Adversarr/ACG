#pragma once

#include <acore/math/common.hpp>

#include "distances/all.hpp"
namespace acg::physics::ccd {
template <typename Scalar> struct EdgeEdgeDistance;

template <typename Scalar> struct VertexFaceDistance;

template <> struct EdgeEdgeDistance<Float32> {
  Float32 Value(Vec3f e00, Vec3f e01, Vec3f e10, Vec3f e11) const noexcept {
    return edge_edge_value(e00.x(), e00.y(), e00.z(), e01.x(), e01.y(), e01.z(),
                           e10.x(), e10.y(), e10.z(), e11.x(), e11.y(), e11.z(),
                           d_hat_);
  }

  Vec<Float32, 12> Grad(Vec3f e00, Vec3f e01, Vec3f e10,
                        Vec3f e11) const noexcept {
    Vec<Float32, 12> grad;
    edge_edge_grad(e00.x(), e00.y(), e00.z(), e01.x(), e01.y(), e01.z(),
                   e10.x(), e10.y(), e10.z(), e11.x(), e11.y(), e11.z(), d_hat_,
                   grad.data());
    return grad;
  }

  Float32 d_hat_;
  explicit EdgeEdgeDistance(Float32 dh) : d_hat_(dh) {}
};

template <> struct EdgeEdgeDistance<Float64> {
  Float64 Value(Vec3d e00, Vec3d e01, Vec3d e10, Vec3d e11) const noexcept {
    return edge_edge_value(e00.x(), e00.y(), e00.z(), e01.x(), e01.y(), e01.z(),
                           e10.x(), e10.y(), e10.z(), e11.x(), e11.y(), e11.z(),
                           d_hat_);
  }

  Vec<Float64, 12> Grad(Vec3d e00, Vec3d e01, Vec3d e10,
                        Vec3d e11) const noexcept {
    Vec<Float64, 12> grad;
    edge_edge_grad(e00.x(), e00.y(), e00.z(), e01.x(), e01.y(), e01.z(),
                   e10.x(), e10.y(), e10.z(), e11.x(), e11.y(), e11.z(), d_hat_,
                   grad.data());
    return grad;
  }

  Float64 d_hat_;
  explicit EdgeEdgeDistance(Float64 dh) : d_hat_(dh) {}
};

template <> struct VertexFaceDistance<Float32> {
  Float32 Value(Vec3f t0, Vec3f t1, Vec3f t2, Vec3f v) const noexcept {
    return vertex_face_value(t0.x(), t0.y(), t0.z(), t1.x(), t1.y(), t1.z(),
                             t2.x(), t2.y(), t2.z(), v.x(), v.y(), v.z(),
                             d_hat_);
  }

  Vec<Float32, 12> Grad(Vec3f t0, Vec3f t1, Vec3f t2, Vec3f v) const noexcept {
    Vec<Float32, 12> grad;
    vertex_face_grad(t0.x(), t0.y(), t0.z(), t1.x(), t1.y(), t1.z(), t2.x(),
                     t2.y(), t2.z(), v.x(), v.y(), v.z(), d_hat_, grad.data());
    return grad;
  }

  Float32 d_hat_;

  explicit VertexFaceDistance(Float32 dh) : d_hat_{dh} {}
};

template <> struct VertexFaceDistance<Float64> {
  Float64 Value(Vec3d t0, Vec3d t1, Vec3d t2, Vec3d v) const noexcept {
    return vertex_face_value(t0.x(), t0.y(), t0.z(), t1.x(), t1.y(), t1.z(),
                             t2.x(), t2.y(), t2.z(), v.x(), v.y(), v.z(),
                             d_hat_);
  }

  Vec<Float64, 12> Grad(Vec3d t0, Vec3d t1, Vec3d t2, Vec3d v) const noexcept {
    Vec<Float64, 12> grad;
    vertex_face_grad(t0.x(), t0.y(), t0.z(), t1.x(), t1.y(), t1.z(), t2.x(),
                     t2.y(), t2.z(), v.x(), v.y(), v.z(), d_hat_, grad.data());
    return grad;
  }

  Float64 d_hat_;

  explicit VertexFaceDistance(Float64 dh) : d_hat_{dh} {}
};
}  // namespace acg::physics::ccd
