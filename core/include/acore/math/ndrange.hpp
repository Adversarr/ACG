#pragma once

#include <array>
#include <autils/common.hpp>
#include <autils/god/utilities.hpp>

#include "../common.hpp"

namespace acg {

namespace details {

template <Index dim> constexpr auto get_zero_array() {
  std::array<Index, dim> arr;
  for (auto &a : arr) {
    a = 0;
  }
  return arr;
}

template <Index dim> constexpr auto get_zero_tuple() {
  if constexpr (dim > 0) {
    return std::tuple_cat(get_zero_tuple<dim - 1>(), std::make_tuple<acg::Index>(0));
  } else {
    return std::tuple_cat();
  }
}

template <Index dim> constexpr auto get_zero_tuple(acg::Index front) {
  return std::tuple_cat(std::tuple<Index>(front), get_zero_tuple<dim - 1>());
}

}  // namespace details

template <int dim> struct NdRangeIterator {
  using iterator_category = std::random_access_iterator_tag;

  using container_type =
      typename utils::god::Duplicate_t<acg::Index, dim>::template cast<std::tuple>;

  constexpr decltype(auto) operator*() const { return dims_; }

  constexpr bool operator!=(const NdRangeIterator &another) const { return dims_ != another.dims_; }

  template <int d> inline void Advance(char (*)[d >= 0] = 0) {
    auto &cur_dim = std::get<d>(dims_);
    ++cur_dim;
    if constexpr (d > 0) {
      if (cur_dim >= std::get<d>(ub_)) UNLIKELY {
          cur_dim = 0;
          Advance<d - 1>();
        }
    }
  }

  template <int d> inline void Advance(char (*)[d < 0] = 0) {
    assert(false && "Internal Error: Advance the NdRangeIndexer at end.");
  }

  NdRangeIterator<dim> &operator++() {
    Advance<dim - 1>();
    return *this;
  }

  constexpr explicit NdRangeIterator(const container_type &ub)
      : dims_(details::get_zero_tuple<dim>()), ub_(ub) {}

  constexpr NdRangeIterator(const NdRangeIterator &) = default;

  constexpr NdRangeIterator(const container_type &dims, const container_type &ub)
      : dims_(dims), ub_(ub) {}
  container_type dims_;
  const container_type ub_;
};

template <int dim> struct NdRange {
  using container_type = utils::god::IndexTuple<dim>;

  // Same as 'Indexer'
  // 1. Tuple input, i.e. container_type initialization
  constexpr explicit NdRange(const container_type &dims) : dims_{dims} {}

  // 2. An input sequence, that is able to construct a container_type.
  template <typename... Args,
            typename
            = std::enable_if_t<std::is_constructible_v<container_type, Index, Args...>
                               && !std::is_same_v<std::remove_cv_t<Index>, container_type>>>
  constexpr explicit NdRange(Index a, Args &&...args) : NdRange(container_type(a, args...)) {}

  // Cpp Iterator.
  constexpr decltype(auto) begin() const { return NdRangeIterator<dim>(dims_); }

  constexpr decltype(auto) end() const {
    return NdRangeIterator<dim>(details::get_zero_tuple<dim>(std::get<0>(dims_)), dims_);
  }
  const container_type dims_;
};

template <typename... Args> NdRange(std::tuple<Args...> &&dims) -> NdRange<sizeof...(Args)>;

template <typename... Args> NdRange(Args &&...dims) -> NdRange<sizeof...(Args)>;
}  // namespace acg
