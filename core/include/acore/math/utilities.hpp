#pragma once
#include <autils/god/utilities.hpp>
#include <tuple>

#include "common.hpp"
#include <fmt/format.h>
#include <fmt/ostream.h>

namespace acg {
template <typename T, int d>
utils::god::DuplicateTuple<T, d> make_tuple_from_vector(acg::Vec<T, d> v);

}  // namespace acg

// Create formatter for Eigen::Matrix
template <typename T>
struct fmt::formatter<T, char,
                      // Double check for Eigen::xxx.
                      std::enable_if_t<std::is_void_v<std::void_t<typename T::Index>>,
                                       std::void_t<decltype(std::declval<T>().derived())>>> {
  // Parses format specifications of the form ['f' | 'e'].
  constexpr auto parse(format_parse_context& ctx) -> decltype(ctx.begin()) { return ctx.begin(); }
  // Formats the point p using the parsed format specification (presentation)
  // stored in this formatter.
  template <typename FormatContext> auto format(const T& p, FormatContext& ctx) const
      -> decltype(ctx.out()) {
    // ctx.out() is an output iterator to write to.
    return fmt::format_to(ctx.out(), "{}", fmt::streamed(p));
  }
};

#include "details/utilities-inl.hpp"
