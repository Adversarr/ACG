#include "acg_gui/async_gui.hpp"

#include <glm/gtc/type_ptr.hpp>
#include <memory>
#include <mutex>

#include "acg_core/geometry/common_models.hpp"
#include "acg_gui/backend/context.hpp"
#include "acg_gui/backend/graphics_pass.hpp"
#include "acg_gui/backend/mesh_ppl.hpp"
#include "acg_gui/backend/point_ppl.hpp"
#include "acg_gui/backend/ui_pass.hpp"
#include "acg_gui/backend/wireframe_ppl.hpp"
#include "acg_gui/convent.hpp"
#include "imgui_impl_glfw.h"
#include "imgui_impl_vulkan.h"

#define ASSERT_IS_HOST()                            \
  ACG_CHECK(std::this_thread::get_id() == host_id_, \
            "This function can only be called in host thread.");

#define ASSERT_IS_DEVICE()                            \
  ACG_CHECK(std::this_thread::get_id() == device_id_, \
            "This function can only be called in device thread.")

namespace acg::gui {

namespace details {

// Default staging buffer size = 1MB
constexpr vk::DeviceSize default_staging_buffer_size = 1024 * 1024;

}  // namespace details

AsyncGui::Config::Config() {
  host_id = std::this_thread::get_id();
  light.light_position_ = Vec3f(3, 0, 3);
  light.light_density_ = .8f;
  light.light_color_ = Vec3f(.7, .7, .7);
  light.parallel_light_color_ = acg::Vec3f(1, 1, 1);
  light.parallel_light_dir_ = acg::Vec3f(0, -1, -1);
  light.parallel_light_density_ = 0.5f;

  camera.SetPosition({4, 4, 4});
  camera.SetFront({-1, -1, -1});
  camera.SetUp({0, 1, 0});
  camera.SetProjectionMode(true);

  scene.Clear();
  auto sphere = acg::geometry::sphere_20({0, 0, 0}, 1);
  scene.AddMesh()
      .SetIndices(sphere.GetFaces())
      .SetVertices(sphere.GetVertices())
      .SetNormals(sphere.GetVertices());
}
// Device Thread Implementation
class AsyncGui::AsyncGui_Impl {
public:
  struct AllocatedBuffers {
    BufferID vertex_buf_id;
    BufferID index_buf_id;
    BufferID instance_buf_id;
  };

  struct StagingBufferManager {
    struct UpdateInfo {
      BufferID dst_buf;
      vk::DeviceSize offset;
      vk::DeviceSize size;
      UpdateInfo(BufferID dst, vk::DeviceSize size) : dst_buf(dst), offset(0), size(size) {}
    };

    std::vector<UpdateInfo> update_info;

    BufferID staging_buffer_id;

    AsyncGui_Impl* parent_;

    /**
     * @brief: check the info and copy data to staging buffer.
     * @return
     *    1. Ok: no error
     *    2. OutOfRange: implies staging buffer is not large enough.
     *    2. Cancelled: staging buffer need to be flushed before this commit.
     */
    Status Commit(UpdateInfo info, void* data) {
      auto&& staging_buffer = parent_->GetBufferWithMemory(staging_buffer_id);

      // Recompute info.
      if (!update_info.empty()) {
        info.offset = update_info.back().offset + update_info.back().size;
      }

      // Check if staging buffer is large enough
      vk::DeviceSize expected_end_position = info.offset + info.size;
      if (expected_end_position > staging_buffer.GetSize()) {
        if (info.offset == 0) {
          return Status::kOutOfRange;
        } else {
          return Status::kCancelled;
        }
      }

      // Ok, do commit:
      update_info.push_back(info);
      char* staging_buffer_mapped_begin = static_cast<char*>(staging_buffer.GetMappedMemory());
      staging_buffer_mapped_begin += info.offset;
      memcpy(staging_buffer_mapped_begin, data, info.size);
      return Status::kOk;
    }

