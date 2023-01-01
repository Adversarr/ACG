#pragma once

#include <acg_gui/backend/mesh_ppl.hpp>
#include <acg_gui/backend/wireframe_ppl.hpp>
#include <acg_gui/scene.hpp>
#include <acg_gui/world_controller.hpp>

using namespace acg::gui;

class Api : public acg::gui::WorldCtrlUiOnly {
public:
  Api() = default;

protected:
  void CleanUpCallback() final;

  void InitCallback() final;

  void RecreateSwapchainCallback() final;

  void RefitBuffers();

  std::vector<vk::CommandBuffer> DrawScene() final;

private:
  std::unique_ptr<details::MeshPipeline> mesh_ppl_{nullptr};

  std::unique_ptr<details::WireframePipeline> wireframe_pipeline_{nullptr};

  std::unique_ptr<details::GraphicsRenderPass> graphics_render_pass_{nullptr};

  std::unique_ptr<VkContext::BufMem> vertex_buffer_, indice_buffer_;
  std::unique_ptr<VkContext::BufMem> wireframe_vertex_buffer_, wireframe_indice_buffer_;

  Camera camera_;

  Light light_;

  Scene scene_;
};
