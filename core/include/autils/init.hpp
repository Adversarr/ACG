/**
 * Hooker for Utilities Library.
 */

#include <acore/init.hpp>
#include <autils/log.hpp>

#include "common.hpp"
namespace acg::utils {

struct UtilsHooker {
  spdlog::level::level_enum default_logging_level{acg::utils::is_debug_mode ? spdlog::level::debug
                                                                            : spdlog::level::info};

  bool use_spdlog_default{false};

  std::string log_pattern;

  void Hook();
};

void hook_default_utils_environment();

}  // namespace acg::utils
