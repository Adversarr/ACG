#pragma once

#if NDEBUG
// release mode.
#  ifdef ACG_IS_RELEASE
static_assert(ACG_IS_RELEASE, "mixed release and debug compile mode, expect release")
#  else
#    define ACG_IS_RELEASE 1
#  endif
#else
// debug mode.
#  ifdef ACG_IS_RELEASE
static_assert(1 - ACG_IS_RELEASE, "mixed release and debug compile mode, expect release")
#  else
#    define ACG_IS_RELEASE 0
#  endif
#endif

#ifndef ACG_IS_DEBUG
#define ACG_IS_DEBUG (1 - ACG_IS_RELEASE)
#endif

#include <cstdlib>
#include <ostream>
#include "acg_utils.hpp"

namespace acg::utils::details {

template <typename... Args> void _print_parameter_pair(const Args&... arg);

template <typename T> void _print_parameter_pair(const T& v) {
  std::cerr << v << std::endl;
}

template <typename T1, typename ... Args> void _print_parameter_pair(const T1& t, const Args&... rest) {
  _print_parameter_pair(t);
  _print_parameter_pair(rest...);
}

template <> void _print_parameter_pair() {}

template <int valid, typename... Args> void make_assert_details(
    bool value, std::string_view cond_text, std::string_view position, int line, std::string_view param, const Args&... args) {
  if constexpr (valid) {
    if (!value) {
      std::cerr << position << ":" << line << ":";
      std::cerr << " Assertion(" << cond_text << ") Failed." << std::endl;
      std::cerr << "assist variables: " << param << std::endl;
      _print_parameter_pair(args...);
      std::exit(-1);
    }
  }
}

template <int valid> void make_assert_details(
    bool value, std::string_view cond_text, std::string_view position, int line) {
  if constexpr (valid) {
    if (!value) {
      std::cerr << position << ":" << line << ":";
      std::cerr << " Assertion(" << cond_text << ") Failed." << std::endl;
      std::exit(-1);
    }
  }
}
}  // namespace acg::utils::details

#ifndef __TO_STR
#define __TO_STR(n) #n
#endif

#ifndef ACG_DEBUG_CHECK
#  define ACG_DEBUG_CHECK(condition, ...)                      \
    acg::utils::details::make_assert_details<ACG_IS_DEBUG>( \
        (condition), #condition, __FILE__ , __LINE__ __VA_OPT__(, #__VA_ARGS__, __VA_ARGS__))
#endif

#ifndef ACG_CHECK
#  define ACG_CHECK(condition, ...) \
    acg::utils::details::make_assert_details<1>((condition), #condition, #__VA_ARGS__, __VA_ARGS__);
#endif