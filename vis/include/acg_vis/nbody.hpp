#pragma once

#include "mesh_pipeline.hpp"
#include "scene.hpp"
#include "world_controller.hpp"
#include "scene.hpp"
#include <acg_core/geometry/particle.hpp>


namespace acg::visualizer::details {
class NBodySim : public WorldCtrlUiOnly {
public:
  using P64 = acg::geometry::Particle<F64>;
  explicit NBodySim(int n);

  ~NBodySim();

protected:
  int RunPhysicsImpl(F64 dt) final;

  void InitCallback() final;

  void CleanUp();

  void RecreateSwapchainCallback() final;

  void RunUiImpl() final;

  void PreRun() final;

  std::vector<vk::CommandBuffer> DrawScene() final;

private:
  void RegenerateScene();

  std::unique_ptr<MeshPipeline> mesh_ppl_;

  Idx n_;

  Scene scene_;
  Camera camera_;
  bool update_camera_{false};

  std::vector<P64> particles_;
  Eigen::VectorXd  mass_;
  std::vector<Vec3f> color_;
  Eigen::Matrix<F64, 3, Eigen::Dynamic, Eigen::ColMajor> velocity_;
  Eigen::Matrix<F64, 3, Eigen::Dynamic, Eigen::ColMajor> acceleration_;

  std::unique_ptr<VkContext::BufMem > vertex_buffer_;
  std::unique_ptr<VkContext::BufMem > indice_buffer_;
};

} // namespace acg::visualizer::details

namespace acg::visualizer {
using MeshWorldCtrl = details::NBodySim;
}
