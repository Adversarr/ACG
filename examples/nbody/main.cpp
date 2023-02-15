#include "nbody.hpp"
#include <autils/init.hpp>
#include <spdlog/spdlog.h>
#include <agui/init.hpp>

#include <acore/init.hpp>
int main(int argc, char** argv) {
  acg::gui::VkContextHooker().Hook();

  acg::utils::UtilsHooker{}.Hook();
  spdlog::set_level(spdlog::level::debug);
  acg::init(argc, argv);
  NBodySim(10).Init().Run().CleanUp();
  acg::clean_up();
  return 0;
}

