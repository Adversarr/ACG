#include <acg_gui/world_controller.hpp>
#include <acg_utils/log.hpp>
#include <spdlog/spdlog.h>
#include <acg_gui/hook.hpp>
#include <acg_core/init.hpp>

int main(int argc, char** argv) {
  acg::gui::VkContextHooker::Hook();

  spdlog::set_level(spdlog::level::debug);
  acg::init(argc, argv);

  acg::gui::WorldCtrlUiOnly().Init().Run();
  acg::clean_up();
  return 0;
}
