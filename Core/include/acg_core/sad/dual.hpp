#pragma once

#include "acg_core/math.hpp"
#include "acg_utils/god/algorithms.hpp"

namespace acg::sad {
namespace details {
using namespace acg::utils::god;
// Dual
template <typename V, typename Dr> struct Dual {
  using Grad = Dr;
  using Val = V;
};

// Expr
template <typename T, typename... E> struct Expr {
  using InputExpr = List<E...>;
  using InputNodes = Unique_t<Reduce_t<Concat, List<List<>, typename E::InputNodes...>>>;
  using type = T;
  using SubNodes = Unique_t<Reduce_t<Concat, List<List<E...>, typename E::SubNodes...>>>;
  static constexpr size_t dim = acg::TensorTrait<T>::dim;  // NOLINT
  static constexpr bool is_constant = false;               // NOLINT
  static constexpr bool is_input = false;                  // NOLINT
};

// Helper classes
template <typename L, typename R> struct IsSubNode {
  static constexpr bool value = Has_v<L, typename R::SubNodes>;  // NOLINT
};

template <typename L> struct GetSubNodes {
  using type = typename L::SubNodes;
};

template <typename T> struct GetInnerType {
  using type = typename T::type;
};

template <typename T> struct Input : public Expr<T> {
  inline void operator()(T&) {}
};

template <typename T> struct Constant : public Expr<T> {
  static constexpr bool is_constant = true;  // NOLINT
};

template <typename T> struct Ones : Constant<T> {
  void operator()(T& t) { t.setOnes(); }
};

template <> struct Ones<float> : Constant<float> {
  void operator()(float& t) { t = 1; }
};
template <> struct Ones<double> : Constant<double> {
  void operator()(double& t) { t = 1; }
};

template <typename T> struct Zeros : Constant<T> {
  void operator()(T& t) { t.setZeros(); }
};

template <> struct Zeros<float> : Constant<float> {
  void operator()(float& t) { t = 0; }
};
template <> struct Zeros<double> : Constant<double> {
  void operator()(double& t) { t = 0; }
};
// Add Operation, L::type should equals to R::type
template <typename L, typename R> struct Add : public Expr<typename L::type, L, R> {
  static_assert(std::is_same_v<typename L::type, typename R::type>,
                "Add between two different type is not permitted.");
  using T = typename L::type;
  void operator()(T& t, const T& l, const T& r) { t = l + r; }
  template <typename X, typename DG> using DualGrad_t = DG;
};
template <typename L, typename R> struct DualAdd
    : public Dual<Add<typename L::Val, typename R::Val>, Add<typename L::Grad, typename R::Grad>> {
};

// Multiplication.
template <typename L, typename R, class = void> struct Mul;
// Scalar * Scalar
template <typename L, typename R>
struct Mul<L, R,
           std::enable_if_t<!std::is_same_v<L, R> && TensorTrait<typename L::type>::is_scalar
                            && TensorTrait<typename R::type>::is_scalar>>
    : public Expr<typename L::type, L, R> {
  using T = typename L::type;
  inline void operator()(T& t, T l, T r) { t = l * r; }
  template <typename X, typename DG> using DualGrad_t
      = std::conditional_t<std::is_same_v<X, R>, Dual<Mul<L, R>, Mul<L, typename DG::Grad>>,
                           Dual<Mul<L, R>, Mul<typename DG::Grad, R>>>;
};

template <typename L> struct Mul<
    L, L,
    std::enable_if_t<TensorTrait<typename L::type>::cols == TensorTrait<typename L::type>::rows
                     && TensorTrait<typename L::type>::is_scalar>>
    : public Expr<typename L::type, L> {
  using T = typename L::type;
  inline void operator()(T& t, const T& l) { t = l * l; }
  template <typename X, typename DG> using DualGrad_t
      = Dual<Mul<L, L>, Add<Mul<L, typename DG::Grad>, Mul<L, typename DG::Grad>>>;
};

// Scalar * Matrix
template <typename L, typename R>
struct Mul<L, R,
           std::enable_if_t<TensorTrait<typename L::type>::is_scalar
                            && !(TensorTrait<typename R::type>::is_scalar)>>
    : public Expr<typename R::type, L, R> {
  using T = typename R::type;
  inline void operator()(T& t, const typename L::type& l, const typename R::type& r) { t = l * r; }
  template <typename X, typename DG> using DualGrad_t
      = std::conditional_t<std::is_same_v<X, R>, Dual<Mul<L, R>, Mul<L, typename DG::Grad>>,
                           Dual<Mul<L, R>, Mul<typename DG::Grad, R>>>;
};
template <typename L, typename R>
struct Mul<L, R,
           std::enable_if_t<!TensorTrait<typename L::type>::is_scalar
                            && TensorTrait<typename R::type>::is_scalar>>
    : public Expr<typename L::type, L, R> {
  using T = typename L::type;
  inline void operator()(T& t, const typename L::type& l, const typename R::type& r) { t = l * r; }
  template <typename X, typename DG> using DualGrad_t
      = std::conditional_t<std::is_same_v<X, R>, Dual<Mul<L, R>, Mul<L, typename DG::Grad>>,
                           Dual<Mul<L, R>, Mul<typename DG::Grad, R>>>;
};
// Mat * Mat
template <typename L, typename R>
struct Mul<L, R,
           std::enable_if_t<!std::is_same_v<L, R> && !(TensorTrait<typename L::type>::is_scalar)
                            && !(TensorTrait<typename R::type>::is_scalar)>>
    : public Expr<
          Eigen::Matrix<typename TensorTrait<typename L::type>::Scalar,
                        TensorTrait<typename L::type>::rows, TensorTrait<typename R::type>::cols>,
          L, R> {
  static_assert(TensorTrait<typename L::type>::cols == TensorTrait<typename R::type>::rows,
                "L.col != R.rows");
  using T = Eigen::Matrix<typename TensorTrait<typename L::type>::Scalar,
                          TensorTrait<typename L::type>::rows, TensorTrait<typename R::type>::cols>;
  inline void operator()(T& t, const typename L::type& l, const typename R::type& r) { t = l * r; }
  template <typename X, typename DG> using DualGrad_t
      = std::conditional_t<std::is_same_v<X, R>, Dual<Mul<L, R>, Mul<L, typename DG::Grad>>,
                           Dual<Mul<L, R>, Mul<typename DG::Grad, R>>>;
};

template <typename L> struct Mul<
    L, L,
    std::enable_if_t<TensorTrait<typename L::type>::cols == TensorTrait<typename L::type>::rows
                     && !TensorTrait<typename L::type>::is_scalar>>
    : public Expr<typename L::type, L> {
  using T = typename L::type;
  inline void operator()(T& t, const T& l) { t = l * l; }
  template <typename X, typename DG> using DualGrad_t
      = Dual<Mul<L, L>, Add<Mul<L, typename DG::Grad>, Mul<typename DG::Grad, L>>>;
};

// Substraction, L::type should equals to R::type
template <typename L, typename R> struct Sub : public Expr<typename L::type, L, R> {
  static_assert(std::is_same_v<typename L::type, typename R::type>,
                "Substract between two different type is not permitted.");
  using T = typename L::type;
  void operator()(T& t, const T& l, const T& r) { t = l - r; }
  template <typename X, typename DG> using DualGrad_t = DG;
};

template <typename L> struct Sub<L, L> : public Expr<typename L::type, L> {
  using T = typename L::type;
  void operator()(T& t, const T& /*l*/) { Zeros<T>{}(t); }
  template <typename X, typename DG> using DualGrad_t = Dual<Zeros<T>, Zeros<T>>;
};

// Simplification Rules
template <typename T> struct Simplify {
  using type = T;  // Fallback.
};
// For mult: Zeros * anything is Zero
template <typename T, typename S> struct Simplify<Mul<T, S>> {
  using type = Mul<typename Simplify<T>::type, typename Simplify<S>::type>;
};

template <typename S> struct Simplify<Mul<Zeros<S>, Ones<S>>> {
  using type = Zeros<S>;
};
template <typename S> struct Simplify<Mul<Ones<S>, Zeros<S>>> {
  using type = Zeros<S>;
};
template <typename S> struct Simplify<Mul<Zeros<S>, Zeros<S>>> {
  using type = Zeros<S>;
};

template <typename T, typename S> struct Simplify<Mul<T, Zeros<S>>> {
  using type = Zeros<typename Mul<T, Zeros<S>>::type>;
};
template <typename T, typename S> struct Simplify<Mul<Zeros<S>, T>> {
  using type = Zeros<typename Mul<Zeros<S>, T>::type>;
};

// Add: Zero + Anything = Anything
template <typename S> struct Simplify<Add<Zeros<S>, Zeros<S>>> {
  using type = Zeros<S>;
};

template <typename T, typename S> struct Simplify<Add<T, S>> {
  using type = Add<typename Simplify<T>::type, typename Simplify<S>::type>;
};
template <typename T, typename S> struct Simplify<Add<T, Zeros<S>>> {
  using type = typename Simplify<T>::type;
};
template <typename T, typename S> struct Simplify<Add<Zeros<S>, T>> {
  using type = typename Simplify<T>::type;
};
// Substraction: A - 0 = A
template <typename L, typename T> struct Simplify<Sub<L, Zeros<T>>> {
  using type = typename Simplify<L>::type;
};
template <typename T> struct Simplify<Sub<Zeros<T>, Zeros<T>>> {
  using type = Zeros<T>;
};
// Dual Chain.
template <typename F, typename I, typename X, typename D> struct DualChain;
// Dual Diff.
template <typename Y, typename X, typename D> struct DirectionalDiff {
  using type = typename DualChain<Y, typename Y::InputExpr, X, D>::type;
};
template <typename F, typename X, typename D> struct DualChain<F, List<>, X, D> {
  using type = Dual<F, Zeros<typename F::type>>;
};
template <typename F, typename I, typename X, typename D> struct DualChain<F, List<I>, X, D> {
  using dual_ix = typename DirectionalDiff<I, X, D>::type;
  using type = typename F::template DualGrad_t<I, dual_ix>;
};
template <typename F, typename X, typename D, typename IH, typename... IT>
struct DualChain<F, List<IH, IT...>, X, D> {
  using dual_hx = typename DirectionalDiff<IH, X, D>::type;
  using dual_fhx = typename F::template DualGrad_t<IH, dual_hx>;
  using type = DualAdd<dual_fhx, typename DualChain<F, List<IT...>, X, D>::type>;
};

template <typename X, typename D> struct DirectionalDiff<X, X, D> {
  using type = Dual<X, D>;
};

template <typename F, typename X> using Diff =
    typename DirectionalDiff<F, X, Ones<typename X::type>>::type;
template <typename E> using Simpliest_t = typename FixedPoint<Simplify, E>::type;

// Context
template <typename... E> struct Context {
  using outputs = Unique_t<List<typename FixedPoint<Simplify, E>::type...>>;
  using internals = Unique_t<Reduce_t<Concat, Map_t<GetSubNodes, outputs>>>;
  using inputs = Unique_t<Reduce_t<Concat, List<List<>, typename E::InputNodes...>>>;
  using data_type = TopoSort_t<IsSubNode, Unique_t<Concat_t<inputs, Concat_t<internals, outputs>>>>;
  using data_actual_type = Map_t<GetInnerType, data_type>;
  template <typename T> static constexpr size_t index                    // NOLINT
      = Find<typename FixedPoint<Simplify, T>::type, data_type>::value;  // NOLINT

  using data_container = typename data_actual_type::template cast<std::tuple>;
  template <typename T> using param_type = std::tuple_element_t<index<T>, data_container>;
  data_container data;

  template <typename T> auto&& Set(const param_type<T>& in) {
    std::get<index<T>>(data) = in;
    return *this;
  }
  template <typename T> std::tuple_element_t<index<T>, data_container>& Get() {
    return std::get<index<T>>(data);
  }
};

template <typename T> struct Runner {
  template <typename Exp, typename IL> struct Task;
  template <typename Exp, typename... I> struct Task<Exp, List<I...>> {
    void operator()(typename T::data_container& data) {
      Exp{}(std::get<T::template index<Exp>>(data), std::get<T::template index<I>>(data)...);
    }
  };
  template <typename Ts> struct RunnerImpl;
  template <typename... Ts> struct RunnerImpl<List<Ts...>> {
    void operator()(typename T::data_container& data) {
      (Task<Ts, typename Ts::InputExpr>{}(data), ...);
    }
  };
  explicit Runner() = default;
  void operator()(T& context) { RunnerImpl<typename T::data_type>{}(context.data); }
};
}  // namespace details

template <typename T> using Input = details::Input<T>;
template <typename E> using Simplify =
    typename acg::utils::god::FixedPoint<details::Simplify, E>::type;
template <typename E> using Simpliest_t = typename Simplify<E>::type;
#define Variable(type, name) \
  struct name : public acg::sad::Input<type> {}
#define Constant_value(type, name, value)            \
  struct name : public acg::sad::Constant<type> {    \
    inline void operator()(type& v) { v = (value); } \
  }  // NOLINT
#define Constant_expr(type, name, expr)           \
  struct name : public acg::sad::Constant<type> { \
    inline void operator()(type& v) { expr; }     \
  }
template <typename L, typename R> using Add = details::Add<L, R>;
template <typename L, typename R> using Sub = details::Sub<L, R>;
template <typename L, typename R> using Mul = details::Mul<L, R>;
template <typename T> inline void run(T& t) { details::Runner<T>()(t); }  // NOLINT

}  // namespace acg::sad
