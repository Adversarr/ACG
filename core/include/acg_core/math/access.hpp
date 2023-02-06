#pragma once
#include <tuple>

#include "common.hpp"

namespace acg {

namespace details {

// Pre transform to field accessor.
struct IdentityTransform {
  // template <typename T> constexpr inline T&& operator()(T&& in) const noexcept {
  //   return static_cast<T&&>(in);
  // }
};

template <int rows, int cols> struct ReshapeTransform {
  template <typename T> inline decltype(auto) operator()(T&& in) const noexcept {
    return in.reshaped(rows, cols);
  }
};

// NOTE: standard field getter
template <Index dim> class MultiDimensionGetter;

template <> class MultiDimensionGetter<0> {
public:
  inline Index operator()() const { return 0; }

  static constexpr Index this_dim = 0;  // NOLINT
protected:
  static inline Index Run(Index result) { return result; }
};

template <> class MultiDimensionGetter<1> {
public:
  inline Index operator()(Index id) const { return id; }

  explicit MultiDimensionGetter(Index this_dim = 0) : this_dim(this_dim) {}

protected:
  inline Index Run(Index result, Index this_size) const { return result * this_dim + this_size; }

  Index this_dim{0};
};

template <Index dim> class MultiDimensionGetter : public MultiDimensionGetter<dim - 1> {
public:
  template <typename... Args> explicit MultiDimensionGetter(Index this_dim, Args... arg)
      : MultiDimensionGetter<dim - 1>(std::forward<Args>(arg)...), this_dim(this_dim) {}

  template <typename... Args> Index operator()(Args... indices) const { return Run(0, indices...); }

protected:
  template <typename... Args> Index Run(Index result, Index this_size, Args... indices) const {
    return MultiDimensionGetter<dim - 1>::Run(result * this_dim + this_size, indices...);
  }

  Index this_dim{0};
};

// NOTE: Field Access Constructor Tags.
struct LvalueTag {};
struct RvalueTag {};
struct ConstRvalueTag {};
struct ConstLvalueTag {};

// NOTE: Standard Field Accessor.
template <typename Type, typename Transform, typename Indexer> struct FieldAccessor {
  using type = Type;
  using origin_type = std::remove_cv_t<Type>;
  Type data;
  Indexer indexer;
  Transform transform;

  inline explicit FieldAccessor(origin_type& x, Indexer getter, LvalueTag)
      : data(x), indexer(getter) {}

  inline explicit FieldAccessor(const origin_type& x, Indexer getter, ConstLvalueTag)
      : data(x), indexer(getter) {}

  inline explicit FieldAccessor(origin_type&& x, Indexer getter, RvalueTag)
      : data(std::forward<origin_type>(x)), indexer(getter) {}

  inline explicit FieldAccessor(const origin_type&& x, Indexer getter, ConstRvalueTag)
      : data(x), indexer(getter) {}

  FieldAccessor(const FieldAccessor&) = delete;

  FieldAccessor(FieldAccessor&&) = default;

  inline decltype(auto) operator[](Index i) const noexcept { return data.col(i); }
  inline decltype(auto) operator[](Index i) noexcept { return data.col(i); }

  inline Index Size() const noexcept { return data.cols(); }

  template <typename... Args> inline decltype(auto) operator()(Args... args) const noexcept {
    if constexpr (std::is_same_v<Transform, IdentityTransform>) {
      return data.col(indexer(std::forward<Args>(args)...));
    } else {
      return transform(data.col(indexer(std::forward<Args>(args)...)));
    }
  }
  template <typename... Args> inline decltype(auto) operator()(Args... args) noexcept {
    if constexpr (std::is_same_v<Transform, IdentityTransform>) {
      return data.col(indexer(std::forward<Args>(args)...));
    } else {
      return transform(data.col(indexer(std::forward<Args>(args)...)));
    }
  }

  inline decltype(auto) begin() const { return data.colwise().begin(); }

  inline decltype(auto) end() const { return data.colwise().end(); }
};

}  // namespace details

// Export Getter and Transform
using IdentityTransform = details::IdentityTransform;
template <int r, int c> using ReshapeTransform = details::ReshapeTransform<r, c>;
template <Index d> using MultiDimensionGetter = details::MultiDimensionGetter<d>;

// NOTE: Access Function for fields.

// 1. For r-value.
template <typename Indexer = details::MultiDimensionGetter<1>,
          typename Transform = details::IdentityTransform, typename Type>
decltype(auto) access(Type&& field, Indexer getter = Indexer()) {
  auto ret = details::FieldAccessor<const std::remove_cv_t<Type>, Transform, Indexer>(
      std::forward<Type>(field), getter, details::RvalueTag{});
  return ret;
}

// 2. For cr-value
template <typename Indexer = details::MultiDimensionGetter<1>,
          typename Transform = details::IdentityTransform, typename Type>
decltype(auto) access(const Type&& field, Indexer getter = Indexer()) {
  auto ret = details::FieldAccessor<Type, Transform, Indexer>(std::forward<decltype(field)>(field),
                                                              getter, details::ConstRvalueTag{});
  return ret;
}

// 3. For cl-value
template <typename Indexer = details::MultiDimensionGetter<1>,
          typename Transform = details::IdentityTransform, typename Type>
decltype(auto) access(const Type& field, Indexer getter = Indexer()) {
  return details::FieldAccessor<const Type&, Transform, Indexer>(field, getter,
                                                                 details::ConstLvalueTag{});
}

// 4. For l-value
template <typename Indexer = details::MultiDimensionGetter<1>,
          typename Transform = details::IdentityTransform, typename Type>
decltype(auto) access(Type& field, Indexer getter = Indexer()) {
  return details::FieldAccessor<Type&, Transform, Indexer>(field, getter, details::LvalueTag{});
}

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
    using difference_type = typename RawType::Index;
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

  inline decltype(auto) Ones() { return Field<Scalar, dim>::Ones(dim, n_); }

  inline decltype(auto) Zeros() { return Field<Scalar, dim>::Zeros(dim, n_); }

  inline decltype(auto) Constant(Scalar s) { return Field<Scalar, dim>::Constant(dim, n_, s); }

  inline decltype(auto) Random() { return Field<Scalar, dim>::Random(dim, n_); }

  inline decltype(auto) FromFunction(std::function<Vec<Scalar, dim>(Index i)> generator) {
    auto ph = Placeholder();
    for (auto [i, b] : FieldEnumerate(ph)) {
      b = generator(i);
    }
    return ph;
  }
};

}  // namespace acg
