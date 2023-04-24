//
// Created by Yang Jerry on 2023/3/31.
//
#pragma once
#ifndef ACG_DISTANCE_HPP
#define ACG_DISTANCE_HPP

#include <Eigen/Geometry>
#include <acore/math/common.hpp>

#include "distances/eed_value.hpp"
#include "distances/vfd_value.hpp"

namespace acg::physics::collision {

/**
 * @brief Vertex-Plane Distance. (only normal direction is computed)
 *
 * @tparam Scalar
 */
template <typename Scalar = acg::Float> struct EdgeEdgeNormalDistance {
  using P = Vec3<Scalar>;
  P e0a_;
  P e0b_;
  P e1a_;
  P e1b_;

  constexpr EdgeEdgeNormalDistance(P e0a, P e0b, P e1a, P e1b) noexcept
      : e0a_(e0a), e0b_(e0b), e1a_(e1a), e1b_(e1b) {}

  inline Scalar RawValue() const noexcept {
    Scalar val = details::eed_value(e0a_.x(), e0a_.y(), e0a_.z(), e0b_.x(),
                                    e0b_.y(), e0b_.z(), e1a_.x(), e1a_.y(),
                                    e1a_.z(), e1b_.x(), e1b_.y(), e1b_.z());

    return val;
  }

  inline Scalar Value() const noexcept { return abs(RawValue()); }

  /**
   * @brief Return the gradient, Ordered by: f0, f1, f2, v.
   *
   * @return Vec<Scalar, 12>
   */
  inline Vec<Scalar, 12> Grad() const noexcept {
    auto val = RawValue();
    Vec<Scalar, 12> g;
    details::eed_grad(e0a_.x(), e0a_.y(), e0a_.z(), e0b_.x(), e0b_.y(),
                      e0b_.z(), e1a_.x(), e1a_.y(), e1a_.z(), e1b_.x(),
                      e1b_.y(), e1b_.z(), g.data());
    if (val < 0) {
      g = -g;
    }
    return g;
  }
};

/**
 * @brief Vertex-Plane Distance. (only normal direction is computed)
 *
 * @tparam Scalar
 */
template <typename Scalar = acg::Float> struct VertexTriangleNormalDistance {
  using P = Vec3<Scalar>;
  P f0_;
  P f1_;
  P f2_;
  P v_;

  constexpr VertexTriangleNormalDistance(P f0, P f1, P f2, P v) noexcept
      : f0_(f0), f1_(f1), f2_(f2), v_(v) {}

  inline Scalar RawValue() const noexcept {
    Scalar val =
        details::vfd_value(f0_.x(), f0_.y(), f0_.z(), f1_.x(), f1_.y(), f1_.z(),
                           f2_.x(), f2_.y(), f2_.z(), v_.x(), v_.y(), v_.z());

    return val;
  }

  inline Scalar Value() const noexcept { return abs(RawValue()); }

  /**
   * @brief Return the gradient, Ordered by: f0, f1, f2, v.
   *
   * @return Vec<Scalar, 12>
   */
  inline Vec<Scalar, 12> Grad() const noexcept {
    auto val = RawValue();
    Vec<Scalar, 12> g;
    details::vfd_grad(f0_.x(), f0_.y(), f0_.z(), f1_.x(), f1_.y(), f1_.z(), f2_.x(),
             f2_.y(), f2_.z(), v_.x(), v_.y(), v_.z(), g.data());
    if (val < 0) {
      g = -g;
    }
    return g;
  }
};
} // namespace acg::physics::collision

#endif // ACG_DISTANCE_HPP
