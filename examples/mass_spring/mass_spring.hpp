#pragma once

#include <acg_core/geometry/particle.hpp>
#include <acg_core/geometry/mesh.hpp>

#include <acg_gui/backend/mesh_pipeline.hpp>
#include <acg_gui/point_light.hpp>
#include <acg_gui/scene.hpp>
#include <acg_gui/world_controller.hpp>
#include <acg_gui/mp_scene.hpp>
using namespace acg;
using namespace acg::gui;

class MassSpring : public MPWorldCtrl {
public:
  using Mesh = geometry::SimpleMesh<F64>;

  explicit MassSpring(Idx n);

protected:

  void LocalStep(F64 dt);

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
  Field<Idx, 2> edges_;
  Field<F64, 1> original_length_;

  // Slack Variable
  Field<F64, 3> d_;

  Mesh mesh_;
  float k_{10};
};


