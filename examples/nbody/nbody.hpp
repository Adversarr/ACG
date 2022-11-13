#pragma once

#include <acg_core/geometry/particle.hpp>

#include <acg_vis/mesh_pipeline.hpp>
#include <acg_vis/point_light.hpp>
#include <acg_vis/scene.hpp>
#include <acg_vis/world_controller.hpp>
#include <acg_vis/mp_scene.hpp>
using namespace acg;
using namespace acg::visualizer;

class NBodySim : public MPWorldCtrl {
public:
  using P64 = acg::geometry::Particle<F64>;
  explicit NBodySim(int n);

  ~NBodySim();

protected:

  int RunPhysicsImpl(F64 dt) final;

  void RunUiImpl() final;

  void PreRun() final;


private:
  void RegenerateScene();

  Idx n_;

  std::vector<P64> particles_;
  Eigen::VectorXd mass_;
  std::vector<Vec3f> color_;
  Eigen::Matrix<F64, 3, Eigen::Dynamic, Eigen::ColMajor> velocity_;
  Eigen::Matrix<F64, 3, Eigen::Dynamic, Eigen::ColMajor> acceleration_;

  AttrVec<Idx, 2> edges_;
};


