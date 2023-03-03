#pragma once

#include <type_traits>
namespace acg::physics::elastic {
/**
 * @brief For any `Elastic Model`, it can computes the value,
 *
 * @tparam F Float point type.
 * @tparam dim Dimensions
 * @tparam Derived Implements the model.
 */
template <typename F, int dim, typename Derived> class HyperElasticModel {
public:
  template <typename... Args,
            typename Enable = std::void_t<typename Derived::HasEnergyFlag>>
  decltype(auto) Energy(Args &&...args) const noexcept {
    return (static_cast<Derived *>(this))
        ->EnergyImpl(std::forward<Args>(args)...);
  }

  template <typename... Args, typename Enable = void>
  decltype(auto) Strain(Args &&...args) const noexcept;

  template <typename... Args, typename Enable = void>
  decltype(auto) Stress(Args &&...args) const noexcept;

  template <typename... Args, typename Enable = void>
  decltype(auto) Hessian(Args &&...args) const noexcept;
};
} // namespace acg::physics::elastic