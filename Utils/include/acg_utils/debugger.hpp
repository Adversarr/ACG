#pragma once
#include <cstdlib>
#include <ostream>

#include "acg_utils.hpp"

namespace acg::utils::details {

  template <typename... Args> inline void _print_helper(std::ostream& os, const Args&... args);

  template <> inline void _print_helper(std::ostream& os) { return; }

  template <typename T> inline void _print_helper(std::ostream& os, const T& t) {
    os << "" << t << std::endl;
  }

  template <typename T, typename... Args>
  inline void _print_helper(std::ostream& os, const T& t, const Args&... args) {
    os << t << std::endl;
    _print_helper(os, args...);
  }

  template <int valid, int errcode = -1, typename... Args>
  inline void make_assert_details(bool value, std::string_view cond_text, std::string_view arg_text,
                                  const Args&... args) {
    if constexpr (valid) {
      if (!value) {
        std::cerr << "Assertion(" << cond_text << ") Failed.";
        if constexpr (sizeof...(args) == 0) {
          return;
        } else {
          std::cerr << "Parameter: <" << arg_text << ">:\n";
          _print_helper(std::cerr, args...);  // TODO: last edit.
          std::cerr << std::endl;
          std::exit(errcode);
        }
      }
    }
  }

}  // namespace acg::utils::details

#ifndef ACG_DEBUG_CHECK
#  ifndef NDEBUG
#    define ACG_DEBUG_CHECK(condition, ...)                                              \
      acg::utils::details::make_assert_details<1>((condition), #condition, #__VA_ARGS__, \
                                                  __VA_ARGS__);
#  else
#    define ACG_DEBUG_CHECK(condition, ...)                                              \
      acg::utils::details::make_assert_details<0>((condition), #condition, #__VA_ARGS__, \
                                                  __VA_ARGS__);
#  endif
#endif

#ifndef ACG_CHECK
#  define ACG_CHECK(condition, ...) \
    acg::utils::details::make_assert_details<1>((condition), #condition, #__VA_ARGS__, __VA_ARGS__);
#endif