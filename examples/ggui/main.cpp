#include <spdlog/spdlog.h>

#include <acg_core/geometry/common_models.hpp>
#include <acg_core/init.hpp>
#include <acg_core/math/access.hpp>
#include <acg_core/math/constants.hpp>
#include <acg_gui/backend/context.hpp>
#include <acg_gui/backend/graphics_context.hpp>
#include <acg_gui/backend/ui_pass.hpp>
#include <acg_gui/convent.hpp>
#include <acg_utils/init.hpp>
#include <acg_utils/time.hpp>
#include <glm/gtx/transform.hpp>

#include "api.hpp"

std::vector<acg::gui::details::MeshVertex> ball_vert() {
  auto result = acg::geometry::sphere_20(acg::Vec3f{0, 0, 0}, 1);
  auto vertices = result.GetVertices();
  std::vector<acg::gui::details::MeshVertex> ret;
  for (auto vert : acg::FieldAccess{vertices}.Xwise()) {
    acg::gui::details::MeshVertex vr;
    vr.position_ = acg::gui::to_glm(acg::Vec3f(vert));
    vr.normal_ = vr.position_;
    vr.color_ = glm::vec3(0.3, 0.3, 0.3);
    ret.push_back(vr);
  }
  return ret;
}

std::vector<uint32_t> ball_index() {
  auto result = acg::geometry::sphere_20(acg::Vec3f{0, 0, 0}, 1);
  auto indices = result.GetFaces();
  std::vector<uint32_t> ret;
  for (auto vert : acg::FieldAccess{indices}.Xwise()) {
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
  acg::gui::Window::Hooker{"ggui"}.Hook();
  auto vkctx_hooker = acg::gui::VkContext2Hooker{};
  vkctx_hooker.app_name = "GGui";
  vkctx_hooker.enable_validation = true;
  vkctx_hooker.Hook();
  VkGraphicsContext::Hooker().Hook();
  acg::init(argc, argv);
  {
    acg::gui::details::GraphicsRenderPass::InitConfig render_pass_config;
    render_pass_config.is_present = false;
    render_pass_config.max_descriptor_set_count = 6;
    render_pass_config.required_descriptor_sizes.push_back(
        {vk::DescriptorType::eUniformBuffer, 12});
    acg::gui::details::GraphicsRenderPass render_pass{render_pass_config};
    acg::gui::Camera cam;
    cam.SetPosition({0, 0, 3});
    cam.SetFront({0, 0, -1});
    cam.SetUp({0, 1, 0});
    cam.SetProjectionMode();
    acg::gui::Light light;
    light.light_position_ = acg::Vec3f(0, 2, 2);
    light.light_color_ = acg::Vec3f(1, 1, 1);
    light.light_density_ = 0.5f;
    light.parallel_light_color_ = acg::Vec3f(1, 1, 1);
    light.parallel_light_dir_ = acg::Vec3f(0, -1, -1);
    light.parallel_light_density_ = 0.5f;
    {
      acg::gui::details::MeshPipeline::Config config;
      acg::gui::details::MeshPipeline ppl(render_pass, config);

      ACG_INFO("Pipeline created");
      auto vb = acg::gui::VkContext2::Instance().CreateBufferWithMemory(
          10240, vk::BufferUsageFlagBits::eVertexBuffer,
          vk::MemoryPropertyFlagBits::eHostVisible | vk::MemoryPropertyFlagBits::eHostCoherent);
      auto ib = acg::gui::VkContext2::Instance().CreateBufferWithMemory(
          10240, vk::BufferUsageFlagBits::eIndexBuffer,
          vk::MemoryPropertyFlagBits::eHostVisible | vk::MemoryPropertyFlagBits::eHostCoherent);

      auto vb2 = acg::gui::VkContext2::Instance().CreateBufferWithMemory(
          10240, vk::BufferUsageFlagBits::eVertexBuffer,
          vk::MemoryPropertyFlagBits::eHostVisible | vk::MemoryPropertyFlagBits::eHostCoherent);
      auto ib2 = acg::gui::VkContext2::Instance().CreateBufferWithMemory(
          10240, vk::BufferUsageFlagBits::eIndexBuffer,
          vk::MemoryPropertyFlagBits::eHostVisible | vk::MemoryPropertyFlagBits::eHostCoherent);

      ACG_INFO("created buffers");
      ppl.SetCamera(cam);
      ppl.SetLight(light);
      ppl.UpdateUbo();
      ACG_INFO("Update Ubo.");

      std::vector<acg::gui::details::WireframePoint> vert2;
      acg::gui::details::WireframePoint x, y, z;
      x.position = glm::vec3(-0.3, 0.3, 0.3);
      x.color = glm::vec3(0.3, 0.3, 0.3);
      y.position = glm::vec3(0.3, -0.3, 0.3);
      y.color = glm::vec3(0, 0, 0.3);
      z.position = glm::vec3(0.3, 0.3, 0.3);
      z.color = glm::vec3(0, 0.3, 0);
      std::vector<uint32_t> index2 = {0, 1, 1, 2, 2, 0};
      vert2.push_back(x);
      vert2.push_back(y);
      vert2.push_back(z);
      acg::gui::VkContext2::Instance().CopyHostToBuffer(vert2.data(), vb2,
                                                        sizeof(vert2.front()) * vert2.size());
      acg::gui::VkContext2::Instance().CopyHostToBuffer(index2.data(), ib2,
                                                        sizeof(index2.front()) * index2.size());

      auto vert = ball_vert();
      auto index = ball_index();
      acg::gui::VkContext2::Instance().CopyHostToBuffer(vert.data(), vb,
                                                        sizeof(vert.front()) * vert.size());
      acg::gui::VkContext2::Instance().CopyHostToBuffer(index.data(), ib,
                                                        sizeof(uint32_t) * index.size());

      acg::gui::details::UiPass::Config ui_pass_config;
      ui_pass_config.is_ui_only = false;
      acg::gui::details::UiPass ui_pass(ui_pass_config);

      acg::gui::details::WireframePipeline wf_ppl(render_pass, {});
      wf_ppl.SetCamera(cam);
      wf_ppl.UpdateUbo();

      float value[3]{1, 1, 1};
      acg::gui::details::MeshPushConstants pc;
      pc.options[0] = 0;
      pc.options[1] = 32;
      int i = 0;
      while (!acg::gui::Window::Instance().ShouldClose()) {
        glfwPollEvents();
        // Render.
        auto result = acg::gui::VkGraphicsContext::Instance().BeginDraw();
        if (!result) {
          acg::gui::VkGraphicsContext::Instance().RecreateSwapchain(true);
          render_pass.RecreateSwapchain();
          ppl.SetCamera(cam);
          ppl.UpdateUbo();
          wf_ppl.SetCamera(cam);
          wf_ppl.UpdateUbo();
          ui_pass.RecreateSwapchain();
          continue;
        }
        auto cbuf = render_pass.BeginRender();
        ppl.BeginPipeline(cbuf);
        pc.model = glm::scale(glm::vec3{value[2], value[2], value[2]})
                   * glm::rotate(static_cast<float>(i * acg::constants::pi<float> / 150.0),
                                 glm::vec3{0, 1, 0});
        cbuf.bindVertexBuffers(0, vb.GetBuffer(), static_cast<vk::DeviceSize>(0));
        cbuf.bindIndexBuffer(ib.GetBuffer(), 0, vk::IndexType::eUint32);
        cbuf.pushConstants(ppl.GetPipelineLayout(),
                           vk::ShaderStageFlagBits::eFragment | vk::ShaderStageFlagBits::eVertex, 0,
                           sizeof(pc), &pc);
        // cbuf.draw(3, 1, 0, 0);
        cbuf.drawIndexed(index.size(), 1, 0, 0, 0);
        ppl.EndPipeline(cbuf);

        wf_ppl.BeginPipeline(cbuf);
        cbuf.setLineWidth(1.0);
        cbuf.bindVertexBuffers(0, vb2.GetBuffer(), static_cast<vk::DeviceSize>(0));
        cbuf.bindIndexBuffer(ib2.GetBuffer(), 0, vk::IndexType::eUint32);
        cbuf.drawIndexed(index.size(), 1, 0, 0, 0);
        wf_ppl.EndPipeline(cbuf);

        render_pass.EndRender();

        // Ui Rendering.
        ImGui_ImplVulkan_NewFrame();
        ImGui_ImplGlfw_NewFrame();
        ImGui::NewFrame();
        ImGui::ShowUserGuide();
        ImGui::Begin("Control");
        bool changed = false;
        changed |= ImGui::SliderFloat3("Weights and size", value, 0, 1);
        changed |= ImGui::SliderFloat3("Parallel Light Direction", light.parallel_light_dir_.data(),
                                       -1, 1);
        if (changed) {
          light.light_density_ = value[0];
          light.parallel_light_density_ = value[1];
          ppl.SetLight(light);
          wf_ppl.SetCamera(cam);
          wf_ppl.UpdateUbo();
          ppl.UpdateUbo();
        }
        ImGui::End();
        ImGui::Render();
        auto* data = ImGui::GetDrawData();

        auto ui_cbuf = ui_pass.Render(data);
        auto result2 = acg::gui::VkGraphicsContext::Instance().EndDraw({cbuf, ui_cbuf});
        if (result2) {
          acg::gui::VkGraphicsContext::Instance().RecreateSwapchain(true);
          render_pass.RecreateSwapchain();
          ui_pass.RecreateSwapchain();
          ppl.SetCamera(cam);
          ppl.UpdateUbo();
          wf_ppl.SetCamera(cam);
          wf_ppl.UpdateUbo();
        }
        i += 1;
        acg::utils::SleepMs(30);
      }
      acg::gui::VkContext2::Instance().graphics_queue_.waitIdle();
      acg::gui::VkContext2::Instance().device_.waitIdle();
      acg::gui::VkContext2::Instance().DestroyBufferWithMemory(ib);
      acg::gui::VkContext2::Instance().DestroyBufferWithMemory(vb);
      acg::gui::VkContext2::Instance().DestroyBufferWithMemory(ib2);
      acg::gui::VkContext2::Instance().DestroyBufferWithMemory(vb2);
    }
  }
  acg::clean_up();
  return 0;
}
