#pragma once

#include <fmt/format.h>
#include <iostream>
#include "acg_utils.hpp"


#ifndef ACG_IS_DEBUG
#  define ACG_IS_DEBUG (1 - ACG_IS_RELEASE)
#endif

#ifndef NDEBUG
  // TODO: Replace debug.
#include <spdlog/spdlog.h>
#  define ACG_DEBUG_LOG(...) spdlog::debug(__VA_ARGS__)
#  define LOG_FUNCTION_CALL_DEBUG \
    acg::utils::details::FunctionDeferredLogger ___(__PRETTY_FUNCTION__, __FILE__, __LINE__)
#else
#  define ACG_DEBUG_LOG(...) ((void*)0)
#  define LOG_FUNCTION_CALL_DEBUG ((void*)0)
#endif

namespace acg::utils::details {
template <int valid, typename... T> void make_assert_details(bool value, const char *cond_text,
                                                             const char* position, int line,
                                                             const char* message, T&&... args) {
  if constexpr (valid) {
    if (!value) {
      auto msg = fmt::format(message, std::forward<T>(args)...);
      // Do not use spdlog here.
      std::cerr << "Assertion(" << cond_text << ") Failed: " << msg << std::endl;
      std::cerr << "Occurs at " << position << ":" << line << std::endl;
      std::cerr.flush();
      throw std::runtime_error("AssertionError");
    }
  }
}

}  // namespace acg::utils::details


#ifndef ACG_CHECK
#define ACG_CHECK(condition, ...)\
  do { \
    bool retval = static_cast<bool>(condition); \
    if (!retval) {\
    auto msg = fmt::format(__VA_ARGS__);\
    std::cerr << "Assertion(" << #condition << ") Failed: " << msg << std::endl;\
    std::cerr << "Occurs at " << __FILE__ << ":" << __LINE__ << std::endl;\
    std::cerr.flush();\
  }} while (false)
#endif



#ifndef ACG_DEBUG_CHECK
#  if ACG_IS_DEBUG
#    define ACG_DEBUG_CHECK(condition, ...) ACG_CHECK(condition, __VA_ARGS__)
#  else
#    define ACG_DEBUG_CHECK(condition, message, ...) (condition)
#  endif
#endif