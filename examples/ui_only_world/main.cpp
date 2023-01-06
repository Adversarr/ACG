#include <spdlog/spdlog.h>

#include <acg_core/init.hpp>
#include <acg_gui/init.hpp>
#include <acg_gui/world_controller.hpp>
#include <acg_utils/init.hpp>

int main(int argc, char** argv) {
  acg::gui::VkContextHooker().Hook();
  acg::utils::UtilsHooker{}.Hook();

  spdlog::set_level(spdlog::level::debug);
  acg::init(argc, argv);

  acg::gui::WorldCtrlUiOnly().Init().Run();
  acg::clean_up();
  return 0;
}
