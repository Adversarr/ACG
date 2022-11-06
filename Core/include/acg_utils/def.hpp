#pragma once
#include <cstddef>
#include "acg_core/core.hpp"
namespace acg{

namespace utils {

#ifdef NDEBUG
constexpr bool is_debug_mode = false;
#else
constexpr bool is_debug_mode = true;
#endif


enum class BuildType {
  kRelease, kDebug
};

inline constexpr BuildType get_build_type() {
  if constexpr (is_debug_mode) {
    return BuildType::kDebug;
  } else {
    return BuildType::kRelease;
  }
}

enum class PlatformType {
  kApple, kWin, kLinux
};

inline constexpr PlatformType get_platform_type() {
#ifdef __APPLE__
  return PlatformType::kApple;
#elifdef _WIN32
  return PlatformType::kWin;
#elifdef __linux__
  return PlatformType::kLinux;
#endif
}
}
}
