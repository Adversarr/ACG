#pragma once
#include <acore/geometry/common.hpp>
#include <acore/access.hpp>
#include <acore/math/common.hpp>

class FemExplicitApp {
public:
  void Init();

  void Step();

  void StepProjDynMF();

  int steps_;

  acg::geometry::topology::TetraList tetra_;

  acg::types::PositionField<acg::F64, 3> position_;

  acg::Field<acg::F64, 3> x_reference_;

  acg::Field<acg::F64, 3> force_;

  acg::Field<acg::F64, 3> velocity_;

  acg::Field<acg::F64, 9> dm_inv_;

  acg::Index num_vert_, num_tetra_;

  acg::F64 mu_{2500}, lambda_{100}, dt_{0.0001};
};
