#include <Eigen/Sparse>
#include <acore/all.hpp>
#include <acore/common.hpp>
#include <acore/math/sparse.hpp>
#include <set>
#include <taskflow/taskflow.hpp>
class App {
public:
  void Init();

  void Step();


  void StepProjDyn();

  void StepProjDynMf();

  void AddSpring(acg::Index, acg::Index);

  acg::Field<float, 3> position_;
  acg::Field<float, 3> origin_position_;

  acg::Field<float, 3> velocity_;
  acg::Field<float, 3> d_;

  acg::geometry::topology::TriangleList faces_;

  acg::Index n_grids_ = 5;
  acg::Index n_vertices_;

  std::set<std::pair<acg::Index, acg::Index>> springs_;

  std::vector<std::pair<acg::Index, acg::Index>> springs_vec_;

  float dt_{0.005};

  float k_{1000};

  float mass_{0.01};

  acg::SpMat<float> hessian_;

  Eigen::SparseLU<acg::SpMat<float>> splu_;

  int steps_{30};

  tf::Executor executor_{8};
};
