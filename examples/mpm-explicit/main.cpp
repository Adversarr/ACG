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
    gui.SetUIDrawCallback([&clear]() {
      ImGui::Begin("GGui User Window");
      ImGui::Text("Hello world!");
      clear = ImGui::Button("Clear Scene!");
    });
    MpmExplictApp app;
    app.Init();

    gui.GetScene().AddParticles()
      .SetPositions(app.particle_position_.cast<F32>())
      .SetUniformColor(types::Rgba(1, 0, 0, 1))
      .SetRadius(5)
      .MarkUpdate();
    while (!Window::Instance().ShouldClose()) {
      glfwPollEvents();
      gui.Tick();
      gui.RenderOnce();
      gui.UpdateScene();
      if (clear) {
        gui.ClearScene();
      }
    }
  }

  return EXIT_SUCCESS;
}
