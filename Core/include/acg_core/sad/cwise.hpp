#pragma once
#include "dual.hpp"

namespace acg::sad {

namespace details {

template <typename T> constexpr bool IsScalar_v = TensorTrait<typename T::type>::is_scalar;
template <typename T> using Scalar_t = typename TensorTrait<typename T::type>::Scalar;

///> CwiseMul
template <typename L, typename R> struct CwiseMul : public Expr<typename L::type, L, R> {
  static_assert(std::is_same_v<typename L::type, typename R::type>,
                "CWiseMul needs two expr with same type");
  template <typename Li, typename Ri>
  inline decltype(auto) operator()(Li&& l, Ri&& r) const noexcept {
    if constexpr (IsScalar_v<L> && IsScalar_v<R>) {
      return std::forward<Li>(l) * std::forward<Ri>(r);
    } else {
      return l.cwiseProduct(r);
    }
  }
  template <typename X, typename DG> using Grad_t = std::conditional_t<
      std::is_same_v<L, R>, Twice<CwiseMul<L, DG>>,
      std::conditional_t<std::is_same_v<L, X>, CwiseMul<DG, R>, CwiseMul<L, DG>>>;
};
///< CwiseMul

///> CwiseAbs2
/// INFO: may leads to bug, when X is complex.
template <typename X> struct CwiseAbs2 : public Expr<GetInnerType_t<X>, X> {
  template <typename Xi> inline decltype(auto) operator()(Xi&& x) const noexcept {
    if constexpr (IsScalar_v<X>) {
      return x * x;
    } else {
      return x.cwiseAbs2();
    }
  }

  template <typename Xi, typename DG> using Grad_t = Twice<CwiseMul<DG, X>>;
};
///< CwiseAbs2

///> CwiseNanToZero
/// TODO: Test is needed.
template <typename X> struct CwiseNanToZero : public Expr<typename X::type, X> {
  static double campd(double x) { return std::isfinite(x) ? x : static_cast<double>(0); }
  static float campf(float x) { return std::isfinite(x) ? x : static_cast<float>(0); }
  template <typename Xi> inline decltype(auto) operator()(Xi&& x) const noexcept {
    if constexpr (IsScalar_v<X>) {
      return std::isfinite(x) ? x : static_cast<std::remove_cv_t<Xi>>(0);
    } else {
      if constexpr (std::is_same_v<Scalar_t<typename X::type>, float>) {
        return x.array().unaryExpr(campf).matrix();
      } else {
        return x.array().unaryExpr(campd).matrix();
      }
    }
  }
  // d(1/x) = - 1/x2
  template <typename, typename G> using Grad_t = CwiseNanToZero<G>;
};
///< CwiseInv

///> CwiseInv
template <typename X> struct CwiseInv : public Expr<typename X::type, X> {
  template <typename Xi> inline decltype(auto) operator()(Xi&& x) const noexcept {
    if constexpr (IsScalar_v<X>) {
      return static_cast<typename X::type>(1) / std::forward<Xi>(x);
    } else {
      return x.cwiseInverse();
    }
  }
  // d(1/x) = - 1/x2
  template <typename, typename G> using Grad_t = CwiseMul<Neg<CwiseInv<CwiseAbs2<X>>>, G>;
};
///< CwiseInv
///> CwiseDiv
template <typename L, typename R> using CwiseDiv = CwiseMul<L, CwiseInv<R>>;
///< CwiseDiv

///> CwiseSqrt
template <typename X> struct CwiseSqrt : public Expr<typename X::type, X> {
  template <typename Xi> inline decltype(auto) operator()(Xi&& xi) const noexcept {
    if constexpr (IsScalar_v<X>) {
      return sqrt(std::forward<Xi>(xi));
    } else {
      return xi.array().sqrt().matrix();
    }
  }

  // 1 / 2 sqrt(x)
  template <typename, typename G> using Grad_t = CwiseMul<Half<CwiseSqrt<X>>, G>;
};
///< CwiseSqrt

/// *** Reductions

///> Sum
template <typename X> struct Sum : public Expr<Scalar_t<X>, X> {
  template <typename Xi> inline decltype(auto) operator()(Xi&& xi) const noexcept {
    if constexpr (IsScalar_v<X>) {
      return xi;
    } else {
      return xi.sum();
    }
  }
  template <typename, typename G> using Grad_t = Sum<G>;
};
///< Sum

///> Norm
///  NOTE: L2 Norm for vector, F-Norm for matrix, (eulc-length)
template <typename X> struct Norm : public Expr<Scalar_t<X>, X> {
  template <typename Xi> inline decltype(auto) operator()(Xi&& xi) const noexcept {
    if constexpr (IsScalar_v<X>) {
      return abs(std::forward<Xi>(xi));
    } else {
      return xi.norm();
    }
  }
  template <typename, typename G> using Grad_t = CwiseDiv<Sum<CwiseMul<X, G>>, Norm<X>>;
};
///< Norm

}  // namespace details

template <typename L, typename R> using CwiseMul = details::CwiseMul<L, R>;
template <typename X> using CwiseAbs2 = details::CwiseAbs2<X>;
template <typename X> using CwiseInv = details::CwiseInv<X>;
template <typename X> using CwiseSqrt = details::CwiseSqrt<X>;
template <typename X> using Norm = details::Norm<X>;
template <typename X> using Sum = details::Sum<X>;

}  // namespace acg::sad
