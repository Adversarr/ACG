#pragma once
#include "dual.hpp"

namespace acg::sad {
namespace details {

// Transpose
template <typename X> struct Transpose : public Expr<typename X::transpose, X> {
  template <typename O, typename XIn, typename YIn>
  void operator()(O& out, const XIn& in_x, const YIn& in_y) {
    out = in_x.dot(in_y);
  }
  template <typename XIn, typename G> using Grad_t = Transpose<G>;
};

// For X Y, compute X^T Y:
template <typename X, typename Y> struct Dot
    : public Expr<typename Trait<typename X::type>::Scalar, X, Y> {
  static_assert(Trait<typename X::type>::is_vector && Trait<typename Y::type>::is_vector,
                "only vector can dot.");
  template <typename XIn, typename YIn> decltype(auto) operator()(XIn&& in_x, YIn&& in_y) {
    return std::forward<XIn>(in_x).dot(std::forward<YIn>(in_y));
  }

  template <typename XIn, typename G> using Grad_t
      = std::conditional_t<std::is_same_v<XIn, X>, Dot<G, Y>, Dot<X, G>>;
};

// For X X, compute X^T X:
template <typename X> struct Dot<X, X> : public Expr<typename Trait<typename X::type>::Scalar, X> {
  static_assert(Trait<typename X::type>::is_vector, "only vector can dot.");
  template <typename XIn> decltype(auto) operator()(XIn&& in_x) {
    return std::forward<XIn>(in_x).dot(std::forward<XIn>(in_x));
  }

  template <typename XIn, typename G> using Grad_t = Add<Dot<X, G>, Dot<X, G>>;
};

// NOTE: inverse matrix
template <typename X> struct Inverse : public Expr<GetRunTimeType_t<X>, X> {
  static_assert(Trait<typename X::type>::rows == Trait<typename X::type>::cols,
                "Inverse can only applied to matrix with r=c.");

  template <typename XIn> decltype(auto) operator()(XIn&& in_x) { return in_x.inverse(); }

  template <typename XIn, typename G> using Grad_t = Neg<Mul<Mul<Inverse<X>, G>, Inverse<X>>>;
};

// NOTE: Trace
template <typename X> struct Trace: 
    public Expr<typename Trait<typename X::type>::Scalar, X> {
  template <typename XIn> decltype(auto) operator()(XIn&& in_x) {
    if constexpr (Trait<GetRunTimeType_t<X>>::is_scalar) {
      return in_x;
    } else {
      return in_x.trace();
    }
  }

  template <typename XIn, typename G> using Grad_t = Trace<G>;
};

// NOTE: Reshaping, same as eigen.
template <typename X, int rows, int cols> struct Reshape
    : Expr<acg::Mat<typename GetRunTimeType_t<X>::Scalar, rows, cols>, X> {
  using _trait = Trait<GetRunTimeType_t<X>>;
  static_assert(rows * cols == _trait::rows * _trait::cols,
                "r * c does not equals to input r * c.");

  template <typename XIn> decltype(auto) operator()(XIn&& in_x) {
    // auto copy = in_x;
    // copy.resize(rows, cols);
    // return copy;
    if constexpr (rows != _trait::rows || cols != _trait::cols) {
      return in_x.reshaped(rows, cols);
    } else {
      return in_x;
    }
  }

  template <typename XIn, typename G> using Grad_t = Reshape<G, rows, cols>;
};

// NOTE: Concatenate Matrices to a long one.
template <typename L, typename R> struct Concatenate
    : public Expr<
          acg::Mat<typename Trait<GetRunTimeType_t<L>>::Scalar, Trait<GetRunTimeType_t<L>>::rows,
                   Trait<GetRunTimeType_t<L>>::cols + Trait<GetRunTimeType_t<R>>::cols>,
          L, R> {
  using _trait_L = Trait<GetRunTimeType_t<L>>;
  using _trait_R = Trait<GetRunTimeType_t<R>>;

  static_assert(_trait_L::rows == _trait_R::rows,
                "L.rows != R.rows is not allowed in concatenate.");

  template <typename XIn, typename YIn> decltype(auto) operator()(XIn&& in_x, YIn&& in_y) {
    acg::Mat<typename Trait<GetRunTimeType_t<L>>::Scalar, Trait<GetRunTimeType_t<L>>::rows,
             Trait<GetRunTimeType_t<L>>::cols + Trait<GetRunTimeType_t<R>>::cols>
        ret;
    ret.block(0, 0, _trait_L::rows, _trait_L::cols) = in_x;
    ret.block(0, _trait_L::cols, _trait_R::rows, _trait_R::cols) = in_y;
    return ret;
  }

  template <typename XIn, typename G> using Grad_t
      = std::conditional_t<std::is_same_v<L, R>, Concatenate<G, G>,
                           std::conditional_t<std::is_same_v<L, XIn>, Concatenate<G, ZerosLike<R>>,
                                              Concatenate<ZerosLike<L>, G>>>;
};

}  // namespace details

// Export symbols.
template <typename X, typename Y> using Dot = details::Dot<X, Y>;
template <typename X> using Transpose = details::Transpose<X>;
template <typename X> using Inverse = details::Inverse<X>;
template <typename X> using Trace = details::Trace<X>;
template <typename X, int rows, int cols> using Reshape = details::Reshape<X, rows, cols>;
template <typename X, typename Y> using Concatenate = details::Concatenate<X, Y>;

}  // namespace acg::sad
