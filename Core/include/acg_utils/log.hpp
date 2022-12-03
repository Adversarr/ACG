#pragma once

#include <fmt/format.h>
#include <spdlog/spdlog.h>
#ifndef ACG_IS_DEBUG
#  define ACG_IS_DEBUG (1 - ACG_IS_RELEASE)
#endif

#ifndef NDEBUG
#  define ACG_DEBUG_LOG(...) spdlog::debug(__VA_ARGS__)
#  include "tracef.hpp"
#  define LOG_FUNCTION_CALL_DEBUG \
    acg::utils::details::FunctionDeferredLogger _dloger(__PRETTY_FUNCTION__, __FILE__, __LINE__)
#else
#  define ACG_DEBUG_LOG(...) ((void)0)
#  define LOG_FUNCTION_CALL_DEBUG ((void)0)
#endif

#ifndef ACG_CHECK
#  include <iostream>
#  define ACG_CHECK(condition, ...)                                                  \
    do {                                                                             \
      bool retval = static_cast<bool>(condition);                                    \
      if (!retval) {                                                                 \
        auto msg = fmt::format(__VA_ARGS__);                                         \
        std::cerr << "Assertion(" << #condition << ") Failed: " << msg << std::endl; \
        std::cerr << "Occurs at " << __FILE__ << ":" << __LINE__ << std::endl;       \
        std::cerr.flush();                                                           \
      }                                                                              \
    } while (false)
#endif

#ifndef ACG_DEBUG_CHECK
#  if ACG_IS_DEBUG
#    define ACG_DEBUG_CHECK(condition, ...) ACG_CHECK(condition, __VA_ARGS__)
#  else
#    define ACG_DEBUG_CHECK(condition, message, ...) (condition)
#  endif
#endif
