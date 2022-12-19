#include "mass_spring.hpp"
#include <acg_utils/log.hpp>
#include <spdlog/spdlog.h>
#include <acg_gui/hook.hpp>
#include <acg_core/init.hpp>
int main(int argc, char** argv) {
  acg::gui::VkContextHooker::Hook();
  spdlog::set_level(spdlog::level::debug);
  acg::init(argc, argv);
  MassSpring(25).Init().Run().CleanUp();
  acg::clean_up();
  return 0;
}

