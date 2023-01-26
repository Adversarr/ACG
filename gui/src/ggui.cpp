#include "acg_gui/ggui.hpp"

#include <acg_core/geometry/common_models.hpp>

#include "glm/gtc/type_ptr.hpp"

namespace acg::gui::details {

static auto get_default_ball() {
  static auto mesh = geometry::sphere_20({0, 0, 0}, 1);
  return std::make_pair(mesh.GetFaces(), mesh.GetVertices());
}

GGui::GGui(const GGui::Config& config)
    : staging_buffer_(VkContext2::Instance().CreateBufferWithMemory(
        config.staging_buffer_size, vk::BufferUsageFlagBits::eTransferSrc,
        vk::MemoryPropertyFlagBits::eHostCoherent | vk::MemoryPropertyFlagBits::eHostVisible)) {
  last_time = std::chrono::steady_clock::now();
  ACG_DEBUG_LOG("Init ggui");
  // Render pass:
  UiPass::Config ui_config;
  ui_config.is_ui_only = false;
  ui_pass_ = std::make_unique<UiPass>(ui_config);
  GraphicsRenderPass::Config gr_config;
  gr_config.is_present = false;
  gr_config.max_descriptor_set_count = 24;
  gr_config.required_descriptor_sizes.push_back({vk::DescriptorType::eUniformBuffer, 48});
  graphics_pass_ = std::make_unique<GraphicsRenderPass>(gr_config);
  // Pipeline:
  MeshPipeline::Config mp_config;
  mp_config.cull_mode_ = vk::CullModeFlagBits::eNone;
  mp_config.enable_color_blending = config.enable_blending;
  mesh_pipeline_ = std::make_unique<MeshPipeline>(*graphics_pass_, mp_config);
  PointPipeline::Config pp_config;
  pp_config.enable_color_blending = config.enable_blending;
  point_pipeline_ = std::make_unique<PointPipeline>(*graphics_pass_, pp_config);
  WireframePipeline::Config wf_config;
  wireframe_pipeline_ = std::make_unique<WireframePipeline>(*graphics_pass_, wf_config);

  // Staging buffer init
  InitStagingBuffer();
  InitDefaultScene(config.init_default_scene);
  UpdateScene();
  UpdateLightCamera();
}

void GGui::InitDefaultScene(bool init_default_scene) {
  scene_.Clear();
  if (init_default_scene) {
    auto [i, v] = get_default_ball();
    scene_.AddMesh()
        .SetIndices(i)
        .SetVertices(v)
        .SetNormals(v)
        .SetUniformColor(Vec4f{.7, .7, .7, 1})
        .SetInstanceCount(1)
        .SetEnableWireframe(true)
        .MarkUpdate();

    scene_.AddParticles()
        .SetPositions(Vec3f(2, 0, 0))
        .SetUniformColor(Vec4f(1, 0, 0, 1))
        .SetRadius(32)
        .MarkUpdate();

    scene_.AddMeshParticles()
        .SetUniformColor(types::Rgba(0, 1, 0, 1))
        .SetRadius(.5)
        .SetPositions(Vec3f(0, 2, 0))
        .MarkUpdate();
  }
  light_.light_position_ = Vec3f(3, 0, 3);
  light_.light_density_ = .8f;
  light_.light_color_ = Vec3f(.7, .7, .7);
  light_.parallel_light_color_ = acg::Vec3f(1, 1, 1);
  light_.parallel_light_dir_ = acg::Vec3f(0, -1, -1);
  light_.parallel_light_density_ = 0.5f;

  camera_.SetPosition({4, 0, 1});
  camera_.SetFront({-1, 0, 0});
  camera_.SetUp({0, 0, 1});
  camera_.SetProjectionMode(true);

  xy_plane_info_.color = Vec4f(.7, .7, 0, 1);
  xy_plane_info_.update_flag = true;
  xy_plane_info_.density = Vec2Index (10, 10);
  xy_plane_info_.range = Vec2f(10, 10);
  xy_plane_info_.enable = true;
  xy_plane_info_.height = 0.0;
}

GGui::~GGui() {
  VkContext2::Instance().device_.waitIdle();

  for (auto& buffer : allocated_buffers_) {
    VkContext2::Instance().DestroyBufferWithMemory(buffer);
  }
  allocated_buffers_.clear();
  DestroyStagingBuffer();
  // Release all the resources.
  mesh_pipeline_.reset();
  point_pipeline_.reset();
  wireframe_pipeline_.reset();
  ui_pass_.reset();
  graphics_pass_.reset();
}

void GGui::InitStagingBuffer() {
  auto* p_data = VkContext2::Instance().device_.mapMemory(staging_buffer_.GetMemory(), 0,
                                                          staging_buffer_.GetSize());
  staging_buffer_.SetMappedMemory(p_data);
}

void GGui::DestroyStagingBuffer() {
  VkContext2::Instance().device_.unmapMemory(staging_buffer_.GetMemory());
  VkContext2::Instance().DestroyBufferWithMemory(staging_buffer_);
}

void GGui::ReserveBuffer(vk::DeviceSize size, BufferID id) {
  auto& buffer = GetAllocatedBuffer(id);
  if (buffer.GetSize() >= size) {
    return;
  }
  VkContext2::Instance().DestroyBufferWithMemory(buffer);
  buffer = VkContext2::Instance().CreateBufferWithMemory(size, buffer.usage_, buffer.properties_);
}

BufferID GGui::CreateVertexBuffer(vk::DeviceSize size) {
  auto id = allocated_buffers_.size();
  auto buffer = VkContext2::Instance().CreateBufferWithMemory(
      size, vk::BufferUsageFlagBits::eTransferDst | vk::BufferUsageFlagBits::eVertexBuffer,
      vk::MemoryPropertyFlagBits::eDeviceLocal);
  ACG_DEBUG_LOG("Created Vertex Buffer: id={}, size={}", id, size);
  allocated_buffers_.push_back(buffer);
  return id;
}

BufferID GGui::CreateIndexBuffer(vk::DeviceSize size) {
  auto id = allocated_buffers_.size();
  auto buffer = VkContext2::Instance().CreateBufferWithMemory(
      size, vk::BufferUsageFlagBits::eTransferDst | vk::BufferUsageFlagBits::eIndexBuffer,
      vk::MemoryPropertyFlagBits::eDeviceLocal);
  ACG_DEBUG_LOG("Created Index Buffer: id={}, size={}", id, size);
  allocated_buffers_.push_back(buffer);
  return id;
}

BufferID GGui::CreateInstanceBuffer(vk::DeviceSize size) { return CreateVertexBuffer(size); }

void GGui::UpdateScene(bool force) {
  VkContext2::Instance().device_.waitIdle();
  PrepareBuffers();
  FillBuffers(force);
}

void GGui::PrepareBuffers() {
  for (const auto& mesh : scene_.GetMesh()) {
    // collect info
    if (mesh.id >= mesh_render_info_.size()) {
      mesh_render_info_.emplace_back();
    }
    ACG_CHECK(mesh.id < mesh_render_info_.size(), "object should be added sequentially.");
    auto& info = mesh_render_info_[mesh.id];
    info.vertex_count = mesh.vertices.cols();
    info.index_count = mesh.faces.cols() * 3;
    info.instance_count = mesh.instance_count;
    info.pc.model = glm::transpose(glm::make_mat4x4(mesh.model.data()));
    info.pc.options[0] = mesh.use_double_side_lighting ? 1 : 0;
    info.pc.options[1] = mesh.specular_shiness;
    info.pc.options[2] = 0;
    info.enable_wireframe = mesh.enable_wireframe;
    PrepareMeshBuffer(info);
  }

  for (const auto& part : scene_.GetMeshParticle()) {
    if (part.id >= mesh_particle_render_info_.size()) {
      mesh_particle_render_info_.emplace_back();
    }
    ACG_CHECK(part.id < mesh_particle_render_info_.size(), "objects should be added sequentially.");

    auto& info = mesh_particle_render_info_[part.id];
    info.vertex_count = 20;
    info.index_count = 60;
    info.instance_count = part.positions.cols();
    info.pc.model = part.radius * glm::identity<glm::mat4x4>();
    info.pc.model[3][3] = 1;
    info.pc.options[0] = 0;
    info.pc.options[1] = 32;
    info.pc.options[2] = 1;
    PrepareMeshBuffer(info);
  }

  for (const auto& part : scene_.GetParticles()) {
    if (part.id >= particle_render_info_.size()) {
      particle_render_info_.emplace_back();
    }
    auto& info = particle_render_info_[part.id];
    ACG_CHECK(part.id < particle_render_info_.size(), "objects should be added sequentially.");
    if (part.colors.cols() > 0) {
      Vec4f color = part.colors.col(0);
      info.pc.color = glm::make_vec4<float>(color.data());
    } else {
      info.pc.color = glm::vec4(0.8, 0.8, 0.8, 1.0);
    }
    info.pc.options[0] = part.use_uniform_color;
    info.pc.size = part.radius;
    info.vertex_count = part.positions.cols();
    PrepareParticleBuffer(info);
  }

  for (const auto& wf : scene_.GetWireframe()) {
    if (wf.id >= wireframe_render_info_.size()) {
      wireframe_render_info_.emplace_back();
    }
    ACG_CHECK(wf.id < wireframe_render_info_.size(), "objects should be added sequentially.");
    auto& info = wireframe_render_info_[wf.id];
    info.index_count = wf.indices.cols() * 2;
    info.vertex_count = wf.positions.cols();
    PrepareWireframeBuffer(info);
  }
  PrepareXyPlaneBuffer();
}

void GGui::PrepareXyPlaneBuffer() {
  if (!xy_plane_info_.enable) {
    return;
  }
  int rows = xy_plane_info_.density.y();
  int cols = xy_plane_info_.density.x();
  // 0, 1, 2, .... cols - 1,
  // cols, ... 2 * cols - 1,
  // ...
  // rows, ...,   r * c - 1
  int seg_count = 8 * rows * cols + 2 * rows + 2 * cols;
  int vert_count = (2 * rows + 1) * (2 * cols + 1);
  xy_plane_render_info_.index_count = seg_count * 2;
  xy_plane_render_info_.vertex_count = vert_count;
  PrepareIndexBufferHelper(sizeof(uint32_t) * 2 * seg_count, xy_plane_render_info_.index);
  PrepareVertexBufferHelper(sizeof(WireframePoint) * vert_count, xy_plane_render_info_.vertex);
}

Status GGui::FlushStagingBuffer() {
  if (staging_upd_info_.empty()) {
    ACG_DEBUG_LOG("Nothing to flush for staging buffer.");
    return Status::kOk;
  }

  auto command = VkGraphicsContext::Instance().BeginSingleTimeCommand();
  ACG_DEBUG_LOG("Flush staging buffer, dst buffer count = {}", staging_upd_info_.size());
  for (const auto& info : staging_upd_info_) {
    ACG_DEBUG_LOG("Flush dst_buffer_id={}, copy_region=[{}, {})", info.dst_buffer_id, info.offset,
                  info.offset + info.size);
    vk::BufferCopy copy_region;
    copy_region.setSrcOffset(info.offset);
    copy_region.setSize(info.size);
    command.copyBuffer(staging_buffer_.GetBuffer(), info.dst_buf, copy_region);
  }
  VkGraphicsContext::Instance().EndSingleTimeCommand(command);
  // wait for flush complete.
  VkContext2::Instance().graphics_queue_.waitIdle();
  ACG_DEBUG_LOG("Flush complete.");
  staging_upd_info_.clear();
  return Status::kOk;
}

Status GGui::TryCommitDeferred(StagingUpdateInfo info, void* data) {
  auto status = CommitStagingBuffer(info, data, false);
  if (status == Status::kOk) {
    return status;
  } else if (status == Status::kCancelled) {
    ACG_DEBUG_LOG("Commit cancelled. flush the buffer befor commit.");
    FlushStagingBuffer();
    status = CommitStagingBuffer(info, data, false);
    if (status == Status::kOk) {
      return Status::kOk;
    }
  }
  ACG_ERROR("Failed to commit. staging buffer is not large enough.");
  return Status::kCancelled;
}

Status GGui::CommitStagingBuffer(StagingUpdateInfo info, void* data, bool flush_immediately) {
  // Recompute info.
  if (!staging_upd_info_.empty()) {
    info.offset = staging_upd_info_.back().offset + staging_upd_info_.back().size;
  }

  info.dst_buf = GetAllocatedBuffer(info.dst_buffer_id).GetBuffer();
  // Check if staging buffer is large enough
  vk::DeviceSize expected_end_position = info.offset + info.size;
  if (expected_end_position > staging_buffer_.GetSize()) {
    if (info.offset == 0) {
      return Status::kOutOfRange;
    } else {
      return Status::kCancelled;
    }
  }

  // Ok, do commit:
  staging_upd_info_.push_back(info);
  char* staging_buffer_mapped_begin = static_cast<char*>(staging_buffer_.GetMappedMemory());
  staging_buffer_mapped_begin += info.offset;
  memcpy(staging_buffer_mapped_begin, data, info.size);
  if (flush_immediately) {
    return FlushStagingBuffer();
  } else {
    return Status::kOk;
  }
}

void GGui::FillBuffers(bool force) {
  for (size_t i = 0; i < scene_.GetMeshCount(); ++i) {
    auto& mesh = scene_.GetMesh(i);
    if (mesh.update_flag | force) {
      mesh.update_flag = false;
      ACG_DEBUG_LOG("Update mesh #{}", mesh.id);
      FillMeshBuffer(mesh, mesh_render_info_[i]);
    }
  }
  // TODO: Process other kinds
  for (size_t i = 0; i < scene_.GetParticlesCount(); ++i) {
    auto& part = scene_.GetParticles(i);
    if (part.update_flag | force) {
      part.update_flag = false;
      ACG_DEBUG_LOG("Update particle #{}", part.id);
      FillParticleBuffer(part, particle_render_info_[i]);
    }
  }
  for (size_t i = 0; i < scene_.GetMeshParticleCount(); ++i) {
    auto& part = scene_.GetMeshParticle(i);
    if (part.update_flag | force) {
      part.update_flag = false;
      ACG_DEBUG_LOG("Update mesh particle #{}", part.id);
      FillMeshParticleBuffer(part, mesh_particle_render_info_[i]);
    }
  }

  for (size_t i = 0; i < scene_.GetWireframeCount(); ++i) {
    auto& wf = scene_.GetWireframe(i);
    if (wf.update_flag | force) {
      wf.update_flag = false;
      FillWireframeBuffer(wf, wireframe_render_info_[i]);
    }
  }
  if (xy_plane_info_.update_flag) {
    xy_plane_info_.update_flag = false;
    FillXyPlaneBuffers();
  }

  if (!staging_upd_info_.empty()) {
    FlushStagingBuffer();
  }
}

void GGui::FillXyPlaneBuffers() {
  std::vector<WireframePoint> vert(xy_plane_render_info_.vertex_count);
  std::vector<uint32_t> inde;
  Index rows = xy_plane_info_.density.y();
  Index cols = xy_plane_info_.density.x();
  Index id = 0;
  for (Index i = 0; i < 2 * rows + 1; ++i) {
    for (Index j = 0; j < 2 * cols + 1; ++j) {
      auto&& ref = vert[id];
      ref.color = glm::make_vec3(xy_plane_info_.color.data());
      float off_x = xy_plane_info_.range.x() * static_cast<float>(i - rows) / rows;
      float off_y = xy_plane_info_.range.y() * static_cast<float>(j - cols) / cols;
      ref.position = glm::vec3(off_x, off_y, xy_plane_info_.height);
      id += 1;
    }
  }

  for (Index i = 0; i < 2 * rows; ++i) {
    for (Index j = 0; j < 2 * cols + 1; ++j) {
      auto up = i * (2 * cols + 1) + j;
      auto bot = up + (2 * cols + 1);
      inde.push_back(up);
      inde.push_back(bot);
    }
  }
  for (Index j = 0; j < 2 * cols; ++j) {
    for (Index i = 0; i < 2 * rows + 1; ++i) {
      auto left = i * (2 * cols + 1) + j;
      auto right = left + 1;
      inde.push_back(left);
      inde.push_back(right);
    }
  }

  TryCommitDeferred({xy_plane_render_info_.vertex, vert.size() * sizeof(WireframePoint)},
                    vert.data());

  TryCommitDeferred({xy_plane_render_info_.index, inde.size() * sizeof(uint32_t)}, inde.data());
}

void GGui::RenderOnce(bool verbose) {
  auto result = acg::gui::VkGraphicsContext::Instance().BeginDraw();
  while (!result) {
    RecreateSwapchain();
    result = acg::gui::VkGraphicsContext::Instance().BeginDraw();
  }
  auto cbuf = graphics_pass_->BeginRender();
  // 1. Mesh
  mesh_pipeline_->BeginPipeline(cbuf);
  for (const auto& info : mesh_render_info_) {
    if (verbose) {
      ACG_DEBUG_LOG("Rendering mesh: buffers = [{} {} {}], #index={}, #instance={}", info.vertex,
                    info.index, info.instance, info.index_count, info.instance_count);
    }
    auto v = GetAllocatedBuffer(info.vertex).GetBuffer();
    auto id = GetAllocatedBuffer(info.index).GetBuffer();
    auto it = GetAllocatedBuffer(info.instance).GetBuffer();
    cbuf.bindVertexBuffers(0, v, static_cast<vk::DeviceSize>(0));
    cbuf.bindVertexBuffers(1, it, static_cast<vk::DeviceSize>(0));
    cbuf.bindIndexBuffer(id, 0, vk::IndexType::eUint32);
    cbuf.pushConstants(mesh_pipeline_->GetPipelineLayout(),
                       vk::ShaderStageFlagBits::eVertex | vk::ShaderStageFlagBits::eFragment, 0,
                       sizeof(MeshPushConstants), &info.pc);
    cbuf.drawIndexed(info.index_count, info.instance_count, 0, 0, 0);
  }

  for (const auto& info : mesh_particle_render_info_) {
    if (verbose) {
      ACG_DEBUG_LOG("Rendering mesh particle: buffers = [{} {} {}], #index={}, #instance={}",
                    info.vertex, info.index, info.instance, info.index_count, info.instance_count);
    }
    auto v = GetAllocatedBuffer(info.vertex).GetBuffer();
    auto id = GetAllocatedBuffer(info.index).GetBuffer();
    auto it = GetAllocatedBuffer(info.instance).GetBuffer();
    cbuf.bindVertexBuffers(0, v, static_cast<vk::DeviceSize>(0));
    cbuf.bindVertexBuffers(1, it, static_cast<vk::DeviceSize>(0));
    cbuf.bindIndexBuffer(id, 0, vk::IndexType::eUint32);
    cbuf.pushConstants(mesh_pipeline_->GetPipelineLayout(),
                       vk::ShaderStageFlagBits::eVertex | vk::ShaderStageFlagBits::eFragment, 0,
                       sizeof(MeshPushConstants), &info.pc);
    cbuf.drawIndexed(info.index_count, info.instance_count, 0, 0, 0);
  }
  mesh_pipeline_->EndPipeline(cbuf);

  point_pipeline_->BeginPipeline(cbuf);
  for (const auto& info : particle_render_info_) {
    if (verbose) {
      ACG_DEBUG_LOG("Rendering particle: buffers=[{}]", info.vertex);
    }
    auto v = info.vertex;
    cbuf.pushConstants(point_pipeline_->GetPipelineLayout(),
                       vk::ShaderStageFlagBits::eVertex | vk::ShaderStageFlagBits::eFragment, 0,
                       sizeof(info.pc), &(info.pc));
    cbuf.bindVertexBuffers(0, GetAllocatedBuffer(v).GetBuffer(), static_cast<vk::DeviceSize>(0));
    cbuf.draw(info.vertex_count, 1, 0, 0);
  }

  wireframe_pipeline_->BeginPipeline(cbuf);
  cbuf.setLineWidth(linewidth_);
  if (xy_plane_info_.enable) {
    cbuf.bindVertexBuffers(0, GetAllocatedBuffer(xy_plane_render_info_.vertex).GetBuffer(),
                           static_cast<vk::DeviceSize>(0));
    cbuf.bindIndexBuffer(GetAllocatedBuffer(xy_plane_render_info_.index).GetBuffer(), 0,
                         vk::IndexType::eUint32);
    cbuf.drawIndexed(xy_plane_render_info_.index_count, 1, 0, 0, 0);
  }
  for (const auto& info : wireframe_render_info_) {
    if (verbose) {
      ACG_DEBUG_LOG("Rendering wireframe: buffers=[{} {}] #index={}", info.index, info.vertex);
    }
    cbuf.bindVertexBuffers(0, GetAllocatedBuffer(info.vertex).GetBuffer(),
                           static_cast<vk::DeviceSize>(0));
    cbuf.bindIndexBuffer(GetAllocatedBuffer(info.index).GetBuffer(), 0, vk::IndexType::eUint32);
    cbuf.drawIndexed(info.index_count, 1, 0, 0, 0);
  }
  for (const auto& info : mesh_render_info_) {
    if (info.enable_wireframe) {
      if (verbose) {
        ACG_DEBUG_LOG("Rendering mesh-wireframe: buffers=[{} {}] #index={}", info.wireframe_vertex,
                      info.wireframe_index, info.index_count * 2);
      }
      cbuf.bindVertexBuffers(0, GetAllocatedBuffer(info.wireframe_vertex).GetBuffer(),
                             static_cast<vk::DeviceSize>(0));
      cbuf.bindIndexBuffer(GetAllocatedBuffer(info.wireframe_index).GetBuffer(), 0,
                           vk::IndexType::eUint32);
      cbuf.drawIndexed(info.index_count * 2, 1, 0, 0, 0);
    }
  }
  graphics_pass_->EndRender();
  ImGui_ImplVulkan_NewFrame();
  ImGui_ImplGlfw_NewFrame();
  ImGui::NewFrame();
  if (ui_draw_callback_.has_value()) {
    (*ui_draw_callback_)();
  }
  DrawDefaultUI();
  ImGui::End();
  ImGui::Render();
  auto* data = ImGui::GetDrawData();
  auto ui_cbuf = ui_pass_->Render(data);
  auto result2 = acg::gui::VkGraphicsContext::Instance().EndDraw({cbuf, ui_cbuf});
  if (result2) {
    RecreateSwapchain();
  }
}

void GGui::RecreateSwapchain() {
  VkGraphicsContext::Instance().RecreateSwapchain();
  graphics_pass_->RecreateSwapchain();
  ui_pass_->RecreateSwapchain();
  UpdateLightCamera();
}

void GGui::UpdateLightCamera() {
  mesh_pipeline_->SetLight(light_);
  mesh_pipeline_->SetCamera(camera_);
  mesh_pipeline_->UpdateUbo();

  wireframe_pipeline_->SetCamera(camera_);
  wireframe_pipeline_->UpdateUbo();

  point_pipeline_->SetCamera(camera_);
  point_pipeline_->UpdateUbo();
}

void GGui::FillMeshBuffer(const Scene2::Mesh& mesh, const MeshRenderInfo& info) {
  auto vert_count = FieldCAccess{mesh.vertices}.Size();
  auto face_count = FieldCAccess{mesh.faces}.Size();
  auto instance_count = mesh.instance_count;
  auto vert_buffer_size = static_cast<vk::DeviceSize>(vert_count * sizeof(details::MeshVertex));
  auto index_buffer_size = static_cast<vk::DeviceSize>(face_count * 3 * sizeof(uint32_t));
  auto instance_buffer_size = instance_count * sizeof(details::MeshInstance);

  // Vertex Buffer
  std::vector<details::MeshVertex> vert_buffer_content;
  for (Index i = 0; i < vert_count; ++i) {
    auto p = mesh.vertices.col(i);
    auto c = mesh.color.col(mesh.use_uniform_color ? 0 : i);
    auto n = mesh.normals.col(i);
    details::MeshVertex v;
    v.position_ = glm::vec3(p.x(), p.y(), p.z());
    v.color_ = glm::vec4(c.x(), c.y(), c.z(), c.w());
    v.normal_ = glm::vec3(n.x(), n.y(), n.z());
    vert_buffer_content.push_back(v);
  }
  TryCommitDeferred(StagingUpdateInfo(info.vertex, vert_buffer_size), vert_buffer_content.data());

  // Index buffer:
  std::vector<uint32_t> index_buffer_content;
  for (Index i = 0; i < face_count; ++i) {
    index_buffer_content.push_back(mesh.faces.col(i).x());
    index_buffer_content.push_back(mesh.faces.col(i).y());
    index_buffer_content.push_back(mesh.faces.col(i).z());
  }
  TryCommitDeferred(StagingUpdateInfo(info.index, index_buffer_size), index_buffer_content.data());

  // Instance Buffer
  std::vector<details::MeshInstance> instance_buffer_content;
  for (Index i = 0; static_cast<size_t>(i) < mesh.instance_count; ++i) {
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
    inst.color = glm::vec4(0);
    instance_buffer_content.push_back(inst);
  }
  TryCommitDeferred(StagingUpdateInfo(info.instance, instance_buffer_size),
                    instance_buffer_content.data());

  if (mesh.enable_wireframe) {
    std::vector<WireframePoint> wf_vert(mesh.vertices.cols());
    std::vector<uint32_t> wf_index(info.index_count * 2);

    for (const auto& [i, p] : FieldCEnumerate(mesh.vertices)) {
      auto& vert = wf_vert[i];
      vert.position = glm::vec3(p.x(), p.y(), p.z());
      vert.color = glm::make_vec4(mesh_outline_color_.data());
    }

    for (const auto& [i, v] : FieldCEnumerate(mesh.faces)) {
      wf_index[i * 6] = v.x();
      wf_index[i * 6 + 1] = v.y();
      wf_index[i * 6 + 2] = v.y();
      wf_index[i * 6 + 3] = v.z();
      wf_index[i * 6 + 4] = v.z();
      wf_index[i * 6 + 5] = v.x();
    }

    TryCommitDeferred(
        StagingUpdateInfo(info.wireframe_vertex, sizeof(wf_vert.front()) * wf_vert.size()),
        wf_vert.data());

    TryCommitDeferred(
        StagingUpdateInfo(info.wireframe_index, sizeof(wf_index.front()) * wf_index.size()),
        wf_index.data());
  }
}

void GGui::PrepareParticleBuffer(ParticleRenderInfo& info) {
  PrepareVertexBufferHelper(sizeof(PointVertex) * info.vertex_count, info.vertex);
}

void GGui::PrepareWireframeBuffer(WireframeRenderInfo& info) {
  PrepareVertexBufferHelper(sizeof(WireframePoint) * info.vertex_count, info.vertex);
  PrepareVertexBufferHelper(sizeof(uint32_t) * info.index_count, info.index);
}

void GGui::PrepareMeshBuffer(MeshRenderInfo& info) {
  PrepareVertexBufferHelper(sizeof(MeshVertex) * info.vertex_count, info.vertex);
  PrepareVertexBufferHelper(sizeof(MeshInstance) * info.instance_count, info.instance);
  PrepareIndexBufferHelper(sizeof(uint32_t) * info.index_count, info.index);

  if (info.enable_wireframe) {
    PrepareVertexBufferHelper(sizeof(WireframePoint) * info.vertex_count, info.wireframe_vertex);
    PrepareIndexBufferHelper(sizeof(uint32_t) * info.index_count * 2, info.wireframe_index);
  }
}

void GGui::PrepareVertexBufferHelper(vk::DeviceSize size, BufferID& id) {
  if (id == invalid_buffer) {
    id = CreateVertexBuffer(size);
  } else {
    ReserveBuffer(size, id);
  }
}

void GGui::PrepareIndexBufferHelper(vk::DeviceSize size, BufferID& id) {
  if (id == invalid_buffer) {
    id = CreateIndexBuffer(size);
  } else {
    ReserveBuffer(size, id);
  }
}

void GGui::FillMeshParticleBuffer(const Scene2::Particles& particle, const MeshRenderInfo& info) {
  auto [ind, v] = get_default_ball();
  auto vert_count = FieldCAccess{v}.Size();
  auto face_count = FieldCAccess{ind}.Size();
  auto vert_buffer_size = static_cast<vk::DeviceSize>(vert_count * sizeof(MeshVertex));
  auto index_buffer_size = static_cast<vk::DeviceSize>(face_count * 3 * sizeof(uint32_t));
  auto instance_buffer_size = info.instance_count * sizeof(details::MeshInstance);

  std::vector<details::MeshVertex> vert_buffer_content;
  for (Index i = 0; i < vert_count; ++i) {
    auto p = v.col(i);
    auto n = v.col(i);
    details::MeshVertex v;
    v.position_ = glm::vec3(p.x(), p.y(), p.z());
    v.color_ = glm::make_vec4(mesh_outline_color_.data());
    v.normal_ = glm::vec3(n.x(), n.y(), n.z());
    vert_buffer_content.push_back(v);
  }
  CommitStagingBuffer(StagingUpdateInfo(info.vertex, vert_buffer_size), vert_buffer_content.data());

  // Index buffer:
  std::vector<uint32_t> index_buffer_content;
  for (Index i = 0; i < face_count; ++i) {
    index_buffer_content.push_back(ind.col(i).x());
    index_buffer_content.push_back(ind.col(i).y());
    index_buffer_content.push_back(ind.col(i).z());
  }
  CommitStagingBuffer(StagingUpdateInfo(info.index, index_buffer_size),
                      index_buffer_content.data());

  // Instance Buffer
  std::vector<details::MeshInstance> instance_buffer_content;
  for (Index i = 0; static_cast<size_t>(i) < info.instance_count; ++i) {
    Vec3f p = particle.positions.col(i);
    details::MeshInstance inst;
    inst.position = glm::make_vec3(p.data());
    inst.rotation = glm::vec4(1, 0, 0, 0);
    Vec4f c = particle.colors.col(particle.use_uniform_color ? 0 : i);
    inst.color = glm::make_vec4(c.data());
    instance_buffer_content.push_back(inst);
  }
  CommitStagingBuffer(StagingUpdateInfo(info.instance, instance_buffer_size),
                      instance_buffer_content.data());
}

void GGui::FillParticleBuffer(const Scene2::Particles& particle, const ParticleRenderInfo& info) {
  std::vector<PointVertex> vert(particle.positions.cols());
  for (const auto& [i, p] : FieldCEnumerate{particle.positions}) {
    auto& v = vert[i];
    v.position = glm::vec3(p.x(), p.y(), p.z());
    Vec4f c = particle.colors.col(particle.use_uniform_color ? 0 : i);
    v.color = glm::make_vec4(c.data());
  }
  ACG_CHECK_STATUS_OK(TryCommitDeferred(
      StagingUpdateInfo(info.vertex, sizeof(PointVertex) * vert.size()), vert.data()));
}

void GGui::FillWireframeBuffer(const Scene2::Wireframe& wireframe,
                               const WireframeRenderInfo& info) {
  std::vector<WireframePoint> vert(wireframe.positions.cols());
  for (const auto& [i, p] : FieldCEnumerate{wireframe.positions}) {
    auto& v = vert[i];
    Vec3f c = wireframe.colors.col(wireframe.colors.cols() > 1 ? i : 0);
    v.position = glm::vec3(p.x(), p.y(), p.z());
    v.color = glm::make_vec3(c.data());
  }
  ACG_CHECK_STATUS_OK(TryCommitDeferred(
      StagingUpdateInfo(info.vertex, vert.size() * sizeof(vert.front())), vert.data()));

  std::vector<uint32_t> indi(wireframe.indices.cols() * 2);
  for (const auto& [id, i] : FieldCEnumerate{wireframe.indices}) {
    indi[id * 2] = i.x();
    indi[id * 2 + 1] = i.y();
  }

  ACG_CHECK_STATUS_OK(TryCommitDeferred(
      StagingUpdateInfo(info.index, indi.size() * sizeof(indi.front())), indi.data()));
}

void GGui::DrawDefaultUI() {
  ImGui::SetNextWindowPos(ImVec2(0, 0));
  auto extent = VkGraphicsContext::Instance().swapchain_extent_;
  ImGui::SetNextWindowSize(ImVec2(360, extent.height * .5), ImGuiCond_Once);
  ImGui::Begin("GGui Control", nullptr, ImGuiWindowFlags_NoMove);
  ImGui::Text("Background Color");
  ImGui::ColorEdit3("Clear Color", graphics_pass_->GetBackgroundColor().float32,
                    ImGuiColorEditFlags_Float);
  ImGui::Text("Mesh-outline Color");
  ImGui::ColorEdit3("Outline Color", mesh_outline_color_.data(), ImGuiColorEditFlags_Float);

  ImGui::Text("Xy plane info");
  bool changed = ImGui::ColorEdit3("Color", xy_plane_info_.color.data(), ImGuiColorEditFlags_Float);
  changed |= ImGui::DragFloat("Height", &xy_plane_info_.height, 0.1f, -5.0f, 5.0f);
  changed |= ImGui::Checkbox("Enable?", &xy_plane_info_.enable);
  if (changed) {
    ACG_DEBUG_LOG("XyPlane Mark Update");
    xy_plane_info_.MarkUpdate();
  }

  ImGui::Text("Camera Info");
  changed = ImGui::InputFloat3("Position", camera_.GetPosition().data());
  changed |= ImGui::InputFloat3("Front", camera_.GetFront().data());
  changed |= ImGui::InputFloat3("Up", camera_.GetUp().data());
  changed |= ImGui::Checkbox("Prespective?", &camera_.GetProjectionMode());
  changed |= ImGui::Checkbox("Fix Up Dir?", &disable_camera_up_update_);

  ImGui::Text("Light Info");
  changed |= ImGui::InputFloat3("Point-Light position", light_.light_position_.data());
  changed |= ImGui::ColorEdit3("Point-Light color", light_.light_color_.data(),
                               ImGuiColorEditFlags_Float);
  changed |= ImGui::DragFloat("Point-Light Density", &light_.light_density_, 0.03, 0, 1);
  changed |= ImGui::ColorEdit3("Para-Light Color", light_.parallel_light_color_.data(),
                               ImGuiColorEditFlags_Float);
  changed |= ImGui::InputFloat3("Para-Light Direction", light_.parallel_light_dir_.data());
  changed |= ImGui::DragFloat("Para-Light Density", &light_.parallel_light_density_, 0.03, 0, 1);
  changed |= ImGui::ColorEdit3("Ambient Color", light_.ambient_light_color_.data(),
                               ImGuiColorEditFlags_Float);
  changed |= ImGui::DragFloat("Ambient Density", &light_.light_density_, 0.03, 0, 1);

  if (changed) {
    UpdateLightCamera();
  }

  ImGui::Text("Render Time Statics");
  float sum = 0;
  for (auto value : render_time_samples_) {
    sum += value;
  }
  auto s = fmt::format("avg={:.3f}", sum / 32);
  ImGui::PlotHistogram("Time(ms)", render_time_samples_.data(), render_time_samples_.size(), 0,
                       s.c_str(), FLT_MAX, FLT_MAX, ImVec2(0, 80));
  ImGui::End();
}

void GGui::Tick() {
  auto cur_time = std::chrono::steady_clock::now();
  dt = std::chrono::duration_cast<std::chrono::duration<float, std::milli>>(cur_time - last_time);
  last_time = cur_time;
  render_time_samples_[render_index_++] = dt.count();
  render_index_ = render_index_ % 32;
  ProcessCamera();
}
void GGui::ProcessCamera() {
  Vec3f camera_velocity = Vec3f::Zero();
  float rot_hori = 0;
  float rot_vert = 0;
  Vec3f right = camera_.GetFront().cross(camera_.GetUp());
  bool changed = false;
  // 1. pressed 'WSAD' => horizonal
  auto pressed = glfwGetKey(Window::Instance().GetWindow(), GLFW_KEY_W);
  if (pressed) {
    changed = true;
    camera_velocity += camera_.GetFront().normalized();
  }
  pressed = glfwGetKey(Window::Instance().GetWindow(), GLFW_KEY_S);
  if (pressed) {
    changed = true;
    camera_velocity -= camera_.GetFront().normalized();
  }
  pressed = glfwGetKey(Window::Instance().GetWindow(), GLFW_KEY_A);
  if (pressed) {
    changed = true;
    camera_velocity -= right.normalized();
  }
  pressed = glfwGetKey(Window::Instance().GetWindow(), GLFW_KEY_D);
  if (pressed) {
    changed = true;
    camera_velocity += right.normalized();
  }
  pressed = glfwGetKey(Window::Instance().GetWindow(), GLFW_KEY_Q);
  if (pressed) {
    changed = true;
    camera_velocity -= camera_.GetUp().normalized();
  }
  pressed = glfwGetKey(Window::Instance().GetWindow(), GLFW_KEY_E);
  if (pressed) {
    changed = true;
    camera_velocity += camera_.GetUp().normalized();
  }

  // 2. hjkl => angle
  pressed = glfwGetKey(Window::Instance().GetWindow(), GLFW_KEY_H);
  if (pressed) {
    changed = true;
    rot_hori -= 1.0;
  }
  pressed = glfwGetKey(Window::Instance().GetWindow(), GLFW_KEY_L);
  if (pressed) {
    changed = true;
    rot_hori += 1.0;
  }
  pressed = glfwGetKey(Window::Instance().GetWindow(), GLFW_KEY_K);
  if (pressed) {
    changed = true;
    rot_vert -= 1.0;
  }
  pressed = glfwGetKey(Window::Instance().GetWindow(), GLFW_KEY_J);
  if (pressed) {
    changed = true;
    rot_vert += 1.0;
  }

  Vec3f front = camera_.GetFront().normalized();
  Vec3f up = camera_.GetUp().normalized();
  camera_.GetFront()
      += (rot_hori * right - rot_vert * up) * dt.count() * .001 * camera_moving_speed;
  camera_.GetFront().normalize();
  if (disable_camera_up_update_) {
    camera_.SetUp({0, 0, 1});
  } else {
    camera_.GetUp() += (-rot_vert * front) * dt.count() * .001 * camera_moving_speed;
    camera_.GetUp().normalize();
  }
  camera_.GetPosition() += camera_velocity * dt.count() * 0.001 * camera_moving_speed;
  if (changed) {
    UpdateLightCamera();
  }
}

void GGui::ClearScene() {
  scene_.Clear();
  std::vector<BufferWithMemory> remapped;
  // Register old buffers.
  remapped.push_back(GetAllocatedBuffer(xy_plane_render_info_.vertex));
  remapped.push_back(GetAllocatedBuffer(xy_plane_render_info_.index));
  for (size_t i = 0; i < allocated_buffers_.size(); ++i) {
    if (i != xy_plane_render_info_.vertex && i != xy_plane_render_info_.index) {
      VkContext2::Instance().DestroyBufferWithMemory(allocated_buffers_[i]);
    }
  }
  xy_plane_render_info_.vertex = 0;
  xy_plane_render_info_.index = 1;
  mesh_render_info_.clear();
  mesh_particle_render_info_.clear();
  wireframe_render_info_.clear();
  particle_render_info_.clear();

  allocated_buffers_ = remapped;
}

}  // namespace acg::gui::details
