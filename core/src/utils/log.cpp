#include "acg_utils/log.hpp"

#include <spdlog/sinks/stdout_color_sinks.h>
#include <spdlog/spdlog.h>

#include "acg_utils/common.hpp"

namespace acg {
namespace utils {
namespace details {

std::shared_ptr<spdlog::logger> default_logger;

static bool is_initialized{false};

std::shared_ptr<spdlog::logger>& get_default_logger(bool cached) {
  if (cached) {
    return default_logger;
  } else {
    default_logger = spdlog::get(default_logger_name);
    return default_logger;
  }
}

std::shared_ptr<spdlog::logger> get_logger(const std::string& name) { return spdlog::get(name); }

void add_logger(const std::shared_ptr<spdlog::logger>& logger) { spdlog::register_logger(logger); }

void init_logger(spdlog::level::level_enum default_level, bool use_spdlog_default, 
                 std::string pattern) {
  if (use_spdlog_default) {
    default_logger = spdlog::default_logger();
  } else {
    default_logger = spdlog::stdout_color_mt(default_logger_name);
    spdlog::set_default_logger(default_logger);
  }

  if (pattern.empty()) {
    default_logger->set_pattern("[%H:%M:%S:%e] [%^%L%$] [T:%t] %v");
  } else {
    default_logger->set_pattern(pattern);
  }
  default_logger->set_level(default_level);
  if constexpr (acg::utils::is_debug_mode) {
    ACG_DEBUG_LOG("Default Logger Initialized.");
  } else {
    ACG_INFO("Default Logger Initialized.");
  }
  is_initialized = true;
}

void cleanup_logger() {
  // Release the default logger.
  default_logger.reset();
  is_initialized = false;
}


}  // namespace details
bool is_logger_inited() {
  return details::is_initialized;
}
}  // namespace utils
}  // namespace acg
