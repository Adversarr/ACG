#include <acg_core/init.hpp>
#include <acg_gui/backend/context.hpp>
#include <acg_gui/backend/graphics_context.hpp>
#include <acg_gui/backend/window.hpp>
#include <acg_gui/ggui.hpp>
#include <acg_utils/init.hpp>
#include <acg_utils/time.hpp>

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
  config.init_default_scene = true;
  config.enable_blending = true;
  {
    GGui gui(config);
    gui.SetUIDrawCallback([]() {
      ImGui::Begin("GGui User Window");
      ImGui::Text("Hello world!");
    });
    while (!Window::Instance().ShouldClose()) {
      glfwPollEvents();
      gui.Tick();
      gui.RenderOnce();
      gui.UpdateScene();
    }
  }

  acg::clean_up();
  return EXIT_SUCCESS;
}
