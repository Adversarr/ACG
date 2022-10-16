#pragma once

#include "def.hpp"

#ifndef ACG_IS_DEBUG
#define ACG_IS_DEBUG (1 - ACG_IS_RELEASE)
#endif

#include <cstdlib>
#include <ostream>
#include "acg_utils.hpp"

namespace acg::utils::details {

template <int valid> void make_assert_details(
    bool value, std::string_view cond_text, std::string_view position, int line, std::string message) {
  if constexpr (valid) {
    if (!value) {
      std::cerr << "Assertion(" << cond_text << ") Failed: " << message << std::endl;
      std::cerr << " Occurs at " << position << ":" << line << "";
      std::exit(-1);
    }
  }
}

}  // namespace acg::utils::details

#ifndef __TO_STR
#define __TO_STR(n) #n
#endif

#ifndef ACG_DEBUG_CHECK
#  define ACG_DEBUG_CHECK(condition, message)                      \
    acg::utils::details::make_assert_details<is_debug_mode>( \
        static_cast<bool>(condition), #condition, __FILE__ , __LINE__, message)
#endif

#ifndef ACG_CHECK
#  define ACG_CHECK(condition, message) \
    acg::utils::details::make_assert_details<1>(static_cast<bool>(condition), #condition, __FILE__, __LINE__, message)
#endif