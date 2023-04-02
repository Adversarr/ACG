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

  /**
   * Definition for Cloth, Softbody, Fluid, and their auxiliary variables
   */

  struct Cloth {
    physics::Cloth<Scalar, 3> data_;
    Field<Scalar, 3> inertia_position_;
    Field<Scalar, 3> substep_position_;
    Field<Scalar, 3> substep_x_;
    Field<Scalar, 3> linesearch_position_;
    Field<Scalar, 3> grad_;
    Index global_solve_index_start_;
  };

  struct Softbody {
    physics::HyperElasticSoftbody<Scalar, 3> data_;
    Field<Scalar, 3> inertia_position_;
    Field<Scalar, 3> substep_position_;
    Field<Scalar, 3> grad_;
    Field<Scalar, 3> substep_solution_;
    Field<Scalar, 3> linesearch_position_;
    Index global_solve_index_start_;
  };

  struct Fluid {
    physics::LagrangeFluid<Scalar, 3> data_;
    Field<Scalar, 3> substep_position_;
    Field<Scalar, 3> linesearch_position_;
    Field<Scalar, 3> inertia_position_;
    Field<Scalar, 3> grad_;
    Field<Scalar, 3> substep_direction_;
    Index global_solve_index_start_;
  };

  // Clothes
  std::vector<Cloth> cloth_;

  // Softbody
  std::vector<Softbody> softbody_;

  // only one kind of fluid is allowed in this demo
  Fluid fluid_;
  Vec3<Scalar> gravity_{0, 0, -9.8};

  std::vector<Constraint> constraints_;
  physics::GroundConstraint<Scalar> ground_;
  std::vector<physics::ExternForceConstraint<Scalar, 3>> external_forces_;

  // Linesearch:
  Scalar linesearch_energy_;
  Scalar linesearch_alpha_;
  Scalar linesearch_gamma_{0.7};
  Scalar linesearch_terminal_thre_;

  // Global solver
  Index global_solve_element_count_{0};
  Vec<Scalar> global_solve_grad_;
  Vec<Scalar> global_solve_direction_;
  using T = Eigen::Triplet<Scalar>;
  SpMat<Scalar> global_hessian_;
  Scalar global_minimal_toi_{1};
  std::vector<T> global_hessian_dyn_data_;

  // Collisions
  Scalar collision_stiffness_{1};
  spatial::SubDivisionAABB<Scalar, physics::PhysicsObject, 3> subdivision_;
  std::unordered_set<physics::CollisionConstraint> collisions_;
  std::vector<T> collision_hessian_data_;

  Eigen::SimplicialLDLT<acg::SpMat<Scalar>> global_solver_;

  // Time step.
  Scalar dt_{.003};
  int steps_{5};


  /**
   * @brief Initialize global hessian.
   *
   */
  void Init();
  
  /**
   * @brief Add hessian data to global_hessian.
   * @param offset
   * @param hessian
   */
  void AddHessian(Index offset, const std::vector<T> &hessian);

  /**
   * @brief Detect the collision between `substep` and `linesearch`,
   *        will also setup `linesearch_initial_alpha_`
   *
   * @return if there has collision in step.
   */
  bool DetectLinesearchCollision(bool verbose = false);

  /**
   * Setup all the `linesearch_position_` property in each object.
   * @param alpha
   */
  void ComputeLinesearchPosition(Scalar alpha = 0);

  /**
   * Time stepping
   * @param verbose
   */
  void Step(bool verbose = false);
  
  void SubStep(bool verbose = false);

  /**
   * Copy substep to data.
   */
  void CopyIterateResult();
  
  /**
   * Compute the step direction.
   * substep -> gradient -> qn solve -> direction
   */
  void ComputeStepDirection();

  /**
   * @brief Compute the largest feasible step size.
   * 
   * TODO: If the stepsize is too small, we should strengthen the stiffness.
   */
  void ComputeLinesearchTerminalThreshold();

  /**
   * @brief Compute the inertia part.
   * 
   */
  void ComputeExtForces();

  /**
   * @brief Compute energy use `linesearch`
   * 
   */
  void ComputeLinesearchEnergy();
  bool ComputeInertiaEnergy();
  bool ComputeConstraintEnergy();
  bool CommitLinesearch();


  void SetFluid(physics::LagrangeFluid<Scalar, 3> fluid);

  void AddCloth(physics::Cloth<Scalar, 3> cloth);

  void AddCloth(Field<Scalar, 3> vert, Field<Index, 3> face, Field<Scalar> mass,
                Scalar stiffness);

  void AddSoftbody(physics::HyperElasticSoftbody<Scalar, 3> softbody);

  void AddSoftbody(Field<Scalar, 3> position, Field<Index, 4> tetras,
                   Field<Scalar> mass, Scalar lambda, Scalar mu);


  bool AddCollision(physics::CollisionConstraint cc);

  void EnforceConstraints();
};

} // namespace acg::app
