#pragma once

#include "mesh_pipeline.hpp"
#include "world_controller.hpp"
#include "scene.hpp"
#include <acg_core/geometry/particle.hpp>


namespace acg::visualizer::details {
class MeshWorldCtrl : public WorldCtrlUiOnly {
public:
  using P64 = acg::geometry::Particle<F64>;
  explicit MeshWorldCtrl(int n);

  ~MeshWorldCtrl();

protected:
  int RunPhysicsImpl(F64 dt) final;

  void InitCallback() final;

  void CleanUp();

  void RecreateSwapchainCallback() final;

  // void RunUiImpl() final;

  void PreRun() final;

  std::vector<vk::CommandBuffer> DrawScene() final;

private:
  std::unique_ptr<MeshPipeline> mesh_ppl_;

  int n_;

  Scene scene_;
  Camera camera_;

  std::vector<P64> geo_;
  std::vector<F64>  mass_;

  std::unique_ptr<VkContext::BufMem > vertex_buffer_;
  std::unique_ptr<VkContext::BufMem > indice_buffer_;

  
};

} // namespace acg::visualizer::details

namespace acg::visualizer {
using MeshWorldCtrl = details::MeshWorldCtrl;
}
