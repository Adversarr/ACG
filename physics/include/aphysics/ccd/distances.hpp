#pragma once

#include <acore/math/common.hpp>

#include "distances/all.hpp"
namespace acg::physics::ccd {
template <typename Scalar> struct EdgeEdgeDistance;

template <> struct EdgeEdgeDistance<float> {
  float Value(Vec3f e00_start, Vec3f e00_end, Vec3f e01_start, Vec3f e01_end,
      Vec3f e10_start, Vec3f e10_end, Vec3f e11_start, Vec3f e11_end) const noexcept {
    return edge_edge_value(e00_start.x(), e00_start.y(), e00_start.z(), e01_end.x(), e01_start.y(), 
        e01_start.z(), e10_start.x(), e10_start.y(), e10_start.z(), e11_start.x(), e11_start.y(), 
        e11_start.z(), d_hat_);
  }

  float d_hat_;
  explicit EdgeEdgeDistance(float dh) : d_hat_(dh) {}
};
}  // namespace acg::physics::ccd
