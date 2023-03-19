#pragma once
#include <acore/spatial/subdivision.hpp>
#include <aphysics/constriants.hpp>
#include <aphysics/objects/cloth.hpp>
#include <aphysics/objects/fluid.hpp>
#include <aphysics/objects/softbody.hpp>
#include <vector>

namespace acg::app {
class HybredApp {
public:
  using Scalar = Float32;
  using Constraint = physics::PositionLimit<Scalar, 3>;
  int substeps_{1};
  physics::ClothData<Scalar, 3> cloth_;
  physics::SoftbodyData<Scalar, 3> softbody_;
  physics::LagrangeFluid<Scalar, 3> fluid_;
  spatial::SubDivisionAABB<Scalar, physics::PhysicsObject, 3> subdivision_;
  std::vector<Constraint> position_limitation_;

  Scalar dt_{.005};

  void Init();

  void Step();

  void Reset();

  void ComputeStepDirection();

private:
  void SubStep();
};

}  // namespace acg::app
