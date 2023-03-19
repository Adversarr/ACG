#pragma once

#include <Eigen/SparseCholesky>
#include <acore/geometry/common.hpp>
#include <acore/math/access.hpp>
#include <acore/math/sparse.hpp>

namespace app {
using namespace acg;
class FemImplicitApp {
public:
  void Step();
  void Init();

  void StepMF();

  void StepQuasi();
  Field<Float32, 3> position_;
  Field<Float32, 3> rest_position_;
  Field<Float32, 3> velocity_;
  types::topology::TetraList tetras_;

  Float32 lambda_{1.0}, mu_{1.};

  Float32 dt_{.001};

  Float32 eps_{0.0001};

  bool apply_force_;

  bool use_stvk_;

  bool explicit_{true};

  Float32 mass_{1};

  int steps_ {10};

  acg::SpMat<Float32> pc_hes_;
  Eigen::SimplicialCholesky<acg::SpMat<Float32>> solver_;
};
}  // namespace app
