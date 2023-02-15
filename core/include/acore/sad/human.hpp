#pragma once
#include "cwise.hpp"
#include "dual.hpp"
#include "instant.hpp"
#include "la.hpp"
#include "simplify.hpp"

namespace acg::sad {

namespace details {

template <typename T> struct Node {
  using type = T;

  Node() = default;

  Node(const Node&) = default;

  Node(Node&&) = default;

  template <typename I, typename D> decltype(auto) Diff() {
    return Node<Simpliest_t<DirectionalDiff_t<type, I, D>>>{};
  }

  /**
   * NOTE: Basic operations: Add, Substract, Multiply
   */
  template <typename P> decltype(auto) operator+(Node<P>) const noexcept {
    return Node<Add<T, P>>{};
  }

  template <typename P> decltype(auto) operator-(Node<P>) const noexcept {
    return Node<Sub<T, P>>{};
  }

  template <typename P> decltype(auto) operator*(Node<P>) const noexcept {
    return Node<Mul<T, P>>{};
  }

  /**
   * NOTE: Linear algebra operations: see la.hpp
   */
  decltype(auto) Transpose() const noexcept { return Node<acg::sad::Transpose<T>>{}; }

  template <typename P> decltype(auto) Dot(Node<P>) const noexcept {
    return Node<acg::sad::Dot<T, P>>{};
  }

  decltype(auto) Inverse() const noexcept { return Node<acg::sad::Inverse<T>>{}; }

  template <int rows, int cols> decltype(auto) Reshape() const noexcept {
    return Node<acg::sad::Reshape<T, rows, cols>>{};
  }

  template <typename P> decltype(auto) Concatenate(Node<P>) const noexcept {
    return Node<acg::sad::Concatenate<T, P>>{};
  }

  decltype(auto) Trace() const noexcept {
    return Node<acg::sad::Trace<T>>{};
  }

  /**
   * NOTE: Coefficient-wise operations: see cwise.hpp
   */
  template <typename P> decltype(auto) CwiseMul(Node<P>) const noexcept {
    return Node<acg::sad::CwiseMul<T, P>>{};
  }

  template <typename P> decltype(auto) CwiseDiv(Node<P>) const noexcept {
    return Node<acg::sad::CwiseDiv<T, P>>{};
  }

  decltype(auto) CwiseAbs2() const noexcept { return Node<acg::sad::CwiseAbs2<T>>{}; }

  decltype(auto) CwiseInv() const noexcept { return Node<acg::sad::CwiseInv<T>>{}; }

  decltype(auto) CwiseSqrt() const noexcept { return Node<acg::sad::CwiseSqrt<T>>{}; }

  decltype(auto) Norm() const noexcept { return Node<acg::sad::Norm<T>>{}; }

  decltype(auto) Sum() const noexcept { return Node<acg::sad::Sum<T>>{}; }

  // TODO: Add other basic functions, e.g. sin, cos, tan, ...
};

// Short hand for Concatenate:
template <typename T, typename P> decltype(auto) operator,(Node<T> x, Node<P> y) {
  return x.Concatenate(y);
}
}  // namespace details
template <typename T> using Node = details::Node<T>;
}  // namespace acg::sad
