#pragma once
#include <Eigen/LU>
#include <acore/geometry/common.hpp>
#include <acore/math/common.hpp>

#include "common.hpp"

namespace acg::physics {
template <typename Scalar = Float, int dim = 3> struct HyperElasticSoftbody {
  /**
   * @brief position for vertices.
   *
   */
  Field<Scalar, dim> position_;

  Field<Scalar> mass_;

  /**
   * @brief Velocity for vertices
   *
   */
  Field<Scalar, dim> velocity_;

  /**
   * @brief (Backuped) rest position for vertices.
   *
   */
  Field<Scalar, dim> rest_position_;

  /**
   * @brief (Helper) Tetras Rest post matrix inverse.
   *
   */
  Field<Scalar, dim * dim> tetra_rinvs_;

  /**
   * @brief Tetras topology
   *
   */
  Field<Index, dim + 1> tetras_;

  /**
   * @brief Coefficients for Hyperelastic models.
   *
   */
  Float32 lambda_, mu_;

  HyperElasticSoftbody() = default;

  HyperElasticSoftbody(Field<Scalar, dim> position,
                       Field<Index, dim + 1> tetras, 
                       Field<Scalar> mass, Scalar lambda, Scalar mu)
      : position_(std::move(position)),
        tetras_(std::move(tetras)),
        mass_(std::move(mass)),
        lambda_(lambda),
        mu_(mu) {
    velocity_.resizeLike(position);
    velocity_.setZero();
    rest_position_ = position;
  }


  void InitAuxiliary() {
    velocity_.resizeLike(position_);
    rest_position_ = position_;
    tetra_rinvs_.resize(Eigen::NoChange, tetras_.cols());
    auto pacc = view(position_);
    for (auto [i, v] : enumerate(view(tetras_))) {
      Mat<Scalar, dim, dim> rest_pose;
      auto x0 = pacc(v(0));
      for (int j = 0; j < dim; ++j) {
        rest_pose.col(j) = pacc(v(j + 1)) - x0;
      }

      tetra_rinvs_.col(i) = rest_pose.inverse().reshaped();
    }
  }
};

}  // namespace acg::physics
