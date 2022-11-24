#pragma once
#include <type_traits>

#include "acg_core/core.hpp"

#ifndef forceinline
#ifdef _MSC_VER_ // for MSVC
#define forceinline inline __forceinline
#elif defined __GNUC__ // for gcc on Linux/Apple OS X
#define forceinline inline __attribute__((always_inline))
#else
#define forceinline inline
#endif
#endif


namespace acg::utils::god {
namespace details {
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
template <int x, typename... T> struct GetElem<x, List<T...>>
    : public GetElem<x - 1, typename Cdr<List<T...>>::type> {};

// Concat
template <typename L, typename R> struct Concat;

// Concat [L...] [R...] => [L... R...]
template <typename... LT, typename... RT> struct Concat<List<LT...>, List<RT...>> {
  using type = List<LT..., RT...>;
};

template <typename L, typename R> using ConcatType = typename Concat<L, R>::type;
template <typename T, typename L> struct IsElem;
template <typename T> struct IsElem<T, List<>> {
  static constexpr bool value = false;  // NOLINT
};
template <typename T, typename H, typename... LT> struct IsElem<T, List<H, LT...>> {
  static constexpr bool value  // NOLINT
      = std::is_same_v<H, T> ? true : IsElem<T, List<LT...>>::value;
};

template <typename T, typename H> struct IsElem<T, List<H>> {
  static constexpr bool value = std::is_same_v<H, T>;  // NOLINT
};

template <typename K, typename V> struct Pair {
  using KeyType = K;
  using ValueType = V;
};

template <typename T> struct GetKey;
template <typename T> struct GetValue;
template <typename K, typename T> struct GetKeyValue;
template <typename K> struct GetKeyValue<K, List<>> {
  using type = Empty;
};
template <typename K, typename H, typename... T> struct GetKeyValue<K, List<H, T...>> {
  using type = std::conditional_t<std::is_same_v<typename H::KeyType, K>, typename H::ValueType,
                                  typename GetKeyValue<K, List<T...>>::type>;
};
template <typename K, typename V> struct GetKey<Pair<K, V>> {
  using type = K;
};
template <typename K, typename V> struct GetValue<Pair<K, V>> {
  using type = V;
};

template <typename H, typename L> struct Update;
template <typename H> struct Update<H, List<>> {
  using type = List<H>;
};
template <typename P, typename H, typename... T> struct Update<P, List<H, T...>> {
  using type
      = std::conditional_t<std::is_same_v<typename H::KeyType, typename P::KeyType>, List<P, T...>,
                           typename Concat<List<H>, typename Update<P, List<T...>>::type>::type>;
};

}  // namespace details

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
template <int x, typename T> using Get_t = typename details::GetElem<x, T>::type;

template <typename S, typename T> using Concat = details::Concat<S, T>;
template <typename S, typename T> using Concat_t = typename Concat<S, T>::type;

template <typename T, typename L> using Has = details::IsElem<T, L>;
template <typename T, typename L> static constexpr bool Has_v = Has<T, L>::value;

template <typename K, typename V> using Pair = details::Pair<K, V>;
template <typename T> using GetKey = details::GetKey<T>;
template <typename T> using GetKey_t = typename details::GetKey<T>::type;
template <typename T> using GetValue = details::GetValue<T>;
template <typename T> using GetValue_t = typename details::GetValue<T>::type;
template <typename T, typename L> using GetKeyValue = details::GetKeyValue<T, L>;
template <typename T, typename L> using GetKeyValue_t = typename GetKeyValue<T, L>::type;
template <typename H, typename L> using Update = details::Update<H, L>;
template <typename H, typename L> using Update_t = typename details::Update<H, L>::type;


template <typename L> using Reverse = details::Reverse<L>;
template <typename L> using Reverse_t = typename Reverse<L>::type;
}  // namespace acg::utils::god
