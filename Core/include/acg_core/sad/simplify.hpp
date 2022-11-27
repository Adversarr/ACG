#pragma once
// NOLINTBEGIN(readability-identifier-naming)
#include "dual.hpp"
#include "la.hpp"

namespace acg::sad {

namespace details {

// Simplification Rules
template <typename T> struct Simplify {
  using type = T;  // Fallback.
};
template <typename T> using Simplify_t = typename Simplify<T>::type;
template <typename E> using Simpliest_t = typename FixedPoint<Simplify, E>::type;

///> Add: Zero + Anything = Anything
template <typename S> struct Simplify<Add<Zeros<S>, Zeros<S>>> {
  using type = Zeros<S>;
};
template <typename T, typename S> struct Simplify<Add<T, Zeros<S>>> {
  using type = T;
};
template <typename T, typename S> struct Simplify<Add<Zeros<S>, T>> {
  using type = T;
};
template <typename T, typename S> struct Simplify<Add<Zeros<S>, Zeros<T>>> {
  using type = Zeros<S>;
};
// Fallback...
template <typename T, typename S> struct Simplify<Add<T, S>> {
  using type = Add<typename Simplify<T>::type, typename Simplify<S>::type>;
};
///< Add

///> Substraction: A - 0 = A
template <typename L> struct Simplify<Sub<L, L>> {
  using type = ZerosLike<L>;
};
template <typename L, typename T> struct Simplify<Sub<L, Zeros<T>>> {
  using type = L;
};
template <typename L, typename T> struct Simplify<Sub<Zeros<T>, L>> {
  using type = Neg<L>;
};
template <typename T, typename S> struct Simplify<Sub<Zeros<T>, Zeros<S>>> {
  using type = Zeros<T>;
};
template <typename L, typename T> struct Simplify<Sub<L, T>> {
  using type = Sub<typename Simplify<L>::type, typename Simplify<T>::type>;
};
///< Sub

///> Neg
template <typename T> struct Simplify<Neg<Neg<T>>> {
  using type = typename Simplify<T>::type;
};
template <typename T> struct Simplify<Neg<Zeros<T>>> {
  using type = Zeros<T>;
};
template <typename T> struct Simplify<Neg<T>> {
  using type = Neg<Simplify_t<T>>;
};
///< Neg

///> Multiply
template <typename T, typename S> struct Simplify<Mul<T, S>> {
  using type = Mul<typename Simplify<T>::type, typename Simplify<S>::type>;
};
template <typename S, typename T> struct Simplify<Mul<Zeros<S>, Zeros<T>>> {
  using type = Zeros<typename Mul<Zeros<S>, Zeros<T>>::type>;
};
template <typename T, typename S> struct Simplify<Mul<T, Zeros<S>>> {
  using type = Zeros<typename Mul<T, Zeros<S>>::type>;
};
template <typename T, typename S> struct Simplify<Mul<Zeros<S>, T>> {
  using type = Zeros<typename Mul<Zeros<S>, T>::type>;
};
///< Multiply

///> Dot
template <typename X, typename Y> struct Simplify<Dot<X, Y>> {
  using type = Dot<Simplify_t<X>, Simplify_t<Y>>;
};
template <typename X, typename Z> struct Simplify<Dot<X, Zeros<Z>>> {
  using type = Zeros<typename TensorTrait<Z>::Scalar>;
};
template <typename X, typename Z> struct Simplify<Dot<Zeros<Z>, X>> {
  using type = Zeros<typename TensorTrait<Z>::Scalar>;
};
template <typename X, typename Z> struct Simplify<Dot<Zeros<Z>, Zeros<X>>> {
  using type = Zeros<typename TensorTrait<Z>::Scalar>;
};
///> Dot

template <typename T, typename S> struct ExprHasSameValue {
  static constexpr bool value = std::is_same_v<T, S>;
};
template <typename LT, typename LS, typename RT, typename RS>
struct ExprHasSameValue<Add<LT, LS>, Add<RT, RS>> {
  static constexpr bool value
      = (ExprHasSameValue<LT, RS>::value && ExprHasSameValue<LS, RT>::value)
        || (ExprHasSameValue<LT, RT>::value && ExprHasSameValue<LS, RS>::value);
};

template <typename LT, typename LS, typename RT, typename RS>
struct ExprHasSameValue<Dot<LT, LS>, Dot<RT, RS>> {
  static constexpr bool value
      = (ExprHasSameValue<LT, RS>::value && ExprHasSameValue<LS, RT>::value)
        || (ExprHasSameValue<LT, RT>::value && ExprHasSameValue<LS, RS>::value);
};
}  // namespace details

template <typename T> using Simpliest_t = details::Simpliest_t<T>;

template <typename T, typename S> using ExprHasSameValue = details::ExprHasSameValue<T, S>;

}  // namespace acg::sad
// NOLINTEND(readability-identifier-naming)
