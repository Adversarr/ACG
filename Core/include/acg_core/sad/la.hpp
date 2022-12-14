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
    : public Expr<typename TensorTrait<typename X::type>::Scalar, X, Y> {
  static_assert(TensorTrait<typename X::type>::is_vector
                    && TensorTrait<typename Y::type>::is_vector,
                "only vector can dot.");
  template <typename XIn, typename YIn> decltype(auto) operator()(XIn&& in_x, YIn&& in_y) {
    return std::forward<XIn>(in_x).dot(std::forward<YIn>(in_y));
  }

  template <typename XIn, typename G> using Grad_t
      = std::conditional_t<std::is_same_v<XIn, X>, Dot<G, Y>, Dot<X, G>>;
};

// For X X, compute X^T X:
template <typename X> struct Dot<X, X>
    : public Expr<typename TensorTrait<typename X::type>::Scalar, X> {
  static_assert(TensorTrait<typename X::type>::is_vector, "only vector can dot.");
  template <typename XIn> decltype(auto) operator()(XIn&& in_x) {
    return std::forward<XIn>(in_x).dot(std::forward<XIn>(in_x));
  }

  template <typename XIn, typename G> using Grad_t = Add<Dot<X, G>, Dot<X, G>>;
};

}  // namespace details

template <typename X, typename Y> using Dot = details::Dot<X, Y>;

}  // namespace acg::sad
