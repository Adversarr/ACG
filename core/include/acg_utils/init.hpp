/**
 * Hooker for Utilities Library.
 */

#include "acg_core/init.hpp"
#include "common.hpp"
#include "acg_utils/log.hpp"
namespace acg::utils {

struct UtilsHooker {
  spdlog::level::level_enum default_logging_level
      = acg::utils::is_debug_mode ? spdlog::level::debug : spdlog::level::info;

  bool use_spdlog_default{false};

  std::string log_pattern;

  void Hook();
};

}  // namespace acg::utils
