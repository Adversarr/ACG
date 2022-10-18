#include <spdlog/spdlog.h>

#include <acg_visualizer/renderer.hpp>

#include "acg_utils/log.hpp"
#include "co/co.h"

using namespace acg::visualizer::details;
using namespace acg::visualizer;
Camera camera;
std::vector<Vertex> vertices = {{.position = {0, .5, 0}, .color = {1, 0, 0}},
                                {.position = {-.5, -.5, 0}, .color = {0, 1, 0}},
                                {.position = {.5, -.5, 0.7}, .color = {0, 0, 1}}};

std::vector<uint32_t> indices = {0, 1, 2};

int main() {
  spdlog::set_level(spdlog::level::debug);
  auto renderer = Renderer::Builder{}.Build();
  auto vbuf = renderer->CreateBuffer(
      sizeof(Vertex) * vertices.size(),
      vk::BufferUsageFlagBits::eTransferDst | vk::BufferUsageFlagBits::eVertexBuffer,
      vk::MemoryPropertyFlagBits::eDeviceLocal);
  auto ibuf = renderer->CreateBuffer(
      sizeof(uint32_t) * indices.size(),
      vk::BufferUsageFlagBits::eTransferDst | vk::BufferUsageFlagBits::eIndexBuffer,
      vk::MemoryPropertyFlagBits::eDeviceLocal);
  auto stag_vbuf = renderer->CreateBuffer(
      sizeof(Vertex) * vertices.size(), vk::BufferUsageFlagBits::eTransferSrc,
      vk::MemoryPropertyFlagBits::eHostVisible | vk::MemoryPropertyFlagBits::eHostCoherent);
  auto stag_ibuf = renderer->CreateBuffer(
      sizeof(uint32_t) * indices.size(), vk::BufferUsageFlagBits::eTransferSrc,
      vk::MemoryPropertyFlagBits::eHostVisible | vk::MemoryPropertyFlagBits::eHostCoherent);

  renderer->CopyToBuffer(vertices.data(), stag_vbuf, vertices.size() * sizeof(Vertex));
  renderer->CopyBuffer(stag_vbuf.buffer, vbuf.buffer, vertices.size() * sizeof(Vertex));
  renderer->CopyToBuffer(vertices.data(), stag_ibuf, indices.size() * sizeof(uint32_t));
  renderer->CopyBuffer(stag_ibuf.buffer, ibuf.buffer, indices.size() * sizeof(uint32_t));

  renderer->DestroyBufferWithMemory(stag_ibuf);
  renderer->DestroyBufferWithMemory(stag_vbuf);

  int i = 0;
  while (renderer->RunOnce()) {
    ACG_DEBUG_LOG("Frame {}", i++);
    // Do nothing.
    renderer->SetCamera(camera);
    auto command_buffer = renderer->BeginDraw();
    if (!command_buffer) {
      continue;
    }
    renderer->BeginRenderPass();
    renderer->BindUboLayout();

    command_buffer.bindVertexBuffers(0, vbuf.buffer, {0});
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