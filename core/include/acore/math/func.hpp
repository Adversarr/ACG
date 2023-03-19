#pragma once

#include "./common.hpp"

namespace acg::math {

template <typename T> constexpr T constant(T val) { return val; }

template <int times, typename T> constexpr auto pow(T value) noexcept {
  if constexpr (std::is_arithmetic_v<std::decay_t<T>>) {
    if constexpr (times > 0) {
      return value * pow<times - 1, T>(value);
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

template <typename Scalar, typename = std::enable_if<std::is_floating_point_v<Scalar>>>
constexpr Scalar cubic_bspline(Scalar v) {
  Scalar av = std::abs(v);
  if (av < 1) {
    return .5 * pow<3>(av) - pow<2>(av) + (constant<Scalar>(2) / constant<Scalar>(3));
  } else if (av < 2) {
    return constant<Scalar>(1) / constant<Scalar>(6) * pow<3>(2 - av);
  } else {
    return 0;
  }
}

}  // namespace acg::math

#include "details/func-inl.hpp"
