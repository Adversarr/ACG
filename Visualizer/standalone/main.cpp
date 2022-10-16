#include <acg_visualizer/vkwindow.hpp>
#include <acg_visualizer/vkinst.hpp>
#include "co/co.h"
#include <spdlog/spdlog.h>

using namespace acg::visualizer::details;

int main() {
  spdlog::set_level(spdlog::level::debug);
  try{
    auto result = Visualizer::VkInstBuilder{}.Build();
    while(result.RunOnce()) {
      // Do nothing.
      co::sleep(30);
    }
  } catch (std::exception& e) {
    std::cout << e.what() << std::endl;
  }
  return 0;
}