#include "mass_spring.hpp"
#include <acg_utils/init.hpp>
#include <spdlog/spdlog.h>
#include <acg_gui/init.hpp>
#include <acg_core/init.hpp>
int main(int argc, char** argv) {
  acg::gui::VkContextHooker().Hook();
  acg::utils::UtilsHooker{}.Hook();
  acg::init(argc, argv);
  MassSpring(10).Init().Run().CleanUp();
  acg::clean_up();
  return 0;
}

