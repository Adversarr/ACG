#pragma once

#include <acg_core/geometry/particle.hpp>
#include <acg_gui/mp_scene.hpp>
#include <acg_gui/light.hpp>
#include <acg_gui/scene.hpp>
#include <acg_gui/world_controller.hpp>
using namespace acg;
using namespace acg::gui;

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

  Index n_;

  std::vector<P64> particles_;
  Eigen::VectorXd mass_;
  std::vector<Vec3f> color_;
  Eigen::Matrix<F64, 3, Eigen::Dynamic, Eigen::ColMajor> velocity_;
  Eigen::Matrix<F64, 3, Eigen::Dynamic, Eigen::ColMajor> acceleration_;

  Field<Index, 2> edges_;
};
