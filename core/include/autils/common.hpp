#pragma once
namespace acg {

namespace utils {

/***************************************
       Likely support for brancing.
***************************************/
#ifdef __clang__
#if __cplusplus >= 201703L
#  define LIKELY [[likely]]
#  define UNLIKELY [[unlikely]]
#else
#  define LIKELY
#  define UNLIKELY
#endif
#else
#if __cplusplus >= 202003L
#  define LIKELY [[likely]]
#  define UNLIKELY [[unlikely]]
#else
#  define LIKELY
#  define UNLIKELY
#endif
#endif

/***************************************
 *           Force inline.
 ***************************************/
#ifndef forceinline
#  ifdef _MSC_VER_  // for MSVC
#    define forceinline inline __forceinline
#  elif defined __GNUC__       // for gcc on Linux/Apple OS X
#    define forceinline inline /* __attribute__((always_inline)) */
#  else
#    define forceinline inline
#  endif
#endif

/***************************************
             Debug Flags
***************************************/
#ifdef NDEBUG
constexpr bool is_debug_mode = false;
#else
constexpr bool is_debug_mode = true;
#endif

#ifndef ACG_IS_DEBUG
#  ifndef NDEBUG
#    define ACG_IS_DEBUG 1
#  else
#    define ACG_IS_DEBUG 0
#  endif
#endif

enum class BuildType { kRelease, kDebug };

inline constexpr BuildType get_build_type() {
  if constexpr (is_debug_mode) {
    return BuildType::kDebug;
  } else {
    return BuildType::kRelease;
  }
}

/***************************************
             Platform Flags
***************************************/
enum class PlatformType : int { kApple, kWin, kLinux };

inline constexpr PlatformType get_platform_type() {
#ifdef __APPLE__
  return PlatformType::kApple;
#elif defined(_WIN32)
  return PlatformType::kWin;
#elif defined(__linux__)
  return PlatformType::kLinux;
#endif
}


// @brief do_nothing function avoid compiler warns 'unused-variable'
template <typename... Args> inline void do_nothing(Args&&... ) {}

}  // namespace utils
}  // namespace acg
