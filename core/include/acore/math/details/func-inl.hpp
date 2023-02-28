#pragma once

#include "../func.hpp"
#include "../traits.hpp"

namespace acg::math {
template <typename InputType, typename = void> struct Func {};

template <typename InputType>
struct Func<InputType, std::enable_if_t<!std::is_arithmetic_v<std::decay_t<InputType>>>> {
  template <typename Derived> constexpr auto sin(const Eigen::MatrixBase<Derived>& mat) const {
    return mat.array().sin().matrix();
  }
};

template <typename InputType>
struct Func<InputType, std::enable_if_t<std::is_arithmetic_v<std::decay_t<InputType>>>> {
  constexpr auto sin(InputType input) const { return ::sin(input); }
};

template <typename T> auto sin(T&& value) { return Func<std::decay_t<T>>{}.sin(value); }
}  // namespace acg::math
