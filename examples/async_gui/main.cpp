#include <acg_core/init.hpp>
#include <acg_gui/async_gui.hpp>
#include <acg_gui/backend/context.hpp>
#include <acg_gui/backend/graphics_context.hpp>
#include <acg_gui/backend/window.hpp>
#include <acg_utils/init.hpp>
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

  do {
    AsyncGui::Config config;
    AsyncGui ag(config);
    std::this_thread::sleep_for(std::chrono::seconds(3));
    ag.SignalShutdown();
    ag.WaitForShutdownActually();
  } while (false);

  acg::clean_up();
  return EXIT_SUCCESS;
}
