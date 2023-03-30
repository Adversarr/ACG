#pragma once
#include <Eigen/SparseCholesky>
#include <acore/math/sparse.hpp>
#include <acore/spatial/subdivision.hpp>
#include <aphysics/constriants.hpp>
#include <aphysics/objects/cloth.hpp>
#include <aphysics/objects/fluid.hpp>
#include <aphysics/objects/softbody.hpp>
#include <set>
#include <unordered_set>
#include <vector>

namespace acg::app {

class HybridApp {
public:
  using Scalar = Float64;
  using Constraint = physics::PositionStaticConstraint<Scalar, 3>;
  struct Cloth {
    physics::Cloth<Scalar, 3> data_;
    Field<Scalar, 3> inertia_position_;
    Field<Scalar, 3> substep_position_;
    Field<Scalar, 3> substep_direction_;
    Field<Scalar, 3> linesearch_position_;
    Field<Scalar, 3> grad_;
    Index global_solve_index_start_;
  };

  struct Softbody {
    physics::HyperElasticSoftbody<Scalar, 3> data_;
    Field<Scalar, 3> inertia_position_;
    Field<Scalar, 3> substep_position_;
    Field<Scalar, 3> grad_;
    Field<Scalar, 3> substep_direction_;
    Field<Scalar, 3> linesearch_position_;
    Index global_solve_index_start_;
  };

  struct Fluid {
    physics::LagrangeFluid<Scalar, 3> data_;
    Field<Scalar, 3> substep_position_;
    Field<Scalar, 3> linesearch_position_;
    Field<Scalar, 3> inertia_position_;
    Field<Scalar, 3> grad_;
    Field<Scalar, 3> update_direction_;
    Index global_solve_index_start_;
  };

  int steps_{5};
  std::vector<Cloth> cloth_;
  std::vector<Softbody> softbody_;
  // only one kind of fluid is allowed in this demo
  Fluid fluid_;
  Vec3<Scalar> gravity_{0, 0, -9.8};

  spatial::SubDivisionAABB<Scalar, physics::PhysicsObject, 3> subdivision_;

  std::vector<Constraint> constraints_;
  physics::GroundConstraint<Scalar> ground_;

  std::vector<physics::ExternForceConstraint<Scalar, 3>> external_forces_;

  Scalar linesearch_energy_;
  Scalar linesearch_alpha_;
  Scalar linesearch_gamma_{0.7};
  Scalar linesearch_terminal_thre_;
  Scalar collision_stiffness_{10};
  Scalar global_minimal_toi_{1};

  Index global_solve_element_count_{0};
  Vec<Scalar> global_solve_grad_;
  Vec<Scalar> global_solve_direction_;
  SpMat<Scalar> global_hessian_;
  using T = Eigen::Triplet<Scalar>;
  std::vector<T> global_hessian_data_;
  std::vector<T> collision_hessian_data_;

  std::unordered_set<physics::CollisionConstraint> collisions_;

  Eigen::SimplicialLDLT<acg::SpMat<Scalar>> global_solver_;
  void AddHessian(Index offset, const std::vector<T> &hessian);

  Scalar dt_{.005};

  Scalar dhat_{0.1};

  void Init();

  /**
   * @brief Detect the collision between `substep` and `linesearch`,
   *        will also setup `linesearch_initial_alpha_`
   * 
   * @return if there has collision in step.
   */
  bool DetectLinesearchCollision(bool verbose=false);

  void ComputeLinesearchPosition(Scalar alpha = 0);

  void Step(bool verbose = false);

  void CopyIterateResult();

  void ComputeStepDirection();

  void ComputeCollisionGrad();

  void ComputeLinesearchTerminalThreshold();

  void ComputeExtForces();

  void ComputeLinesearchEnergy();

  bool ComputeInertiaEnergy();

  bool ComputeConstraintEnergy();

  bool CommitLinesearch();

  void SetFluid(physics::LagrangeFluid<Scalar, 3>);

  void AddCloth(physics::Cloth<Scalar, 3> cloth);

  void AddCloth(Field<Scalar, 3> vert, Field<Index, 3> face, Field<Scalar> mass,
                Scalar stiffness);

  void AddSoftbody(physics::HyperElasticSoftbody<Scalar, 3> softbody);

  void AddSoftbody(Field<Scalar, 3> position, Field<Index, 4> tetras,
                   Field<Scalar> mass, Scalar lambda, Scalar mu);

  void EnforceConstraints();

  bool AddCollision(physics::CollisionConstraint cc);
};

} // namespace acg::app
