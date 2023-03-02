#pragma once
#include <autils/god/utilities.hpp>
#include <tuple>

#include "spdlog/spdlog.h"
#include "spdlog/fmt/ostr.h"

#include "common.hpp"

namespace acg {
template <typename T, int d>
utils::god::DuplicateTuple<T, d> make_tuple_from_vector(acg::Vec<T, d> v);

}  // namespace acg

// template<typename OStream, typename Derived>
// OStream &operator<<(OStream &os, const Derived& mat)
// {
//     std::stringstream ss;
//     Eigen::IOFormat octave_fmt(Eigen::StreamPrecision, 0, ", ", ";\n", "", "", "[", "]");
//     ss << mat;
//     fmt::format_to(std::ostream_iterator<char>(os), "{}", ss.str());
//     return os;
// }


#include "details/utilities-inl.hpp"
