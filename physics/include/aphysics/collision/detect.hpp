/**
 * @file detect.hpp
 * @brief Here is a small story of this algorithm:
 *  NOTE: The story of Fast CCD
 *   Originally, this algorithm is proposed on my Gitee, and is an exprimental
 *  one. After join GCL at USTC, we have a seminar for every to propose their
 * own ideas, and I proposed this algorithm. However, we found that the paper
 *  "Penetration-free Projective Dynamics on the GPU" stared the algorithm as a
 *  small contribution. I'm surprised that, the mathematics behind the algorithm
 *  is SOO SIMPLE, and the algorithm is SOO EASY to implement, but people just
 *  spend a lot of time on this simple problem, and bunches of papers were
 *  talking about bunches of methods.
 */

#pragma once

#include <acore/math/common.hpp>
#include <autils/god/utilities.hpp>

namespace acg::physics::ccd {

template <typename Scalar> struct VertexTriangleDynamic {
  int max_iter_{10};
  Scalar toi_{std::numeric_limits<Scalar>::max()};
  Scalar tolerance_{1e-5};
  Scalar min_distance_{1e-3};
  bool valid_{false};
  bool operator()(const Vec3<Scalar> &vertex_start,
                  const Vec3<Scalar> &face_vertex0_start,
                  const Vec3<Scalar> &face_vertex1_start,
                  const Vec3<Scalar> &face_vertex2_start,
                  const Vec3<Scalar> &vertex_end,
                  const Vec3<Scalar> &face_vertex0_end,
                  const Vec3<Scalar> &face_vertex1_end,
                  const Vec3<Scalar> &face_vertex2_end) noexcept;
};

template <typename Scalar> struct EdgeEdgeDynamic {
  int max_iter_{10};
  Scalar toi_;
  Scalar tolerance_{1e-6};
  Scalar min_distance_{1e-3};
  bool valid_{false};
  bool operator()(const Vec3<Scalar> &edge0_vertex0_start,
                  const Vec3<Scalar> &edge0_vertex1_start,
                  const Vec3<Scalar> &edge1_vertex0_start,
                  const Vec3<Scalar> &edge1_vertex1_start,
                  const Vec3<Scalar> &edge0_vertex0_end,
                  const Vec3<Scalar> &edge0_vertex1_end,
                  const Vec3<Scalar> &edge1_vertex0_end,
                  const Vec3<Scalar> &edge1_vertex1_end) noexcept;
};

template <typename Scalar> struct VertexVertexDynamic {
  int max_iter_{10};

  Scalar min_distance_{1e-3};
  bool valid_{false};
  Scalar toi_{std::numeric_limits<Scalar>::max()};
  Scalar tolerance_{1e-7};

  bool operator()(const Vec3<Scalar> &v0_start, const Vec3<Scalar> &v0_end,
                  const Vec3<Scalar> &v1_start,
                  const Vec3<Scalar> &v1_end) noexcept;
};


template <typename Scalar> struct VertexTriangleStatic {
  int max_iter_{10};

  Scalar min_distance_{1e-3};
  bool valid_{false};

  Scalar tolerance_{1e-7};

  bool operator()(const Vec3<Scalar> &f0, const Vec3<Scalar> &f1,
                  const Vec3<Scalar> &f2,
                  const Vec3<Scalar> &v) noexcept;
};
} // namespace acg::physics::ccd

#include "details/detect-simple-inl.hpp" // IWYU pragma: export
#include "details/detect-vtee-inl.hpp"   // IWYU pragma: export