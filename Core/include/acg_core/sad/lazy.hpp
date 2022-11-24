#include <acg_utils/log.hpp>
#include <iostream>

#include "dual.hpp"
// NOLINTBEGIN(readability-identifier-naming)

namespace acg::sad {
namespace details {

template <typename L> struct DetermineLazyDataType;
template <> struct DetermineLazyDataType<List<>> {
  using type = List<>;
};
template <typename T> struct DetermineLazyDataType<List<T>> {
  using type = List<Pair<T, typename T::type>>;
};
template <typename E, typename L> struct LazyEval;
template <typename I, typename L> struct GetInputLazy;
template <typename L> struct GetInputLazy<List<>, L> {
  using type = List<>;
};
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
  using lazy_exprs = typename Filter<NotLazy, exprs>::contract;
  using data_actual_type = Map_t<GetInnerType, non_lazy_exprs>;
  using data_container = typename data_actual_type::template cast<std::tuple>;
  using lazy_container_dict = Reverse_t<typename DetermineLazyDataType<Reverse_t<exprs>>::type>;

  template <typename T> static constexpr size_t index = Find<T, non_lazy_exprs>::value;
  template <typename T> using param_type = std::tuple_element_t<index<T>, data_container>;
  data_container data;

  template <typename T> void Set(const param_type<T>& in) { std::get<index<T>>(data) = in; }
  template <typename T, std::enable_if_t<Has_v<Simpliest_t<T>, non_lazy_exprs>, bool> = true>
  forceinline decltype(auto) Get() {
    return std::get<index<T>>(data);
  }

  template <typename T, std::enable_if_t<!Has_v<Simpliest_t<T>, non_lazy_exprs>, bool> = true>
  forceinline decltype(auto) Get() {
    static_assert(!std::is_same_v<T, T>, "Error: Cannot Get this type");
  }
  forceinline LazyContext() noexcept {
    LazySetupNonLazy<std::remove_reference_t<decltype(*this)>>{}(*this);
  }
};

template <typename C, typename LayersToCompute> struct LazyResultImpl {
  using layer_output = Car_t<LayersToCompute>;
  using layer_last = Cdr_t<LayersToCompute>;
  // using layer_output_inner_type = Map_t<GetInnerType, layer_output>;
  using lazy_container_dict = typename C::lazy_container_dict;
  template <typename T> struct map_to_lazy {
    using type = GetKeyValue_t<T, lazy_container_dict>;
  };
  using layer_output_inner_type = Map_t<map_to_lazy, layer_output>;
  using data_container = typename layer_output_inner_type::template cast<std::tuple>;
  using deeper_layer_result = LazyResultImpl<C, layer_last>;
  template <typename T> static constexpr size_t index
      = Find<typename FixedPoint<Simplify, T>::type, layer_output>::value;
  deeper_layer_result deeper_result_;
  data_container data_;

  template <typename E, typename L> struct Task;
  template <typename E, typename... I> struct Task<E, List<I...>> {
    forceinline decltype(auto) operator()(deeper_layer_result& impl) {
      return E{}(impl.template Get<I>()...);
    }
  };

  template <typename L> struct Init;
  template <typename... E> struct Init<List<E...>> {
    forceinline decltype(auto) operator()(deeper_layer_result& impl) {
      return std::make_tuple(Task<E, typename E::InputExpr>()(impl)...);
    }
  };

  template <typename T> forceinline decltype(auto) GetThisLayer() {
    return std::get<index<T>>(data_);
  }

  template <typename T, std::enable_if_t<Has_v<Simpliest_t<T>, layer_output>, int> = 0>
  forceinline decltype(auto) Get() {
    return GetThisLayer<T>();
  }
  template <typename T, std::enable_if_t<!Has_v<Simpliest_t<T>, layer_output>, int> = 0>
  forceinline decltype(auto) Get() {
    return deeper_result_.template Get<T>();
  }
  forceinline explicit LazyResultImpl(C& context)
      : deeper_result_(context), data_(Init<layer_output>{}(deeper_result_)) {}
};

template <typename C> struct LazyResultImpl<C, List<>> {
  C& context_;
  template <typename T> forceinline decltype(auto) Get() {
    return context_.template Get<T>();
  }
  explicit LazyResultImpl(C& context) : context_(context) {}
};

}  // namespace details
template<typename L>
using LazyContext =details::LazyContext<L>;


template <typename C> struct LazyResult : public details::LazyResultImpl<C, typename C::lazy_layers> {
  inline explicit LazyResult(C& c) : 
    details::LazyResultImpl<C, typename C::lazy_layers>(c) {}
};
}  // namespace acg::sad
// NOLINTEND(readability-identifier-naming)
