#include <acg_core/init.hpp>
#include <acg_gui/backend/context.hpp>
#include <acg_gui/backend/graphics_context.hpp>
#include <acg_gui/backend/window.hpp>
#include <acg_gui/ggui.hpp>
#include <acg_utils/init.hpp>
#include <acg_utils/time.hpp>

#include "app.hpp"

using namespace acg::gui;

int main(int argc, char** argv) {
  // Setup all the global instances.
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
  GGui::Config config;
  config.init_default_scene = false;
  config.enable_blending = true;
  {
    GGui gui(config);
    bool clear = false;
    bool running = false;
    MpmExplictApp app;
    app.Init();
    gui.SetUIDrawCallback([&clear, &running, &app]() {
      ImGui::Begin("GGui User Window");
      clear = ImGui::Button("Reset Scene.");
      // running = ImGui::Button("Run Once");
      ImGui::Checkbox("Run", &running);
      acg::F64 vel_eng = app.particle_velocity_.array().square().sum();
      Vec3f pos = app.particle_position_.rowwise().mean().cast<F32>();
      ImGui::Text("Velocity Energy = %lf, Weight %lf", vel_eng, app.weight_sum);
      ImGui::InputFloat3("Position", pos.data());
      ImGui::DragFloat3("Gravity", app.grav_.data(), 0.1, -3, 3);
    });

    gui.GetScene().AddMeshParticles()
      .SetPositions(app.particle_position_.cast<F32>())
      .SetUniformColor(types::Rgba(1, 0, 0, 1))
      .SetRadius(0.01)
      .MarkUpdate();
    while (!Window::Instance().ShouldClose()) {
      glfwPollEvents();
      gui.Tick();
      gui.RenderOnce();
      gui.UpdateScene();
      if (clear) {
        app.Init();
        gui.GetScene().GetMeshParticles().front()
          .SetPositions(app.particle_position_.cast<F32>())
          .MarkUpdate();
      }

      if (running) {
        app.Run();
        gui.GetScene().GetMeshParticles().front()
          .SetPositions(app.particle_position_.cast<F32>())
          .MarkUpdate();
        // running = false;
        // acg::utils::SleepMs(500);
      }
    }
  }
  acg::clean_up();
  return EXIT_SUCCESS;
}
