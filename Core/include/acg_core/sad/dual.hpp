#pragma once
// NOLINTBEGIN(readability-identifier-naming)

#include "acg_core/math.hpp"
#include "acg_utils/god/algorithms.hpp"

namespace acg::sad {
namespace details {
using namespace acg::utils::god;

// Expr
template <typename T, typename... E> struct Expr {
  using InputExpr = List<E...>;
  using InputNodes = Unique_t<Reduce_t<Concat, List<List<>, typename E::InputNodes...>>>;
  using type = T;
  using SubNodes = Unique_t<Reduce_t<Concat, List<List<E...>, typename E::SubNodes...>>>;
  static constexpr bool is_constant = false;
  static constexpr bool is_input = false;
};

template<typename T> struct IsConstant {
  static constexpr bool value = T::is_constant;
};

template<typename T> struct IsInput {
  static constexpr bool value = T::is_input;
};

// Helper classes
template <typename L, typename R> struct IsSubNode {
  static constexpr bool value = Has_v<L, typename R::SubNodes>;
};

template <typename L> struct GetSubNodes {
  using type = typename L::SubNodes;
};

template <typename T> struct GetInnerType {
  using type = typename T::type;
};

template <typename T> struct Input : public Expr<T> {
  static constexpr bool is_input = true;
};

template <typename T> struct Constant : public Expr<T> {
  static constexpr bool is_constant = true;
};

template <typename T> struct Ones : Constant<T> {
  inline decltype(auto) operator()() const noexcept { return T::Ones(); }
};

template <> struct Ones<float> : Constant<float> {
  inline float operator()() const noexcept { return static_cast<float>(1); }
};
template <> struct Ones<double> : Constant<double> {
  inline double operator()() const noexcept { return static_cast<double>(1); }
};

template <typename X> using OnesLike = Ones<typename X::type>;

template <typename T> struct Zeros : Constant<T> {
  inline decltype(auto) operator()() const noexcept { return T::Zero(); }
};

template <> struct Zeros<float> : Constant<float> {
  inline decltype(auto) operator()() const noexcept { return static_cast<float>(0); }
};
template <> struct Zeros<double> : Constant<double> {
  inline decltype(auto) operator()() const noexcept { return static_cast<double>(0); }
};
template <typename X> struct ZerosLike : public Zeros<typename X::type> {};

template <typename T, int... dmd> struct Dirac;

template <typename T> struct Dirac<T> : public Ones<T> {};

template <typename T, int r, int c> struct Dirac<T, r, c> : public Constant<T> {
  inline decltype(auto) operator()() const noexcept {
    constexpr int64_t rows = T::RowsAtCompileTime;
    constexpr int64_t cols = T::ColsAtCompileTime;
    return Eigen::Vector<typename T::Scalar, rows * cols>::Unit(rows * cols, c * rows + r)
        .reshaped(rows, cols);
  }
};

// Add Operation, L::type should equals to R::type
template <typename L, typename R> struct Add : public Expr<typename L::type, L, R> {
  static_assert(std::is_same_v<typename L::type, typename R::type>,
                "Add between two different type is not permitted.");
  using T = typename L::type;
  template <typename Li, typename Ri>
  inline decltype(auto) operator()(Li&& l, Ri&& r) const noexcept {
    return l + r;
  }
  template <typename X, typename DG> using Grad_t = DG;
};

template <typename X> struct Neg : public Expr<typename X::type, X> {
  using T = typename X::type;
  template <typename XI> inline decltype(auto) operator()(XI&& in_x) const noexcept {
    return -in_x;
  }

  template <typename, typename G> using Grad_t = Neg<G>;
};
// template <typename L, typename R> struct DualAdd
//     : public Dual<Add<typename L::Val, typename R::Val>, Add<typename L::Grad, typename R::Grad>>
//     {
// };

// Multiplication.
template <typename L, typename R, class = void> struct Mul;
template <typename L, typename R> struct OpMul {
  using type = decltype(std::declval<typename L::type>() * std::declval<typename R::type>());
};
template <typename L, typename R> struct OpAdd {
  using type = decltype(std::declval<typename L::type>() + std::declval<typename R::type>());
};
template <typename L, typename R> struct OpSub {
  using type = decltype(std::declval<typename L::type>() - std::declval<typename R::type>());
};
// Scalar * Scalar
template <typename L, typename R>
struct Mul<L, R,
           std::enable_if_t<!std::is_same_v<L, R> && TensorTrait<typename L::type>::is_scalar
                            && TensorTrait<typename R::type>::is_scalar>>
    : public Expr<typename L::type, L, R> {
  // TODO: Replace all the T.
  // using T = typename OpMul<L, R>::type;
  using T = typename L::type;

  template <typename Li, typename Ri>
  inline decltype(auto) operator()(Li&& l, Ri&& r) const noexcept {
    return std::forward<Li>(l) * std::forward<Ri>(r);
  }

  template <typename X, typename G> using Grad_t
      = std::conditional_t<std::is_same_v<X, R>, Mul<L, G>, Mul<G, R>>;
};

template <typename L> struct Mul<
    L, L,
    std::enable_if_t<TensorTrait<typename L::type>::cols == TensorTrait<typename L::type>::rows
                     && TensorTrait<typename L::type>::is_scalar>>
    : public Expr<typename L::type, L> {
  using T = typename L::type;
  template <typename Li> inline decltype(auto) operator()(Li&& l) const noexcept { return l * l; }
  template <typename X, typename G> using Grad_t = Add<Mul<L, G>, Mul<G, L>>;
};

// Scalar * Matrix
template <typename L, typename R>
struct Mul<L, R,
           std::enable_if_t<TensorTrait<typename L::type>::is_scalar
                            && !(TensorTrait<typename R::type>::is_scalar)>>
    : public Expr<typename R::type, L, R> {
  using T = typename R::type;
  template <typename Li, typename Ri>
  inline decltype(auto) operator()(Li&& l, Ri&& r) const noexcept {
    return std::forward<Li>(l) * std::forward<Ri>(r);
  }
  template <typename X, typename G> using Grad_t
      = std::conditional_t<std::is_same_v<X, R>, Mul<L, G>, Mul<G, R>>;
};

template <typename L, typename R>
struct Mul<L, R,
           std::enable_if_t<!TensorTrait<typename L::type>::is_scalar
                            && TensorTrait<typename R::type>::is_scalar>>
    : public Expr<typename L::type, L, R> {
  using T = typename L::type;
  template <typename Li, typename Ri>
  inline decltype(auto) operator()(Li&& l, Ri&& r) const noexcept {
    return std::forward<Li>(l) * std::forward<Ri>(r);
  }
  template <typename X, typename G> using Grad_t
      = std::conditional_t<std::is_same_v<X, R>, Mul<L, G>, Mul<G, R>>;
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
  template <typename Li, typename Ri>
  inline decltype(auto) operator()(Li&& l, Ri&& r) const noexcept {
    return std::forward<Li>(l) * std::forward<Ri>(r);
  }
  template <typename X, typename G> using Grad_t
      = std::conditional_t<std::is_same_v<X, R>, Mul<L, G>, Mul<G, R>>;
};

template <typename L> struct Mul<
    L, L,
    std::enable_if_t<TensorTrait<typename L::type>::cols == TensorTrait<typename L::type>::rows
                     && !TensorTrait<typename L::type>::is_scalar>>
    : public Expr<typename L::type, L> {
  using T = typename L::type;
  template <typename Li, typename Ri>
  inline decltype(auto) operator()(Li&& l, Ri&& r) const noexcept {
    return std::forward<Li>(l) * std::forward<Ri>(r);
  }
  template <typename X, typename G> using Grad_t = Add<Mul<L, G>, Mul<G, L>>;
};

// Substraction, L::type should equals to R::type
template <typename L, typename R> struct Sub : public Expr<typename L::type, L, R> {
  static_assert(std::is_same_v<typename L::type, typename R::type>,
                "Substract between two different type is not permitted.");
  using T = typename L::type;
  template <typename Li, typename Ri>
  inline decltype(auto) operator()(Li&& l, Ri&& r) const noexcept {
    return l - r;
  }
  template <typename X, typename G> using Grad_t
      = std::conditional_t<std::is_same_v<L, X>, G, Neg<G>>;
};

template <typename L> struct Sub<L, L> : public Expr<typename L::type, L> {
  using T = typename L::type;
  template <typename Li> inline decltype(auto) operator()(Li&&) const noexcept {
    return Zeros<T>{}();
  }
  template <typename X, typename G> using Grad_t = ZerosLike<G>;
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
template <typename T, typename S> struct Simplify<Add<Zeros<S>, Zeros<T>>> {
  using type = Zeros<S>;
};
// Substraction: A - 0 = A
template <typename L, typename T> struct Simplify<Sub<L, Zeros<T>>> {
  using type = typename Simplify<L>::type;
};
template <typename L, typename T> struct Simplify<Sub<Zeros<T>, L>> {
  using type = typename Simplify<Neg<L>>::type;
};
template <typename T> struct Simplify<Sub<Zeros<T>, Zeros<T>>> {
  using type = Zeros<T>;
};
// Neg:
template <typename T> struct Simplify<Neg<Neg<T>>> {
  using type = typename Simplify<T>::type;
};
template <typename T> struct Simplify<Neg<Zeros<T>>> {
  using type = Zeros<T>;
};
// Dual Chain.
template <typename F, typename I, typename X, typename D> struct Chain;
// Dual Diff.
template <typename Y, typename X, typename D> struct DirectionalDiff {
  using type = typename Chain<Y, typename Y::InputExpr, X, D>::type;
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
template<typename L> struct Context{};
template <typename... E> struct Context<List<E...>> {
  using outputs = Unique_t<List<typename FixedPoint<Simplify, E>::type...>>;
  using internals = Unique_t<Reduce_t<Concat, Map_t<GetSubNodes, outputs>>>;
  using inputs = Unique_t<Reduce_t<Concat, List<List<>, typename E::InputNodes...>>>;
  using data_type = TopoSort_t<IsSubNode, Unique_t<Concat_t<inputs, Concat_t<internals, outputs>>>>;
  using data_actual_type = Map_t<GetInnerType, data_type>;
  template <typename T> static constexpr size_t index
      = Find<typename FixedPoint<Simplify, T>::type, data_type>::value;

  using data_container = typename data_actual_type::template cast<std::tuple>;
  template <typename T> using param_type = std::tuple_element_t<index<T>, data_container>;
  data_container data;

  template <typename T> void Set(const param_type<T>& in) {
    std::get<index<T>>(data) = in;
  }
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

template <typename E> using Simpliest_t = typename FixedPoint<Simplify, E>::type;

}  // namespace details

template <typename F, typename X, typename D> using DirectionalDiff_t =
    details::DirectionalDiff_t<F, X, D>;
template <typename T> using Input = details::Input<T>;
template <typename E> using Simpliest_t =
    typename acg::utils::god::FixedPoint<details::Simplify, E>::type;

template <typename T> using Ones = details::Ones<T>;
template <typename E> using OnesLike = details::OnesLike<E>;
template <typename T> using Zeros = details::Zeros<T>;
template <typename E> using ZerosLike = details::ZerosLike<E>;
template <typename L> using Context = details::Context<L>;
template <typename T, int... dmd> using Dirac = details::Dirac<T, dmd...>;

// NOLINTBEGIN(bugprone-macro-parentheses)
#define Variable(type, name) \
  struct name : public acg::sad::details::Input<type> {}
#define Constant_value(type, name, value)                                                  \
  struct name : public acg::sad::details::Constant<type> {                                 \
    inline decltype(auto) operator()() const noexcept { return static_cast<type>(value); } \
  }
#define Constant_expr(type, name, expr)                    \
  struct name : public acg::sad::details::Constant<type> { \
    inline decltype(auto) operator()() const noexcept {    \
      type v;                                              \
      expr;                                                \
      return v;                                            \
    }                                                      \
  }
// NOLINTEND(bugprone-macro-parentheses)

template <typename L, typename R> using Add = details::Add<L, R>;
template <typename L, typename R> using Sub = details::Sub<L, R>;
template <typename L, typename R> using Mul = details::Mul<L, R>;
template <typename T> inline void run(T& t) { details::Runner<T>()(t); }

}  // namespace acg::sad
// NOLINTEND(readability-identifier-naming)
