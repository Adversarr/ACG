#pragma once
#include <Eigen/Core>
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
    if constexpr (std::is_const_v<T>) {
      return ConstVer(in);
    } else {
      return NonConstVer(in);
    }
  }

  template<typename T> inline Eigen::Reshaped<const T, rows, cols>
  ConstVer (const Eigen::MatrixBase<T>& in) const noexcept{
    return Eigen::Reshaped<const T, rows, cols>(in.derived());
  }

  template<typename T> inline Eigen::Reshaped<T, rows, cols>
  NonConstVer (Eigen::MatrixBase<T>& in) const noexcept{
    return Eigen::Reshaped<T, rows, cols>(in.derived());
  }
};
}
