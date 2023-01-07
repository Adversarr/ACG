#pragma once
#include <fmt/format.h>
#include <spdlog/common.h>

#include "spdlog/logger.h"

#ifndef ACG_IS_DEBUG
#  ifdef NDEBUG
#    define ACG_IS_DEBUG 1
#  else
#    define ACG_IS_DEBUG 0
#  endif
#endif
namespace acg {
namespace utils {
namespace details {

std::shared_ptr<spdlog::logger>& get_default_logger(bool cached = true);

std::shared_ptr<spdlog::logger> get_logger(const std::string& name);

void add_logger(const std::shared_ptr<spdlog::logger>& logger);

constexpr const char* default_logger_name = "AcgDefaultLogger";

void init_logger(spdlog::level::level_enum default_level, bool use_spdlog_default,
                 std::string pattern);

void cleanup_logger();

}  // namespace details

bool is_logger_inited();

inline auto get_default_logger() { return details::get_default_logger(); }

inline auto get_logger(const std::string& name) { return details::get_logger(name); }

using LogLevel = spdlog::level::level_enum;

inline void set_default_log_level(LogLevel lvl) {
  get_default_logger()->set_level(lvl);
}


#ifndef ACG_LOG
// Standard Logging methods.
#  define ACG_LOG(...) ((acg::utils::details::get_default_logger())->log(__VA_ARGS__))
#  define ACG_DEBUG(...) ACG_LOG(spdlog::level::level_enum::debug, __VA_ARGS__)
#  define ACG_TRACE(...) ACG_LOG(spdlog::level::level_enum::trace, __VA_ARGS__)
#  define ACG_INFO(...) ACG_LOG(spdlog::level::level_enum::info, __VA_ARGS__)
#  define ACG_WARN(...) ACG_LOG(spdlog::level::level_enum::warn, __VA_ARGS__)
#  define ACG_ERROR(...) ACG_LOG(spdlog::level::level_enum::err, __VA_ARGS__)
#  define ACG_CRITICAL(...) ACG_LOG(spdlog::level::level_enum::critical, __VA_ARGS__)
#endif

#ifndef ACG_DEBUG_LOG
// Logging methods which only takes effect in debug mode.
#  ifndef NDEBUG
#    define ACG_DEBUG_LOG(...)                                                                \
      ACG_LOG(spdlog::source_loc(__FILE__, __LINE__, static_cast<const char*>(__FUNCTION__)), \
              spdlog::level::level_enum::debug, __VA_ARGS__)
#  else
#    define ACG_DEBUG_LOG(...) ((void*)0)
#  endif
#endif

}  // namespace utils
}  // namespace acg

#ifndef ACG_CHECK
#  include <iostream>
#  define ACG_CHECK(condition, ...)                                              \
    do {                                                                         \
      bool retval = static_cast<bool>(condition);                                \
      if (!retval) {                                                             \
        auto msg = fmt::format(__VA_ARGS__);                                     \
        if (acg::utils::is_logger_inited()) {                                    \
          ACG_CRITICAL("Assertion({}) Failed: {}", #condition, msg);             \
          ACG_CRITICAL("Occurs at {}:{}", __FILE__, __LINE__);                   \
        } else {                                                                 \
          std::cerr << "Assertion(" #condition ") Failed: " << msg << std::endl; \
          std::cerr << "Occurs at " << __FILE__ << ":" << __LINE__ << std::endl; \
          std::cerr.flush();                                                     \
        }                                                                        \
        std::exit(-1);                                                           \
      }                                                                          \
    } while (false)
#endif

#ifndef ACG_DEBUG_CHECK
#  if ACG_IS_DEBUG
#    define ACG_DEBUG_CHECK(condition, ...) ACG_CHECK(condition, __VA_ARGS__)
#  else
#    define ACG_DEBUG_CHECK(condition, message, ...) ((void*)0)
#  endif
#endif
