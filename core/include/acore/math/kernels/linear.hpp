#pragma once
#include <acore/math/common.hpp>
#include <acore/math/func.hpp>
namespace acg::math {

template <typename Scalar> struct Lerp {
  template <typename Derived, typename = std::enable_if_t<std::is_same_v<
                                  typename Trait<Derived>::Scalar, Scalar>>>
  auto
  operator()(const Eigen::MatrixBase<Derived> &local_coord) const noexcept {
    static constexpr int dim = Trait<Derived>::rows;
    Vec<Scalar, 2> current = Lerp{}(local_coord(dim - 1));
    Vec<Scalar, pow<dim>(2)> result;
    if constexpr (dim == 2) {
      Vec<Scalar, pow<dim - 1>(2)> last = Lerp{}(local_coord(1));
      result.template topRows<pow<dim - 1>(2)>() = current(0) * last;
      result.template bottomRows<pow<dim - 1>(2)>() = current(1) * last;
    } else {
      Vec<Scalar, pow<dim - 1>(2)> last =
          Lerp{}(local_coord.template topRows<dim - 1>());
      result.template topRows<pow<dim - 1>(2)>() = current(0) * last;
      result.template bottomRows<pow<dim - 1>(2)>() = current(1) * last;
    }
    return result;
  }

  Vec<Scalar, 2> operator()(Scalar coord) const noexcept {
    return {coord, constant(1) - coord};
  }
};

} // namespace acg::math
