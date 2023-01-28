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
template <typename X> struct Inverse : public Expr<X, X> {
  static_assert(Trait<typename X::type>::rows == Trait<typename X::type>::cols,
                "Inverse can only applied to matrix with r=c.");

  template <typename XIn> decltype(auto) operator()(XIn&& in_x) { return in_x.inverse(); }

  template <typename XIn, typename G> using Grad_t = Mul<Inverse<X>, G, Inverse<X>>;
};

}  // namespace details

template <typename X, typename Y> using Dot = details::Dot<X, Y>;
template <typename X> using Transpose = details::Transpose<X>;
template <typename X> using Inverse = details::Inverse<X>;

}  // namespace acg::sad
