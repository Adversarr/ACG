#include <spdlog/spdlog.h>

#include <acore/init.hpp>
#include <agui/init.hpp>
#include <agui/world_controller.hpp>
#include <autils/init.hpp>

int main(int argc, char** argv) {
  acg::gui::VkContextHooker().Hook();
  acg::utils::UtilsHooker{}.Hook();

  spdlog::set_level(spdlog::level::debug);
  acg::init(argc, argv);

  acg::gui::WorldCtrlUiOnly().Init().Run();
  acg::clean_up();
  return 0;
}
