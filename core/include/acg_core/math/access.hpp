#pragma once
#include <tuple>

#include "common.hpp"

namespace acg {

template <typename Scalar, int dim> class FieldCAccess {
  using RawType = Field<Scalar, dim>;
  const RawType& field_ref_;

public:
  inline explicit FieldCAccess(const Field<Scalar, dim>& field_ref) noexcept
      : field_ref_(field_ref) {}

  struct Iterator {
    using iterator_category = std::random_access_iterator_tag;
    using difference_type = typename RawType::Index;
    using value_type = decltype(std::declval<RawType>().col(0));

    Idx index;
    const RawType& raw_data;

    Iterator(difference_type index, RawType& raw_data) : index(index), raw_data(raw_data) {}

    value_type operator*() const { return raw_data.col(index); }

    Iterator& operator++() {
      index++;
      return *this;
    }

    bool operator==(const Iterator& rhs) const { return index == rhs.index; }

    bool operator!=(const Iterator& rhs) const { return index != rhs.index; }
  };

  inline auto operator[](Idx i) const noexcept { return field_ref_.col(i); }

  inline auto operator()(Idx i) const noexcept { return field_ref_.col(i); }

  inline Idx Size() const noexcept { return field_ref_.cols(); }

  inline auto Xwise() const noexcept { return field_ref_.colwise(); }

  inline auto Array() const noexcept { return field_ref_.array(); }
};

template <typename Scalar, int dim> class FieldAccess {
  using RawType = Field<Scalar, dim>;
  RawType& field_ref_;

public:
  inline explicit FieldAccess(Field<Scalar, dim>& field_ref) noexcept : field_ref_(field_ref) {}

  struct Iterator {
    using iterator_category = std::random_access_iterator_tag;
    using difference_type = typename RawType::Index;
    using value_type = decltype(std::declval<RawType>().col(0));

    Idx index;
    RawType& raw_data;

    Iterator(difference_type index, RawType& raw_data) : index(index), raw_data(raw_data) {}

    value_type operator*() const { return raw_data.col(index); }

    Iterator& operator++() {
      index++;
      return *this;
    }

    bool operator==(const Iterator& rhs) const { return index == rhs.index; }

    bool operator!=(const Iterator& rhs) const { return index != rhs.index; }
  };

  inline auto operator[](Idx i) noexcept { return field_ref_.col(i); }

  inline auto operator()(Idx i) noexcept { return field_ref_.col(i); }

  inline Idx Size() const noexcept { return field_ref_.cols(); }

  inline auto Xwise() noexcept { return field_ref_.colwise(); }

  inline auto Array() noexcept { return field_ref_.array(); }
};
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

template <typename Scalar, int dim> class FieldBuilder {
  using RawType = Field<Scalar, dim>;
  int n_{0};

public:
  explicit inline FieldBuilder(int n) : n_(n) {}

  inline RawType Placeholder() { return Field<Scalar, dim>(dim, n_); }

  inline auto Ones() { return Field<Scalar, dim>::Ones(dim, n_); }

  inline auto Zeros() { return Field<Scalar, dim>::Zeros(dim, n_); }

  inline auto Constant(Scalar s) { return Field<Scalar, dim>::Constant(s); }

  inline auto Random() { return Field<Scalar, dim>::Random(dim, n_); }

  inline RawType FromFunction(std::function<Vec<Scalar, dim>(Idx i)> generator) {
    auto ph = Placeholder();
    for (auto [i, b] : FieldEnumerate(ph)) {
      b = generator(i);
    }
    return ph;
  }
};

}  // namespace acg
