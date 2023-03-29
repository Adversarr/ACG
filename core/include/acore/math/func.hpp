#pragma once
#include "details/func-inl.hpp"
#include <Eigen/Core>
#include <type_traits>

namespace acg::math {

template <typename T> constexpr T constant(T val) {
  // TODO: add other types.
  return val;
}

template <typename Functor, typename Derived>
inline auto matrix_apply(const Eigen::MatrixBase<Derived>& v) {
  auto result = v.eval();
  for (auto &c : result.reshaped()) {
    c = Functor{}(c);
  }
  return result;
}

template <int times, typename T> constexpr auto pow(T value) noexcept {
  static_assert(times >= 0, "constexpr pow requires times >= 0.");
  return Func<T>{}.template Pow<times>(value);
}

template <typename T,
          typename = std::enable_if_t<std::is_arithmetic_v<std::decay_t<T>>>>
constexpr auto square(T &&value) noexcept {
  return value * value;
}

template <typename T> constexpr auto sqrt(T &&value) {
  if constexpr (std::is_arithmetic_v<std::decay_t<T>>) {
    return std::sqrt(value);
  } else {
    return value.sqrt();
  }
}

template <typename... Args> constexpr auto sum(Args &&...args) {
  return (args + ...);
}

template <typename... Args> constexpr auto mean(Args &&...args) {
  static_assert(sizeof...(args) > 0,
                "Mean requires #args > 0, for #args is the denominator.");
  return sum(args...) / sizeof...(args);
}

template <typename... Args> constexpr auto product(Args &&...args) {
  static_assert(sizeof...(args) > 0,
                "Product requires #args > 0 for an initial value.");
  return (args * ...);
}

template <typename Scalar,
          typename = std::enable_if<std::is_floating_point_v<Scalar>>>
constexpr Scalar cubic_bspline(Scalar v) {
  Scalar av = std::abs(v);
  if (av < 1) {
    return .5 * pow<3>(av) - pow<2>(av) +
           (constant<Scalar>(2) / constant<Scalar>(3));
  } else if (av < 2) {
    return constant<Scalar>(1) / constant<Scalar>(6) * pow<3>(2 - av);
  } else {
    return 0;
  }
}

} // namespace acg::math
