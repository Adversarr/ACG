#pragma once
#include <acore/math/common.hpp>
#include <autils/common.hpp>
#include <tuple>

#include "./details/access-inl.hpp"
#include "indexer.hpp"
#include "transform.hpp"

namespace acg {
using DefaultIndexer = NdRangeIndexer<1>;

/****************************************
 * NOTE: When access a field(dense), two things should be considered:
 *  1. How to Index your data, i.e. what is the relationship between
 *     mathematics index and physical device's index. For example:
 *     (See indexer.hpp for more details.)
 *        For a field in physics world, we may index each cell (grid)
 *        by (i, j, k). However, we only have one dimension I for the
 *        field. Indexer provides the Bi-Map (i, j, k) <-> I
 *  2. How to view your data, i.e. what is the relation ship between
 *     (See transform.hpp for more details.)
 *     mathematics symbol and physical data. For example:
 *        For a matrix field, we have to use a Vectorize transform to
 *        store it in a column. ReshapeTransform provides the function.
 *
 ****************************************/

// NOTE: Access Function for fields.

// 1. For r-value.
template <typename Indexer = NdRangeIndexer<1>, typename Transform = IdentityTransform,
          typename Type>
decltype(auto) access(Type&& field, Indexer getter = Indexer()) {
  getter.Fit(field);
  return details::FieldAccessor<const std::remove_cv_t<Type>, Transform, Indexer>(
      std::forward<Type>(field), getter, utils::god::RvalueTag{});
}

// 2. For cr-value
template <typename Indexer = NdRangeIndexer<1>, typename Transform = IdentityTransform,
          typename Type>
decltype(auto) access(const Type&& field, Indexer getter = Indexer()) {
  getter.Fit(field);
  return details::FieldAccessor<Type, Transform, Indexer>(std::forward<decltype(field)>(field),
                                                          getter, utils::god::ConstRvalueTag{});
}

// 3. For cl-value
template <typename Indexer = NdRangeIndexer<1>, typename Transform = IdentityTransform,
          typename Type>
decltype(auto) access(const Type& field, Indexer getter = Indexer()) {
  getter.Fit(field);
  return details::FieldAccessor<const Type&, Transform, Indexer>(field, getter,
                                                                 utils::god::ConstLvalueTag{});
}

// 4. For l-value
template <typename Indexer = NdRangeIndexer<1>, typename Transform = IdentityTransform,
          typename Type>
decltype(auto) access(Type& field, Indexer getter = Indexer()) {
  getter.Fit(field);
  return details::FieldAccessor<Type&, Transform, Indexer>(field, getter, utils::god::LvalueTag{});
}

template <typename A> decltype(auto) enumerate(A&& access) {
  return details::FieldEnumerate<std::decay_t<A>>(access);
}

template <typename Scalar, int dim> class FieldBuilder {
  using RawType = Field<Scalar, dim>;

  Index n_{0};

public:
  explicit inline FieldBuilder(Index n = 0) : n_(n) {}

  inline auto Placeholder() { return Field<Scalar, dim>(dim, n_); }

  inline auto Ones() { return Field<Scalar, dim>::Ones(dim, n_).eval(); }

  inline auto Zeros() { return Field<Scalar, dim>::Zero(dim, n_).eval(); }

  inline auto Constant(Scalar s) { return Field<Scalar, dim>::Constant(dim, n_, s).eval(); }

  inline auto Random() { return Field<Scalar, dim>::Random(dim, n_).eval(); }
};

}  // namespace acg
