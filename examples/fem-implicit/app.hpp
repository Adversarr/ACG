#pragma once

#include <Eigen/SparseCholesky>
#include <acore/geometry/common.hpp>
#include <acore/math/view.hpp>
#include <acore/math/sparse.hpp>

namespace app {
using namespace acg;
class FemImplicitApp {
public:
  void Step();
  void Init();

  void StepMF();

  using Scalar = Float64;
  void StepQuasi();
  Field<Scalar, 3> position_;
  Field<Scalar, 3> rest_position_;
  Field<Scalar, 3> velocity_;
  types::topology::TetraList tetras_;

  Scalar lambda_{1.0}, mu_{1.};

  Scalar dt_{.001};

  Scalar eps_{0.0001};

  bool apply_force_;

  bool use_stvk_;

  bool explicit_{true};

  Scalar mass_{1};

  int steps_ {10};

  acg::SpMat<Scalar> pc_hes_;
  Eigen::SimplicialCholesky<acg::SpMat<Scalar>> solver_;
};
}  // namespace app
