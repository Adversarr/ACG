#pragma once
#include "../utilities.hpp"

namespace acg {
namespace details {

template <typename T, int d, int cur = 0>
inline void do_copy_tuple(utils::god::DuplicateTuple<T, d>& tup, const acg::Vec<T, d>& v) {
  if constexpr (cur < d) {
    std::get<cur>(tup) = v(cur);
    do_copy_tuple<T, d, cur + 1>(tup, v);
  }
}
}  // namespace details

template <typename T, int d>
utils::god::DuplicateTuple<T, d> make_tuple_from_vector(acg::Vec<T, d> v) {
  utils::god::DuplicateTuple<T, d> retval;
  details::do_copy_tuple(retval, v);
  return retval;
}
}  // namespace acg
