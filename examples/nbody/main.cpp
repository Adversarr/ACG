#include "nbody.hpp"
#include <acg_utils/log.hpp>
#include <spdlog/spdlog.h>
#include <acg_gui/hook.hpp>

int main(int argc, char** argv) {
  acg::gui::VkContextHooker::Hook();
  spdlog::set_level(spdlog::level::debug);
  acg::init(argc, argv);
  NBodySim(10).Init().Run().CleanUp();
  acg::clean_up();
  return 0;
}

