#include <spdlog/spdlog.h>

#include <acg_core/init.hpp>
#include <acg_gui/backend/context.hpp>
#include <acg_gui/backend/graphics_context.hpp>
#include <acg_utils/init.hpp>
#include <acg_core/geometry/common_models.hpp>
#include <acg_core/math/access.hpp>
#include <acg_gui/convent.hpp>

#include "api.hpp"

std::vector<acg::gui::details::MeshVertex> ball_vert() {
  auto result = acg::geometry::sphere_20(acg::Vec3f{0, 0, 0}, 1);
  auto vertices = result.GetVertices();
  std::vector<acg::gui::details::MeshVertex> ret;
  for (auto vert: acg::FieldAccess{vertices}.Xwise()){
    acg::gui::details::MeshVertex vr;
    vr.position_ = acg::gui::to_glm(acg::Vec3f(vert));
    vr.normal_ = vr.position_;
    vr.color_ = glm::vec3(.3, .5, .7);
    ret.push_back(vr);
  }
  return ret;
}

std::vector<uint32_t> ball_index() {
  auto result = acg::geometry::sphere_20(acg::Vec3f{0, 0, 0}, 1);
  auto indices = result.GetFaces();
  std::vector<uint32_t> ret;
  for (auto vert: acg::FieldAccess{indices}.Xwise()){
    ret.push_back(vert.x());
    ret.push_back(vert.y());
    ret.push_back(vert.z());
  }
  return ret;
}

int main(int argc, char** argv) {
  auto utils_hooker = acg::utils::UtilsHooker();
  utils_hooker.default_logging_level = spdlog::level::debug;
  utils_hooker.Hook();
  acg::gui::WindowHooker{"ggui"}.Hook();
  auto vkctx_hooker = acg::gui::VkContext2Hooker{};
  vkctx_hooker.app_name = "Vulkan Viewer";
  vkctx_hooker.enable_validation = true;
  vkctx_hooker.Hook();
  VkGraphicsContext::Hooker().Hook();
  acg::init(argc, argv);

  {
    acg::gui::details::GraphicsRenderPass::InitConfig render_pass_config;
    render_pass_config.max_descriptor_set_count = 4;
    render_pass_config.required_descriptor_sizes.push_back(
        {vk::DescriptorType::eUniformBuffer, 20});
    acg::gui::details::GraphicsRenderPass render_pass{render_pass_config};
    {
      acg::gui::details::MeshPipeline::Config config;
      acg::gui::details::MeshPipeline ppl(render_pass, config);
      auto vb = acg::gui::VkContext2::Instance().CreateBufferWithMemory(
          10240, vk::BufferUsageFlagBits::eVertexBuffer,
          vk::MemoryPropertyFlagBits::eHostVisible | vk::MemoryPropertyFlagBits::eHostCoherent);
      auto ib = acg::gui::VkContext2::Instance().CreateBufferWithMemory(
          10240, vk::BufferUsageFlagBits::eIndexBuffer,
          vk::MemoryPropertyFlagBits::eHostVisible | vk::MemoryPropertyFlagBits::eHostCoherent);
      ACG_INFO("created buffers");
      ppl.UpdateUbo();
      ACG_INFO("Update Ubo.");
      auto vert = ball_vert();
      auto index = ball_index();
      acg::gui::VkContext2::Instance().CopyHostToBuffer(vert.data(), vb, sizeof(vert.front()) * vert.size());
      acg::gui::VkContext2::Instance().CopyHostToBuffer(index.data(), ib, sizeof(uint32_t) * index.size());

      while (!acg::gui::Window::Instance().ShouldClose()) {
        glfwPollEvents();
        // Render.
        auto result = acg::gui::VkGraphicsContext::Instance().BeginDraw();
        if (!result) {
          render_pass.RecreateSwapchain();
          ppl.Recreate(render_pass);
          continue;
        }
        auto cbuf = render_pass.BeginRender();
        ppl.BeginPipeline(cbuf);
        cbuf.bindVertexBuffers(0, vb.GetBuffer(), static_cast<vk::DeviceSize>(0));
        cbuf.bindIndexBuffer(ib.GetBuffer(), 0, vk::IndexType::eUint32);
        ppl.EndPipeline(cbuf);
        render_pass.EndRender();
        auto result2 = acg::gui::VkGraphicsContext::Instance().EndDraw({cbuf});
        if (!result2) {
          render_pass.RecreateSwapchain();
          ppl.Recreate(render_pass);
        }
      }
      acg::gui::VkContext2::Instance().device_.waitIdle();
      acg::gui::VkContext2::Instance().DestroyBufferWithMemory(ib);
      acg::gui::VkContext2::Instance().DestroyBufferWithMemory(vb);
    }
  }
  acg::clean_up();
  return 0;
}
