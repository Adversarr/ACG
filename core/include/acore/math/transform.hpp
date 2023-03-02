#pragma once
#include <utility>

namespace acg {

// Pre transform to field accessor.
struct IdentityTransform {
  template <typename T> inline decltype(auto) operator()(T&& in) const noexcept {
    return std::forward<T>(in);
  }
};

// For field, reshape
template <int rows, int cols> struct ReshapeTransform {
  template <typename T> inline decltype(auto) operator()(T&& in) const noexcept {
    return in.reshaped(rows, cols);
  }
};
}
