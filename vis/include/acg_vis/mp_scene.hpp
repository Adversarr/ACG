
#pragma once

#include <acg_core/geometry/particle.hpp>

#include <acg_vis/mesh_pipeline.hpp>
#include <acg_vis/point_light.hpp>
#include <acg_vis/scene.hpp>
#include <acg_vis/world_controller.hpp>

namespace acg::visualizer::details {

class MPWorldCtrl : public WorldCtrlUiOnly {
protected:

  void CleanUpCallback() final;

  void RecreateSwapchainCallback() final;
  
  void InitCallback() final;

  std::vector<vk::CommandBuffer> DrawScene() final;

  void RefitBuffers();

  std::unique_ptr<MeshPipeline> mesh_ppl_;

  Scene scene_;

  Camera camera_;

  Light light_;

  bool update_camera_{false};

  std::unique_ptr<VkContext::BufMem> vertex_buffer_;

  std::unique_ptr<VkContext::BufMem> indice_buffer_;

  const vk::DeviceSize buffer_minimum_size_ = 1024;
};

}

namespace acg::visualizer {

using details::MPWorldCtrl;

}
