#pragma once

#include <acg_core/geometry/particle.hpp>
#include <acg_gui/mp_scene.hpp>
#include <acg_gui/point_light.hpp>
#include <acg_gui/scene.hpp>
#include <acg_gui/world_controller.hpp>
using namespace acg;
using namespace acg::visualizer;

class NBodySim : public MPWorldCtrl {
public:
  using P64 = acg::geometry::Particle<F64>;
  explicit NBodySim(int n);

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

  Attr<Idx, 2> edges_;
};
