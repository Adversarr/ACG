#include "ff.hpp"
#include <acg_utils/log.hpp>
#include <spdlog/spdlog.h>
#include <acg_vis/hook.hpp>

int main(int argc, char** argv) {
  acg::visualizer::VkContextHooker::Hook();
  spdlog::set_level(spdlog::level::debug);
  acg::init(argc, argv);
  FreeFall(10).Init().Run().CleanUp();
  acg::cleanup();
  return 0;
}

