#pragma once

#include <acore/geometry/particle.hpp>

#include "light.hpp"
#include "scene.hpp"
#include "world_controller.hpp"
#include "backend/mesh_pipeline.hpp"

namespace acg::gui::details {

class MPWorldCtrl : public WorldCtrlUiOnly {
protected:
  void CleanUpCallback() final;

  void RecreateSwapchainCallback() final;

  void InitCallback() final;

  std::vector<vk::CommandBuffer> DrawScene() final;

  void RefitBuffers();

  std::unique_ptr<MeshPipelineWithRenderPass> mesh_ppl_;

  Scene scene_;

  Camera camera_;

  Light light_;

  bool update_camera_{false};

  std::unique_ptr<VkContext::BufMem> vertex_buffer_;

  std::unique_ptr<VkContext::BufMem> indice_buffer_;

  const vk::DeviceSize buffer_minimum_size_ = 1024;
};

}  // namespace acg::gui::details

namespace acg::gui {
using details::MPWorldCtrl;
}
