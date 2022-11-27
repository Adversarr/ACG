#include "dual.hpp"

namespace acg::sad {

namespace details {

// template <typename L, typename R,
//           class = std::enable_if_t<TensorTrait<GetInnerType_t<L>>::is_scalar>>
// struct CwiseMul : public Expr<typename L::type, L, R> {
//   static_assert(std::is_same_v<GetInnerType_t<L>, GetInnerType_t<R>>,
//                 "Cwise Operation need same type.");
//   // TODO: not fully implemented.
//   template <typename Xi, typename Ri>
//   inline decltype(auto) operator()(Xi&& xi, Ri&& ri) const noexcept {
//     return std::forward<Xi>(xi) * std::forward<Ri>(ri);
//   }
// };

// template <typename I, typename = std::enable_if_t<acg::TensorTrait<typename I::type>::is_scalar>>
// struct ElemwiseSqr : public Expr<typename I::type, I> {
//   template <typename Xi> inline decltype(auto) operator()(Xi&& in_x) const noexcept {
//     return in_x * in_x;
//   }
//   template <typename G> using Grad_t = Add<G, G>;
// };
// template <typename I, typename = std::enable_if_t<acg::TensorTrait<typename I::type>::is_scalar>>
// struct ElemwiseInv : public Expr<typename I::type, I> {
//   template <typename Xi> inline decltype(auto) operator()(Xi&& in_x) const noexcept {
//     return static_cast<Xi>(1.0f) / std::forward<Xi>(in_x);
//   }

//   template <typename G> using Grad_t = Neg<CwiseMul<CwiseMul<ElemwiseInv<I>, ElemwiseInv<I>>, G>>;
// };

// template <typename I, typename = std::enable_if_t<acg::TensorTrait<typename I::type>::is_scalar>>
// struct Sqrt : public Expr<typename I::type, I> {
//   template <typename Xi> inline decltype(auto) operator()(Xi&& in_x) const noexcept {
//     return sqrt(std::forward<Xi>(in_x));
//   }
// };

}  // namespace details

}  // namespace acg::sad
