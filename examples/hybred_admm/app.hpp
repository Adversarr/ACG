#pragma once
#include "aphysics/elastic/neohookean.hpp"
#include "aphysics/solver/admm/hyperelastic.hpp"
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

namespace acg::app {

class HybridApp {
public:
  using Scalar = Float64;
  using PositionConstraint = physics::PositionStaticConstraint<Scalar, 3>;

  /**
   * Definition for Cloth, Softbody, Fluid, and their auxiliary variables
   */
  struct Cloth {
    physics::Cloth<Scalar, 3> data_;
    Field<Scalar, 3> inertia_position_;
    Field<Scalar, 3> substep_position_;
    Field<Scalar, 3> substep_x_;
    Field<Scalar, 3> substep_solution_;
    Field<Scalar, 3> constraint_admm_multiplier_;

    physics::admm::SpringConstraint<Scalar> admm_compute_;
    Scalar admm_weight_{0.7};
    Index global_solve_index_start_;
  };

  struct Softbody {
    physics::HyperElasticSoftbody<Scalar, 3> data_;
    Field<Scalar, 3> inertia_position_;
    Field<Scalar, 3> substep_position_;
    Field<Scalar, 3> substep_solution_;
    Field<Scalar, 3> substep_x_;
    Field<Scalar, 9> constraint_admm_multiplier_;
    physics::admm::HyperElasticConstraint<
        Scalar, physics::elastic::OgdenNeoHookean<Scalar, 3>>
        admm_compute_;
    Scalar admm_weight_{.5};
    Scalar hessian_coefficient_;
    Index global_solve_index_start_;
  };

  struct Fluid {
    physics::LagrangeFluid<Scalar, 3> data_;
    Field<Scalar, 3> substep_position_;
    Field<Scalar, 3> inertia_position_;
    Field<Scalar, 3> grad_;
    Field<Scalar, 3> substep_direction_;

    // Remain empty: No implicit in-compressibility is derived.
    Field<Scalar, 3> constraint_admm_;
    Index global_solve_index_start_;
  };

  // Clothes
  std::vector<Cloth> cloth_;

  // Softbody
  std::vector<Softbody> softbody_;

  // Optionally one fluid.
  std::unique_ptr<Fluid> fluid_;

  // ADMM Helper.
  Index global_variable_count_{0};
  Vec<Scalar> global_x_;
  Vec<Scalar> global_solution_;
  using Trip = Eigen::Triplet<Scalar>;
  std::vector<Trip> global_hessian_dyn_data_;
  SpMat<Scalar> hessian_dyn_;
  Eigen::SimplicialLDLT<SpMat<Scalar>> global_solver_;

  using PConstraint = physics::PositionStaticConstraint<Scalar, 3>;
  using GConstraint = physics::GroundConstraint<Scalar>;
  using EConstraint = physics::ExternForceConstraint<Scalar, 3>;

  std::vector<PConstraint> position_constraints_;
  GConstraint ground_constraints_;
  std::vector<EConstraint> ext_force_constraints_;

  Index PutHessianDyn(Index count, const std::vector<Trip> &hessian);

  void Step();

  void AddCloth(physics::Cloth<Scalar, 3> cloth);

  void AddCloth(Field<Scalar, 3> vert, Field<Index, 3> face, Field<Scalar> mass,
                Scalar stiffness);

  void ComputeExtForce();

  void CommitStepResult();

  void Init();

  void ComputeLocal();

  void ComputeGlobal();

  void EnforceConstraints();

  void AddSoftbody(physics::HyperElasticSoftbody<Scalar, 3> softbody);

  void AddSoftbody(Field<Scalar, 3> position, Field<Index, 4> tetras,
                   Field<Scalar> mass, Scalar lambda, Scalar mu);

  Scalar dt_{.005};
};

} // namespace acg::app