    void Flush() {
      if (update_info.empty()) {
        ACG_DEBUG_LOG("Nothing to flush for staging buffer.");
        return;
      }

      auto&& staging_buffer = parent_->GetBufferWithMemory(staging_buffer_id);
      auto command = VkGraphicsContext::Instance().BeginSingleTimeCommand();
      ACG_DEBUG_LOG("Flush staging buffer, dst buffer count = {}", update_info.size());
      for (const auto& info : update_info) {
        ACG_DEBUG_LOG("Flush dst_buffer_id={}, copy_region=[{}, +{}]", info.dst_buf, info.offset,
                      info.size);
        auto&& dst_buf = parent_->GetBufferWithMemory(info.dst_buf);
        vk::BufferCopy copy_region;
        copy_region.setSrcOffset(info.offset);
        copy_region.setSize(info.size);
        command.copyBuffer(staging_buffer.GetBuffer(), dst_buf.GetBuffer(), copy_region);
      }
      VkGraphicsContext::Instance().EndSingleTimeCommand(command);
      // wait for flush complete.
      VkContext2::Instance().graphics_queue_.waitIdle();
      ACG_DEBUG_LOG("Flush complete.");
    }
  };

  void RecreateSwapchain() const;
  StagingBufferManager staging_buffer_manager_;

  explicit AsyncGui_Impl(AsyncGui* parent);

  ~AsyncGui_Impl();

  Status Render();
  // Initializers
  void InitStagingBuffer();
  void InitRenderPasses();
  void InitPipelines();

  // Destroy all buffers.
  void DestroyAllBuffers();

  // Event loop
  [[nodiscard]] acg::Status ProcessEvents(std::vector<GuiEvent>& events);
  std::vector<GuiEvent> PollEvents();

  // Update commands
  void RebuildAllBuffers();
  void UpdateMeshBuffer(size_t mesh_id, const Scene2::Mesh& mesh);
  void UpdateMeshParticleBuffer(size_t mesh_id, const Scene2::Particles& particles);
  void UpdateParticleBuffer(size_t particle_id, const Scene2::Particles& particles);
  void UpdateWireframeBuffer(size_t wf_id, const Scene2::Wireframe& wireframe);

  // Register all the allocated buffers, destroy when ~AsyncGui is called.
  std::vector<BufferWithMemory> allocated_buffers_;

  // Mesh pipeline and buffers:
  std::unique_ptr<acg::gui::details::MeshPipeline> mesh_pipeline_;
  std::vector<uint32_t> mesh_index_count_;
  std::vector<uint32_t> mesh_instance_count_;
  std::vector<AllocatedBuffers> mesh_buffers_;
  std::vector<details::MeshPushConstants> mesh_push_constants_;
  std::vector<AllocatedBuffers> mesh_particle_buffers_;

  // Wireframe pipeline and buffers:
  std::unique_ptr<details::WireframePipeline> wireframe_pipeline_;
  std::vector<AllocatedBuffers> wireframe_buffers_;
  // Special wireframe buffer:
  AllocatedBuffers xy_plane_buffer_;

  // Point pipeline and buffers:
  std::unique_ptr<details::PointPipeline> point_pipeline_;
  std::vector<AllocatedBuffers> point_buffers_;

  // Render passes:
  std::unique_ptr<details::GraphicsRenderPass> graphics_pass_;
  std::unique_ptr<details::UiPass> ui_pass_;

  // Scene backup.
  Scene2 scene_;
  Camera camera_;
  Light light_;
  XyPlaneInfo xy_plane_info_;
  AsyncGui* parent_;

  // Allocate Mesh buffer
  BufferID AllocateMesh(const Scene2::Mesh& mesh);

  AllocatedBuffers& AllocateMeshParticle(const Scene2::Particles& particles);

  // Allocate Particle buffer
  AllocatedBuffers& AllocateParticle(const Scene2::Particles& particles);

  // Allocate Wireframe
  AllocatedBuffers& AllocateWireframe(const Scene2::Particles& wireframe);

  BufferWithMemory& GetBufferWithMemory(BufferID id);

  Result<BufferID> AllocateBuffer(vk::DeviceSize size, vk::BufferUsageFlags usage,
                                  vk::MemoryPropertyFlags properties);

  Status BufferReserveSize(BufferID id, vk::DeviceSize size);

  Status PrepareMeshBuffer(const Scene2::Mesh& mesh);

  Status PreparePointBuffer(const Scene2::Particles& particle);

  Status PrepareMeshPointBuffer(const Scene2::Particles& particle);

