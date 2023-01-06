#include <spdlog/spdlog.h>

#include <acg_core/init.hpp>
#include <acg_gui/init.hpp>
#include <acg_utils/init.hpp>
#include <acg_gui/backend/context.hpp>

#include "api.hpp"
int main(int argc, char** argv) {
  auto utils_hooker = acg::utils::UtilsHooker();
  utils_hooker.default_logging_level = spdlog::level::debug;
  utils_hooker.Hook();
  acg::gui::WindowHooker{"ggui"}.Hook();
  auto vkctx_hooker = acg::gui::VkContext2Hooker{};
  vkctx_hooker.app_name = "Vulkan Viewer";
  vkctx_hooker.enable_validation = true;
  vkctx_hooker.Hook();
  acg::init(argc, argv);
  acg::clean_up();
  return 0;
}
