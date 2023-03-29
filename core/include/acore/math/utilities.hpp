#pragma once
#include <autils/god/utilities.hpp>
#include <tuple>

#include "common.hpp"
#include <fmt/format.h>
#include <fmt/ostream.h>

namespace acg {
template <typename T, int d>
utils::god::DuplicateTuple<T, d> make_tuple_from_vector(acg::Vec<T, d> v);

} // namespace acg

#include "details/utilities-inl.hpp"
