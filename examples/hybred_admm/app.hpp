#pragma once
#include <Eigen/IterativeLinearSolvers>
#include <Eigen/SparseCholesky>
#include <acore/math/sparse.hpp>
#include <acore/spatial/subdivision.hpp>
#include <aphysics/constriants.hpp>
#include <aphysics/objects/cloth.hpp>
#include <aphysics/objects/fluid.hpp>
#include <aphysics/objects/softbody.hpp>
#include <aphysics/solver/admm/spring.hpp>
#include <set>
#include <unordered_set>
#include <vector>

#include "aphysics/elastic/neohookean.hpp"
#include "aphysics/mpm/transfer.hpp"
#include "aphysics/solver/admm/hyperelastic.hpp"

namespace acg::app {

class HybridAdmmApp {
public:
  using Scalar = Float64;
  using PositionConstraint = physics::PositionStaticConstraint<Scalar, 3>;
  using SolverType = Eigen::SimplicialLDLT<SpMat<Scalar>>;

  using FluidSolver = Eigen::BiCGSTAB<SpMat<Scalar>>;

  /**
   * Definition for Cloth, Softbody, Fluid, and their auxiliary variables
   */
  struct Cloth {
    physics::Cloth<Scalar, 3> data_;
    Field<Scalar, 3> inertia_position_;
    Field<Scalar, 3> substep_position_;
    Field<Scalar, 3> substep_solution_;
    Field<Scalar, 3> ccd_dst_position_;
    Field<Scalar, 3> substep_x_;
    Field<Scalar, 3> constraint_admm_multiplier_;

    physics::admm::SpringConstraint<Scalar> admm_compute_;
    Scalar admm_weight_{0.7};
    Index global_solve_index_start_;

    // ADMM Solver
    std::unique_ptr<Eigen::SimplicialLDLT<SpMat<Scalar>>> solver_;
  };

  struct Softbody {
    physics::HyperElasticSoftbody<Scalar, 3> data_;
    Field<Scalar, 3> inertia_position_;
    Field<Scalar, 3> substep_position_;
    Field<Scalar, 3> substep_solution_;
    Field<Scalar, 3> ccd_dst_position_;
    Field<Scalar, 3> substep_x_;
    Field<Scalar, 9> constraint_admm_multiplier_;
    physics::admm::HyperElasticConstraint<Scalar, physics::elastic::OgdenNeoHookean<Scalar, 3>> admm_compute_;
    Scalar admm_weight_{.5};
    Scalar hessian_coefficient_;
    Index global_solve_index_start_;

    // ADMM Solver
    std::unique_ptr<Eigen::SimplicialLDLT<SpMat<Scalar>>> solver_;
  };

  struct Fluid {
    physics::LagrangeFluid<Scalar, 3> data_;
    Field<Scalar, 3> substep_position_;
    Field<Scalar, 3> inertia_position_;
    Field<Scalar, 3> grad_;
    Field<Scalar, 3> ccd_dst_position_;
    Field<Scalar, 3> substep_solution_;
    Scalar pressure_scale_ = 10;
    physics::EulerFluidRegular<Scalar, 3> euler_;

    using APIC = physics::mpm::ApicRegular<Scalar, 3, physics::mpm::CubicBSplineKernel>;
    std::unique_ptr<APIC> apic_;
    Scalar sph_kern_size_{1e-1};
    spatial::SubDivisionAABB<Scalar, Index, 3> sd_;
  };

  // Clothes
  std::vector<Cloth> cloth_;

  // Softbody
  std::vector<Softbody> softbody_;

  // Optionally one fluid.
  std::unique_ptr<Fluid> fluid_;

  bool enable_collision_detect_ = false;

  // ADMM Helper.
  Index global_variable_count_{0};
  using Trip = Eigen::Triplet<Scalar>;
  std::vector<Trip> global_hessian_dyn_data_;

  using PConstraint = physics::PositionStaticConstraint<Scalar, 3>;
  using GConstraint = physics::GroundConstraint<Scalar>;
  using WConstraint = physics::WallConstraint<Scalar>;
  using EConstraint = physics::ExternForceConstraint<Scalar, 3>;

  std::vector<PConstraint> position_constraints_;
  GConstraint ground_constraints_;
  WConstraint wall_x_low_;
  WConstraint wall_y_low_;
  WConstraint wall_x_high_;
  WConstraint wall_y_high_;
  std::vector<EConstraint> ext_force_constraints_;

  // Collisions:
  using Collision = physics::CollisionConstraint;
  std::vector<Collision> collisions_;
  Scalar minimum_toi_{1};
  Scalar min_distance_{1e-3};

  bool enable_result_check_{true};

  void Step();

  void SolveFluid();

  void AddCloth(physics::Cloth<Scalar, 3> cloth);

  void AddCloth(Field<Scalar, 3> vert, Field<Index, 3> face, Field<Scalar> mass, Scalar stiffness);

  void AddSoftbody(physics::HyperElasticSoftbody<Scalar, 3> softbody);

  void AddSoftbody(Field<Scalar, 3> position, Field<Index, 4> tetras, Field<Scalar> mass, Scalar lambda, Scalar mu);

  void SetFluid(physics::LagrangeFluid<Scalar, 3> fluid, Index grid_size = 20);

  void ComputeExtForce();

  void CommitStepResult();

  void Init();

  void ComputeLocal();

  void ComputeGlobal();

  void EnforceConstraints();

  void DetectCollisions(bool verbose = false);

  void SubStepCollisionFree();

  void CcdEpilogue();

  void CcdPrologue();

  Scalar dt_{2e-3};

  Scalar velocity_damping_{0.99999};

  int max_iteration_{10};

  bool enable_subd_{false};
};

}  // namespace acg::app
