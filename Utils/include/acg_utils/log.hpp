#pragma once
#include <spdlog/spdlog.h>

#include "acg_utils.hpp"
#include "def.hpp"

#ifndef ACG_IS_DEBUG
#  define ACG_IS_DEBUG (1 - ACG_IS_RELEASE)
#endif

#ifndef NDEBUG
#  define ACG_DEBUG_LOG(...) spdlog::debug(__VA_ARGS__)
#  define LOG_FUNCTION_CALL_DEBUG \
    acg::utils::details::FunctionDeferredLogger ___(__PRETTY_FUNCTION__, __FILE__, __LINE__)
#else
#  define ACG_DEBUG_LOG(...) ((void*)0)
#  define LOG_FUNCTION_CALL_DEBUG ((void*)0)
#endif

namespace acg::utils::details {

class FunctionDeferredLogger {
private:
  const char* function_name_;

public:
  inline explicit FunctionDeferredLogger(const char* fname, const char* file, const int line)
      : function_name_{fname} {
    spdlog::debug("Function Call: <{}> at {}:{}", fname, file, line);
  }

  inline ~FunctionDeferredLogger() { spdlog::debug("Function Exiting. <{}>", function_name_); }
};

template <int valid, typename... T> void make_assert_details(bool value, std::string_view cond_text,
                                                             std::string_view position, int line,
                                                             const char* message, T&&... args) {
  if constexpr (valid) {
    if (!value) {
      spdlog::critical("Assertion({}) Failed: {}\n Occurs at {}:{}", cond_text,
                       fmt::format(message, std::forward<T>(args)...), position, line);
      std::exit(-1);
    }
  }
}

}  // namespace acg::utils::details

#ifndef ACG_DEBUG_CHECK
#  if ACG_IS_DEBUG
#    define ACG_DEBUG_CHECK(condition, message, ...)                                          \
      acg::utils::details::make_assert_details<is_debug_mode>(static_cast<bool>(condition),   \
                                                              #condition, __FILE__, __LINE__, \
                                                              message __VA_OPT__(, __VA_ARGS__))
#  else
#    define ACG_DEBUG_CHECK(condition, message, ...) (condition)
#  endif
#endif

#ifndef ACG_CHECK
#  define ACG_CHECK(condition, message, ...)                                              \
    acg::utils::details::make_assert_details<1>(static_cast<bool>(condition), #condition, \
                                                __FILE__, __LINE__,                       \
                                                message __VA_OPT__(, __VA_ARGS__))
#endif
