#include "acg_gui/async_gui.hpp"

#include <memory>
#include <mutex>

#include "acg_gui/backend/context.hpp"
#include "acg_gui/backend/graphics_pass.hpp"
#include "acg_gui/backend/mesh_ppl.hpp"
#include "acg_gui/backend/point_ppl.hpp"
#include "acg_gui/backend/ui_pass.hpp"
#include "acg_gui/backend/wireframe_ppl.hpp"

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

std::unique_ptr<AsyncGui> async_gui_instance;

}  // namespace details

// Device Thread Implementation
class AsyncGui::AsyncGui_Impl {
public:
  using BufferID = size_t;
  struct AllocatedBuffers {
    BufferID vertex_buf_id;
    std::optional<BufferID> index_buf_id;
    std::optional<BufferID> instance_buf_id;
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

  StagingBufferManager staging_buffer_manager_;

  explicit AsyncGui_Impl(AsyncGui* parent);

  Status Render();
  // Initializers
  void InitStagingBuffer();
  void InitRenderPasses();
  void InitPipelines();

  // Destroy all buffers.
  void DestroyAllBuffers();
  void BuildBuffers();

  // Event loop
  [[nodiscard]] acg::Status ProcessEvents(std::vector<GuiEvent>& events);
  std::vector<GuiEvent> PollEvents();

  // Update commands
  void RebuildAllBuffers();
  void UpdateMeshBuffer(AllocatedBuffers& buffers, const Scene2::Mesh& mesh);
  void UpdateMeshParticleBuffer(AllocatedBuffers& buffers, const Scene2::Particles& particles);
  void UpdateParticleBuffer(AllocatedBuffers& buffers, const Scene2::Particles& particles);
  void UpdateWireframeBuffer(AllocatedBuffers& buffer, const Scene2::Wireframe& wireframe);

  // Register all the allocated buffers, destroy when ~AsyncGui is called.
  std::vector<BufferWithMemory> allocated_buffers_;
  Status BufferReserveSize(BufferID id, vk::DeviceSize size);

  // Mesh pipeline and buffers:
  std::unique_ptr<acg::gui::details::MeshPipeline> mesh_pipeline_;
  std::vector<AllocatedBuffers> mesh_buffers_;
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
  std::unique_ptr<Scene2> scene_;
  AsyncGui* parent_;

  // Allocate Mesh buffer
  AllocatedBuffers& AllocateMesh(const Scene2::Mesh& mesh);

  AllocatedBuffers& AllocateMeshParticle(const Scene2::Particles& particles);

  // Allocate Particle buffer
  AllocatedBuffers& AllocateParticle(const Scene2::Particles& particles);

  // Allocate Wireframe
  AllocatedBuffers& AllocateWireframe(const Scene2::Particles& wireframe);

  BufferWithMemory& GetBufferWithMemory(BufferID id);

  Result<BufferID> AllocateBuffer(vk::DeviceSize size, vk::BufferUsageFlags usage,
                                  vk::MemoryPropertyFlags properties);
};

/************* Ctor *************/

AsyncGui::AsyncGui(const AsyncGui::Config& config)
    : ignore_unprocessed_device_event(config.ignore_unprocessed_device_event),
      try_lock_duration_(config.device_lock_try_duration),
      try_lock_rounds_(config.device_lock_try_rounds) {
  // Construct in host thread
  host_id_ = std::this_thread::get_id();
  should_shutdown_ = false;
  host_light_ = config.light;
  host_camera_ = config.camera;
  host_scene_ = config.scene;

  // start child thread.
  child_thread_ = std::make_unique<std::thread>([this]() {
    ACG_DEBUG_LOG("Device thread start.");
    this->Run();
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
  impl_ = std::make_unique<AsyncGui_Impl>(this);
  should_shutdown_ = false;
  ASSERT_IS_DEVICE();
  while (!should_shutdown_.load()) {
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
    impl_->Render();
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

std::unique_lock<std::timed_mutex> AsyncGui::Lock() { return std::unique_lock<std::timed_mutex>(mutex_); }

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

  xy_plane_density_ = density;
  xy_plane_color_ = color;
  xy_plane_enable_ = true;
  xy_plane_height_ = height;
  RegisterHostEvent(GuiEvent(GuiEvent::Kind::kPlaneUpdate));
  return Status::kOk;
}

void AsyncGui::DisableXyPlaneDraw() {
  xy_plane_enable_ = false;
  RegisterHostEvent(GuiEvent(GuiEvent::Kind::kPlaneUpdate));
}

/************* GETTER SETTER *************/

AsyncGui& AsyncGui::SetUIDrawCallback(std::function<void()> callback) {
  ui_draw_callback_ = callback;
  return *this;
}

Scene2& AsyncGui::GetScene() { return host_scene_; }
Camera& AsyncGui::GetCamera() { return host_camera_; }
Light& AsyncGui::GetLight() { return host_light_; }

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
  // 2. Create Mesh Pipeline.

  // Init staging buffer
  InitStagingBuffer();
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

}  // namespace acg::gui
