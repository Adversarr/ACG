/**
 * @file
 * @brief Here is a small story of this algorithm:
 *  NOTE: The story of Fast CCD
 *  Originally, this algorithm is proposed on my Gitee, and is an exprimental one.
 *  After join GCL at USTC, we have a seminar for every to propose their own ideas,
 *  and I proposed this algorithm. However, we found that the paper "Penetration-free 
 *  Projective Dynamics on the GPU" stared the algorithm as a small contribution.
 *  I'm surprised that, the mathematics behind the algorithm is SOO SIMPLE, and the
 *  algorithm is SOO EASY to implement, but people just spend a lot of time on this
 *  simple problem, and bunches of papers were talking about bunches of methods.
 */




#pragma once
#include <acore/math/common.hpp>
#include <autils/god/utilities.hpp>

namespace acg::physics::ccd {
namespace details {
template <typename Scalar> inline void compute_value(Scalar k1, Scalar k2, Scalar k3, Scalar k4,
                                                     Scalar t, Scalar& value, Scalar& grad) {
  Scalar t1 = k4 * t;
  Scalar t2 = k3 + t1;
  Scalar t3 = k2 + t * t2;
  value = k1 + t * t3;
  grad = t3 + t * (t1 + t2);
}

template <typename Scalar> inline bool inside_test(const Vec3<Scalar>& e1, const Vec3<Scalar>& e2,
                                                   const Vec3<Scalar>& v, Scalar tolerance) {
  Vec3<Scalar> v1 = e1.cross(v);
  Vec3<Scalar> v2 = v.cross(e2);
  Vec3<Scalar> v3 = (e1 - v).cross(e2 - v);
  if (v1.dot(v2) <= -tolerance || v2.dot(v3) <= -tolerance) {
    return false;
  }
  double s1 = v1.norm();
  double s2 = v2.norm();
  double s3 = v3.norm();
  double s = e1.cross(e2).norm();
  return (std::fabs(s - s1 - s2 - s3) <= tolerance);
}

template <typename Scalar> inline bool crossing_test(const Vec3<Scalar>& e01,
                                                     const Vec3<Scalar>& e10,
                                                     const Vec3<Scalar>& e11, Scalar tolerance) {
  Vec3<Scalar> b = e10 - e01, c = e01 - e11;
  if (e10.cwiseAbs().sum() < tolerance || e11.cwiseAbs().sum() < tolerance
      || b.cwiseAbs().sum() < tolerance || c.cwiseAbs().sum() < tolerance) {
    return true;
  }
  Vec3<Scalar> e0110 = e01.cross(e10);
  Vec3<Scalar> e1101 = e11.cross(e01);
  Vec3<Scalar> e1110 = e11.cross(e10);
  Vec3<Scalar> bc = c.cross(b);
  if (e0110.dot(e1101) <= -tolerance) {
    return false;
  } else if (e1110.dot(bc) <= -tolerance) {
    return false;
  }
  auto s1 = e0110.norm();
  auto s2 = e1101.norm();
  auto s3 = e1110.norm();
  auto s4 = bc.norm();
  return std::fabs(s1 + s2 - (s3 + s4)) <= tolerance;
}
}  // namespace details
template <typename Scalar> struct VertexTriangle {
  int max_iter;
  Scalar toi;
  bool valid;
  bool operator()(const Vec3<Scalar>& vertex_start, const Vec3<Scalar>& face_vertex0_start,
                  const Vec3<Scalar>& face_vertex1_start, const Vec3<Scalar>& face_vertex2_start,
                  const Vec3<Scalar>& vertex_end, const Vec3<Scalar>& face_vertex0_end,
                  const Vec3<Scalar>& face_vertex1_end, const Vec3<Scalar>& face_vertex2_end,
                  const Scalar tolerance) const noexcept {
    Vec3<Scalar> x10 = face_vertex1_start - face_vertex0_start;
    Vec3<Scalar> x20 = face_vertex2_start - face_vertex0_start;
    Vec3<Scalar> v_relative_start = vertex_start - face_vertex0_start;
    Vec3<Scalar> v_relative_end = vertex_end - face_vertex0_end;
    Vec3<Scalar> y10 = face_vertex1_end - face_vertex0_end;
    Vec3<Scalar> y20 = face_vertex2_end - face_vertex0_end;

    auto a11 = x10.x();
    auto a12 = x10.y();
    auto a13 = x10.z();
    auto b11 = x20.x();
    auto b12 = x20.y();
    auto b13 = x20.z();
    auto c11 = v_relative_start.x();
    auto c12 = v_relative_start.y();
    auto c13 = v_relative_start.z();
    auto a21 = y10.x();
    auto a22 = y10.y();
    auto a23 = y10.z();
    auto b21 = y20.x();
    auto b22 = y20.y();
    auto b23 = y20.z();
    auto c21 = v_relative_end.x();
    auto c22 = v_relative_end.y();
    auto c23 = v_relative_end.z();

    Scalar k1 = a22 * b23 * c21 - a21 * b23 * c22 + a23 * (-(b22 * c21) + b21 * c22)
                - a22 * b21 * c23 + a21 * b22 * c23;
    Scalar k2 = a22 * b23 * c11 + b23 * (-(a21 * c12) + a12 * c21 - a11 * c22 + 3 * a21 * c22)
                + a23 * (-(b12 * c21) + b22 * (-c11 + 3 * c21) + b21 * (c12 - 3 * c22) + b11 * c22)
                + b22 * (-(a13 * c21) + a21 * (c13 - 3 * c23) + a11 * c23)
                + b21 * (a13 * c22 - a12 * c23) + a21 * (-(b13 * c22) + b12 * c23)
                + a22 * (b13 * c21 - 3 * b23 * c21 - b11 * c23 + b21 * (-c13 + 3 * c23));

    Scalar k3 = ((-a13) * b22 + a22 * (b13 - 2 * b23) + a12 * b23) * c11
                + b23 * ((-a11 + 2 * a21) * c12 + 2 * a11 * c22 - 3 * a21 * c22)
                + b13 * ((-a21) * c12 + a12 * c21 - a11 * c22 + 2 * a21 * c22)
                + a13 * (b21 * c12 + 2 * b22 * c21 + b11 * c22 - 2 * b21 * c22)
                + a23
                      * (2 * b22 * c11 - 2 * b21 * c12 - 3 * b22 * c21 + b12 * (-c11 + 2 * c21)
                         + b11 * (c12 - 2 * c22) + 3 * b21 * c22)
                + b12 * ((-a13) * c21 + a21 * (c13 - 2 * c23) + a11 * c23)
                + a22
                      * (-2 * b13 * c21 + 3 * b23 * c21 + b21 * (2 * c13 - 3 * c23)
                         + b11 * (-c13 + 2 * c23))
                + a12 * (-2 * b23 * c21 - b11 * c23 + b21 * (-c13 + 2 * c23))
                + b22 * (a11 * (c13 - 2 * c23) + a21 * (-2 * c13 + 3 * c23));
    Scalar k4 = (-(a22 * b13) - a23 * b22 + a12 * (b13 - b23) + a22 * b23) * c11
                + b13 * ((-a11 + a21) * c12 + a22 * c21 + a11 * c22 - a21 * c22)
                + b23 * ((a11 - a21) * c12 - a11 * c22 + a21 * c22)
                + a13
                      * (b22 * c11 - b21 * c12 - b22 * c21 + b12 * (-c11 + c21) + b11 * (c12 - c22)
                         + b21 * c22)
                + a23 * (b21 * c12 + b22 * c21 - b21 * c22 + b11 * (-c12 + c22))
                + b22 * (a21 * (c13 - c23) + a11 * (-c13 + c23))
                + b12 * (a23 * c11 - a23 * c21 + a11 * (c13 - c23) + a21 * (-c13 + c23))
                + a12 * (-(b13 * c21) + b23 * c21 + b21 * (c13 - c23) + b11 * (-c13 + c23))
                + a22 * (-(b23 * c21) + b11 * (c13 - c23) + b21 * (-c13 + c23));

    Scalar t1 = 0;
    Scalar t2 = 1;
    Scalar t3 = -k2 / (3 * k3);
    Scalar value, grad;

    for (int i = 0; i < max_iter; ++i) {
      details::compute_value(k1, k2, k3, k4, t1, value, grad);
      t1 = t1 - value / grad;
      details::compute_value(k1, k2, k3, k4, t2, value, grad);
      t2 = t2 - value / grad;
      details::compute_value(k1, k2, k3, k4, t3, value, grad);
      t3 = t3 - value / grad;
      acg::utils::god::sort3(t1, t2, t3);
    }

    if (t1 >= 0 && t1 <= 1
        && details::inside_test(x10 * t1 + (1 - t1) * y10, x20 * t1 + y20 * (1 - t1),
                                v_relative_start * t1 + v_relative_end * (1 - t1), tolerance)) {
      toi = t1;
      valid = true;
      return true;
    }

    if (0 <= t2 && t2 <= 1
        && details::inside_test(x10 * t2 + (1 - t2) * y10, x20 * t2 + y20 * (1 - t2),
                                v_relative_start * t2 + v_relative_end * (1 - t2), tolerance)) {
      toi = t2;
      valid = true;
      return true;
    }

    if (0 <= t3 && t3 <= 1
        && details::inside_test(x10 * t3 + (1 - t3) * y10, x20 * t3 + y20 * (1 - t3),
                                v_relative_start * t3 + v_relative_end * (1 - t3), tolerance)) {
      toi = t3;
      valid = true;
      return true;
    }

    valid = false;
    return false;
  }
};
template <typename Scalar> struct EdgeEdge {
  int max_iter;

  Scalar toi;
  bool valid;
  bool operator()(const Vec3<Scalar>& edge0_vertex0_start, const Vec3<Scalar>& edge0_vertex1_start,
                  const Vec3<Scalar>& edge1_vertex0_start, const Vec3<Scalar>& edge1_vertex1_start,
                  const Vec3<Scalar>& edge0_vertex0_end, const Vec3<Scalar>& edge0_vertex1_end,
                  const Vec3<Scalar>& edge1_vertex0_end, const Vec3<Scalar>& edge1_vertex1_end,
                  Scalar tolerance) {
    Vec3<Scalar> e0_v1rs = edge0_vertex1_start - edge0_vertex0_start;
    Vec3<Scalar> e1_v0rs = edge1_vertex0_start - edge0_vertex0_start;
    Vec3<Scalar> e1_v1rs = edge1_vertex1_start - edge0_vertex0_start;
    Vec3<Scalar> e0_v1re = edge0_vertex1_end - edge0_vertex0_end;
    Vec3<Scalar> e1_v0re = edge1_vertex0_end - edge0_vertex0_end;
    Vec3<Scalar> e1_v1re = edge1_vertex1_end - edge0_vertex0_end;

    auto a11 = e1_v0rs.x();
    auto a12 = e1_v0rs.y();
    auto a13 = e1_v0rs.z();
    auto b11 = e1_v1rs.x();
    auto b12 = e1_v1rs.y();
    auto b13 = e1_v1rs.z();
    auto c11 = e0_v1rs.x();
    auto c12 = e0_v1rs.y();
    auto c13 = e0_v1rs.z();
    auto a21 = e1_v0re.x();
    auto a22 = e1_v0re.y();
    auto a23 = e1_v0re.z();
    auto b21 = e1_v1re.x();
    auto b22 = e1_v1re.y();
    auto b23 = e1_v1re.z();
    auto c21 = e0_v1re.x();
    auto c22 = e0_v1re.y();
    auto c23 = e0_v1re.z();

    Scalar k1 = a22 * b23 * c21 - a21 * b23 * c22 + a23 * (-(b22 * c21) + b21 * c22)
                - a22 * b21 * c23 + a21 * b22 * c23;
    Scalar k2 = a22 * b23 * c11 + b23 * (-(a21 * c12) + a12 * c21 - a11 * c22 + 3 * a21 * c22)
                + a23 * (-(b12 * c21) + b22 * (-c11 + 3 * c21) + b21 * (c12 - 3 * c22) + b11 * c22)
                + b22 * (-(a13 * c21) + a21 * (c13 - 3 * c23) + a11 * c23)
                + b21 * (a13 * c22 - a12 * c23) + a21 * (-(b13 * c22) + b12 * c23)
                + a22 * (b13 * c21 - 3 * b23 * c21 - b11 * c23 + b21 * (-c13 + 3 * c23));
    Scalar k3 = ((-a13) * b22 + a22 * (b13 - 2 * b23) + a12 * b23) * c11
                + b23 * ((-a11 + 2 * a21) * c12 + 2 * a11 * c22 - 3 * a21 * c22)
                + b13 * ((-a21) * c12 + a12 * c21 - a11 * c22 + 2 * a21 * c22)
                + a13 * (b21 * c12 + 2 * b22 * c21 + b11 * c22 - 2 * b21 * c22)
                + a23
                      * (2 * b22 * c11 - 2 * b21 * c12 - 3 * b22 * c21 + b12 * (-c11 + 2 * c21)
                         + b11 * (c12 - 2 * c22) + 3 * b21 * c22)
                + b12 * ((-a13) * c21 + a21 * (c13 - 2 * c23) + a11 * c23)
                + a22
                      * (-2 * b13 * c21 + 3 * b23 * c21 + b21 * (2 * c13 - 3 * c23)
                         + b11 * (-c13 + 2 * c23))
                + a12 * (-2 * b23 * c21 - b11 * c23 + b21 * (-c13 + 2 * c23))
                + b22 * (a11 * (c13 - 2 * c23) + a21 * (-2 * c13 + 3 * c23));
    Scalar k4 = (-(a22 * b13) - a23 * b22 + a12 * (b13 - b23) + a22 * b23) * c11
                + b13 * ((-a11 + a21) * c12 + a22 * c21 + a11 * c22 - a21 * c22)
                + b23 * ((a11 - a21) * c12 - a11 * c22 + a21 * c22)
                + a13
                      * (b22 * c11 - b21 * c12 - b22 * c21 + b12 * (-c11 + c21) + b11 * (c12 - c22)
                         + b21 * c22)
                + a23 * (b21 * c12 + b22 * c21 - b21 * c22 + b11 * (-c12 + c22))
                + b22 * (a21 * (c13 - c23) + a11 * (-c13 + c23))
                + b12 * (a23 * c11 - a23 * c21 + a11 * (c13 - c23) + a21 * (-c13 + c23))
                + a12 * (-(b13 * c21) + b23 * c21 + b21 * (c13 - c23) + b11 * (-c13 + c23))
                + a22 * (-(b23 * c21) + b11 * (c13 - c23) + b21 * (-c13 + c23));

    Scalar t1 = 0;
    Scalar t2 = 1;
    Scalar t3 = -k2 / (3 * k3);
    Scalar value, grad;

    for (int i = 0; i < max_iter; ++i) {
      details::compute_value(k1, k2, k3, k4, t1, value, grad);
      t1 = t1 - value / grad;
      details::compute_value(k1, k2, k3, k4, t2, value, grad);
      t2 = t2 - value / grad;
      details::compute_value(k1, k2, k3, k4, t3, value, grad);
      t3 = t3 - value / grad;
      acg::utils::god::sort3(t1, t2, t3);
    }
    if (t1 >= 0.0 && t1 <= 1.0
        && details::crossing_test(e0_v1rs * t1 + e0_v1re * (1 - t1),
                                  e1_v0rs * t1 + e1_v0re * (1 - t1),
                                  e1_v1rs * t1 + e1_v1re * (1 - t1), tolerance)) {
      toi = t1;
      valid = true;
      return true;
    }

    if (0.0 <= t2 && t2 <= 1.0
        && details::crossing_test(e0_v1rs * t2 + e0_v1re * (1 - t2),
                                  e1_v0rs * t2 + e1_v0re * (1 - t2),
                                  e1_v1rs * t2 + e1_v1re * (1 - t2), tolerance)) {
      toi = t2;
      valid = true;
      return true;
    }
    if (0.0 <= t3 && t3 <= 1.0
        && details::crossing_test(e0_v1rs * t3 + e0_v1re * (1 - t3),
                                  e1_v0rs * t3 + e1_v0re * (1 - t3),
                                  e1_v1rs * t3 + e1_v1re * (1 - t3), tolerance)) {
      toi = t3;
      valid = true;
      return true;
    }

    valid = false;
    return false;
  }
};
}  // namespace acg::physics::ccd
