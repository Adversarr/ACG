#pragma once

#include "dual.hpp"
#include "simplify.hpp"
namespace acg::sad {
namespace details {

// Dual Chain.
template <typename F, typename I, typename X, typename D> struct Chain;
// Dual Diff.
template <typename Y, typename X, typename D> struct DirectionalDiffActual {
  using type = typename Chain<Y, typename Y::InputExpr, X, D>::type;
};
template <typename Y, typename X, typename D> struct DirectionalDiff {
  using type = typename DirectionalDiffActual<Y, X, D>::type;
};
template <typename F, typename X, typename D> struct Chain<F, List<>, X, D> {
  using type = Zeros<typename F::type>;
};
template <typename F, typename I, typename X, typename D> struct Chain<F, List<I>, X, D> {
  using dual_ix = typename DirectionalDiff<I, X, D>::type;
  using type = typename F::template Grad_t<I, dual_ix>;
};
template <typename F, typename X, typename D, typename IH, typename... IT>
struct Chain<F, List<IH, IT...>, X, D> {
  using dual_hx = typename DirectionalDiff<IH, X, D>::type;
  using dual_fhx = typename F::template Grad_t<IH, dual_hx>;
  using type = Add<dual_fhx, typename Chain<F, List<IT...>, X, D>::type>;
};

template <typename X, typename D> struct DirectionalDiff<X, X, D> {
  using type = D;
};

// Context
template <typename L> struct Context {};
template <typename... E> struct Context<List<E...>> {
  using outputs = Unique_t<List<typename FixedPoint<Simplify, E>::type...>>;
  using internals = Unique_t<Reduce_t<Concat, Map_t<GetSubNodes, outputs>>>;
  using inputs = Unique_t<Reduce_t<Concat, List<List<>, typename E::InputNodes...>>>;
  using data_type
      = TopoSort_t<IsSubNode,
                   Unique_t<Concat_t<inputs, Concat_t<internals, outputs>>, ExprHasSameValue>>;
  using data_actual_type = Map_t<GetInnerType, data_type>;
  template <typename T> static constexpr size_t index
      = Find<Simpliest_t<T>, data_type, ExprHasSameValue>::value;

  using data_container = typename data_actual_type::template cast<std::tuple>;
  template <typename T> using param_type = std::tuple_element_t<index<T>, data_container>;
  data_container data;

  template <typename T> void Set(const param_type<T>& in) { std::get<index<T>>(data) = in; }
  template <typename T> std::tuple_element_t<index<T>, data_container>& Get() {
    return std::get<index<T>>(data);
  }
};

template <typename T> struct Runner {
  template <typename Exp, typename IL> struct Task;
  template <typename Exp, typename... I> struct Task<Exp, List<I...>> {
    inline void operator()(typename T::data_container& data) const noexcept {
      if constexpr (!Exp::is_input)
        std::get<T::template index<Exp>>(data) = Exp{}(std::get<T::template index<I>>(data)...);
    }
  };
  template <typename Ts> struct RunnerImpl;
  template <typename... Ts> struct RunnerImpl<List<Ts...>> {
    inline void operator()(typename T::data_container& data) const noexcept {
      (Task<Ts, typename Ts::InputExpr>{}(data), ...);
    }
  };
  explicit Runner() = default;
  inline void operator()(T& context) const noexcept {
    RunnerImpl<typename T::data_type>{}(context.data);
  }
};

template <typename F, typename X, typename D> using DirectionalDiff_t =
    typename DirectionalDiff<F, X, D>::type;

}  // namespace details

template <typename L> using Context = details::Context<L>;
template <typename T> inline T& run(T& t) {
  details::Runner<T>()(t);
  return t;
}
template <typename F, typename X, typename D> using DirectionalDiff_t
    = details::DirectionalDiff_t<F, X, D>;
}  // namespace acg::sad
