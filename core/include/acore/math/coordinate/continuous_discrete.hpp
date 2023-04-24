#pragma once
#include "common.hpp"

namespace acg {

// Regular Grid discretization. (e.g. for dx = 1)
// 0, 1 -- 1, 1
//  | [0, 0] |
// 0, 0 -- 1, 0
template <typename Scalar, int dim>
struct ContinuousDiscreteTransformRegular
    : public CoordinateTransformBase<
          ContinuousDiscreteTransformRegular<Scalar, dim>, Vec<Scalar, dim>,
          IndexVec<dim>> {
  using domain_type = Vec<Scalar, dim>;
  using range_type = IndexVec<dim>;

  ContinuousDiscreteTransformRegular() : grid_size_(0), grid_size_inverse_(0) {}

  static_assert(std::is_floating_point_v<Scalar>,
                "Type argument Scalar is not float point.");
  explicit ContinuousDiscreteTransformRegular(Scalar grid_size)
      : grid_size_(grid_size),
        grid_size_inverse_(static_cast<Scalar>(1) / grid_size) {
    assert(grid_size_ > 0 && "Found grid size is negative or zero.");
  }

  range_type Forward(domain_type x) const noexcept {
    domain_type x0 = (x * grid_size_inverse_).array().floor();
    return x0.template cast<Index>();
  }

  domain_type Backward(range_type x) const noexcept {
    domain_type x0 = x.template cast<Scalar>();
    return x0 * grid_size_;
  }

  Scalar grid_size_;
  Scalar grid_size_inverse_;
};

template <typename Scalar, int dim>
struct ContinuousDiscreteTransformBoundedRegular
    : CoordinateTransformBase<
          ContinuousDiscreteTransformBoundedRegular<Scalar, dim>,
          Vec<Scalar, dim>, IndexVec<dim>> {
  using domain_type = Vec<Scalar, dim>;
  using range_type = Vec<Index, dim>;

  explicit ContinuousDiscreteTransformBoundedRegular(domain_type lower_bound,
                                                     Scalar grid_size)
      : c_d_(grid_size),
        bias_(
            -(lower_bound / grid_size).array().floor().template cast<Index>()) {
  }

  range_type Forward(domain_type x) const noexcept {
    return bias_.Forward(c_d_.Forward(x));
  }

  domain_type Backward(range_type x) const noexcept {
    return c_d_.Backward(bias_.Backward(x));
  }

  ContinuousDiscreteTransformRegular<Scalar, dim> c_d_;
  BiasTransform<Index, dim> bias_;
};

} // namespace acg
