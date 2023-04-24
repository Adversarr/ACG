#pragma once

#include "../func.hpp"   // IWYU pragma: export
#include "../traits.hpp" // IWYU pragma: export

namespace acg::math {
template <typename InputType, typename = void> struct Func;

template <typename InputType>
struct Func<InputType,
            std::enable_if_t<!std::is_arithmetic_v<std::decay_t<InputType>>>> {
  // assert is `Eigen::Matrix`
  template <typename Derived>
  constexpr auto Sin(const Eigen::MatrixBase<Derived> &mat) const {
    return mat.array().sin().matrix();
  }

  template<int times, typename Derived>
  constexpr auto Pow(const Eigen::MatrixBase<Derived>& v) const noexcept {
    return v.array().pow(times).matrix();
  }

  template<typename Derived>
  constexpr auto Sqrt(const Eigen::MatrixBase<Derived>& v) const noexcept {
    return v.array.sqrt().matrix();
  }
};

template <typename T>
struct Func<T, std::enable_if_t<std::is_arithmetic_v<std::decay_t<T>>>> {
  constexpr T Sin(T input) const { return sin(input); }

  template <int times> constexpr T Pow(T value) const noexcept {
    if constexpr (times > 0) {
      if constexpr (times % 2 == 0) {
        return Pow<times / 2>(value) * Pow<times / 2>(value);
      } else {
        return value * Pow<times / 2>(value) * Pow<times / 2>(value);
      }
    } else {
      return static_cast<T>(1);
    }
  }
  
  constexpr T Sqrt(T input) const noexcept  {
    return std::sqrt(input);
  }
};

template <typename T> auto sin(T &&value) {
  return Func<std::decay_t<T>>{}.Sin(value);
}
} // namespace acg::math
