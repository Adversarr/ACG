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

  template <typename I, typename D> auto Diff() {
    return Node<Simpliest_t<DirectionalDiff_t<type, I, D>>>{};
  }

  /**
   * NOTE: Basic operations: Add, Substract, Multiply
   */
  template <typename P> auto operator+(Node<P>) const noexcept {
    return Node<Add<T, P>>{};
  }

  template <typename P> auto operator-(Node<P>) const noexcept {
    return Node<Sub<T, P>>{};
  }

  template <typename P> auto operator*(Node<P>) const noexcept {
    return Node<Mul<T, P>>{};
  }

  /**
   * NOTE: Linear algebra operations: see la.hpp
   */
  auto Transpose() const noexcept { return Node<acg::sad::Transpose<T>>{}; }

  template <typename P> auto Dot(Node<P>) const noexcept {
    return Node<acg::sad::Dot<T, P>>{};
  }

  auto Inverse() const noexcept { return Node<acg::sad::Inverse<T>>{}; }

  template <int rows, int cols> auto Reshape() const noexcept {
    return Node<acg::sad::Reshape<T, rows, cols>>{};
  }

  template <typename P> auto Concatenate(Node<P>) const noexcept {
    return Node<acg::sad::Concatenate<T, P>>{};
  }

  auto Trace() const noexcept { return Node<acg::sad::Trace<T>>{}; }

  /**
   * NOTE: Coefficient-wise operations: see cwise.hpp
   */
  template <typename P> auto CwiseMul(Node<P>) const noexcept {
    return Node<acg::sad::CwiseMul<T, P>>{};
  }

  template <typename P> auto CwiseDiv(Node<P>) const noexcept {
    return Node<acg::sad::CwiseDiv<T, P>>{};
  }

  auto CwiseAbs2() const noexcept { return Node<acg::sad::CwiseAbs2<T>>{}; }

  auto CwiseInv() const noexcept { return Node<acg::sad::CwiseInv<T>>{}; }

  auto CwiseSqrt() const noexcept { return Node<acg::sad::CwiseSqrt<T>>{}; }

  auto Norm() const noexcept { return Node<acg::sad::Norm<T>>{}; }

  auto Sum() const noexcept { return Node<acg::sad::Sum<T>>{}; }
};

// Short hand for Concatenate:
template <typename T, typename P> auto operator,(Node<T> x, Node<P> y) {
  return x.Concatenate(y);
}

// zip[Input, D]  -> DirDiff
template <typename F, typename InputList, typename DList> struct ZipDiff;

template <typename F, typename InputFront, typename DFront,
          typename... InputArgs, typename... DArgs>
struct ZipDiff<F, acg::utils::god::List<InputFront, InputArgs...>,
               acg::utils::god::List<DFront, DArgs...>> {
  using last_diff = typename ZipDiff<F, acg::utils::god::List<InputArgs...>,
                                     acg::utils::god::List<DArgs...>>::type;
  using type = typename last_diff::template append<
      Simpliest_t<DirectionalDiff_t<F, InputFront, DFront>>>;
};

template <typename F, typename InputFront, typename DFront>
struct ZipDiff<F, acg::utils::god::List<InputFront>,
               acg::utils::god::List<DFront>> {
  using type = acg::utils::god::List<
      Simpliest_t<DirectionalDiff_t<F, InputFront, DFront>>>;
};

// All Directions:
template <typename T, int r, int c> struct AllDirections {
  template <int cr, int cc> struct Gen {
    using last
        = std::conditional_t<cc == c - 1, Gen<cr + 1, 0>, Gen<cr, cc + 1>>;
    using last_type = typename last::type;
    using type = typename last_type::template prepend<Dirac<T, cr, cc>>;
  };

  template <> struct Gen<r - 1, c - 1> {
    using type = acg::utils::god::List<Dirac<T, r - 1, c - 1>>;
  };

  using type = typename Gen<0, 0>::type;
};

template <typename T> struct VariableAllDirections {
  using type = std::conditional_t<
      std::is_arithmetic_v<typename T::type>, List<Ones<typename T::type>>,
      typename AllDirections<typename T::type, Trait<typename T::type>::rows,
                             Trait<typename T::type>::cols>::type>;

  using orig = acg::utils::god::Duplicate_t<T, type::size_>;
};

template <typename F, typename... Vad> struct GradGenerate {
  using directions
      = acg::utils::god::Reduce_t<Concat, List<typename Vad::type...>>;
  using variables
      = acg::utils::god::Reduce_t<Concat, List<typename Vad::orig...>>;
  using type = typename ZipDiff<F, variables, directions>::type;
};

}  // namespace details
template <typename T> using Node = details::Node<T>;
}  // namespace acg::sad
