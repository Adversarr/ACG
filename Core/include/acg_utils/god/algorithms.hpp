#pragma once

#include <limits>

#include "god.hpp"

namespace acg::utils::god {

namespace details {

// Map
template <template <typename> typename M, typename L> struct Map;

// Map M [T...] => [MT ... ]
template <template <typename> typename M, typename... LT> struct Map<M, List<LT...>> {
  using type = List<typename M<LT>::type...>;
};

// Filter
template <template <typename> typename B, typename L> struct Filter;

// Filter B [] => []
template <template <typename> typename B> struct Filter<B, List<>> {
  using type = List<>;
  using contract = List<>;
};

template <template <typename> typename B, typename LTh, typename... LTt>
struct Filter<B, List<LTh, LTt...>> {
  using type =
      typename std::conditional<B<LTh>::value,
                                typename Filter<B, List<LTt...>>::type::template prepend<LTh>,
                                typename Filter<B, List<LTt...>>::type>::type;
  using contract =
      typename std::conditional_t<B<LTh>::value, typename Filter<B, List<LTt...>>::type,
                                  typename Filter<B, List<LTt...>>::type::template prepend<LTh>>;
};

// Reduce
template <template <typename L, typename R> typename F, typename T> struct Reduce;

// Reduce f [] => Empty
template <template <typename L, typename R> typename F> struct Reduce<F, List<>> {
  using type = Empty;
};

// Reduce f [x] => x
template <template <typename L, typename R> typename F, typename T> struct Reduce<F, List<T>> {
  using type = T;
};

// Reduce f [a, t...] => f a (Reduce f [t ...])
template <template <typename, typename> typename F, typename A, typename... T>
struct Reduce<F, List<A, T...>> {
  using type = typename F<A, typename Reduce<F, List<T...>>::type>::type;
};
// All ? =>
template <template <typename> typename P, typename T> struct All;

// ALL P [] => true
template <template <typename> typename P> struct All<P, List<>> {
  static constexpr bool value = true;  // NOLINT
};

// All P [H, T...] => P<H>::value && (All P [T...])::value
template <template <typename> typename P, typename H, typename... T> struct All<P, List<H, T...>> {
  static constexpr bool value = P<H>::value && All<P, List<T...>>::value;  // NOLINT
};

// Any ? =>
template <template <typename> typename P, typename T> struct Any;

// ALL P [] => true
template <template <typename> typename P> struct Any<P, List<>> {
  static constexpr bool value = false;  // NOLINT
};

// All P [H, T...] => P<H>::value && (All P [T...])::value
template <template <typename> typename P, typename H, typename... T> struct Any<P, List<H, T...>> {
  static constexpr bool value = P<H>::value || All<P, List<T...>>::value;  // NOLINT
};

// Find
template <typename T, typename L> struct Find {};

template <typename T> struct Find<T, List<>> {};

template <typename T, typename... L> struct Find<T, List<T, L...>> {
  static constexpr size_t value = 0;  // NOLINT
};

template <typename T, typename H, typename... L> struct Find<T, List<H, L...>> {
  static constexpr size_t value = Find<T, List<L...>>::value + 1;  // NOLINT
};

template <int n, typename T> struct Partition {};

template <typename... T> struct Partition<0, List<T...>> {
  using left = List<>;
  using right = List<T...>;
};

template <int n, typename... T> struct Partition<n, List<T...>> {
  using _next = Partition<n - 1, List<T...>>;
  using left = typename _next::left::template append<typename Car<typename _next::right>::type>;
  using right = typename Cdr<typename _next::right>::type;
};

template <template <typename, typename> typename P, typename L, typename R> struct Merge;

template <template <typename, typename> typename P, typename H, typename... T>
struct Merge<P, List<H, T...>, List<>> {
  using type = List<H, T...>;
};

template <template <typename, typename> typename P, typename... T>
struct Merge<P, List<>, List<T...>> {
  using type = List<T...>;
};

template <template <typename, typename> typename P, typename... LT, typename... RT>
struct Merge<P, List<LT...>, List<RT...>> {
  using _lf = typename Front<LT...>::type;
  using _rf = typename Front<RT...>::type;
  using _lt = typename Cdr<List<LT...>>::type;
  using _rt = typename Cdr<List<RT...>>::type;
  using type = typename std::conditional<
      P<_lf, _rf>::value, typename Merge<P, _lt, List<RT...>>::type::template prepend<_lf>,
      typename Merge<P, List<LT...>, _rt>::type::template prepend<_rf>>::type;
};

template <template <typename, typename> typename P, typename T> struct MergeSort {};

template <template <typename, typename> typename P> struct MergeSort<P, List<>> {
  using type = List<>;
};

template <template <typename, typename> typename P, typename T> struct MergeSort<P, List<T>> {
  using type = List<T>;
};

template <template <typename, typename> typename P, typename... T> struct MergeSort<P, List<T...>> {
  using _p = Partition<sizeof...(T) / 2, List<T...>>;
  using type = typename Merge<P, typename MergeSort<P, typename _p::left>::type,
                              typename MergeSort<P, typename _p::right>::type>::type;
};

template <typename T> struct Unique;
template <> struct Unique<List<>> {
  using type = List<>;
};
template <typename H, typename... T> struct Unique<List<H, T...>> {
  using last = typename Unique<List<T...>>::type;
  using type = std::conditional_t<IsElem<H, last>::value, last, typename last::template append<H>>;
};

template <typename T> struct Box {
  using type = T;
  static constexpr bool value = true;  // NOLINT
};

template <> struct Box<Empty> {
  using type = Empty;
  static constexpr bool value = false;  // NOLINT
};

template <template <typename, typename> typename P, typename T> struct Cmp {
  template <typename L> struct More {
    // True  => L < T
    // False => L >= T
    static constexpr bool value = P<L, T>::value;  // NOLINT
  };

  template <typename L> struct NoMoreThan {
    // L < T => False. otherwise true
    static constexpr bool value = !P<L, T>::value;
  };
};

template <template <typename, typename> typename P, typename L> struct Is {};
template <template <typename, typename> typename P, typename... L> struct Is<P, List<L...>> {
  template <typename T> struct Least {
    // Returns T is the least in L,
    //    i.e. T <= Li forall Li in L,
    //    i.e. not exists (T > Li for any Li in L)
    static constexpr bool value = All<Cmp<P, T>::template NoMoreThan, List<L...>>::value;
  };

  template <typename T> struct NotContains {
    static constexpr bool value = !Has<T, List<L...>>::value;
  };
};

template <template <typename, typename> typename P, typename L> struct TopologySort;
template <template <typename, typename> typename P> struct TopologySort<P, List<>> {
  using type = List<>;
};

template <template <typename, typename> typename P, typename T> struct TopologySort<P, List<T>> {
  using type = List<T>;
};

template <template <typename, typename> typename P, typename... T>
struct TopologySort<P, List<T...>> {
  using least = typename Filter<Is<P, List<T...>>::template Least, List<T...>>::type;
  using last = typename Filter<Is<P, least>::template NotContains, List<T...>>::type;
  using type = typename Concat<least, typename TopologySort<P, last>::type>::type;
};

template <template <typename, typename> typename P, typename L> struct TopoFlatten;

template <template <typename, typename> typename P> struct TopoFlatten<P, List<>> {
  using type = List<>;
};
template <template <typename, typename> typename P, typename T> struct TopoFlatten<P, List<T>> {
  using type = List<List<T>>;
};
template <template <typename, typename> typename P, typename... T>
struct TopoFlatten<P, List<T...>> {
  using least = typename Filter<Is<P, List<T...>>::template Least, List<T...>>::type;
  using last = typename Filter<Is<P, least>::template NotContains, List<T...>>::type;
  using last_topo_flat = typename TopoFlatten<P, last>::type;
  using type = typename last_topo_flat::template prepend<least>;
};

// Cross
template <typename L, typename R> struct Cross;
template <typename... T> struct Cross<List<>, List<T...>> {
  using type = List<>;
};
template <typename L, typename... T> struct Cross<List<L>, List<T...>> {
  using type = List<Pair<L, T>...>;
};
template <typename H, typename... L, typename... T> struct Cross<List<H, L...>, List<T...>> {
  using type = Concat_t<List<Pair<H, T>...>, typename Cross<List<L...>, List<T...>>::type>;
};

}  // namespace details

template <template <typename> typename P, typename T> using All = details::All<P, T>;

template <template <typename> typename P, typename T> using Filter = details::Filter<P, T>;

template <template <typename> typename F, typename T> using Map = details::Map<F, T>;
template <template <typename> typename F, typename T> using Map_t = typename Map<F, T>::type;

template <template <typename, typename> typename P, typename T> using Sort
    = details::MergeSort<P, T>;

template <template <typename, typename> typename P, typename T> using Sort_t =
    typename Sort<P, T>::type;
template <template <typename, typename> typename P, typename T> using TopoSort
    = details::TopologySort<P, T>;
template <template <typename, typename> typename P, typename T> using TopoSort_t =
    typename TopoSort<P, T>::type;
template <template <typename, typename> typename P, typename T> using TopoFlatten
    = details::TopoFlatten<P, T>;
template <template <typename, typename> typename P, typename T> using TopoFlatten_t =
    typename TopoFlatten<P, T>::type;
template <template <typename, typename> typename F, typename T> using Reduce
    = details::Reduce<F, T>;
template <template <typename, typename> typename F, typename T> using Reduce_t =
    typename Reduce<F, T>::type;

template <template <typename> typename G, typename T, class = void> struct FixedPoint;
template <template <typename> typename G, typename T>
struct FixedPoint<G, T, std::enable_if_t<std::is_same_v<typename G<T>::type, T>, void>> {
  using type = T;
};

template <template <typename> typename G, typename T>
struct FixedPoint<G, T, std::enable_if_t<!std::is_same_v<typename G<T>::type, T>, void>> {
  using type = typename FixedPoint<G, typename G<T>::type>::type;
};

template <typename T> using Unique = details::Unique<T>;

template <typename T> using Unique_t = typename details::Unique<T>::type;

template <typename T, typename L> using Find = details::Find<T, L>;
template <typename T, typename L> constexpr size_t Find_v = Find<T, L>::value;

template <typename L, typename R> using Cross = details::Cross<L, R>;
template <typename L, typename R> using Cross_t = typename details::Cross<L, R>::type;
}  // namespace acg::utils::god
