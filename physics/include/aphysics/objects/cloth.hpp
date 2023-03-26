#pragma once
#include <acore/geometry/common.hpp>
#include <acore/math/common.hpp>

#include "common.hpp"

namespace acg::physics {

template <typename Scalar, int dim> struct ClothData {
  // For each vertex:
  Field<Scalar, dim> position_;
  Field<Scalar, dim> velocity_;
  Field<Scalar> mass_;
  // For constraints:
  Field<Index, 2> constraints_;
  Field<Scalar> original_length_;
  Field<Index, 3> face_;
  Field<Scalar> stiffness_;
  void InitAuxiliary() {
    velocity_.resizeLike(position_);
    original_length_.resizeLike(stiffness_);
    auto pacc = access(position_);
    for (auto [i, v] : enumerate(access(constraints_))) {
      original_length_(i) = (pacc(v.x()) - pacc(v.y())).norm();
    }
  }

  ClothData() = default;

  ClothData(Field<Scalar, dim> position, Field<Scalar> mass, Field<Index, 2> constraints,
      Field<Index, 3> face,
            Field<Scalar> stiffness)
      : position_(std::move(position)),
        mass_(std::move(mass)),
        constraints_(std::move(constraints)),
        face_(std::move(face)),
        stiffness_(std::move(stiffness)) {
    velocity_.resizeLike(position_);
    velocity_.setZero();
    InitAuxiliary();
  }
};

}  // namespace acg::physics
