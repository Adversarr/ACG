#pragma once
#include <cassert>
#include <iostream>

inline void RTE(std::string_view msg) {
  std::cerr << msg << std::endl;
  std::exit(-1);
}

#define CHECK_ELSE_RETURN_NULLOPT(C) if (! ((bool) (C))) return std::nullopt

namespace agl {}