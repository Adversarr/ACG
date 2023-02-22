#include "autils/init.hpp"

#include "acore/init.hpp"
void acg::utils::UtilsHooker::Hook() {
  acg::details::InitHook hook;
  hook.name = "Logger";
  hook.priority = 100;
  hook.on_init
      = [*this]() { acg::utils::details::init_logger(default_logging_level, use_spdlog_default, log_pattern); };

  hook.on_exit = []() {
    acg::utils::details::cleanup_logger();
  };

  acg::details::add_hook(hook);
}

void acg::utils::hook_default_utils_environment() {
  auto utils_hooker = acg::utils::UtilsHooker();
  utils_hooker.default_logging_level = spdlog::level::debug;
  utils_hooker.Hook();
}
