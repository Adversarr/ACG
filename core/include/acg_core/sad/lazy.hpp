#pragma once
#include <acg_utils/log.hpp>
#include <iostream>

#include "dual.hpp"
#include "runner.hpp"
// NOLINTBEGIN(readability-identifier-naming)

namespace acg::sad {
namespace details {

template <typename L> struct DetermineLazyDataType;
template <> struct DetermineLazyDataType<List<>> { using type = List<>; };
template <typename T> struct DetermineLazyDataType<List<T>> {
  using type = List<Pair<T, typename T::type>>;
};

template <typename E, typename L> struct LazyEval;
template <typename I, typename L> struct GetInputLazy;
template <typename L> struct GetInputLazy<List<>, L> { using type = List<>; };
template <typename L, typename H, typename... I> struct GetInputLazy<List<H, I...>, L> {
  using hv = GetKeyValue_t<H, L>;
  using iv = typename GetInputLazy<List<I...>, L>::type;
  using type = std::conditional_t<std::is_same_v<hv, Empty>, iv, typename iv::template prepend<hv>>;
};
template <typename E, typename L> struct LazyEvalActual;
template <typename E, typename... T> struct LazyEvalActual<E, List<T...>> {
  using type = decltype(std::declval<E>()(std::declval<T>()...));
};
template <typename E> struct LazyEvalActual<E, List<>> : public GetInnerType<E> {};
template <typename E, typename L> struct LazyEval {
  // Map: E input -> E input's Lazy Type
  using e_input = typename GetInputLazy<typename E::InputExpr, L>::type;
  using type = std::conditional_t<E::is_input, typename E::type,
                                  typename LazyEvalActual<E, e_input>::type>;
};

template <typename H, typename... T> struct DetermineLazyDataType<List<H, T...>> {
  using tt = typename DetermineLazyDataType<List<T...>>::type;
  using ht = typename LazyEval<H, tt>::type;
  using type = typename tt::template prepend<Pair<H, ht>>;
};

template <typename T> struct NotLazy {
  static constexpr bool value = T::is_input || T::is_constant;
};

template <typename L> struct LazyContext;

template <typename C> struct LazySetupNonLazy {
  template <typename Exp> struct Task {
    forceinline void operator()(C& context) {
      if constexpr (!Exp::is_input) {
        context.template Set<Exp>(Exp{}());
      }
    }
  };

  template <typename IL> struct Impl;
  template <typename... E> struct Impl<List<E...>> {
    forceinline void operator()(C& context) { (Task<E>()(context), ...); }
  };

  forceinline void operator()(C& context) { Impl<typename C::non_lazy_exprs>{}(context); }
};

template <typename... E> struct LazyContext<List<E...>> {
  using exprs = typename Context<List<E...>>::data_type;
  using layers = TopoFlatten_t<IsSubNode, exprs>;
  using lazy_layers = Reverse_t<Cdr_t<TopoFlatten_t<IsSubNode, exprs>>>;
  using non_lazy_exprs = typename Filter<NotLazy, exprs>::type;

  static_assert(std::is_same_v<non_lazy_exprs, Car_t<layers>>, "eval layers[0] != non lazy");
  using non_lazy_data_actual_type = Map_t<GetInnerType, non_lazy_exprs>;
  using data_container = typename non_lazy_data_actual_type::template cast<std::tuple>;
  // using lazy_container_dict = Reverse_t<typename DetermineLazyDataType<Reverse_t<exprs>>::type>;

  template <typename T> static constexpr size_t index
      = Find<T, non_lazy_exprs, ExprHasSameValue>::value;
  template <typename T> using param_type = std::tuple_element_t<index<T>, data_container>;
  data_container data;

  template <typename T> void Set(const param_type<T>& in) { std::get<index<T>>(data) = in; }
  template <typename T, std::enable_if_t<Has_v<T, non_lazy_exprs, ExprHasSameValue>, bool> = true>
  forceinline decltype(auto) Get() {
    return std::get<index<T>>(data);
  }

  template <typename T, std::enable_if_t<!Has_v<T, non_lazy_exprs, ExprHasSameValue>, bool> = true>
  forceinline decltype(auto) Get() {
    static_assert(!std::is_same_v<T, T>, "Error: Cannot Get this type in context");
  }
  forceinline LazyContext() noexcept {
    LazySetupNonLazy<std::remove_reference_t<decltype(*this)>>{}(*this);
  }
};

template <typename C, typename LayersToCompute> struct LazyResultImpl2;
template <typename C, typename... Rest, typename... E>
struct LazyResultImpl2<C, List<List<E...>, Rest...>> : private LazyResultImpl2<C, List<Rest...>> {
  using layer_output = List<E...>;
  using layer_last = List<Rest...>;
  using base_type = LazyResultImpl2<C, layer_last>;
  template <typename T> static constexpr size_t index
      = Find<T, layer_output, ExprHasSameValue>::value;