  Status PrepareWireframeBuffer(const Scene2::Wireframe& wireframe);
};

/************* Ctor *************/

AsyncGui::AsyncGui(const AsyncGui::Config& config)
    : ignore_unprocessed_device_event(config.ignore_unprocessed_device_event),
      try_lock_duration_(config.device_lock_try_duration),
      try_lock_rounds_(config.device_lock_try_rounds) {
  // Construct in host thread
  host_id_ = std::this_thread::get_id();
  should_shutdown_ = false;
  is_running_ = true;
  light_ = config.light;
  camera_ = config.camera;
  scene_ = config.scene;

  // start child thread.
  child_thread_ = std::make_unique<std::thread>([this]() {
    this->device_id_ = std::this_thread::get_id();
    this->is_running_ = true;
    ACG_DEBUG_LOG("Device thread start.");
    this->Run();
    this->is_running_ = false;
    ACG_DEBUG_LOG("Device thread exit.");
  });
}

/************* Sync Interface *************/

acg::Status AsyncGui::SyncHostToDevice() {
  ASSERT_IS_DEVICE();
  auto lock = Lock();
  ACG_DEBUG_LOG("Device thread has acquired the lock.");
  auto result = ProcessEvents();
  ACG_DEBUG_LOG("Device thread sync done.");
  return result;
}

Status AsyncGui::ProcessEvents() {
  auto lock = TryLock();
  auto status = Status::kOk;
  if (lock.owns_lock()) {
    ACG_DEBUG_LOG("Device thread got the lock, process event begin!");
    status = impl_->ProcessEvents(host_events_);
  } else {
    status = Status::kDeadlineExceeded;
  }
  return status;
}

void AsyncGui::Run() {
  ASSERT_IS_DEVICE();
  impl_ = std::make_unique<AsyncGui_Impl>(this);
  {
    auto lock = Lock();
    ACG_DEBUG_LOG("Setup device scene, camera and light.");
    impl_->scene_ = scene_;
    impl_->camera_ = camera_;
    impl_->light_ = light_;
    impl_->xy_plane_info_ = xy_plane_info_;
  }
  should_shutdown_ = false;
  while (!should_shutdown_.load()) {
    std::this_thread::sleep_for(std::chrono::milliseconds(30));
    Status status = ProcessEvents();
    if (status == Status::kCancelled) {
      ACG_DEBUG_LOG("ProcessEvents returns cancelled, shutdown now.");
      should_shutdown_ = true;
    } else if (status == Status::kDeadlineExceeded) {
      ACG_WARN("Failed to acquire the lock. Host events are not processed.");
    } else if (status != Status::kOk) {
      ACG_ERROR("ProcessEvents failed, see console log for more details.");
      should_shutdown_ = true;
    }

    if (should_shutdown_) {
      break;
    }

    // Do rendering:
    auto result = impl_->Render();
    if (result != Status::kOk) {
      impl_->RecreateSwapchain();
    }
    auto events = impl_->PollEvents();
    if (!events.empty()) {
      ACG_DEBUG_LOG("Got device events, count = {}", events.size());
      if (!device_events_.empty() && !ignore_unprocessed_device_event) {
        ACG_WARN("Host thread has unprocessed events, count = {}", device_events_.size());
      }
      auto lock = Lock();
      device_events_ = events;
    }
  }

  VkContext2::Instance().device_.waitIdle();
  impl_.reset();
  ACG_INFO("Device thread shutdown.");
}

/************* Async Locker *************/

std::unique_lock<std::timed_mutex> AsyncGui::TryLock() {
  auto lock = Lock(std::try_to_lock);
  if (!lock.owns_lock()) {
    for (uint32_t i = 0; i < try_lock_rounds_; ++i) {
      ACG_INFO("Trying to lock mutex... {} / {}", i, try_lock_rounds_);
      lock.try_lock_for(std::chrono::milliseconds(try_lock_duration_));
      if (lock.owns_lock()) {
        break;
      }
    }
  }
  return lock;
}

std::unique_lock<std::timed_mutex> AsyncGui::Lock() {
  return std::unique_lock<std::timed_mutex>(mutex_);
}

std::unique_lock<std::timed_mutex> AsyncGui::Lock(std::defer_lock_t) {
  return std::unique_lock<std::timed_mutex>(mutex_, std::defer_lock);
}

std::unique_lock<std::timed_mutex> AsyncGui::Lock(std::try_to_lock_t) {
  return std::unique_lock<std::timed_mutex>(mutex_, std::try_to_lock);
}

/************* Plane controller *************/

acg::Status AsyncGui::EnableXyPlaneDraw(acg::Vec4f color, acg::Vec2f range, acg::Vec2f density,
                                        F32 height) {
  if (range.x() <= 0 || range.y() <= 0) {
    return Status::kInvalidArgument;
  }

  if (density.x() <= 0 || density.y() <= 0) {
    return Status::kInvalidArgument;
  }

  xy_plane_info_.density = density;
  xy_plane_info_.color = color;
  xy_plane_info_.enable = true;
  xy_plane_info_.height = height;
  RegisterHostEvent(GuiEvent(GuiEvent::Kind::kXyPlaneUpdate));
  return Status::kOk;
}

void AsyncGui::DisableXyPlaneDraw() {
  xy_plane_info_.enable = false;
  RegisterHostEvent(GuiEvent(GuiEvent::Kind::kXyPlaneUpdate));
}

/************* GETTER SETTER *************/

AsyncGui& AsyncGui::SetUIDrawCallback(std::function<void()> callback) {
  ui_draw_callback_ = callback;
  return *this;
}

Scene2& AsyncGui::GetScene() { return scene_; }
Camera& AsyncGui::GetCamera() { return camera_; }
Light& AsyncGui::GetLight() { return light_; }

/************* Event Register *************/

AsyncGui& AsyncGui::RegisterDeviceEvent(const GuiEvent& event) {
  ASSERT_IS_DEVICE();
  device_events_.push_back(event);
  return *this;
}
AsyncGui& AsyncGui::RegisterHostEvent(const GuiEvent& event) {
  ASSERT_IS_HOST();
  host_events_.push_back(event);
  return *this;
}

/************* Shutdown *************/

void AsyncGui::SignalShutdown(bool need_lock) {
  if (need_lock) {
    auto lock = Lock();
    RegisterHostEvent(GuiEvent(GuiEvent::Kind::kShutdown));
  } else {
    RegisterHostEvent(GuiEvent(GuiEvent::Kind::kShutdown));
  }
}

void AsyncGui::ShutdownImmediately() { should_shutdown_ = true; }

/************* Impl part *************/
AsyncGui::AsyncGui_Impl::AsyncGui_Impl(AsyncGui* parent) : parent_(parent) {
  // Init all the pipelines and render pass
  InitRenderPasses();
  InitPipelines();

  // Init staging buffer
  InitStagingBuffer();
  // RebuildAllBuffers();
}

void AsyncGui::AsyncGui_Impl::InitRenderPasses() {
  using namespace acg::gui::details;

  GraphicsRenderPass::Config config;
  config.is_present = false;
  config.max_descriptor_set_count = 24;
  config.required_descriptor_sizes.emplace_back(vk::DescriptorType::eUniformBuffer, 48);
  graphics_pass_ = std::make_unique<GraphicsRenderPass>(config);

  UiPass::Config ui_config;
  config.is_present = true;
  ui_pass_ = std::make_unique<UiPass>(ui_config);

  ACG_DEBUG_LOG("Render passes created.");
}

void AsyncGui::AsyncGui_Impl::InitPipelines() {
  using namespace acg::gui::details;
  // Mesh
  MeshPipeline::Config mesh_config;
  mesh_config.enable_color_blending = true;
  mesh_config.cull_mode_ = vk::CullModeFlagBits::eNone;
  mesh_pipeline_ = std::make_unique<MeshPipeline>(*graphics_pass_, mesh_config);

  // Wireframe
  WireframePipeline::Config wf_config;
  wireframe_pipeline_ = std::make_unique<WireframePipeline>(*graphics_pass_, wf_config);

  // Point
  PointPipeline::Config point_config;
  point_config.enable_color_blending = true;
  point_pipeline_ = std::make_unique<PointPipeline>(*graphics_pass_, point_config);
  ACG_DEBUG_LOG("Pipeline Created.");
}

void AsyncGui::AsyncGui_Impl::InitStagingBuffer() {
  // Allocate buffer:
  auto bm = AllocateBuffer(
      details::default_staging_buffer_size, vk::BufferUsageFlagBits::eTransferSrc,
      vk::MemoryPropertyFlagBits::eHostCoherent | vk::MemoryPropertyFlagBits::eHostVisible);
  ACG_CHECK(bm.HasValue(), "Failed to init staging buffer.");
  staging_buffer_manager_.parent_ = this;
  staging_buffer_manager_.staging_buffer_id = bm.Value();
  ACG_DEBUG_LOG("Staging buffer created, size = {} Bytes", details::default_staging_buffer_size);
}

void AsyncGui::AsyncGui_Impl::DestroyAllBuffers() {
  for (auto& buf : allocated_buffers_) {
    VkContext2::Instance().DestroyBufferWithMemory(buf);
  }
  allocated_buffers_.clear();
}

AsyncGui::AsyncGui_Impl::~AsyncGui_Impl() {
  // TODO: Destroy everyting.
  DestroyAllBuffers();

  mesh_pipeline_.reset();
  point_pipeline_.reset();
  wireframe_pipeline_.reset();
  ui_pass_.reset();
  graphics_pass_.reset();
}

Status AsyncGui::AsyncGui_Impl::ProcessEvents(std::vector<GuiEvent>& events) {
  for (auto event : events) {
    if (event.kind == GuiEvent::Kind::kShutdown) {
      return Status::kCancelled;
    }
  }

  events.clear();
  return Status::kOk;
}

Status AsyncGui::AsyncGui_Impl::Render() {
  ACG_DEBUG_LOG("Renderer.");
  // TODO: Render

  // NOTE: glfwPollEvents();

  // Render.
  auto result = acg::gui::VkGraphicsContext::Instance().BeginDraw();
  if (!result) {
    return Status::kCancelled;
  }
  auto cbuf = graphics_pass_->BeginRender();
  mesh_pipeline_->BeginPipeline(cbuf);
  for (size_t i = 0; i < mesh_buffers_.size(); ++i) {
    const auto& buf = mesh_buffers_[i];
    const auto& pc = mesh_push_constants_[i];
    cbuf.bindVertexBuffers(0, GetBufferWithMemory(buf.vertex_buf_id).GetBuffer(),
                           static_cast<vk::DeviceSize>(0));
    cbuf.bindVertexBuffers(1, GetBufferWithMemory(buf.instance_buf_id).GetBuffer(),
                           static_cast<vk::DeviceSize>(0));
    cbuf.bindIndexBuffer(GetBufferWithMemory(buf.index_buf_id).GetBuffer(), 0,
                         vk::IndexType::eUint32);
    cbuf.pushConstants(mesh_pipeline_->GetPipelineLayout(),
                       vk::ShaderStageFlagBits::eFragment | vk::ShaderStageFlagBits::eVertex, 0,
                       sizeof(pc), &pc);
    cbuf.drawIndexed(mesh_index_count_[i], mesh_instance_count_[i], 0, 0, 0);
  }
  graphics_pass_->EndRender();

  ImGui_ImplVulkan_NewFrame();
  ImGui_ImplGlfw_NewFrame();
  ImGui::NewFrame();
  ImGui::SetNextWindowBgAlpha(0.3);
  ImGui::SetNextWindowPos(ImVec2(0, 0));
  ImGui::Begin("Control", nullptr, ImGuiWindowFlags_NoMove);
  ImGui::LogText("Hello!");
  ImGui::End();
  ImGui::Render();
  auto* data = ImGui::GetDrawData();
  auto ui_cbuf = ui_pass_->Render(data);

  auto result2 = acg::gui::VkGraphicsContext::Instance().EndDraw({cbuf, ui_cbuf});
  if (result2) {
    return Status::kCancelled;
  }
  return Status::kOk;
}

std::vector<GuiEvent> AsyncGui::AsyncGui_Impl::PollEvents() {
  // TODO: Events
  return {};
}

AsyncGui::~AsyncGui() {
  if (is_running_) {
    ShutdownImmediately();
    ACG_INFO("Waiting for shutdown.");
    WaitForShutdownActually();
  }
  ACG_DEBUG_LOG("Exit async gui.");
}

Result<AsyncGui::BufferID> AsyncGui::AsyncGui_Impl::AllocateBuffer(
    vk::DeviceSize size, vk::BufferUsageFlags usage, vk::MemoryPropertyFlags properties) {
  allocated_buffers_.push_back(
      VkContext2::Instance().CreateBufferWithMemory(size, usage, properties));
  return make_result<BufferID>(allocated_buffers_.size() - 1);
}

void AsyncGui::WaitForShutdownActually() const {
  int t = 1;
  while (is_running_.load()) {
    ACG_DEBUG_LOG("Wait for shutdown, time = {}", t);
    std::this_thread::sleep_for(std::chrono::milliseconds(100));
  }

  child_thread_->join();
}

void AsyncGui::AsyncGui_Impl::RebuildAllBuffers() {
  auto status = Status::kOk;
  for (const auto& mesh : scene_.GetMesh()) {
    status = PrepareMeshBuffer(mesh);
    ACG_CHECK(status == Status::kOk, "Failed to prepare mesh buffer.");
  }

  // TODO: add support.
  // for (const auto& particle : scene_.GetParticles()) {
  //   if (particle.use_instance_rendering) {
  //     PrepareMeshPointBuffer(particle);
  //   } else {
  //     PreparePointBuffer(particle);
  //   }
  // }
  //
  // for (const auto& wf: scene_.GetWireframe()) {
  //   PrepareWireframeBuffer(wf);
  // }
}

Status AsyncGui::AsyncGui_Impl::PrepareMeshBuffer(const Scene2::Mesh& mesh) {
  auto vert_count = FieldCAccess{mesh.vertices}.Size();
  auto face_count = FieldCAccess{mesh.faces}.Size();
  auto instance_count = mesh.instance_count;
  auto vert_buffer_size = static_cast<vk::DeviceSize>(vert_count * sizeof(details::MeshVertex));
  auto index_buffer_size = static_cast<vk::DeviceSize>(face_count * 3 * sizeof(uint32_t));
  auto instance_buffer_size = instance_count * sizeof(details::MeshInstance);
  auto mesh_id = mesh.id;
  if (mesh_id >= mesh_buffers_.size()) {
    // init buffer first.
    AllocatedBuffers bid;
    auto r_v = AllocateBuffer(
        vert_buffer_size,
        vk::BufferUsageFlagBits::eVertexBuffer | vk::BufferUsageFlagBits::eTransferDst,
        vk::MemoryPropertyFlagBits::eDeviceLocal);
    if (r_v.HasValue()) {
      bid.vertex_buf_id = r_v.Value();
    } else {
      return r_v.Error();
    }

    auto r_id = AllocateBuffer(
        index_buffer_size,
        vk::BufferUsageFlagBits::eIndexBuffer | vk::BufferUsageFlagBits::eTransferDst,
        vk::MemoryPropertyFlagBits::eDeviceLocal);

    if (r_id.HasValue()) {
      bid.index_buf_id = r_id.Value();
    } else {
      return r_id.Error();
    }

    auto r_it = AllocateBuffer(
        index_buffer_size,
        vk::BufferUsageFlagBits::eVertexBuffer | vk::BufferUsageFlagBits::eTransferDst,
        vk::MemoryPropertyFlagBits::eDeviceLocal);

    if (r_it.HasValue()) {
      bid.instance_buf_id = r_it.Value();
    } else {
      return r_it.Error();
    }
    mesh_buffers_.push_back(bid);
    mesh_index_count_.push_back(index_buffer_size);
    mesh_instance_count_.push_back(instance_count);
    details::MeshPushConstants pc;
    mesh_push_constants_.push_back(pc);
  } else {
    // only make sure that the buffer is large enough.
    // TODO: check status.
    auto bid = mesh_buffers_[mesh_id];
    BufferReserveSize(bid.vertex_buf_id, vert_buffer_size);
    BufferReserveSize(bid.index_buf_id, index_buffer_size);
    BufferReserveSize(bid.instance_buf_id, instance_buffer_size);
    mesh_instance_count_[mesh_id] = instance_count;
    mesh_index_count_[mesh_id] = 3 * face_count;
  }

  // Update the buffer:
  UpdateMeshBuffer(mesh_id, mesh);
  // 1. vertex buffer:
  return Status::kOk;
}

void AsyncGui::AsyncGui_Impl::UpdateMeshBuffer(size_t mesh_id, const Scene2::Mesh& mesh) {
  auto vert_count = FieldCAccess{mesh.vertices}.Size();
  auto face_count = FieldCAccess{mesh.faces}.Size();
  auto instance_count = mesh.instance_count;
  auto vert_buffer_size = static_cast<vk::DeviceSize>(vert_count * sizeof(details::MeshVertex));
  auto index_buffer_size = static_cast<vk::DeviceSize>(face_count * 3 * sizeof(uint32_t));
  auto instance_buffer_size = instance_count * sizeof(details::MeshInstance);
  auto bid = mesh_buffers_[mesh_id];
  std::vector<details::MeshVertex> vert_buffer_content;
  for (Idx i = 0; i < vert_count; ++i) {
    auto p = mesh.vertices.col(i);
    auto c = mesh.color.col(i);
    auto n = mesh.normals.col(i);
    details::MeshVertex v;
    v.position_ = glm::vec3(p.x(), p.y(), p.z());
    v.color_ = glm::vec4(c.x(), c.y(), c.z(), c.w());
    v.normal_ = glm::vec3(n.x(), n.y(), n.z());
    vert_buffer_content.push_back(v);
  }
  staging_buffer_manager_.Commit(
      StagingBufferManager::UpdateInfo(vert_buffer_size, bid.vertex_buf_id),
      vert_buffer_content.data());
  staging_buffer_manager_.Flush();

  std::vector<uint32_t> index_buffer_content;
  for (Idx i = 0; i < face_count; ++i) {
    index_buffer_content.push_back(mesh.faces.col(i).x());
    index_buffer_content.push_back(mesh.faces.col(i).y());
    index_buffer_content.push_back(mesh.faces.col(i).z());
  }
  staging_buffer_manager_.Commit(
      StagingBufferManager::UpdateInfo(index_buffer_size, bid.index_buf_id),
      index_buffer_content.data());
  staging_buffer_manager_.Flush();

  std::vector<details::MeshInstance> instance_buffer_content;
  for (Idx i = 0; static_cast<size_t>(i) < mesh.instance_count; ++i) {
    details::MeshInstance inst;
    if (i >= mesh.instance_position.cols()) {
      inst.position = glm::vec3(0, 0, 0);
    } else {
      Vec3f p = mesh.instance_position.col(i);
      inst.position = to_glm(p);
    }
    if (i >= mesh.instance_rotation.cols()) {
      inst.rotation = glm::vec4(1, 0, 0, 0);
    } else {
      Vec4f r = mesh.instance_rotation.col(i);
      inst.rotation = glm::vec4(r.x(), r.y(), r.z(), r.w());
    }
    instance_buffer_content.push_back(inst);
  }
  staging_buffer_manager_.Commit(
      StagingBufferManager::UpdateInfo(instance_buffer_size, bid.instance_buf_id),
      instance_buffer_content.data());
  staging_buffer_manager_.Flush();
  Mat4x4f model_t = mesh.model.transpose();
  mesh_push_constants_[mesh_id].model = glm::make_mat4x4(model_t.data());
}

BufferWithMemory& AsyncGui::AsyncGui_Impl::GetBufferWithMemory(BufferID id) {
  return allocated_buffers_[id];
}
Status AsyncGui::AsyncGui_Impl::BufferReserveSize(BufferID id, vk::DeviceSize size) {
  auto& buffer = GetBufferWithMemory(id);
  if (buffer.GetSize() >= size) {
    return Status::kCancelled;
  }

  VkContext2::Instance().DestroyBufferWithMemory(buffer);
  buffer = VkContext2::Instance().CreateBufferWithMemory(size, buffer.usage_, buffer.properties_);

  return Status::kOk;
}

void AsyncGui::AsyncGui_Impl::RecreateSwapchain() const {
  ACG_DEBUG_LOG("Recreate swapchain.");
  VkGraphicsContext::Instance().RecreateSwapchain();
  graphics_pass_->RecreateSwapchain();
  ui_pass_->RecreateSwapchain();
  mesh_pipeline_->SetCamera(camera_);
  mesh_pipeline_->UpdateUbo();
  wireframe_pipeline_->SetCamera(camera_);
  wireframe_pipeline_->UpdateUbo();
  point_pipeline_->SetCamera(camera_);
  point_pipeline_->UpdateUbo();
}

}  // namespace acg::gui
