#pragma once
#include <acore/common.hpp>
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

template <int count, typename T> constexpr auto duplicate(T value) {
  if constexpr (count > 0) {
    return std::tuple_cat(std::tuple<T>(value), god::duplicate<count - 1>(value));
  } else {
    return std::tuple<>();
  }
}
/**
 * @brief compute the product of input arguments
 */
template <typename... Args> constexpr decltype(auto) product(Args... args) {
  if constexpr (sizeof...(args) == 0) {
    return 1;
  } else {
    return (args * ...);
  }
}
template <int alpha, typename T> constexpr T pow(T value) {
  // return std::apply(product, tuple_duplicate<alpha>(value));
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