  template <typename Exp, typename L> struct Task;
  template <typename Exp, typename... I> struct Task<Exp, List<I...>> {
    forceinline decltype(auto) operator()(base_type& impl) {
      return Exp{}(impl.template Get<I>()...);
    }
  };

  decltype(std::make_tuple(Task<E, typename E::InputExpr>{}(std::declval<base_type&>())...)) data_;

  template <typename T, std::enable_if_t<Has_v<T, layer_output, ExprHasSameValue>, int> = 0>
  forceinline decltype(auto) Get() {
    return std::get<index<T>>(data_);
  }
  template <typename T, std::enable_if_t<!Has_v<T, layer_output, ExprHasSameValue>, int> = 0>
  forceinline decltype(auto) Get() {
    return static_cast<base_type&>(*this).template Get<T>();
  }
  forceinline explicit LazyResultImpl2(C& context)
      : base_type(context),
        data_(Task<E, typename E::InputExpr>{}(static_cast<base_type&>(*this))...) {}
};

template <typename C> struct LazyResultImpl2<C, List<>> {
  C& context_;
  template <typename T> forceinline decltype(auto) Get() { return context_.template Get<T>(); }
  explicit LazyResultImpl2(C& context) : context_(context) {}
};

template <typename C, typename LayersToCompute> struct LazyResultImpl;

template <typename L> struct NotContianOperator {
  template <typename R> struct Tester {
    static constexpr bool value = Count_v<L, typename R::SubNodes, ExprHasSameValue> == 1;
  };
};

template <typename C, typename... Rest, typename... E>
struct LazyResultImpl<C, List<List<E...>, Rest...>> : private LazyResultImpl<C, List<Rest...>> {
  using layer_output = List<E...>;
  using layer_last = List<Rest...>;
  using future_exprs = Reduce_t<Concat, List<List<>, Rest...>>;
  template <typename T> using Tester = All<NotContianOperator<T>::template Tester, future_exprs>;
  using base_type = LazyResultImpl<C, layer_last>;
  template <typename T> static constexpr size_t index
      = Find<T, layer_output, ExprHasSameValue>::value;

  template <bool use_lazy, typename Exp, typename L> struct Task;
  template <typename Exp, typename... I> struct Task<true, Exp, List<I...>> {
    forceinline decltype(auto) operator()(base_type& impl) {
      return Exp{}(impl.template Get<I>()...);
    }
  };
  template <typename Exp, typename... I> struct Task<false, Exp, List<I...>> {
    forceinline decltype(auto) operator()(base_type& impl) {
      return typename Exp::type(Exp{}(impl.template Get<I>()...));
    }

    forceinline decltype(auto) lazy_eval(base_type& impl) {
      return Exp{}(impl.template Get<I>() ...);
    }

    forceinline decltype(auto) eval(base_type& impl) {
      return Exp{}(impl.template Get<I>() ...).eval();
    }
  };
  using output_type = decltype(std::make_tuple(
      Task<Tester<E>::value, E, typename E::InputExpr>{}(std::declval<base_type&>())...));
  decltype(std::make_tuple(
      Task<Tester<E>::value, E, typename E::InputExpr>{}(std::declval<base_type&>())...)) data_;

  template <typename T, std::enable_if_t<Has_v<T, layer_output, ExprHasSameValue>, int> = 0>
  forceinline decltype(auto) Get() {
    return std::get<index<T>>(data_);
  }
  template <typename T, std::enable_if_t<!Has_v<T, layer_output, ExprHasSameValue>, int> = 0>
  forceinline decltype(auto) Get() {
    return static_cast<base_type&>(*this).template Get<T>();
  }
  forceinline explicit LazyResultImpl(C& context)
      : base_type(context),
        data_(
            Task<Tester<E>::value, E, typename E::InputExpr>{}(static_cast<base_type&>(*this))...) {
  }
};

template <typename C> struct LazyResultImpl<C, List<>> {
  C& context_;
  template <typename T> forceinline decltype(auto) Get() { return (context_.template Get<T>()); }
  explicit LazyResultImpl(C& context) : context_(context) {}
};

}  // namespace details

template <typename L> using LazyContext = details::LazyContext<L>;

template <typename C> struct LazyResult2
    : private details::LazyResultImpl<C, typename C::lazy_layers> {
  using impl_type = details::LazyResultImpl<C, typename C::lazy_layers>;

  inline explicit LazyResult2(C& c) : impl_type(c) {}

  template <typename T> inline decltype(auto) Get() {
    return static_cast<impl_type&>(*this).template Get<T>();
  }
};

template <typename C> struct LazyResult
    : private details::LazyResultImpl2<C, typename C::lazy_layers> {
  using impl_type = details::LazyResultImpl2<C, typename C::lazy_layers>;

  inline explicit LazyResult(C& c) : impl_type(c) {}

  template <typename T> inline decltype(auto) Get() {
    return static_cast<impl_type&>(*this).template Get<T>();
  }
};
}  // namespace acg::sad
// NOLINTEND(readability-identifier-naming)
