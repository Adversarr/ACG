#pragma once

#include <acg_core/geometry/particle.hpp>
#include <acg_core/geometry/mesh.hpp>

#include <acg_vis/mesh_pipeline.hpp>
#include <acg_vis/point_light.hpp>
#include <acg_vis/scene.hpp>
#include <acg_vis/world_controller.hpp>
#include <acg_vis/mp_scene.hpp>
using namespace acg;
using namespace acg::visualizer;

class MassSpring : public MPWorldCtrl {
public:
  using Mesh = geometry::Mesh<F64>;

  explicit MassSpring(Idx n);

protected:

  int RunPhysicsImpl(F64 dt) final;

  void RunUiImpl() final;

  void PreRun() final;

  void ApplyForce(Idx i, Idx j);


private:
  void RegenerateScene();
  Idx n_;
  Vec3f color_;
  Eigen::Matrix<F64, 3, Eigen::Dynamic, Eigen::ColMajor> position_;
  Eigen::Matrix<F64, 3, Eigen::Dynamic, Eigen::ColMajor> velocity_;
  Eigen::Matrix<F64, 3, Eigen::Dynamic, Eigen::ColMajor> acceleration_;
  AttrVec<Idx, 2> edges_;
  Mesh mesh_;
  Mesh new_mesh_;

  float k_{10};
};


