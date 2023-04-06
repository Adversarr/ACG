#pragma once

#include "agui/backend/context.hpp"
#include "agui/backend/graphics_pass.hpp"
#include "agui/backend/mesh_ppl.hpp"
#include "agui/backend/point_ppl.hpp"
#include "agui/backend/ui_pass.hpp"
#include "agui/backend/wireframe_ppl.hpp"
#include "agui/convent.hpp"
#include "imgui_impl_glfw.h"
#include "imgui_impl_vulkan.h"
#include "scene.hpp"

namespace acg::gui {
namespace details {

struct XyPlaneRenderInfo {
  Vec2f range;
  Vec4f color;
  Vec2Index density;
  float height;
  bool enable = false;
  bool update_flag = false;

  void MarkUpdate() { update_flag = true; }
};

struct GuiEvent {
  enum class Kind {
    kShutdown,
    kSceneFullUpdate,
    kSceneMeshUpdate,
    kSceneParticleUpdate,
    kSceneMeshParticleUpdate,
    kSceneWireframeUpdate,
    kXyPlaneUpdate,
    kCameraUpdate,
    kLightUpdate,
  };

  enum Component : int { kVertex = 1, kIndex = 2, kInstance = 4, kModel = 8, kAll = 15 };

  Kind kind;
  int component;
  size_t object_id;

  // all update flag
  static constexpr size_t ALL_UPD_ = std::numeric_limits<size_t>::max();

  explicit GuiEvent(Kind k, size_t o, int component)
      : kind(k), component(component), object_id(o) {}

  explicit GuiEvent(Kind k, size_t o) : GuiEvent(k, o, kAll) {}

  explicit GuiEvent(Kind k) : GuiEvent(k, ALL_UPD_) {}

  bool IsAllUpdate() const { return object_id == ALL_UPD_; }
};

using BufferID = size_t;
constexpr BufferID invalid_buffer = std::numeric_limits<BufferID>::max();

struct MeshRenderInfo {
  BufferID vertex{invalid_buffer};
  BufferID index{invalid_buffer};
  BufferID instance{invalid_buffer};

  BufferID wireframe_vertex{invalid_buffer};
  BufferID wireframe_index{invalid_buffer};

  bool enable_wireframe{false};
  uint32_t vertex_count;
  uint32_t index_count;
  uint32_t instance_count;
  MeshPushConstants pc;
  glm::mat4 model;
};

struct ParticleRenderInfo {
  BufferID vertex{invalid_buffer};
  uint32_t vertex_count;
  PointPushConstants pc;

  ParticleRenderInfo() : vertex(invalid_buffer), vertex_count(0) {}
};

struct WireframeRenderInfo {
  BufferID vertex{invalid_buffer};
  BufferID index{invalid_buffer};
  uint32_t vertex_count;
  uint32_t index_count;

  WireframeRenderInfo() : vertex(invalid_buffer), index(invalid_buffer) {}
};

struct StagingUpdateInfo {
  vk::Buffer dst_buf;
  size_t dst_buffer_id;
  vk::DeviceSize offset;
  vk::DeviceSize size;
  StagingUpdateInfo(size_t dst, vk::DeviceSize size) : dst_buffer_id(dst), offset(0), size(size) {}
};

class Gui {
public:
  struct Config {
    vk::DeviceSize staging_buffer_size = 16 * 1024 * 1024;
    bool enable_blending = false;
    bool init_default_scene = false;
    void Hook() const;
  };
  static Gui& Instance();

  Scene2& GetScene() { return scene_; }

  Light& GetLight() { return light_; }

  Camera& GetCamera() { return camera_; }

  void UpdateScene(bool force_all = false);

  void ClearScene();

  void UpdateLightCamera();

  void SetUIDrawCallback(std::function<void()> callback) { ui_draw_callback_ = callback; }

  void RenderOnce(bool verbose = false);

  void DrawDefaultUI();

  explicit Gui(const Config& config);

  void Tick(bool glfw_poll = false);

  ~Gui();

private:
  void ProcessCamera();

  void PrepareBuffers();
  void PrepareXyPlaneBuffer();
  void PrepareMeshBuffer(MeshRenderInfo& info);
  void PrepareParticleBuffer(ParticleRenderInfo& info);
  void PrepareWireframeBuffer(WireframeRenderInfo& info);
  void PrepareMeshWireframeBuffer(MeshRenderInfo& info);

  void FillBuffers(bool force);
  void FillXyPlaneBuffers();
  void FillMeshBuffer(const Scene2::Mesh& mesh, const MeshRenderInfo& info);
  void FillMeshParticleBuffer(const Scene2::Particles& particle, const MeshRenderInfo& info);
  void FillParticleBuffer(const Scene2::Particles& particle, const ParticleRenderInfo& info);
  void FillWireframeBuffer(const Scene2::Wireframe& wireframe, const WireframeRenderInfo& info);

  BufferID CreateVertexBuffer(vk::DeviceSize size);
  BufferID CreateInstanceBuffer(vk::DeviceSize size);
  BufferID CreateIndexBuffer(vk::DeviceSize size);

  void PrepareVertexBufferHelper(vk::DeviceSize required_size, BufferID& id);
  void PrepareIndexBufferHelper(vk::DeviceSize required_size, BufferID& id);

  void ReserveBuffer(vk::DeviceSize size, BufferID id);

  void InitStagingBuffer();

  void DestroyStagingBuffer();

  void InitDefaultScene(bool init_default_scene);

  void Destroy();

  void TryUpdateLinewidth();

  void RecreateSwapchain();

  Status TryCommitDeferred(StagingUpdateInfo update_info, void* data);

  Status FlushStagingBuffer();

  Status CommitStagingBuffer(StagingUpdateInfo update_info, void* data,
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
  WireframeRenderInfo xy_plane_render_info_;
  // Point
  std::unique_ptr<PointPipeline> point_pipeline_;
  std::vector<ParticleRenderInfo> particle_render_info_;

  // Internal data.
  Scene2 scene_;
  Camera camera_;
  Light light_;
  bool disable_camera_up_update_{true};
  XyPlaneRenderInfo xy_plane_info_;
  float camera_moving_speed{1.0f};

  using TimePoint = std::chrono::time_point<std::chrono::steady_clock>;
  std::array<float, 32> render_time_samples_;
  size_t render_index_{0};
  TimePoint last_time;
  std::chrono::duration<float, std::milli> dt;
  float linewidth_{1.0f};

  Vec4f mesh_outline_color_ = {.1, .1, .1, 1.0};
  // Staging buffer and manipulator
  BufferWithMemory staging_buffer_;
  std::vector<StagingUpdateInfo> staging_upd_info_;

  bool scene_update_flag_{true};
  std::optional<std::function<void()>> ui_draw_callback_{std::nullopt};
  
  vk::CommandBuffer &WriteMeshRenderCommand(bool verbose, vk::CommandBuffer &cbuf);
  
  vk::CommandBuffer &WriteMeshParticleRenderCommand(bool verbose, vk::CommandBuffer &cbuf);
  
  vk::CommandBuffer &WritePointRenderCommand(bool verbose, vk::CommandBuffer &cbuf);
  
  vk::CommandBuffer &WriteWireframeRenderCommand(bool verbose, vk::CommandBuffer &cbuf);
};

}  // namespace details
using details::Gui;
}  // namespace acg::gui
