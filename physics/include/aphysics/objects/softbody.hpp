#pragma once
#include <acore/geometry/common.hpp>
#include <acore/math/common.hpp>

#include <Eigen/LU>

#include "common.hpp"

namespace acg::physics {
template <typename Scalar = Float, int dim = 3> struct SoftbodyData {
  Field<Scalar, dim> position_;
  Field<Scalar, dim> velocity_;
  Field<Scalar, dim> rest_position_;

  Field<Scalar, dim * dim> tetra_rinvs_;
  Field<Index, dim + 1> tetras_;
  Float32 lambda_, mu_;

  SoftbodyData() = default;

  SoftbodyData(Field<Scalar, dim> position, Field<Index, dim + 1> tetras, Scalar lambda, Scalar mu)
      : position_(std::move(position)), tetras_(std::move(tetras)), lambda_(lambda), mu_(mu) {
    velocity_.resizeLike(position);
    velocity_.setZero();
    rest_position_ = position;
  }

  void InitAuxiliary() {
    tetra_rinvs_.resize(Eigen::NoChange, tetras_.cols());

    auto pacc = access(position_);
    for (auto [i, v] : enumerate(access(tetras_))) {
      Mat<Scalar, dim, dim> rest_pose;
      auto x0 = pacc(v(0));
      for (int i = 0; i < dim; ++i) {
        rest_pose.col(i) = pacc(v(i + 1)) - x0;
      }

      tetra_rinvs_.col(i) = rest_pose.inverse().reshaped();
    }
  }
};

}  // namespace acg::physics
