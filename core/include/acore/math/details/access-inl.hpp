#pragma once
#include <acore/math/traits.hpp>
#include <autils/common.hpp>
#include <tuple>
#include <utility>

#include "acore/common.hpp"
#include "autils/god/utilities.hpp"

namespace acg {

namespace details {

// NOTE: Standard Field Accessor.
template <typename Type, typename Transform, typename Indexer>
struct FieldAccessor {
  using type = Type;
  using origin_type = std::decay_t<Type>;

  Type data_;
  Indexer indexer_;
  Transform transform_;

  inline explicit FieldAccessor(origin_type& x, Indexer getter,
                                utils::god::LvalueTag)
      : data_(x), indexer_(getter) {}

  inline explicit FieldAccessor(const origin_type& x, Indexer getter,
                                utils::god::ConstLvalueTag)
      : data_(x), indexer_(getter) {}

  inline explicit FieldAccessor(origin_type&& x, Indexer getter,
                                utils::god::RvalueTag)
      : data_(std::forward<origin_type>(x)), indexer_(getter) {}

  inline explicit FieldAccessor(const origin_type&& x, Indexer getter,
                                utils::god::ConstRvalueTag)
      : data_(x), indexer_(getter) {}

  FieldAccessor(const FieldAccessor&) = default;

  FieldAccessor(FieldAccessor&&) = default;

  /****************************************
   * @brief size of field.
   ****************************************/
  inline Index Size() const noexcept { return data_.cols(); }

  /****************************************
   * Access internal data without indexer
   ****************************************/
  inline decltype(auto) operator[](Index i) noexcept { return data_.col(i); }

  inline decltype(auto) operator[](Index i) const noexcept {
    return (*this)(i);
  }

  template <typename... Args>
  inline decltype(auto) operator()(Args... args) const noexcept {
    if constexpr (details::Trait<origin_type>::rows == 1) {
      return data_(indexer_(std::forward<Args>(args)...));
    } else {
      auto transformed
          = transform_(data_.col(indexer_(std::forward<Args>(args)...)));
      return transformed;
    }
  }

  template <typename... Args>
  inline decltype(auto) operator()(Args... args) noexcept {
    if constexpr (details::Trait<origin_type>::rows == 1) {
      return data_(indexer_(std::forward<Args>(args)...));
    } else {
      auto transformed
          = transform_(data_.col(indexer_(std::forward<Args>(args)...)));
      return transformed;
    }
  }

  inline decltype(auto) begin() const { return data_.colwise().begin(); }

  inline decltype(auto) end() const { return data_.colwise().end(); }
};

/****************************************
 * NOTE: Enumerator
 *
 ****************************************/
template <typename Access, typename It> struct Iterator {
  const Access& acc_;
  It it_;

  inline decltype(auto) operator*() const noexcept {
    auto index = *it_;
    auto result = std::apply(acc_, index);
    return std::tuple_cat(index, std::make_tuple(result));
  }

  inline Iterator& operator++() noexcept {
    ++it_;
    return *this;
  }

  inline bool operator==(const Iterator& rhs) const noexcept {
    return it_ == rhs.it_;
  }
  inline bool operator!=(const Iterator& rhs) const noexcept {
    return it_ != rhs.it_;
  }

  explicit Iterator(const Access& a, It i) : acc_(a), it_(i) {}
};
template <typename Access> class FieldEnumerate {
public:
  Access acc_;

  inline decltype(auto) begin() const noexcept {
    return Iterator(acc_, acc_.indexer_.Iterate().begin());
  }
  inline decltype(auto) end() const noexcept {
    return Iterator(acc_, acc_.indexer_.Iterate().end());
  }

  explicit FieldEnumerate(Access a) : acc_(a) {}
};

}  // namespace details

}  // namespace acg
