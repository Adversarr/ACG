#pragma once
#include <Eigen/SparseCholesky>
#include <acore/math/sparse.hpp>
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
  using Constraint = physics::PositionStaticConstraint<Scalar, 3>;
  struct Cloth {
    physics::ClothData<Scalar, 3> data_;
    Field<Scalar, 3> inertia_position_;
    Field<Scalar, 3> update_position_;
    Field<Scalar, 3> update_direction_;
    Field<Scalar, 3> grad_;

    acg::SpMat<Scalar> hessian_;
    std::unique_ptr<Eigen::SimplicialCholesky<acg::SpMat<Scalar>>> solver_;
  };

  struct Softbody {
    physics::HyperElasticSoftbody<Scalar, 3> data_;
    Field<Scalar, 3> inertia_position_;
    Field<Scalar, 3> update_position_;
    Field<Scalar, 3> grad_;
    Field<Scalar, 3> update_direction_;

    acg::SpMat<Scalar> hessian_;
    Eigen::SimplicialCholesky<acg::SpMat<Scalar>> solver_;
  };

  struct Fluid {
    physics::LagrangeFluid<Scalar, 3> data_;
    Field<Scalar, 3> update_position_;
    Field<Scalar, 3> grad_;
    Field<Scalar, 3> update_direction_;
  };

  int steps_{1};
  std::vector<Cloth> cloth_;
  std::vector<Softbody> softbody_;
  // only one kind of fluid is allowed in this demo
  Fluid fluid_;
  Vec3<Scalar> gravity_{0, 0, -9.8};

  spatial::SubDivisionAABB<Scalar, physics::PhysicsObject, 3> subdivision_;

  std::vector<Constraint> constraints_;

  std::vector<physics::ExternForceConstraint<Scalar, 3>> external_forces_;

  Scalar linesearch_energy_;
  Scalar linesearch_alpha_;
  Scalar linesearch_gamma_{0.7};
  Scalar linesearch_energy_initial_;
  Scalar linesearch_terminal_thre_;

  Scalar dt_{.005};

  Scalar cloth_k_{1000};

  void Init();

  void Step();

  void Reset();

  void CopyIterateResult();

  // for both, use Quasi Newton method.
  void ComputeStepDirection();

  void ComputeLinesearchTerminalThreshold();

  void ComputeExtForces();

  void ComputeEnergy();

  void AddCloth(physics::ClothData<Scalar, 3> cloth);

  void AddCloth(Field<Scalar, 3> vert, Field<Index, 3> face, Scalar stiffness);

  void AddSoftbody(physics::ClothData<Scalar, 3> softbody);

  void ForceConstraints();

  void DetectAllCollisions();

private:
  void SubStep();
};

}  // namespace acg::app
