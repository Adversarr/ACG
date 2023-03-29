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
  using Scalar = Float64;
  using Constraint = physics::PositionStaticConstraint<Scalar, 3>;
  struct Cloth {
    physics::Cloth<Scalar, 3> data_;
    Field<Scalar, 3> inertia_position_;
    Field<Scalar, 3> update_position_;
    Field<Scalar, 3> update_direction_;
    Field<Scalar, 3> linesearch_position_;
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
    Field<Scalar, 3> linesearch_position_;

    acg::SpMat<Scalar> hessian_;
    std::unique_ptr<Eigen::SimplicialCholesky<acg::SpMat<Scalar>>> solver_;
  };

  struct Fluid {
    physics::LagrangeFluid<Scalar, 3> data_;
    Field<Scalar, 3> update_position_;
    Field<Scalar, 3> grad_;
    Field<Scalar, 3> update_direction_;
  };

  int steps_{10};
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

  Scalar dt_{.003};

  void Init();

  void Step(bool verbose = false);

  void Reset();

  void CopyIterateResult();

  // for both, use Quasi Newton method.
  void ComputeStepDirection();

  void ComputeLinesearchTerminalThreshold();

  void ComputeExtForces();

  void ComputeEnergy();

  bool ComputeInertiaEnergy();

  bool ComputeConstraintEnergy();

  void AddCloth(physics::Cloth<Scalar, 3> cloth);

  void AddCloth(Field<Scalar, 3> vert, Field<Index, 3> face, Field<Scalar> mass,
                Scalar stiffness);

  void AddSoftbody(physics::HyperElasticSoftbody<Scalar, 3> softbody);

  void AddSoftbody(Field<Scalar, 3> position, Field<Index, 4> tetras,
                   Field<Scalar> mass, Scalar lambda, Scalar mu);

  void ForceConstraints();

  void DetectAllCollisions();

  void EnforceConstraints();

private:
  void SubStep();
};

} // namespace acg::app
