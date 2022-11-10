#include <acg_vis/nbody.hpp>
#include <acg_utils/log.hpp>
#include <spdlog/spdlog.h>
#include <acg_vis/hook.hpp>

int main(int argc, char** argv) {
  acg::visualizer::VkContextHooker::Hook();

  spdlog::set_level(spdlog::level::debug);
  acg::init(argc, argv);

  acg::visualizer::MeshWorldCtrl(5).Run();
  acg::cleanup();
  return 0;
}

