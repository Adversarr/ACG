#include <acore/init.hpp>
#include <agui/backend/context.hpp>
#include <agui/backend/graphics_context.hpp>
#include <agui/backend/window.hpp>
#include <agui/gui.hpp>
#include <autils/init.hpp>
#include <autils/time.hpp>

using namespace acg::gui;

int main(int argc, char** argv) {
  // Setup all the global instances.
  auto utils_hooker = acg::utils::UtilsHooker();
  utils_hooker.default_logging_level = spdlog::level::debug;
  utils_hooker.Hook();
  acg::gui::Window::Hooker{"GGui"}.Hook();
  auto vkctx_hooker = acg::gui::VkContext2Hooker{};
  vkctx_hooker.app_name = "GGui";
  vkctx_hooker.enable_validation = true;
  vkctx_hooker.Hook();
  VkGraphicsContext::Hooker().Hook();
  Gui::Config config;
  config.init_default_scene = true;
  config.enable_blending = true;
  config.Hook();
  acg::init(argc, argv);
  auto& gui = Gui::Instance();
  bool clear = false;
  gui.SetUIDrawCallback([&clear]() {
    ImGui::Begin("GGui User Window");
    ImGui::Text("Hello world!");
    clear = ImGui::Button("Clear Scene!");
  });
  while (!Window::Instance().ShouldClose()) {
    glfwPollEvents();
    gui.Tick();
    gui.RenderOnce();
    gui.UpdateScene();
    if (clear) {
      gui.ClearScene();
    }
  }
  acg::clean_up();
  return EXIT_SUCCESS;
}
