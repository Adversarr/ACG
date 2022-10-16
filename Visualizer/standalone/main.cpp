#include <spdlog/spdlog.h>

#include <acg_visualizer/renderer.hpp>

#include "co/co.h"

using namespace acg::visualizer::details;

int main() {
  spdlog::set_level(spdlog::level::debug);
  try{
    auto result = Renderer::Builder{}.Build();
    while(result->RunOnce()) {
      // Do nothing.
      co::sleep(30);
    }
  } catch (std::exception& e) {
    std::cout << e.what() << std::endl;
  }
  return 0;
}