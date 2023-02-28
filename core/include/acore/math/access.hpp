#pragma once
#include <acore/math/common.hpp>
#include <autils/common.hpp>
#include <tuple>

#include "./details/access-inl.hpp"
#include "autils/common.hpp"

namespace acg {
// Export Getter and Transform
using IdentityTransform = details::IdentityTransform;
template <int r, int c> using ReshapeTransform = details::ReshapeTransform<r, c>;
template <Index d> using NdRangeIndexer = details::NdRangeIndexer<d>;
using DefaultIndexer = NdRangeIndexer<1>;

// NOTE: Access Function for fields.

// 1. For r-value.
template <typename Indexer = details::NdRangeIndexer<1>,
          typename Transform = details::IdentityTransform, typename Type>
decltype(auto) access(Type&& field, Indexer getter = Indexer()) {
  auto ret = details::FieldAccessor<const std::remove_cv_t<Type>, Transform, Indexer>(
      std::forward<Type>(field), getter, utils::RvalueTag{});
  return ret;
}

// 2. For cr-value
template <typename Indexer = details::NdRangeIndexer<1>,
          typename Transform = details::IdentityTransform, typename Type>
decltype(auto) access(const Type&& field, Indexer getter = Indexer()) {
  auto ret = details::FieldAccessor<Type, Transform, Indexer>(std::forward<decltype(field)>(field),
                                                              getter, utils::ConstRvalueTag{});
  return ret;
}

// 3. For cl-value
template <typename Indexer = details::NdRangeIndexer<1>,
          typename Transform = details::IdentityTransform, typename Type>
decltype(auto) access(const Type& field, Indexer getter = Indexer()) {
  return details::FieldAccessor<const Type&, Transform, Indexer>(field, getter,
                                                                 utils::ConstLvalueTag{});
}

// 4. For l-value
template <typename Indexer = details::NdRangeIndexer<1>,
          typename Transform = details::IdentityTransform, typename Type>
decltype(auto) access(Type& field, Indexer getter = Indexer()) {
  return details::FieldAccessor<Type&, Transform, Indexer>(field, getter, utils::LvalueTag{});
}

// TODO: Modify
template <typename Scalar, int dim> class FieldEnumerate {
  using RawType = Field<Scalar, dim>;
  RawType& field_ref_;

public:
  inline explicit FieldEnumerate(Field<Scalar, dim>& field_ref) noexcept : field_ref_(field_ref) {}

  struct Iterator {
    using iterator_category = std::random_access_iterator_tag;
    using difference_type = typename RawType::Index;
    using value_type = std::pair<difference_type, decltype(std::declval<RawType>().col(0))>;

    difference_type index;
    RawType& raw_data;

    Iterator(difference_type index, RawType& raw_data) : index(index), raw_data(raw_data) {}

    value_type operator*() const { return std::make_pair(index, raw_data.col(index)); }

    Iterator& operator++() {
      index++;
      return *this;
    }

    bool operator==(const Iterator& rhs) const { return index == rhs.index; }

    bool operator!=(const Iterator& rhs) const { return index != rhs.index; }
  };

  inline Iterator begin() { return Iterator(0, field_ref_); }

  inline Iterator end() { return Iterator(field_ref_.cols(), field_ref_); }
};

template <typename Scalar, int dim> class FieldCEnumerate {
  using RawType = Field<Scalar, dim>;
  const RawType& field_ref_;

public:
  inline explicit FieldCEnumerate(const Field<Scalar, dim>& field_ref) noexcept
      : field_ref_(field_ref) {}

  struct Iterator {
    using iterator_category = std::random_access_iterator_tag;
    using difference_type = acg::Index;
    using value_type = std::pair<difference_type, decltype(std::declval<RawType>().col(0))>;

    difference_type index;

    const RawType& raw_data;

    Iterator(difference_type index, const RawType& raw_data) : index(index), raw_data(raw_data) {}

    decltype(auto) operator*() const { return std::make_pair(index, raw_data.col(index)); }

    Iterator& operator++() {
      index++;
      return *this;
    }

    bool operator==(const Iterator& rhs) const { return index == rhs.index; }

    bool operator!=(const Iterator& rhs) const { return index != rhs.index; }
  };

  inline Iterator begin() { return Iterator(0, field_ref_); }

  inline Iterator end() { return Iterator(field_ref_.cols(), field_ref_); }
};

template <typename Scalar, int dim> class FieldBuilder {
  using RawType = Field<Scalar, dim>;
  int n_{0};

public:
  explicit inline FieldBuilder(int n) : n_(n) {}

  inline decltype(auto) Placeholder() { return Field<Scalar, dim>(dim, n_); }

  inline decltype(auto) Ones() { return Field<Scalar, dim>::Ones(dim, n_).eval(); }

  inline decltype(auto) Zeros() { return Field<Scalar, dim>::Zero(dim, n_).eval(); }

  inline decltype(auto) Constant(Scalar s) {
    return Field<Scalar, dim>::Constant(dim, n_, s).eval();
  }

  inline decltype(auto) Random() { return Field<Scalar, dim>::Random(dim, n_).eval(); }


  // TODO: Generate from function
};

}  // namespace acg
