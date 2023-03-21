#pragma once
#include <acore/common.hpp>
#include <autils/common.hpp>
#include <tuple>
#include <utility>

#include "god.hpp"

namespace acg::utils {

namespace god {

struct LvalueTag {};
struct RvalueTag {};
struct ConstRvalueTag {};
struct ConstLvalueTag {};

template <typename T, int count> using DuplicateTuple =
    typename Duplicate_t<T, count>::template cast<std::tuple>;

template <int count> using IndexTuple = DuplicateTuple<Index, count>;

template <int count, typename T> constexpr auto tuple_duplicate(T value) {
  static_assert(count >= 0, "You cannot have a tuple with negative size.");

  if constexpr (count > 0) {
    return std::tuple_cat(std::tuple<T>(value),
                          god::tuple_duplicate<count - 1>(value));
  } else {
    return std::tuple<>();
  }
}
/**
 * @brief compute the product of input arguments
 */
template <typename... Args> constexpr auto product(Args... args) {
  static_assert(sizeof...(args) > 0,
                "with no argument, cannot deduct the initial value.");
  return (args * ...);
}

template <int alpha, typename T> constexpr auto pow(T value) {
  if constexpr (alpha > 0) {
    return value * pow<alpha - 1>(value);
  } else {
    return static_cast<T>(1);
  }
}

template <typename Arg> void sort3(Arg& x, Arg& y, Arg& z) {
  if (x > y) {
    std::swap(x, y);
  }

  if (x > z) {
    std::swap(x, z);
  }

  if (y > z) {
    std::swap(y, z);
  }
}

}  // namespace god

}  // namespace acg::utils
