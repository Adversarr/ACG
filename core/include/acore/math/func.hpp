#pragma once

#include "./common.hpp"

namespace acg::math {

template <typename T> constexpr T constant(T val) { return val; }

template <typename T, int times, typename = std::enable_if_t<std::is_arithmetic_v<std::decay_t<T>>>>
constexpr auto pow(T&& value) noexcept {
  if constexpr (times > 0) {
    return value;
  } else {
    return static_cast<T>(1);
  }
}

template <typename T, typename = std::enable_if_t<std::is_arithmetic_v<std::decay_t<T>>>>
constexpr auto square(T&& value) noexcept {
  return value * value;
}

}  // namespace acg::math

#include "details/func-inl.hpp"
