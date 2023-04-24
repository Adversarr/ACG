#pragma once

#include "../detect.hpp"

namespace acg::physics::collision {

template <typename Scalar>
bool VertexVertexDynamic<Scalar>::operator()(const Vec3<Scalar> &v0_start,
                                          const Vec3<Scalar> &v0_end,
                                          const Vec3<Scalar> &v1_start,
                                          const Vec3<Scalar> &v1_end) noexcept {
  auto v0 = v0_start - v0_end;
  auto v1 = v1_start - v1_end;
  // x1 - x2 - y1 + y2
  Vec3<Scalar> x12y12 = v0 - v1;
  Vec3<Scalar> xy2 = v1_end - v0_end;
  Scalar coef_2 = x12y12.squaredNorm();
  Scalar coef_1 = x12y12.dot(xy2);
  Scalar coef_0 = xy2.squaredNorm();
  Scalar d0 = (v1_start - v0_start).norm();
  if (d0 < tolerance_ + min_distance_) {
    toi_ = 0;
    valid_ = true;
  }
  Scalar d1 = (v1_end - v0_end).norm();
  if (d1 < tolerance_ + min_distance_) {
    toi_ = std::min<Scalar>(toi_, 1);
    valid_ = true;
  }
  if (coef_2 > tolerance_) {
    Scalar b = coef_1 / coef_2;
    if (b > -tolerance_ && b < 1 + tolerance_) {
      Scalar d2 =
          ((v1_start - v0_start) * b + (v1_end - v0_end) * (1 - b)).norm();
      if (d2 < tolerance_ + min_distance_) {
        toi_ = std::min(1 - b, toi_);
        valid_ = true;
      }
    }
  }
  return valid_;
}


template <typename Scalar>
bool VertexTriangleStatic<Scalar>::operator()(const Vec3<Scalar> &f0,
                                          const Vec3<Scalar> &f1,
                                          const Vec3<Scalar> &f2,
                                          const Vec3<Scalar> &v) noexcept {
  Vec3<Scalar> f20 = f2 - f0;
  Vec3<Scalar> f10 = f1 - f0;
  Vec3<Scalar> fv = v - f0;
  // Test the normal:
  Vec3<Scalar> n = f20.cross(f10).normalized();
  if (std::abs(n.dot(fv)) >= min_distance_ + tolerance_) {
    valid_ = false;
    return false;
  }

  // Test internal condition
  Mat<Scalar, 3, 2> a;
  a << f10, f20;
  Mat2x2<Scalar> ata = a.transpose() * a;
  Vec2<Scalar> local_coordinate = ata.inverse() * a.transpose() * fv;
  if (local_coordinate.sum() > 1 + tolerance_) {
    valid_ = false;
    return false;
  }

  if (local_coordinate.x() <= -tolerance_) {
    valid_ = false;
    return false;
  }

  if (local_coordinate.y() <= -tolerance_) {
    valid_ = false;
    return false;
  }

  valid_ = true;
  return valid_;
}

} // namespace acg::physics::collision