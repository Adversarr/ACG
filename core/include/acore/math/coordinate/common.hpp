/**
 * @brief Coordinate Transform:
 * 1. Continuous -> Discrete: this transform may be not bijective, but always
 *    valid.
 *    Example: (x, y, z) in R^3 |-> (i, j, k) in Z^3 for fluid simulation.
 *        i = Round[ x / dx ]
 * 2. Discrete -> Discrete: this transform is bijective, but Domain Check
 *    should be performed (input is not always valid).
 *    Example: (i, j, k) in Z^3 |-> (p, q, r) in Z^3,
 *        p = i - i_lower_bound
 * 3. Discrete -> Storage: you may need Morton Curve in your application for
 *    better locality. but a standard index gives
 *        (p, q, r) in N^3 |-> i in N
 *        i = p * (ub1 * ub2) + q * ub2 + r
 *
 *
 * @note
 *  1. For stl type, prefer `array<IndexType, dim>` over tuple.
 *  2. both [1] and [2] does not change `dim`.
 *  3. You can combine several Indexer to get a new indexer
 *  4. To use operator() instead of forward, you need make sure the Domain is an
       Eigen::Vector.
 */

#pragma once
#include <acore/math/common.hpp>
#include <acore/math/ndrange.hpp>
#include <array>
#include <type_traits>

namespace acg {

/**
 * The base type provides a helper operator(), support several input format.
 *
 * 1. Same as `Forward`, i.e. `Domain`.
 * 2. A variadic input, that can construct a `Domain`
 * 3. A tuple, that can be used to construct `Domain`
 *
 */
template <typename Derived, typename Domain, typename Range>
struct CoordinateTransformBase {
  using domain_type = Domain;
  using range_type = Range;

  // Input is `Domain`
  inline auto operator()(const Domain &arg) const noexcept {
    return static_cast<const Derived *>(this)->Forward(arg);
  }

  // Input can construct a `Domain`
  template <typename Front, typename... Args,
            typename = std::enable_if_t<
                std::is_constructible_v<Domain, Front, Args...> &&
                !std::is_same_v<std::remove_cv_t<Front>, Domain>>>
  inline auto operator()(Front &&arg0, Args &&...arg) const noexcept {
    return static_cast<const Derived *>(this)->Forward(
        Domain(std::forward<Front>(arg0), std::forward<Args>(arg)...));
  }

  // Input is tuple, but can construct a `Domain`.
  template <typename... Args, typename = std::enable_if_t<
                                  std::is_constructible_v<Domain, Args...>>>
  inline auto operator()(std::tuple<Args...> tup) const noexcept {
    return static_cast<const Derived *>(this)->Forward(
        std::make_from_tuple<Domain>(tup));
  }
};

template <typename Scalar, int dim>
struct BiasTransform
    : public CoordinateTransformBase<BiasTransform<Scalar, dim>,
                                     Vec<Scalar, dim>, Vec<Scalar, dim>> {
  using domain_type = Vec<Scalar, dim>;
  using range_type = Vec<Scalar, dim>;

  BiasTransform() : BiasTransform(domain_type::Zero()) {}

  explicit BiasTransform(domain_type bias) noexcept
      : bias_(bias) {
    // assert(bias_.isFinite() && "Bias is not finite.");
  }

  range_type Forward(domain_type x) const noexcept { return x + bias_; }

  domain_type Backward(domain_type x) const noexcept { return x - bias_; }

  domain_type bias_;
};

/**
 * @brief Discrete -> Storage
 * Assuming the lower bound is [0, ...]
 *
 * @tparam dim
 */
template <int dim>
struct DiscreteStorageSequentialTransform
    : public CoordinateTransformBase<DiscreteStorageSequentialTransform<dim>,
                                     IndexVec<dim>, Index> {
  using domain_type = IndexVec<dim>;
  using range_type = Index;

  DiscreteStorageSequentialTransform(
      const DiscreteStorageSequentialTransform &) = default;

  DiscreteStorageSequentialTransform() = default;

  explicit DiscreteStorageSequentialTransform(domain_type bound) noexcept
      : bound_(bound) {
    assert((bound_.array() > 0).all() &&
           "Invalid bound. bound should be all greater than zero.");
    Index scale = 1;
    for (Index i = 0; i < static_cast<Index>(dim); ++i) {
      multiplier_[dim - i - 1] = scale;
      scale *= bound_[dim - i - 1];
    }
  }

  inline void Set(domain_type bound) noexcept {
    bound_ = bound;
    Index scale = 1;
    for (Index i = 0; i < static_cast<Index>(dim); ++i) {
      multiplier_[dim - i - 1] = scale;
      scale *= bound_[dim - i - 1];
    }
  }

  range_type Forward(domain_type x) const noexcept {
    assert(ForwardIsValid(x) && "Invalid index");
    return (x.array() * multiplier_.array()).sum();
  }

  domain_type Backward(range_type x) const noexcept {
    assert((x >= 0) && "Invalid Index");
    auto x_back = x;
    domain_type x0;
    for (Index i = 0; i < static_cast<Index>(dim); ++i) {
      x0[i] = x_back / multiplier_[i];
      x_back = x_back % multiplier_[i];
    }
    assert(Forward(x0) == x && "Internal Impl Error.");
    return x0;
  }

  NdRange<dim> Iterate() const noexcept {
    return NdRange<dim>(to_tuple(bound_));
  }

  template <typename Arg> constexpr void Fit(Arg &&data) {
    if constexpr (dim == 1) {
      bound_[0] = data.cols();
      multiplier_[0] = 1;
    }
  }

  bool ForwardIsValid(domain_type x) const noexcept {
    return (x.array() >= 0).all() && (x.array() < bound_.array()).all();
  }

  domain_type bound_;
  domain_type multiplier_;
};

template <typename Front, typename Back>
struct CombinedTransform
    : public CoordinateTransformBase<CombinedTransform<Front, Back>,
                                     typename Front::domain_type,
                                     typename Back::range_type> {
  explicit CombinedTransform(Front f, Back b) : front_(f), back_(b) {}
  using domain_type = typename Front::domain_type;
  using range_type = typename Back::range_type;

  range_type Forward(domain_type x) const noexcept {
    auto mid = front_.Forward(x);
    return back_.Forward(mid);
  }

  domain_type Backward(range_type x) const noexcept {
    auto mid = back_.Backward(x);
    return front_.Backward(mid);
  }

  Front front_;
  Back back_;
};

template <typename Front, typename Back>
auto combine_transform(Front &&f, Back &&b) {
  return CombinedTransform<std::decay_t<Front>, std::decay_t<Back>>(f, b);
}

} // namespace acg
