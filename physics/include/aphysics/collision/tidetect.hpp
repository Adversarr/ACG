#pragma once

#include <acore/math/common.hpp>
#include <tight_inclusion/ccd.hpp>

namespace acg::physics::ccd {

template <typename Scalar> struct VertexTriangleTightInclusion {
  int max_iter_{1000};
  Scalar toi_{std::numeric_limits<Scalar>::max()};
  Scalar tolerance_{1e-6};
  Scalar out_tolerance_;
  bool valid_{false};
  Scalar minimum_seperation_{0.1};

  bool operator()(const Vec3<Scalar> &vertex_start,
                  const Vec3<Scalar> &face_vertex0_start,
                  const Vec3<Scalar> &face_vertex1_start,
                  const Vec3<Scalar> &face_vertex2_start,
                  const Vec3<Scalar> &vertex_end,
                  const Vec3<Scalar> &face_vertex0_end,
                  const Vec3<Scalar> &face_vertex1_end,
                  const Vec3<Scalar> &face_vertex2_end) noexcept {
    return ticcd::vertexFaceCCD(vertex_start, face_vertex0_start, face_vertex1_start,
                         face_vertex2_start, vertex_end, face_vertex0_end,
                         face_vertex1_end, face_vertex2_end,
                         Eigen::Array3d::Constant(-1), minimum_seperation_,
                         toi_, tolerance_, 1, max_iter_, out_tolerance_);
  }
};

} // namespace acg::physics::ccd