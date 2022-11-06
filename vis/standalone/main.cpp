#include <spdlog/spdlog.h>

#include <acg_core/geometry/common_models.hpp>
#include <acg_vis/renderer.hpp>

#include "acg_utils/log.hpp"
#include "acg_vis/mesh_pipeline.hpp"
#include "acg_vis/ui_pipeline.hpp"
#include "co/co.h"
#include "imgui_impl_glfw.h"
#include "imgui_impl_vulkan.h"


using namespace acg::visualizer::details;
using namespace acg::visualizer;
Camera camera;

std::vector<Vertex> vertices;

std::vector<acg::Idx> indices;

std::tuple<std::vector<Vertex>, std::vector<acg::Idx>> from_mesh(
    const acg::models::TriangleMesh<acg::F32>& mesh) {
  std::vector<acg::Idx> indices(mesh.GetIndices().cols() * 3);
  std::memcpy(indices.data(), mesh.GetIndices().data(),
              mesh.GetIndices().size() * sizeof(acg::Idx));
  std::vector<Vertex> vertices;
  for (acg::Idx i = 0; i < mesh.GetVertices().cols(); ++i) {
    Vertex v{
        .position = {mesh.GetVertices().col(i).x(), mesh.GetVertices().col(i).y(),
                     mesh.GetVertices().col(i).z()},
    };
    v.color = v.position;
    // v.normal = v.position;
    std::cout << v.position.x << v.position.y << v.position.z << std::endl;
    vertices.emplace_back(v);
  }
  return {vertices, indices};
}

int main() {
  spdlog::set_level(spdlog::level::debug);
  std::tie(vertices, indices) = from_mesh(acg::models::sphere_uv({0, 0, 0}, .5));
  auto vert_size = sizeof(vertices.front()) * vertices.size();
  auto renderer = VkContext::Builder{}.Build();
  auto vbuf = renderer->CreateBuffer(
      vert_size, vk::BufferUsageFlagBits::eTransferDst | vk::BufferUsageFlagBits::eVertexBuffer,
      vk::MemoryPropertyFlagBits::eDeviceLocal);

  auto stag_vbuf = renderer->CreateBuffer(
      vert_size, vk::BufferUsageFlagBits::eTransferSrc,
      vk::MemoryPropertyFlagBits::eHostVisible | vk::MemoryPropertyFlagBits::eHostCoherent);
  renderer->CopyHostToBuffer(vertices.data(), stag_vbuf, vert_size);
  renderer->CopyBuffer(stag_vbuf.GetBuffer(), vbuf.GetBuffer(), vert_size);
  stag_vbuf.Release();

  auto index_size = sizeof(indices.front()) * indices.size();
  auto ibuf = renderer->CreateBuffer(
      index_size, vk::BufferUsageFlagBits::eTransferDst | vk::BufferUsageFlagBits::eIndexBuffer,
      vk::MemoryPropertyFlagBits::eDeviceLocal);
  auto stag_ibuf = renderer->CreateBuffer(
      index_size, vk::BufferUsageFlagBits::eTransferSrc,
      vk::MemoryPropertyFlagBits::eHostVisible | vk::MemoryPropertyFlagBits::eHostCoherent);
  renderer->CopyHostToBuffer(indices.data(), stag_ibuf, index_size);
  renderer->CopyBuffer(stag_ibuf.GetBuffer(), ibuf.GetBuffer(), index_size);
  stag_ibuf.Release();

  bool show_demo = true;
  MeshPipeline mesh_ppl{*renderer, false};
  UiPipeline ui_ppl{*renderer};
  mesh_ppl.Init();
  ui_ppl.Init();
  auto start = std::chrono::steady_clock::now();
  int i = 0;

  mesh_ppl.SetCamera(camera, true);
  while (!renderer->GetWindow()->ShouldClose()) {
    uint32_t fps = (1000000000.0 / (std::chrono::steady_clock::now() - start).count()) * i;
    glfwPollEvents();
    // Do nothing.
    if (!renderer->BeginDraw()) {
      renderer->RecreateSwapchain();
      mesh_ppl.RecreateSwapchain();
      ui_ppl.RecreateSwapchain();
      continue;
    }
    std::vector<vk::CommandBuffer> buffer_submit;

    // Mesh
    auto cb = mesh_ppl.BeginRender();
    cb.bindVertexBuffers(0, vbuf.GetBuffer(), static_cast<vk::DeviceSize>(0));
    cb.bindIndexBuffer(ibuf.GetBuffer(), 0, vk::IndexType::eUint32);
    cb.drawIndexed(static_cast<uint32_t>(indices.size()), 1, 0, 0, 0);
    buffer_submit.emplace_back(mesh_ppl.EndRender());
    i += 1;
    // Imgui
    ImGui_ImplVulkan_NewFrame();
    ImGui_ImplGlfw_NewFrame();
    ImGui::NewFrame();
    // ImGui::ShowDemoWindow();
    ImGui::Begin("Fps");
    ImGui::Text("Value = %d Frame Count = %d", fps, i);
    ImGui::End();
    ImGuiIO& io = ImGui::GetIO();
    ImGui::Render();
    auto* data = ImGui::GetDrawData();
    buffer_submit.emplace_back(ui_ppl.Render(data));

    auto recreation = renderer->EndDrawSubmitPresent(buffer_submit);
    if (recreation) {
      renderer->RecreateSwapchain();
      mesh_ppl.RecreateSwapchain();
      ui_ppl.RecreateSwapchain();
    }
  }
  renderer->GetDevice().waitIdle();
  mesh_ppl.Cleanup();
  ui_ppl.Cleanup();
  ibuf.Release();
  vbuf.Release();
  return 0;
}
