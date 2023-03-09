#pragma once

#include <acore/geometry/common.hpp>
#include <acore/math/access.hpp>

namespace app {
using namespace acg;
class FemImplicitApp {
public:
  void Step();
  void Init();
  Field<Float32, 3> position_;
  Field<Float32, 3> rest_position_;
  Field<Float32, 3> velocity_;
  geometry::topology::TetraList tetras_;

  Float32 lambda_{1.0}, mu_{1.};

  Float32 dt_{.005};

  bool apply_force_;

  bool use_stvk_;

  bool explicit_{true};

  Float32 mass_{1};

  int steps_ {10};
};
}  // namespace app
