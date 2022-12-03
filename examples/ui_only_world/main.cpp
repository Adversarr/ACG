#include <acg_gui/world_controller.hpp>
#include <acg_utils/log.hpp>
#include <spdlog/spdlog.h>
#include <acg_gui/hook.hpp>

int main(int argc, char** argv) {
  acg::visualizer::VkContextHooker::Hook();

  spdlog::set_level(spdlog::level::debug);
  acg::init(argc, argv);

  acg::visualizer::WorldCtrlUiOnly().Init().Run();
  acg::cleanup();
  return 0;
}
