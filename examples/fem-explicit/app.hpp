#pragma once
#include <acg_core/geometry/common.hpp>
#include <acg_core/math/access.hpp>
#include <acg_core/math/common.hpp>

class FemExplicitApp {
public:
  void Init();

  void Step();

  acg::geometry::topology::TetraList tetra_;

  acg::types::PositionField<acg::F64, 3> position_;

  acg::Field<acg::F64, 3> x_reference_;

  acg::Field<acg::F64, 3> force_;

  acg::Field<acg::F64, 3> velocity_;

  acg::Field<acg::F64, 9> dm_inv_;

  acg::Index num_vert_, num_tetra_;

  acg::F64 mu_{5000}, lambda_{100}, dt_{0.001};
};
