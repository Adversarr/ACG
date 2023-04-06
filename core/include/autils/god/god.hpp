/**
 * God is a tiny Compile Time Computation and Meta Programming Library
 */

#pragma once
#include <type_traits>

// NOLINTBEGIN(readability-identifier-naming)
#include "acore/common.hpp"

namespace acg::utils::god {
namespace details {

/**
 * NOTE: Compile Time Programming Utilities
 */
struct Empty {};

// H, T... => H
template <typename... T> struct Front {};

template <> struct Front<> {
  using type = Empty;
};

template <typename H, typename... T> struct Front<H, T...> {
  using type = H;
};

// H..., T => T
template <typename... T> struct Back;

template <> struct Back<> {
  using type = Empty;
};

template <typename T> struct Back<T> {
  using type = T;
};

template <typename H, typename... T> struct Back<H, T...> {
  using type = typename Back<T...>::type;
};

// Type List Template
template <typename... T> struct List {
  using type = List<T...>;

  constexpr static acg::size_t size_ = sizeof...(T);

  template <template <typename...> typename C> using cast = C<T...>;

  template <typename... Ts> using extend = List<T..., Ts...>;

  template <typename Ts> using append = List<T..., Ts>;

  template <typename Ts> using prepend = List<Ts, T...>;

  using front = typename Front<T...>::type;

  using back = typename Back<T...>::type;
};

template <int N> struct ListRange {
  using last_type = typename ListRange<N - 1>::type;
  using type =
      typename last_type::template append<std::integral_constant<int, N>>;
};

template <> struct ListRange<0> {
  using type = List<std::integral_constant<int, 0>>;
};

template <typename T> struct Reverse;
template <> struct Reverse<List<>> {
  using type = List<>;
};
template <typename H, typename... T> struct Reverse<List<H, T...>> {
  using last_type = typename Reverse<List<T...>>::type;
  using type = typename last_type::template append<H>;
};

template <typename T> struct IsList {
  static constexpr bool value = false;
};
template <typename... T> struct IsList<List<T...>> {
  static constexpr bool value = true;
};
// Car: [h, t...] => h
//      [h] => h
//      [] => Empty
template <typename T> struct Car {};

template <typename... T> struct Car<List<T...>> {
  using type = typename List<T...>::front;
};

// Cdr: [h, t...] => [t...]
//      [h] => []
//      [] => Empty
//      ?  =>
template <typename T> struct Cdr {};

template <typename H, typename... T> struct Cdr<List<H, T...>> {
  using type = List<T...>;
};

template <typename H> struct Cdr<List<H>> {
  using type = List<>;
};

// GetElem
template <int x, typename T> struct GetElem {};

// GetElem x [] => Empty
template <int x> struct GetElem<x, List<>> {
  using type = Empty;
};

// GetElem 0 [H T ...] => H
template <typename H, typename... T> struct GetElem<0, List<H, T...>> {
  using type = H;
};

// GetElem x [H T ...] => GetElem (x - 1) [T ...]
template <int x, typename... T>
struct GetElem<x, List<T...>>
    : public GetElem<x - 1, typename Cdr<List<T...>>::type> {};

// Concat
template <typename L, typename R> struct Concat;

// Concat [L...] [R...] => [L... R...]
template <typename... LT, typename... RT>
struct Concat<List<LT...>, List<RT...>> {
  using type = List<LT..., RT...>;
};

template <typename L, typename R>
using ConcatType = typename Concat<L, R>::type;
template <typename T, typename L,
          template <typename, typename> typename Eq = std::is_same>
struct IsElem;
template <typename T, template <typename, typename> typename Eq>
struct IsElem<T, List<>, Eq> {
  static constexpr bool value = false; // NOLINT
  static constexpr int count = 0;
};
template <typename T, template <typename, typename> typename Eq, typename H,
          typename... LT>
struct IsElem<T, List<H, LT...>, Eq> {
  static constexpr bool value // NOLINT
      = Eq<H, T>::value ? true : IsElem<T, List<LT...>, Eq>::value;
  static constexpr int count = Eq<H, T>::value
                                   ? IsElem<T, List<LT...>, Eq>::value
                                   : (1 + IsElem<T, List<LT...>, Eq>::value);
};

template <typename T, typename H, template <typename, typename> typename Eq>
struct IsElem<T, List<H>, Eq> {
  static constexpr bool value = Eq<H, T>::value;         // NOLINT
  static constexpr bool count = Eq<H, T>::value ? 1 : 0; // NOLINT
};

template <typename T, int count> struct Duplicate;
template <typename T> struct Duplicate<T, 0> {
  using type = List<>;
};
template <typename T, int count> struct Duplicate {
  static_assert(count >= 0, "Count < 0 detected.");
  using last_type = typename Duplicate<T, count - 1>::type;
  using type = typename last_type::template append<T>;
};

} // namespace details

using Empty = details::Empty;

template <typename... T> using List = details::List<T...>;

template <typename... T> using Front = details::Front<T...>;
template <typename... T> using Front_t = typename Front<T...>::type;

template <typename... T> using Back = details::Back<T...>;
template <typename... T> using Back_t = typename details::Back<T...>::type;

template <typename T> using Car = details::Car<T>;
template <typename T> using Car_t = typename details::Car<T>::type;

template <typename T> using Cdr = details::Cdr<T>;
template <typename T> using Cdr_t = typename details::Cdr<T>::type;

template <int x, typename T> using Get = details::GetElem<x, T>;
template <int x, typename T>
using Get_t = typename details::GetElem<x, T>::type;

template <typename S, typename T> using Concat = details::Concat<S, T>;
template <typename S, typename T> using Concat_t = typename Concat<S, T>::type;

template <typename T, typename L,
          template <typename, typename> typename Eq = std::is_same>
using Has = details::IsElem<T, L, Eq>;
template <typename T, typename L,
          template <typename, typename> typename Eq = std::is_same>
static constexpr bool Has_v = Has<T, L, Eq>::value;
template <typename T, typename L,
          template <typename, typename> typename Eq = std::is_same>
using Count = details::IsElem<T, L, Eq>;
template <typename T, typename L,
          template <typename, typename> typename Eq = std::is_same>
static constexpr bool Count_v = Count<T, L, Eq>::count;

template <typename L> using Reverse = details::Reverse<L>;
template <typename L> using Reverse_t = typename Reverse<L>::type;

template <typename L, int count> using Duplicate = details::Duplicate<L, count>;
template <typename L, int count>
using Duplicate_t = typename Duplicate<L, count>::type;
} // namespace acg::utils::god
// NOLINTEND(readability-identifier-naming)
