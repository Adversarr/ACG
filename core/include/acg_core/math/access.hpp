#pragma once
#include <tuple>

#include "common.hpp"

namespace acg {

namespace details {

constexpr Index Dynamic = Eigen::Dynamic;

constexpr Index Ignore = Eigen::NoChange;

/* standard field getter */
template <Index dim> class SequentialGetter;

template <> class SequentialGetter<0> {
public:
  inline Index operator()() const { return 0; }

  static constexpr Index this_dim = 0;  // NOLINT
protected:
  static inline Index Run(Index result) { return result; }
};

template <> class SequentialGetter<1> {
public:
  Index operator()(Index id) const { return id; }

  explicit SequentialGetter(Index this_dim = 0) : this_dim(this_dim) {}

protected:
  Index Run(Index result, Index this_size) const { return result * this_dim + this_size; }

  Index this_dim{0};
};

template <Index dim> class SequentialGetter : public SequentialGetter<dim - 1> {
public:
  template <typename... Args> explicit SequentialGetter(Index this_dim, Args... arg)
      : SequentialGetter<dim - 1>(std::forward<Args>(arg)...), this_dim(this_dim) {}

  template <typename... Args> Index operator()(Args... indices) const { return Run(0, indices...); }

protected:
  template <typename... Args> Index Run(Index result, Index this_size, Args... indices) const {
    return SequentialGetter<dim - 1>::Run(result * this_dim + this_size, indices...);
  }

  Index this_dim;
};

template <typename Type, typename Indexer = SequentialGetter<1>> struct FieldAccessor {
  using type = Type;
  Type data;
  Indexer getter;

  // inline explicit FieldAccessor(Type x, Indexer getter) : data(x), getter(getter) {}

  inline explicit FieldAccessor(Type&& x, Indexer getter) : data(x), getter(getter) {}

  inline decltype(auto) operator[](Index i) const noexcept { return data.col(i); }
  inline decltype(auto) operator[](Index i) noexcept { return data.col(i); }

  inline Index Size() const noexcept { return data.cols(); }

  template <typename... Args> inline decltype(auto) operator()(Args... args) const noexcept {
    return data.col(getter(std::forward<Args>(args)...));
  }
  template <typename... Args> inline decltype(auto) operator()(Args... args) noexcept {
    return data.col(getter(std::forward<Args>(args)...));
  }

  inline decltype(auto) begin() const { return data.colwise().begin(); }
  inline decltype(auto) end() const { return data.colwise().end(); }
};

}  // namespace details
template <typename Indexer = details::SequentialGetter<1>, typename Scalar, int n_attrib>
decltype(auto) access(Field<Scalar, n_attrib>&& field, Indexer getter = Indexer()) {
  auto ret = details::FieldAccessor<Field<Scalar, n_attrib>, Indexer>((field), getter);
  return ret;
}

template <typename Indexer = details::SequentialGetter<1>, typename Scalar, int n_attrib>
decltype(auto) access(const Field<Scalar, n_attrib>& field, Indexer getter = Indexer()) {
  return details::FieldAccessor<const Field<Scalar, n_attrib>&, Indexer>(field, getter);
}

template <typename Indexer = details::SequentialGetter<1>, typename Scalar, int n_attrib>
decltype(auto) access(Field<Scalar, n_attrib>& field, Indexer getter = Indexer()) {
  return details::FieldAccessor<Field<Scalar, n_attrib>&, Indexer>(field, getter);
}

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

    Index index;
    const RawType& raw_data;

    Iterator(difference_type index, RawType& raw_data) : index(index), raw_data(raw_data) {}

    decltype(auto) operator*() const { return raw_data.col(index); }

    Iterator& operator++() {
      index++;
      return *this;
    }

    bool operator==(const Iterator& rhs) const { return index == rhs.index; }

    bool operator!=(const Iterator& rhs) const { return index != rhs.index; }
  };

  inline auto operator[](Index i) const noexcept { return field_ref_.col(i); }

  inline auto operator()(Index i) const noexcept { return field_ref_.col(i); }

  inline Index Size() const noexcept { return field_ref_.cols(); }

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

    Index index;
    RawType& raw_data;

    Iterator(difference_type index, RawType& raw_data) : index(index), raw_data(raw_data) {}

    decltype(auto) operator*() const { return raw_data.col(index); }

    Iterator& operator++() {
      index++;
      return *this;
    }

    bool operator==(const Iterator& rhs) const { return index == rhs.index; }

    bool operator!=(const Iterator& rhs) const { return index != rhs.index; }
  };

  inline auto operator[](Index i) noexcept { return field_ref_.col(i); }

  inline auto operator()(Index i) noexcept { return field_ref_.col(i); }

  inline Index Size() const noexcept { return field_ref_.cols(); }

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

  inline RawType Placeholder() { return Field<Scalar, dim>(dim, n_); }

  inline auto Ones() { return Field<Scalar, dim>::Ones(dim, n_); }

  inline auto Zeros() { return Field<Scalar, dim>::Zeros(dim, n_); }

  inline auto Constant(Scalar s) { return Field<Scalar, dim>::Constant(s); }

  inline auto Random() { return Field<Scalar, dim>::Random(dim, n_); }

  inline RawType FromFunction(std::function<Vec<Scalar, dim>(Index i)> generator) {
    auto ph = Placeholder();
    for (auto [i, b] : FieldEnumerate(ph)) {
      b = generator(i);
    }
    return ph;
  }
};

}  // namespace acg
