#pragma once

#include "acg_gui/backend/context.hpp"
#include "acg_gui/backend/graphics_pass.hpp"
#include "acg_gui/backend/mesh_ppl.hpp"
#include "acg_gui/backend/point_ppl.hpp"
#include "acg_gui/backend/ui_pass.hpp"
#include "acg_gui/backend/wireframe_ppl.hpp"
#include "acg_gui/convent.hpp"
#include "imgui_impl_glfw.h"
#include "imgui_impl_vulkan.h"
#include "scene.hpp"

namespace acg::gui {
namespace details {
using BufferID = size_t;

struct MeshRenderInfo {
  BufferID vertex;
  BufferID index;
  BufferID instance;
  uint32_t vertex_count;
  uint32_t index_count;
  uint32_t instance_count;
  glm::mat4 model;
  bool use_double_side_lighting = false;
  int specular = 32;
};

struct ParticleRenderInfo {
  BufferID vertex;
  uint32_t vertex_count;
  float size;
  glm::vec4 color;
  bool use_uniform_color = false;
};

struct WireframeRenderInfo {
  BufferID vertex;
  BufferID index;
  uint32_t vertex_count;
  uint32_t index_count;
};

struct StagingUpdateInfo {
  vk::Buffer dst_buf;
  vk::DeviceSize offset;
  vk::DeviceSize size;
  StagingUpdateInfo(vk::Buffer dst, vk::DeviceSize size) : dst_buf(dst), offset(0), size(size) {}
};

// TODO: add torch light support
class GGui {
public:
  struct Config {
    vk::DeviceSize staging_buffer_size = 16 * 1024 * 1024;
  };
  static GGui& Instance();

  Scene2& GetScene() { return scene_; }

  Light& GetLight() { return light_; }

  Camera& GetCamera() { return camera_; }

  void UpdateScene();

  void UpdateLightCamera();

  void SetUIDrawCallback(std::function<void()> callback) { ui_draw_callback_ = callback; }
  
  void RenderOnce();

private:

  explicit GGui(const Config& config);

  void PrepareBuffers();
  void PrepareMeshBuffer(MeshRenderInfo& info);
  void PrepareParticleBuffer(ParticleRenderInfo& info);
  void PrepareWireframeBuffer(WireframeRenderInfo& info);

  void FillBuffers();

  BufferID CreateVertexBuffer(vk::DeviceSize size);
  BufferID CreateInstanceBuffer(vk::DeviceSize size);
  BufferID CreateIndexBuffer(vk::DeviceSize size);

  void ReserveBuffer(vk::DeviceSize size, BufferID id);

  void InitStagingBuffer();

  void DestroyStagingBuffer();

  void Init();


  void Destroy();

  void RecreateSwapchain();

  void FlushStagingBuffer();

  void CommitStagingBuffer(StagingUpdateInfo update_info, void* data,
                           bool flush_immediately = true);

  BufferWithMemory& GetAllocatedBuffer(BufferID id) { return allocated_buffers_[id]; }

  std::vector<BufferWithMemory> allocated_buffers_;
  std::unique_ptr<GraphicsRenderPass> graphics_pass_;
  std::unique_ptr<UiPass> ui_pass_;
  // Mesh
  std::unique_ptr<MeshPipeline> mesh_pipeline_;
  std::vector<MeshRenderInfo> mesh_render_info_;
  std::vector<MeshRenderInfo> mesh_particle_render_info_;
  // Wireframe
  std::unique_ptr<WireframePipeline> wireframe_pipeline_;
  std::vector<WireframeRenderInfo> wireframe_render_info_;
  // Point
  std::unique_ptr<PointPipeline> point_pipeline_;
  std::vector<ParticleRenderInfo> particle_render_info_;

  // Internal data.
  Scene2 scene_;
  Camera camera_;
  Light light_;

  // Staging buffer and manipulator
  BufferWithMemory staging_buffer_;
  std::vector<StagingUpdateInfo> staging_upd_info_;

  bool scene_update_flag_{true};
  std::optional<std::function<void()>> ui_draw_callback_{std::nullopt};
};
}  // namespace details
using details::GGui;  // NOLINT
}  // namespace acg::gui
