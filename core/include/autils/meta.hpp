#pragma once
namespace acg ::utils {

struct LvalueTag {};
struct RvalueTag {};
struct ConstRvalueTag {};
struct ConstLvalueTag {};

template <typename T, int alpha> constexpr auto ctpow(T value) {
  if constexpr (alpha > 0) {
    return value * ctpow<alpha - 1>(value);
  } else {
    return 1;
  }
}

}  // namespace acg::utils
