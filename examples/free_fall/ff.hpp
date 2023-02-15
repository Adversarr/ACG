#pragma once

#include <acore/geometry/particle.hpp>

#include <agui/backend/mesh_pipeline.hpp>
#include <agui/light.hpp>
#include <agui/scene.hpp>
#include <agui/world_controller.hpp>
using namespace acg;
using namespace acg::gui;
using namespace acg::gui::details;

class FreeFall : public WorldCtrlUiOnly {
public:
  using P64 = acg::geometry::Particle<F64>;
  explicit FreeFall(int n);

  ~FreeFall();

protected:

  int RunPhysicsImpl(F64 dt) final;

  void CleanUpCallback() final;

  void RecreateSwapchainCallback() final;
  
  void InitCallback() final;

  void RunUiImpl() final;

  void PreRun() final;

  std::vector<vk::CommandBuffer> DrawScene() final;

private:
  void RegenerateScene();

  std::unique_ptr<MeshPipelineWithRenderPass> mesh_ppl_;

  Index n_;

  Scene scene_;
  Camera camera_;
  Light light_;
  bool update_camera_{false};

  std::vector<P64> particles_;
  Eigen::VectorXd mass_;
  std::vector<Vec3f> color_;
  Vec3f grav;
  Eigen::Matrix<F64, 3, Eigen::Dynamic, Eigen::ColMajor> velocity_;
  Eigen::Matrix<F64, 3, Eigen::Dynamic, Eigen::ColMajor> acceleration_;

  std::unique_ptr<VkContext::BufMem> vertex_buffer_;
  std::unique_ptr<VkContext::BufMem> indice_buffer_;
};


