#pragma once
#include <acore/common.hpp>
#include <utility>

namespace acg::utils {

/**
 * @brief compute the product of input arguments
 */
template <typename... Args> constexpr decltype(auto) product(Args... args) {
  if constexpr (sizeof...(args) == 0) {
    return 1;
  } else {
    return (args * ...);
  }
}



}  // namespace acg::utils
