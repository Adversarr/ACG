#pragma once
// NOLINTBEGIN(readability-identifier-naming)
#include "dual.hpp"
#include "cwise.hpp"
#include "la.hpp"

namespace acg::sad {

namespace details {

// NOTE: Simplification Rules
template <typename T> struct Simplify {
  using type = T;  // Fallback.
};
template <typename T> using Simplify_t = typename Simplify<T>::type;
template <typename E> using Simpliest_t = typename FixedPoint<Simplify, E>::type;

///> Add: Zero + Anything = Anything
template <typename S> struct Simplify<Add<Zeros<S>, Zeros<S>>> { using type = Zeros<S>; };
template <typename T, typename S> struct Simplify<Add<T, Zeros<S>>> { using type = T; };
template <typename T, typename S> struct Simplify<Add<Zeros<S>, T>> { using type = T; };
template <typename T, typename S> struct Simplify<Add<Zeros<S>, Zeros<T>>> {
  using type = Zeros<S>;
};
// Fallback...
template <typename T, typename S> struct Simplify<Add<T, S>> {
  using type = Add<typename Simplify<T>::type, typename Simplify<S>::type>;
};
///< Add

///> Substraction: A - 0 = A
// 1. a - a = 0
template <typename L> struct Simplify<Sub<L, L>> { using type = ZerosLike<L>; };
// 2. a - 0 = a
template <typename L, typename T> struct Simplify<Sub<L, Zeros<T>>> { using type = L; };
// 3. 0 - a = -a
template <typename L, typename T> struct Simplify<Sub<Zeros<T>, L>> { using type = Neg<L>; };
// 4. 0 - 0 = 0
template <typename T, typename S> struct Simplify<Sub<Zeros<T>, Zeros<S>>> {
  using type = Zeros<T>;
};
// 5. fallback
template <typename L, typename T> struct Simplify<Sub<L, T>> {
  using type = Sub<typename Simplify<L>::type, typename Simplify<T>::type>;
};
///< Sub

///> Neg
// -( -x) = x
template <typename T> struct Simplify<Neg<Neg<T>>> { using type = typename Simplify<T>::type; };
// 2. - 0 = 0
template <typename T> struct Simplify<Neg<Zeros<T>>> { using type = Zeros<T>; };
// 3. fallback:
template <typename T> struct Simplify<Neg<T>> { using type = Neg<Simplify_t<T>>; };
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

///> Transpose
// 1. x^T^T = x
template <typename T> struct Simplify<Transpose<Transpose<T>>> { using type = Simplify_t<T>; };
// 2. fallback:
template <typename T> struct Simplify<Transpose<T>> { using type = Transpose<Simplify_t<T>>; };
///< Transpose

///> Dot
template <typename X, typename Y> struct Simplify<Dot<X, Y>> {
  using type = Dot<Simplify_t<X>, Simplify_t<Y>>;
};
template <typename X, typename Z> struct Simplify<Dot<X, Zeros<Z>>> {
  using type = Zeros<typename Trait<Z>::Scalar>;
};
template <typename X, typename Z> struct Simplify<Dot<Zeros<Z>, X>> {
  using type = Zeros<typename Trait<Z>::Scalar>;
};
template <typename X, typename Z> struct Simplify<Dot<Zeros<Z>, Zeros<X>>> {
  using type = Zeros<typename Trait<Z>::Scalar>;
};
///> Dot

///> Inverse
template <typename X> struct Simplify<Inverse<X>> { using type = Inverse<Simplify_t<X>>; };
template <typename X> struct Simplify<Inverse<Inverse<X>>> { using type = Simplify_t<X>; };
///< Inverse

///> Reshape
template <typename X, int r, int c> struct Simplify<Reshape<X, r, c>> {
  using type = Reshape<Simplify_t<X>, r, c>;
};
///< Reshape

///> CwiseMul
template <typename T, typename S> struct Simplify<CwiseMul<T, S>> {
  using type = CwiseMul<Simplify_t<T>, Simplify_t<S>>;
};
template <typename S> struct Simplify<CwiseMul<Zeros<S>, Zeros<S>>> { using type = Zeros<S>; };
template <typename T, typename S> struct Simplify<CwiseMul<T, Zeros<S>>> {
  using type = ZerosLike<T>;
};
template <typename T, typename S> struct Simplify<CwiseMul<Zeros<S>, T>> {
  using type = ZerosLike<T>;
};
///< CwiseMul

///> CwiseAbs2
template <typename S> struct Simplify<CwiseAbs2<S>> { using type = CwiseAbs2<Simplify_t<S>>; };
///< CwiseAbs2

///> CwiseInv
template <typename S> struct Simplify<CwiseInv<S>> { using type = CwiseInv<Simplify_t<S>>; };
///< CwiseInv

///> CwiseSqrt
template <typename S> struct Simplify<CwiseSqrt<S>> { using type = CwiseSqrt<Simplify_t<S>>; };
///< CwiseSqrt

///> Sum
template <typename S> struct Simplify<Sum<S>> { using type = Sum<Simplify_t<S>>; };
///< Sum

///> Norm
template <typename S> struct Simplify<Norm<S>> { using type = Norm<Simplify_t<S>>; };
///< Norm

///> NOTE: ExprHasSameValue

// 1. Fallback
template <typename T, typename S> struct ExprHasSameValue {
  static constexpr bool value = std::is_same_v<T, S>;
};

// 1. (a + b) = (b + a)
template <typename LT, typename LS, typename RT, typename RS>
struct ExprHasSameValue<Add<LT, LS>, Add<RT, RS>> {
  static constexpr bool value
      = (ExprHasSameValue<LT, RS>::value && ExprHasSameValue<LS, RT>::value)
        || (ExprHasSameValue<LT, RT>::value && ExprHasSameValue<LS, RS>::value);
};

// 2. a .* b = b .* a
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
