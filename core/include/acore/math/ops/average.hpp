#pragma once
#include <acore/math/common.hpp>
namespace acg::math {
struct WeightedAverage {
  template <typename Scalar, int dim>
  Vec<Scalar, dim> operator()(const Field<Scalar, dim>& field,
                              const Field<Scalar, 1>& weight) const noexcept {
    return (field.array().rowwise() * weight.array()).rowwise().sum() / weight.sum();
  }
};
}  // namespace acg::math
