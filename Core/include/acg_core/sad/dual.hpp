#pragma once
// NOLINTBEGIN(readability-identifier-naming)

#include "../math/all.hpp"
#include "acg_utils/god/algorithms.hpp"

namespace acg::sad {
namespace details {
using namespace acg::utils::god;
using namespace acg::details;

// Expr
template <typename T, typename... E> struct Expr {
  using InputExpr = List<E...>;
  using InputNodes = Unique_t<Reduce_t<Concat, List<List<>, typename E::InputNodes...>>>;
  using type = T;
  using SubNodes = Unique_t<Reduce_t<Concat, List<List<E...>, typename E::SubNodes...>>>;
  static constexpr bool is_constant = false;
  static constexpr bool is_input = false;
};

template <typename T> struct IsConstant {
  static constexpr bool value = T::is_constant;
};

template <typename T> struct IsInput {
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

template <typename T> using GetInnerType_t = typename T::type;
template <typename T, typename Derived> struct Input : public Expr<T> {
  static constexpr bool is_input = true;
  using InputNodes = List<Derived>;
};

///> Constants.
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
template <typename X> using ZerosLike = Zeros<typename X::type>;

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
///< Constants

///> Add Operation, L::type should equals to R::type
/// NOTE: Add is cwise.
template <typename L, typename R> struct Add : public Expr<typename L::type, L, R> {
  static_assert(std::is_same_v<typename L::type, typename R::type>,
                "Add between two different type is not permitted.");
  template <typename Li, typename Ri>
  inline decltype(auto) operator()(Li&& l, Ri&& r) const noexcept {
    return std::forward<Li>(l) + std::forward<Ri>(r);
  }
  template <typename X, typename DG> using Grad_t = DG;
};
///< Add

///> Neg
template <typename X> struct Neg : public Expr<typename X::type, X> {
  template <typename XI> inline decltype(auto) operator()(XI&& in_x) const noexcept {
    return -std::forward<XI>(in_x);
  }

  template <typename, typename G> using Grad_t = Neg<G>;
};
///< Neg

///> Twice
template <typename X> struct Twice : public Expr<typename X::type, X> {
  template <typename Xi> inline decltype(auto) operator()(Xi&& x) const noexcept { return x + x; }

  template <typename, typename G> using Grad_t = Twice<G>;
};
///< Twice

///> Half
template <typename X> struct Half : public Expr<typename X::type, X> {
  template <typename Xi> inline decltype(auto) operator()(Xi&& x) const noexcept { return x / 2; }

  template <typename, typename G> using Grad_t = Twice<G>;
};
///< Half

///> Substraction, L::type should equals to R::type
/// NOTE: sub is cwise.
template <typename L, typename R> struct Sub : public Expr<typename L::type, L, R> {
  static_assert(std::is_same_v<typename L::type, typename R::type>,
                "Substract between two different type is not permitted.");
  template <typename Li, typename Ri>
  inline decltype(auto) operator()(Li&& l, Ri&& r) const noexcept {
    return std::forward<Li>(l) - std::forward<Ri>(r);
  }
  template <typename X, typename G> using Grad_t
      = std::conditional_t<std::is_same_v<L, X>, G, Neg<G>>;
};

template <typename L> struct Sub<L, L> : public Expr<typename L::type, L> {
  template <typename Li> inline decltype(auto) operator()(Li&&) const noexcept {
    return ZerosLike<Li>{}();
  }
  template <typename X, typename G> using Grad_t = ZerosLike<G>;
};
///< Substraction

///> Multiplication.
template <typename L, typename R, class = void> struct Mul;
// Scalar * Scalar
template <typename L, typename R>
struct Mul<L, R,
           std::enable_if_t<!std::is_same_v<L, R> && TensorTrait<typename L::type>::is_scalar
                            && TensorTrait<typename R::type>::is_scalar>>
    : public Expr<typename L::type, L, R> {
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
  template <typename Li> inline decltype(auto) operator()(Li&& l) const noexcept { return l * l; }
  template <typename X, typename G> using Grad_t = Add<Mul<L, G>, Mul<G, L>>;
};

// Scalar * Matrix
template <typename L, typename R>
struct Mul<L, R,
           std::enable_if_t<TensorTrait<typename L::type>::is_scalar
                            && !(TensorTrait<typename R::type>::is_scalar)>>
    : public Expr<typename R::type, L, R> {
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
  template <typename Li, typename Ri>
  inline decltype(auto) operator()(Li&& l, Ri&& r) const noexcept {
    return std::forward<Li>(l) * std::forward<Ri>(r);
  }
  template <typename X, typename G> using Grad_t = Add<Mul<L, G>, Mul<G, L>>;
};
///< Multiplication

}  // namespace details

template <typename T> using Ones = details::Ones<T>;
template <typename E> using OnesLike = details::OnesLike<E>;
template <typename T> using Zeros = details::Zeros<T>;
template <typename E> using ZerosLike = details::ZerosLike<E>;
template <typename T, int... dmd> using Dirac = details::Dirac<T, dmd...>;

// NOLINTBEGIN(bugprone-macro-parentheses)
#define Variable(type, name)                                  \
  struct name : public acg::sad::details::Input<type, name> { \
  }

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
template <typename L> using Neg = details::Neg<L>;
template <typename L> using Half = details::Half<L>;
template <typename L> using Twice = details::Add<L, L>;

}  // namespace acg::sad
// NOLINTEND(readability-identifier-naming)
