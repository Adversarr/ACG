#pragma once
#include <acore/geometry/common.hpp>
#include <acore/math/access.hpp>
#include <acore/math/common.hpp>

class FemExplicitApp {
public:
  void Init();

  void Step();

  void StepProjDynMF();

  int steps_{30};

  acg::geometry::topology::TetraList tetra_;

  acg::types::PositionField<acg::Float64, 3> position_;

  acg::Field<acg::Float64, 3> x_reference_;

  acg::Field<acg::Float64, 3> force_;

  acg::Field<acg::Float64, 3> velocity_;

  acg::Field<acg::Float64, 9> dm_inv_;

  acg::Float64 mass_;

  acg::Index num_vert_, num_tetra_;

  acg::Float64 mu_{2500}, lambda_{100}, dt_{0.0001};
};
