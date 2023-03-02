#pragma once
#include <acore/common.hpp>
#include <acore/math/ndrange.hpp>
#include <autils/god/utilities.hpp>

namespace acg {
/****************************************
 * NOTE: Foreach Indexer, it have:
 *  1. value_type: indicates the return value type
 *  2. operator(...): Physical Range -> Device Range
 *  3. operator[d]: Device Range -> Physical Range
 *  4. Iterate(): To corresponding Iterator.
 *  5. Fit(field) for each indexer.
 ****************************************/

template <Index dim> class NdRangeIndexer;

template <> class NdRangeIndexer<0> {
public:
  inline constexpr Index operator()() const noexcept { return 0; }

  inline constexpr std::tuple<> operator[](Index) const noexcept { return {}; }

  static inline constexpr std::tuple<> Shape() noexcept { return {}; }
};

template <> class NdRangeIndexer<1> {
public:
  inline Index operator()(Index id) const { return id; }

  explicit constexpr NdRangeIndexer(Index this_dim = 0) : this_dim(this_dim) {}

  constexpr auto operator[](Index id) const noexcept { return std::tuple<Index>(id); }

  constexpr auto Shape() const noexcept { return std::tuple<Index>(this_dim); }

  constexpr auto Iterate() const noexcept { return NdRange<1>(this_dim); }

  template <typename T> inline void Fit(T&& field) { this_dim = field.cols(); }

protected:
  Index this_dim;
};

template <Index dim> class NdRangeIndexer : public NdRangeIndexer<dim - 1> {
public:
  template <typename... Args> constexpr explicit NdRangeIndexer(Index this_dim, Args... arg)
      : NdRangeIndexer<dim - 1>(arg...),
        this_dim(this_dim),
        multiplier(acg::utils::god::product(static_cast<Index>(arg)...)) {}

  constexpr explicit NdRangeIndexer(Index dim_universal)
      : NdRangeIndexer<dim - 1>(dim_universal),
        this_dim(dim_universal),
        multiplier(acg::utils::god::pow<dim - 1>(dim_universal)) {}

  template <typename... Args>
  constexpr Index operator()(Index this_size, Args... indices) const noexcept {
    return this_size * multiplier
           + NdRangeIndexer<dim - 1>::operator()(static_cast<Index>(indices)...);
  }

  constexpr auto operator[](Index id) const noexcept {
    return std::tuple_cat(std::tuple<Index>(id / multiplier),
                          NdRangeIndexer<dim - 1>::operator[](id % multiplier));
  }

  constexpr auto Shape() const noexcept {
    return std::tuple_cat(std::tuple<Index>(this_dim), NdRangeIndexer<dim - 1>::Shape());
  }

  constexpr auto Iterate() const noexcept { return NdRange<dim>(Shape()); }

  template <typename T> inline void Fit(T&& /*field*/) {}

protected:
  const Index this_dim{0};
  const Index multiplier{0};
};

}  // namespace acg
