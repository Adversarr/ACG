#pragma once

#include "./common.hpp"

namespace acg::math {

template <typename T> constexpr T constant(T val) { return val; }

template <int times, typename T> constexpr auto pow(T&& value) noexcept {
  if constexpr (std::is_arithmetic_v<std::decay_t<T>>) {
    if constexpr (times > 0) {
      return value;
    } else {
      return static_cast<T>(1);
    }
  } else {
    return value.pow(times);
  }
}

template <typename T, typename = std::enable_if_t<std::is_arithmetic_v<std::decay_t<T>>>>
constexpr auto square(T&& value) noexcept {
  return value * value;
}

template <typename T> constexpr auto sqrt(T&& value) {
  if constexpr (std::is_arithmetic_v<std::decay_t<T>>) {
    return std::sqrt(value);
  } else {
    return value.sqrt();
  }
}

template <typename... Args> constexpr auto sum(Args&&... args) { return (args + ...); }

template <typename... Args> constexpr auto mean(Args&&... args) {
  return sum(args...) / sizeof...(args);
}

template <typename... Args> constexpr auto product(Args&&... args) { return (args * ...); }

}  // namespace acg::math

#include "details/func-inl.hpp"
