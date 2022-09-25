#pragma once
#include <cassert>
#include <iostream>
#include <string_view>

inline void RTE(std::string_view msg) {
  std::cerr << msg << std::endl;
  std::exit(-1);
}

#define CHECK_ELSE_RETURN_NULLOPT(C) if (! ((bool) (C))) return std::nullopt

namespace acg {

#ifdef NDEBUG
constexpr bool release_mode = true;
#else
constexpr bool release_mode = false;
#endif

}