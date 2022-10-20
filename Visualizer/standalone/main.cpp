#include <spdlog/spdlog.h>

#include <acg_visualizer/renderer.hpp>

#include "acg_utils/log.hpp"
#include "acg_visualizer/mesh_pipeline.hpp"
#include "acg_visualizer/ui_pipeline.hpp"
#include "co/co.h"
#include "imgui_impl_glfw.h"
#include "imgui_impl_vulkan.h"


using namespace acg::visualizer::details;
using namespace acg::visualizer;
Camera camera;

std::vector<Vertex> vertices = {
    {{-0.5f, -0.5f, 0.0f}, {1.0f, 0.0f, 0.0f}}, {{0.0f, 0.0f, 0.0f}, {0.0f, 1.0f, 0.0f}},
    {{-0.5f, 0.5f, 0.0f}, {0.0f, 0.0f, 1.0f}},  {{0.0f, 0.0f, 1.0f}, {1.0f, 1.0f, 1.0f}},
    {{0.5f, -0.5f, 0.0f}, {1.0f, 0.0f, 1.0f}},  {{0.5f, 0.5f, 0.0f}, {1.0f, 0.0f, 1.0f}},
};

std::vector<uint32_t> indices = {0, 1, 2, 3, 4, 5, 2, 3, 5};
int main() {


  spdlog::set_level(spdlog::level::debug);
  auto vert_size = sizeof(vertices.front()) * vertices.size();
  auto renderer = Renderer::Builder{}.Build();

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
  ImGuiPipeline ui_ppl{*renderer};
  mesh_ppl.Init();
  ui_ppl.Init();
  while (! renderer->GetWindow()->ShouldClose()) {
    glfwPollEvents();
    // Do nothing.
    mesh_ppl.SetCamera(camera);
    if (!renderer->BeginDraw()) {
      renderer->RecreateSwapchain();
      mesh_ppl.RecreateSwapchain();
      ui_ppl.RecreateSwapchain();
      continue;
    }

    spdlog::info("Demo state: {}", show_demo);
    std::vector<vk::CommandBuffer> buffer_submit;

    // Mesh
    auto cb = mesh_ppl.BeginRender();
    cb.bindVertexBuffers(0, vbuf.GetBuffer(), static_cast<vk::DeviceSize>(0));
    cb.bindIndexBuffer(ibuf.GetBuffer(), 0, vk::IndexType::eUint32);
    cb.drawIndexed(static_cast<uint32_t>(indices.size()), 1, 0, 0, 0);
    buffer_submit.emplace_back(mesh_ppl.EndRender());

    // Imgui
    ImGui_ImplVulkan_NewFrame();
    ImGui_ImplGlfw_NewFrame();
    ImGui::NewFrame();
    ImGui::ShowDemoWindow();
    ImGuiIO& io = ImGui::GetIO();
    ImGui::Render();
    auto *data = ImGui::GetDrawData();
    buffer_submit.emplace_back(ui_ppl.Render(data));

    auto recreation = renderer->EndDrawSubmitPresent(buffer_submit);
    if (recreation) {
      renderer->RecreateSwapchain();
      mesh_ppl.RecreateSwapchain();
      ui_ppl.RecreateSwapchain(); 
    }
    // break;
  }
  renderer->GetDevice().waitIdle();
  mesh_ppl.Cleanup();
  ui_ppl.Cleanup();
  ibuf.Release();
  vbuf.Release();

  return 0;
}