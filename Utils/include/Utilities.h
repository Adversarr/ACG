#pragma once
#include <cassert>
#include <iostream>

inline void RTE(std::string_view msg) {
  std::cerr << msg << std::endl;
  std::exit(-1);
}


#include "Serializer.h"