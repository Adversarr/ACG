/**
 * @file
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
namespace details {

template <typename Scalar>
Vec3<Scalar> interpolate(double t, Vec3<Scalar> a, Vec3<Scalar> b) {
  return t * a + (1 - t) * b;
}
template <typename Scalar>
inline bool test_2d(Vec3<Scalar> v00, Vec3<Scalar> v01, Vec3<Scalar> v10,
                    Vec3<Scalar> v11, Vec3<Scalar> v20, Vec3<Scalar> v21,
                    Scalar epslion, Scalar &toi) {
  auto x00 = v00.x();
  auto y00 = v00.y();
  auto z00 = v00.z();
  auto x01 = v01.x();
  auto y01 = v01.y();
  auto z01 = v01.z();
  auto x10 = v10.x();
  auto y10 = v10.y();
  auto z10 = v10.z();
  auto x11 = v11.x();
  auto y11 = v11.y();
  auto z11 = v11.z();
  auto x20 = v20.x();
  auto y20 = v20.y();
  auto z20 = v20.z();
  auto x21 = v21.x();
  auto y21 = v21.y();
  auto z21 = v21.z();

  auto c = -(x21 * y01) - x01 * y11 + x21 * y11 + x01 * y21 + x21 * z01 +
           y11 * z01 - y21 * z01 + x01 * z11 - x21 * z11 - y01 * z11 +
           y21 * z11 - x01 * z21 + y01 * z21 - y11 * z21 +
           x11 * (y01 - y21 - z01 + z21);
  auto b =
      (x10 * y01 - x20 * y01 - x01 * y10 - x00 * y11 + 2 * x01 * y11 +
       x20 * y11 + x01 * y20 + x00 * y21 - 2 * x01 * y21 - x10 * y21 +
       y11 * z00 - y21 * z00 - x10 * z01 + x20 * z01 + y10 * z01 -
       2 * y11 * z01 - y20 * z01 + 2 * y21 * z01 + x01 * z10 - y01 * z10 +
       y21 * z10 + x00 * z11 - 2 * x01 * z11 - x20 * z11 - y00 * z11 +
       2 * y01 * z11 + y20 * z11 - 2 * y21 * z11 +
       x21 * (-y00 + 2 * y01 + y10 - 2 * y11 + z00 - 2 * z01 - z10 + 2 * z11) -
       x01 * z20 + y01 * z20 - y11 * z20 +
       x11 * (y00 - 2 * y01 - y20 + 2 * y21 - z00 + 2 * z01 + z20 - 2 * z21) -
       x00 * z21 + 2 * x01 * z21 + x10 * z21 + y00 * z21 - 2 * y01 * z21 -
       y10 * z21 + 2 * y11 * z21);
  auto a =
      (-(x20 * y00) + x21 * y00 + x20 * y01 - x21 * y01 - x00 * y10 +
       x01 * y10 + x20 * y10 - x21 * y10 + x00 * y11 - x01 * y11 - x20 * y11 +
       x21 * y11 + x00 * y20 - x01 * y20 - x00 * y21 + x01 * y21 + x20 * z00 -
       x21 * z00 + y10 * z00 - y11 * z00 - y20 * z00 + y21 * z00 - x20 * z01 +
       x21 * z01 - y10 * z01 + y11 * z01 + y20 * z01 - y21 * z01 + x00 * z10 -
       x01 * z10 - x20 * z10 + x21 * z10 - y00 * z10 + y01 * z10 + y20 * z10 -
       y21 * z10 - x00 * z11 + x01 * z11 + x20 * z11 - x21 * z11 + y00 * z11 -
       y01 * z11 - y20 * z11 + y21 * z11 - x00 * z20 + x01 * z20 + y00 * z20 -
       y01 * z20 - y10 * z20 + y11 * z20 +
       x10 * (y00 - y01 - y20 + y21 - z00 + z01 + z20 - z21) + x00 * z21 -
       x01 * z21 - y00 * z21 + y01 * z21 + y10 * z21 - y11 * z21 +
       x11 * (-y00 + y01 + y20 - y21 + z00 - z01 - z20 + z21));

  // Want to solve a t2 + b t + c = 0 in [0, 1]
  auto eval_grad = [a, b, c](double value) { return 2 * a * value + b; };

  auto eval_value = [a, b, c](double value) {
    return a * value * value + b * value + c;
  };

  // Start from half:

  if (abs(a) + abs(b) < epslion) {
    // Constant.
    toi = 1 - abs(c);
    return abs(c) < epslion;
  }
  double x = 0;
  int i = 0;
  while (x <= 1 && x >= 0 && i < 10) {
    x -= eval_value(x) / eval_grad(x);
    i += 1;
  }

  double y = 1;
  i = 0;
  while (y <= 1 && y >= 0 && i < 10) {
    y -= eval_value(y) / eval_grad(y);
    i += 1;
  }
  if (x <= 1 && x >= 0 && abs(eval_value(x)) < epslion) {
    auto v0 = interpolate(x, v00, v01);
    auto v1 = interpolate(x, v10, v11);
    auto v2 = interpolate(x, v20, v21);
    if ((v0 - v1).dot(v0 - v2) < 0) {
      toi = 1 - x;
      return true;
    }
  }
  if (y <= 1 && y >= 0 && abs(eval_value(y)) < epslion) {
    auto v0 = interpolate(y, v00, v01);
    auto v1 = interpolate(y, v10, v11);
    auto v2 = interpolate(y, v20, v21);
    if ((v0 - v1).dot(v0 - v2) < 0) {
      toi = 1 - y;
      return true;
    }
  }

  return false;
}

template <typename Scalar>
inline void compute_value(Scalar k1, Scalar k2, Scalar k3, Scalar k4, Scalar t,
                          Scalar &value, Scalar &grad) {
  Scalar t1 = k4 * t;
  Scalar t2 = k3 + t1;
  Scalar t3 = k2 + t * t2;
  value = k1 + t * t3;
  grad = t3 + t * (t1 + t2);
}

template <typename Scalar>
inline bool inside_test(const Vec3<Scalar> &e1, const Vec3<Scalar> &e2,
                        const Vec3<Scalar> &v, Scalar tolerance) {
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

template <typename Scalar>
inline bool crossing_test(const Vec3<Scalar> &e01, const Vec3<Scalar> &e10,
                          const Vec3<Scalar> &e11, Scalar tolerance) {
  Vec3<Scalar> b = e10 - e01, c = e01 - e11;
  if (e10.cwiseAbs().sum() < tolerance || e11.cwiseAbs().sum() < tolerance ||
      b.cwiseAbs().sum() < tolerance || c.cwiseAbs().sum() < tolerance) {
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
} // namespace details
template <typename Scalar> struct VertexTriangle {
  int max_iter_{7};
  Scalar toi_;
  Scalar tolerance_{1e-5};
  bool valid_{false};

  bool operator()(const Vec3<Scalar> &vertex_start,
                  const Vec3<Scalar> &face_vertex0_start,
                  const Vec3<Scalar> &face_vertex1_start,
                  const Vec3<Scalar> &face_vertex2_start,
                  const Vec3<Scalar> &vertex_end,
                  const Vec3<Scalar> &face_vertex0_end,
                  const Vec3<Scalar> &face_vertex1_end,
                  const Vec3<Scalar> &face_vertex2_end) noexcept {
    Vec3<Scalar> x10 = face_vertex1_start - face_vertex0_start;
    Vec3<Scalar> x20 = face_vertex2_start - face_vertex0_start;
    Vec3<Scalar> v_relative_start = vertex_start - face_vertex0_start;
    Vec3<Scalar> y10 = face_vertex1_end - face_vertex0_end;
    Vec3<Scalar> y20 = face_vertex2_end - face_vertex0_end;
    Vec3<Scalar> v_relative_end = vertex_end - face_vertex0_end;

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

    Scalar k1 = a22 * b23 * c21 - a21 * b23 * c22 +
                a23 * (-(b22 * c21) + b21 * c22) - a22 * b21 * c23 +
                a21 * b22 * c23;
    Scalar k2 =
        a22 * b23 * c11 +
        b23 * (-(a21 * c12) + a12 * c21 - a11 * c22 + 3 * a21 * c22) +
        a23 * (-(b12 * c21) + b22 * (-c11 + 3 * c21) + b21 * (c12 - 3 * c22) +
               b11 * c22) +
        b22 * (-(a13 * c21) + a21 * (c13 - 3 * c23) + a11 * c23) +
        b21 * (a13 * c22 - a12 * c23) + a21 * (-(b13 * c22) + b12 * c23) +
        a22 * (b13 * c21 - 3 * b23 * c21 - b11 * c23 + b21 * (-c13 + 3 * c23));

    Scalar k3 =
        ((-a13) * b22 + a22 * (b13 - 2 * b23) + a12 * b23) * c11 +
        b23 * ((-a11 + 2 * a21) * c12 + 2 * a11 * c22 - 3 * a21 * c22) +
        b13 * ((-a21) * c12 + a12 * c21 - a11 * c22 + 2 * a21 * c22) +
        a13 * (b21 * c12 + 2 * b22 * c21 + b11 * c22 - 2 * b21 * c22) +
        a23 * (2 * b22 * c11 - 2 * b21 * c12 - 3 * b22 * c21 +
               b12 * (-c11 + 2 * c21) + b11 * (c12 - 2 * c22) + 3 * b21 * c22) +
        b12 * ((-a13) * c21 + a21 * (c13 - 2 * c23) + a11 * c23) +
        a22 * (-2 * b13 * c21 + 3 * b23 * c21 + b21 * (2 * c13 - 3 * c23) +
               b11 * (-c13 + 2 * c23)) +
        a12 * (-2 * b23 * c21 - b11 * c23 + b21 * (-c13 + 2 * c23)) +
        b22 * (a11 * (c13 - 2 * c23) + a21 * (-2 * c13 + 3 * c23));
    Scalar k4 =
        (-(a22 * b13) - a23 * b22 + a12 * (b13 - b23) + a22 * b23) * c11 +
        b13 * ((-a11 + a21) * c12 + a22 * c21 + a11 * c22 - a21 * c22) +
        b23 * ((a11 - a21) * c12 - a11 * c22 + a21 * c22) +
        a13 * (b22 * c11 - b21 * c12 - b22 * c21 + b12 * (-c11 + c21) +
               b11 * (c12 - c22) + b21 * c22) +
        a23 * (b21 * c12 + b22 * c21 - b21 * c22 + b11 * (-c12 + c22)) +
        b22 * (a21 * (c13 - c23) + a11 * (-c13 + c23)) +
        b12 * (a23 * c11 - a23 * c21 + a11 * (c13 - c23) + a21 * (-c13 + c23)) +
        a12 * (-(b13 * c21) + b23 * c21 + b21 * (c13 - c23) +
               b11 * (-c13 + c23)) +
        a22 * (-(b23 * c21) + b11 * (c13 - c23) + b21 * (-c13 + c23));

    Scalar t1 = 0;
    Scalar t2 = 1;
    Scalar t3 = -k2 / (3 * k3);
    Scalar value, grad;

    if (abs(k2) + abs(k3) + abs(k4) < tolerance_) {
      if (abs(k1) > tolerance_) {
        valid_ = false;
        return false;
      }
      bool collid = false;
      Scalar minimal_toi = std::numeric_limits<Scalar>::max();
      toi_ = minimal_toi;
      collid |= details::test_2d<Scalar>(
          vertex_start, vertex_end, face_vertex0_start, face_vertex0_end,
          face_vertex1_start, face_vertex1_end, tolerance_, minimal_toi);
      toi_ = std::min(toi_, minimal_toi);
      collid |= details::test_2d<Scalar>(
          vertex_start, vertex_end, face_vertex0_start, face_vertex0_end,
          face_vertex2_start, face_vertex2_end, tolerance_, minimal_toi);
      toi_ = std::min(toi_, minimal_toi);

      collid |= details::test_2d<Scalar>(
          vertex_start, vertex_end, face_vertex2_start, face_vertex2_end,
          face_vertex1_start, face_vertex1_end, tolerance_, minimal_toi);
      toi_ = std::min(toi_, minimal_toi);
      if (collid) {
        valid_ = true;
        return true;
      } else {
        valid_ = false;
        return false;
      }
    }
    for (int i = 0; i < max_iter_; ++i) {
      details::compute_value(k1, k2, k3, k4, t1, value, grad);
      t1 = t1 - value / grad;
      details::compute_value(k1, k2, k3, k4, t2, value, grad);
      t2 = t2 - value / grad;
      details::compute_value(k1, k2, k3, k4, t3, value, grad);
      t3 = t3 - value / grad;
      acg::utils::god::sort3(t1, t2, t3);
    }

    if (t1 >= 0 && t1 <= 1 &&
        details::inside_test<Scalar>(
            x10 * t1 + (1 - t1) * y10, x20 * t1 + y20 * (1 - t1),
            v_relative_start * t1 + v_relative_end * (1 - t1), tolerance_)) {
      toi_ = 1 - t1;
      valid_ = true;
      return true;
    }

    if (0 <= t2 && t2 <= 1 &&
        details::inside_test<Scalar>(
            x10 * t2 + (1 - t2) * y10, x20 * t2 + y20 * (1 - t2),
            v_relative_start * t2 + v_relative_end * (1 - t2), tolerance_)) {
      toi_ = 1 - t2;
      valid_ = true;
      return true;
    }

    if (0 <= t3 && t3 <= 1 &&
        details::inside_test<Scalar>(
            x10 * t3 + (1 - t3) * y10, x20 * t3 + y20 * (1 - t3),
            v_relative_start * t3 + v_relative_end * (1 - t3), tolerance_)) {
      toi_ = 1 - t3;
      valid_ = true;
      return true;
    }

    valid_ = false;
    return false;
  }
};
template <typename Scalar> struct EdgeEdge {
  int max_iter_{7};
  Scalar toi_;
  bool valid_{false};
  bool operator()(const Vec3<Scalar> &edge0_vertex0_start,
                  const Vec3<Scalar> &edge0_vertex1_start,
                  const Vec3<Scalar> &edge1_vertex0_start,
                  const Vec3<Scalar> &edge1_vertex1_start,
                  const Vec3<Scalar> &edge0_vertex0_end,
                  const Vec3<Scalar> &edge0_vertex1_end,
                  const Vec3<Scalar> &edge1_vertex0_end,
                  const Vec3<Scalar> &edge1_vertex1_end,
                  Scalar tolerance) noexcept {
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

    Scalar k1 = a22 * b23 * c21 - a21 * b23 * c22 +
                a23 * (-(b22 * c21) + b21 * c22) - a22 * b21 * c23 +
                a21 * b22 * c23;
    Scalar k2 =
        a22 * b23 * c11 +
        b23 * (-(a21 * c12) + a12 * c21 - a11 * c22 + 3 * a21 * c22) +
        a23 * (-(b12 * c21) + b22 * (-c11 + 3 * c21) + b21 * (c12 - 3 * c22) +
               b11 * c22) +
        b22 * (-(a13 * c21) + a21 * (c13 - 3 * c23) + a11 * c23) +
        b21 * (a13 * c22 - a12 * c23) + a21 * (-(b13 * c22) + b12 * c23) +
        a22 * (b13 * c21 - 3 * b23 * c21 - b11 * c23 + b21 * (-c13 + 3 * c23));
    Scalar k3 =
        ((-a13) * b22 + a22 * (b13 - 2 * b23) + a12 * b23) * c11 +
        b23 * ((-a11 + 2 * a21) * c12 + 2 * a11 * c22 - 3 * a21 * c22) +
        b13 * ((-a21) * c12 + a12 * c21 - a11 * c22 + 2 * a21 * c22) +
        a13 * (b21 * c12 + 2 * b22 * c21 + b11 * c22 - 2 * b21 * c22) +
        a23 * (2 * b22 * c11 - 2 * b21 * c12 - 3 * b22 * c21 +
               b12 * (-c11 + 2 * c21) + b11 * (c12 - 2 * c22) + 3 * b21 * c22) +
        b12 * ((-a13) * c21 + a21 * (c13 - 2 * c23) + a11 * c23) +
        a22 * (-2 * b13 * c21 + 3 * b23 * c21 + b21 * (2 * c13 - 3 * c23) +
               b11 * (-c13 + 2 * c23)) +
        a12 * (-2 * b23 * c21 - b11 * c23 + b21 * (-c13 + 2 * c23)) +
        b22 * (a11 * (c13 - 2 * c23) + a21 * (-2 * c13 + 3 * c23));
    Scalar k4 =
        (-(a22 * b13) - a23 * b22 + a12 * (b13 - b23) + a22 * b23) * c11 +
        b13 * ((-a11 + a21) * c12 + a22 * c21 + a11 * c22 - a21 * c22) +
        b23 * ((a11 - a21) * c12 - a11 * c22 + a21 * c22) +
        a13 * (b22 * c11 - b21 * c12 - b22 * c21 + b12 * (-c11 + c21) +
               b11 * (c12 - c22) + b21 * c22) +
        a23 * (b21 * c12 + b22 * c21 - b21 * c22 + b11 * (-c12 + c22)) +
        b22 * (a21 * (c13 - c23) + a11 * (-c13 + c23)) +
        b12 * (a23 * c11 - a23 * c21 + a11 * (c13 - c23) + a21 * (-c13 + c23)) +
        a12 * (-(b13 * c21) + b23 * c21 + b21 * (c13 - c23) +
               b11 * (-c13 + c23)) +
        a22 * (-(b23 * c21) + b11 * (c13 - c23) + b21 * (-c13 + c23));

    Scalar t1 = 0;
    Scalar t2 = 1;
    Scalar t3 = -k2 / (3 * k3);
    Scalar value, grad;

    if (abs(k2) + abs(k3) + abs(k4) < tolerance) {
      if (abs(k1) > tolerance) {
        valid_ = false;
        return false;
      }
      bool collid = false;
      collid |=
          test_2d(edge0_vertex0_start, edge0_vertex0_end, edge1_vertex0_start,
                  edge1_vertex1_end, edge1_vertex1_start, edge1_vertex1_end,
                  tolerance, toi_);
      collid |=
          test_2d(edge0_vertex1_start, edge0_vertex1_end, edge1_vertex0_start,
                  edge1_vertex1_end, edge1_vertex1_start, edge1_vertex1_end,
                  tolerance, toi_);
      collid |=
          test_2d(edge1_vertex0_start, edge1_vertex0_end, edge0_vertex0_start,
                  edge0_vertex1_end, edge0_vertex1_start, edge0_vertex1_end,
                  tolerance, toi_);
      collid |=
          test_2d(edge1_vertex1_start, edge1_vertex1_end, edge0_vertex0_start,
                  edge0_vertex1_end, edge0_vertex1_start, edge0_vertex1_end,
                  tolerance, toi_);
      if (collid) {
        valid_ = true;
        return true;
      } else {
        valid_ = false;
        return false;
      }
    }

    for (int i = 0; i < max_iter_; ++i) {
      details::compute_value(k1, k2, k3, k4, t1, value, grad);
      t1 = t1 - value / grad;
      details::compute_value(k1, k2, k3, k4, t2, value, grad);
      t2 = t2 - value / grad;
      details::compute_value(k1, k2, k3, k4, t3, value, grad);
      t3 = t3 - value / grad;
      acg::utils::god::sort3(t1, t2, t3);
    }
    if (t1 >= 0.0 && t1 <= 1.0 &&
        details::crossing_test<Scalar>(e0_v1rs * t1 + e0_v1re * (1 - t1),
                                       e1_v0rs * t1 + e1_v0re * (1 - t1),
                                       e1_v1rs * t1 + e1_v1re * (1 - t1),
                                       tolerance)) {
      toi_ = 1 - t1;
      valid_ = true;
      return true;
    }

    if (0.0 <= t2 && t2 <= 1.0 &&
        details::crossing_test<Scalar>(e0_v1rs * t2 + e0_v1re * (1 - t2),
                                       e1_v0rs * t2 + e1_v0re * (1 - t2),
                                       e1_v1rs * t2 + e1_v1re * (1 - t2),
                                       tolerance)) {
      toi_ = 1 - t2;
      valid_ = true;
      return true;
    }
    if (0.0 <= t3 && t3 <= 1.0 &&
        details::crossing_test<Scalar>(e0_v1rs * t3 + e0_v1re * (1 - t3),
                                       e1_v0rs * t3 + e1_v0re * (1 - t3),
                                       e1_v1rs * t3 + e1_v1re * (1 - t3),
                                       tolerance)) {
      toi_ = 1 - t3;
      valid_ = true;
      return true;
    }

    valid_ = false;
    return false;
  }
};
} // namespace acg::physics::ccd
