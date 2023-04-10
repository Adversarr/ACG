/**
 * @file indexer.hpp
 * @author Zherui Yang (yangzherui2001@foxmail.com)
 * @brief ACG default Indexers.
 * @version 0.0.1
 * @date 2023-04-05
 *
 * @copyright Copyright (c) 2023
 *
 */

#pragma once
#include <acore/common.hpp>
#include <acore/math/ndrange.hpp>
#include <autils/god/utilities.hpp>

#include "common.hpp"

namespace acg {
using utils::god::IndexTuple;

/****************************************
 * NOTE: Foreach Indexer, it have:
 *  1. value_type: indicates the return value type
 *  2. operator(...): Physical Range -> Device Range
 *  3. operator[d]: Device Range -> Physical Range
 *  4. Iterate(): To corresponding Iterator.
 *  5. Fit(field) for each indexer.
 ****************************************/
template <Index dim> class NdRangeIndexer;

template <> class NdRangeIndexer<0> {
public:
  inline constexpr Index operator()() const noexcept { return 0; }

  inline constexpr std::tuple<> operator[](Index) const noexcept { return {}; }

  inline constexpr std::tuple<> Shape() const noexcept { return {}; }
};

template <> class NdRangeIndexer<1> {
public:
  inline Index operator()(Index id) const { return id; }

  constexpr bool IsValid(Index this_size) const noexcept {
    return 0 <= this_size && this_size < this_dim_;
  }

  explicit constexpr NdRangeIndexer(Index this_dim = 0) : this_dim_(this_dim) {}

  constexpr auto operator[](Index id) const noexcept { return std::tuple<Index>(id); }

  constexpr auto Shape() const noexcept { return std::tuple<Index>(this_dim_); }

  constexpr auto Iterate() const noexcept { return NdRange<1>(this_dim_); }

  template <typename T> inline void Fit(T &&field) { this_dim_ = field.cols(); }

protected:
  Index this_dim_;
};

template <Index dim> class NdRangeIndexer : public NdRangeIndexer<dim - 1> {
public:
  template <typename... Args> constexpr explicit NdRangeIndexer(Index this_dim, Args... arg)
      : NdRangeIndexer<dim - 1>(arg...),
        this_dim_(this_dim),
        multiplier_(acg::utils::god::product(static_cast<Index>(arg)...)) {}

  constexpr explicit NdRangeIndexer(Index dim_universal)
      : NdRangeIndexer<dim - 1>(dim_universal),
        this_dim_(dim_universal),
        multiplier_(acg::utils::god::pow<dim - 1>(dim_universal)) {}

  constexpr NdRangeIndexer() : NdRangeIndexer<dim - 1>(), this_dim_(0), multiplier_(0) {}

  template <typename... Args>
  constexpr bool IsValid(Index this_size, Args... indices) const noexcept {
    return (0 <= this_size && this_size < this_dim_)
           && NdRangeIndexer<dim - 1>::IsValid(static_cast<Index>(indices)...);
  }

  template <typename... Args>
  constexpr Index operator()(Index this_size, Args... indices) const noexcept {
    return this_size * multiplier_
           + NdRangeIndexer<dim - 1>::operator()(static_cast<Index>(indices)...);
  }

  constexpr utils::god::IndexTuple<dim> operator[](Index id) const noexcept {
    return std::tuple_cat(std::make_tuple<Index>(id / multiplier_),
                          NdRangeIndexer<dim - 1>::operator[](id % multiplier_));
  }

  constexpr utils::god::IndexTuple<dim> Shape() const noexcept {
    return std::tuple_cat(std::tuple<Index>(this_dim_), NdRangeIndexer<dim - 1>::Shape());
  }

  constexpr auto Iterate() const noexcept { return NdRange<dim>(Shape()); }

  template <typename T> inline void Fit(T && /*field*/) {}

protected:
  Index this_dim_{0};
  Index multiplier_{0};
};

template <typename Scalar, int dim> struct GridCoordConventer {
  GridCoordConventer(Vec<Scalar, dim> lower_bound, Vec<Scalar, dim> upper_bound, Scalar grid_size)
      : lower_bound_(lower_bound), upper_bound_(upper_bound), grid_size_(grid_size) {
    physical_local_scaling_.setConstant(static_cast<Scalar>(1) / grid_size);
    local_physical_scaling_ = physical_local_scaling_.cwiseInverse();
  }

  Vec<Scalar, dim> World2LocalContinuous(Vec<Scalar, dim> world_position) const noexcept {
    return (world_position - lower_bound_).array() * physical_local_scaling_.array();
  }

  Vec<Index, dim> World2LocalDiscreteTrunc(Vec<Scalar, dim> world_position) const noexcept {
    return World2LocalContinuous(world_position).template cast<Index>();
  }
  Vec<Index, dim> World2LocalDiscreteRound(Vec<Scalar, dim> world_position) const noexcept {
    return World2LocalContinuous(world_position).array().round().template cast<Index>();
  }

  Vec<Scalar, dim> LocalDiscrete2World(Vec<Index, dim> local_position) const noexcept {
    return local_position.template cast<Scalar>() * local_physical_scaling_ + lower_bound_;
  }

  Vec<Scalar, dim> lower_bound_;
  Vec<Scalar, dim> upper_bound_;
  Scalar grid_size_;
  Vec<Scalar, dim> physical_local_scaling_;
  Vec<Scalar, dim> local_physical_scaling_;
};

}  // namespace acg
