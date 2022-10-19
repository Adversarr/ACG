#include <spdlog/spdlog.h>

#include <acg_visualizer/renderer.hpp>

#include "acg_utils/log.hpp"
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
  renderer->CopyToBuffer(vertices.data(), stag_vbuf, vert_size);
  renderer->CopyBuffer(stag_vbuf.buffer, vbuf.buffer, vert_size);
  renderer->DestroyBufferWithMemory(stag_vbuf);

  auto index_size = sizeof(indices.front()) * indices.size();
  auto ibuf = renderer->CreateBuffer(
      index_size, vk::BufferUsageFlagBits::eTransferDst | vk::BufferUsageFlagBits::eIndexBuffer,
      vk::MemoryPropertyFlagBits::eDeviceLocal);
  auto stag_ibuf = renderer->CreateBuffer(
      index_size, vk::BufferUsageFlagBits::eTransferSrc,
      vk::MemoryPropertyFlagBits::eHostVisible | vk::MemoryPropertyFlagBits::eHostCoherent);
  renderer->CopyToBuffer(indices.data(), stag_ibuf, index_size);
  renderer->CopyBuffer(stag_ibuf.buffer, ibuf.buffer, index_size);
  renderer->DestroyBufferWithMemory(stag_ibuf);

  bool show_demo = true;
  int i = 0;
  while (renderer->RunOnce()) {
    // Do nothing.
    renderer->SetCamera(camera);
    auto command_buffer = renderer->BeginDraw();
    if (!command_buffer) {
      spdlog::info("Command Buffer is null, skip rendering...");
      continue;
    }
    spdlog::info("Demo state: {}", show_demo);
    {
      ImGui_ImplVulkan_NewFrame();
      ImGui_ImplGlfw_NewFrame();
      ImGui::NewFrame();
      ImGui::ShowDemoWindow();
      ImGuiIO& io = ImGui::GetIO();
      ImGui::Render();
      auto data = ImGui::GetDrawData();
      auto imgui_cmd = renderer->BeginImGuiRenderPass();
      ImGui_ImplVulkan_RenderDrawData(data, imgui_cmd);
      imgui_cmd.endRenderPass();
      imgui_cmd.end();
    }

    renderer->BeginRenderPass();
    renderer->BindUboLayout();
    command_buffer.bindVertexBuffers(0, vbuf.buffer, (vk::DeviceSize)0);
    command_buffer.bindIndexBuffer(ibuf.buffer, 0, vk::IndexType::eUint32);
    command_buffer.drawIndexed(static_cast<uint32_t>(indices.size()), 1, 0, 0, 0);
    renderer->EndRenderPass();
    renderer->EndDrawSubmit();
    co::sleep(100);
  }
  renderer->GetDevice().waitIdle();
  renderer->DestroyBufferWithMemory(ibuf);
  renderer->DestroyBufferWithMemory(vbuf);

  return 0;
}