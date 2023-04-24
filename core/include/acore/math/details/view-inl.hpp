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

  /**
   * Initializers.
   */

  inline explicit FieldAccessor(origin_type &x, Indexer getter,
                                utils::god::LvalueTag)
      : data_(x), indexer_(getter) {}

  inline explicit FieldAccessor(const origin_type &x, Indexer getter,
                                utils::god::ConstLvalueTag)
      : data_(x), indexer_(getter) {}

  inline explicit FieldAccessor(origin_type &&x, Indexer getter,
                                utils::god::RvalueTag)
      : data_(std::forward<origin_type>(x)), indexer_(getter) {}

  inline explicit FieldAccessor(origin_type x, Indexer getter,
                                utils::god::ConstRvalueTag)
      : data_(std::move(x)), indexer_(getter) {}

  FieldAccessor(const FieldAccessor &) = default;

  FieldAccessor(FieldAccessor &&) = default;

  /****************************************
   * @brief size of field.
   ****************************************/
  inline Index Size() const noexcept { return data_.cols(); }

  /****************************************
   * Access internal data without indexer
   ****************************************/
  inline decltype(auto) operator[](Index i) noexcept {
    if constexpr (Trait<std::decay_t<Type>>::is_vector) {
      return data_(i);
    } else {
      auto transformed = transform_(data_.col(i));
      return transformed;
    }
  }

  inline decltype(auto) operator[](Index i) const noexcept {
    if constexpr (Trait<std::decay_t<Type>>::is_vector) {
      return data_(i);
    } else {
      auto transformed = transform_(data_.col(i));
      return transformed;
    }
  }

  /**
   * @brief Access internal data, using given indexer.
   *
   * @tparam Args
   * @param args
   * @return decltype(auto)
   */
  template <typename... Args>
  inline decltype(auto) operator()(Args... args) const noexcept {
    // Get the physical id
    auto id = indexer_(std::forward<Args>(args)...);
    return operator[](id);
  }

  template <typename... Args>
  inline decltype(auto) operator()(Args... args) noexcept {
    auto id = indexer_(std::forward<Args>(args)...);
    return operator[](id);
  }

  inline decltype(auto) begin() const { return data_.colwise().begin(); }

  inline decltype(auto) end() const { return data_.colwise().end(); }
};

/****************************************
 * NOTE: Enumerator
 ****************************************/
template <typename Access, typename It> struct FieldEnumerateIterator {
  const Access &acc_;
  It it_;

  inline decltype(auto) operator*() const noexcept {
    auto index = *it_;
    auto result = std::apply(acc_, index);
    return std::tuple_cat(index, std::make_tuple(result));
  }

  inline FieldEnumerateIterator &operator++() noexcept {
    ++it_;
    return *this;
  }

  inline bool operator==(const FieldEnumerateIterator &rhs) const noexcept {
    return it_ == rhs.it_;
  }
  inline bool operator!=(const FieldEnumerateIterator &rhs) const noexcept {
    return it_ != rhs.it_;
  }

  explicit FieldEnumerateIterator(const Access &a, It i) : acc_(a), it_(i) {}
};

template <typename Access> class FieldEnumerate {
public:
  Access acc_;

  inline decltype(auto) begin() const noexcept {
    return FieldEnumerateIterator(acc_, acc_.indexer_.Iterate().begin());
  }
  inline decltype(auto) end() const noexcept {
    return FieldEnumerateIterator(acc_, acc_.indexer_.Iterate().end());
  }

  explicit FieldEnumerate(Access a) : acc_(a) {}
};

} // namespace details

} // namespace acg
