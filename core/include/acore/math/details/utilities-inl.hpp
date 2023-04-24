#pragma once
#include "../utilities.hpp"

namespace acg {
namespace details {

template <typename T, int d, int cur = 0>
inline void copy_vector_to_tuple(utils::god::DuplicateTuple<T, d> &tup,
                                 const acg::Vec<T, d> &v) {
  if constexpr (cur < d) {
    std::get<cur>(tup) = v(cur);
    copy_vector_to_tuple<T, d, cur + 1>(tup, v);
  }
}
} // namespace details

template <typename T, int d>
utils::god::DuplicateTuple<T, d> make_tuple_from_vector(acg::Vec<T, d> v) {
  utils::god::DuplicateTuple<T, d> retval;
  details::copy_vector_to_tuple(retval, v);
  return retval;
}
} // namespace acg

// Create formatter for Eigen::Matrix
template <typename T>
struct fmt::formatter<
    T, char,
    // Double check for Eigen::xxx.
    std::enable_if_t<std::is_void_v<std::void_t<typename T::Index>>,
                     std::void_t<decltype(std::declval<T>().derived())>>> {
  // Parses format specifications of the form ['f' | 'e'].
  constexpr auto parse(format_parse_context &ctx) // NOLINT
      -> decltype(ctx.begin()) {                  // NOLINT
    return ctx.begin();
  }
  // Formats the point p using the parsed format specification (presentation)
  // stored in this formatter.
  template <typename FormatContext>
  auto format(const T &p, FormatContext &ctx) const // NOLINT
      -> decltype(ctx.out()) {                      // NOLINT
    // ctx.out() is an output iterator to write to.
    return fmt::format_to(ctx.out(), "{}", fmt::streamed(p));
  }
};
