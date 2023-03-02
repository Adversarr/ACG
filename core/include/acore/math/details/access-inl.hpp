#pragma once
#include <autils/common.hpp>
#include <tuple>
#include <utility>

#include "acore/common.hpp"
#include "autils/common.hpp"
#include "autils/god/utilities.hpp"

namespace acg {

namespace details {

// NOTE: Standard Field Accessor.
template <typename Type, typename Transform, typename Indexer> struct FieldAccessor {
  using type = Type;
  using origin_type = std::remove_cv_t<Type>;

  Type data;
  Indexer indexer;
  Transform transform;

  inline explicit FieldAccessor(origin_type& x, Indexer getter, utils::god::LvalueTag)
      : data(x), indexer(getter) {}

  inline explicit FieldAccessor(const origin_type& x, Indexer getter, utils::god::ConstLvalueTag)
      : data(x), indexer(getter) {}

  inline explicit FieldAccessor(origin_type&& x, Indexer getter, utils::god::RvalueTag)
      : data(std::forward<origin_type>(x)), indexer(getter) {}

  inline explicit FieldAccessor(const origin_type&& x, Indexer getter, utils::god::ConstRvalueTag)
      : data(x), indexer(getter) {}

  FieldAccessor(const FieldAccessor&) = default;

  FieldAccessor(FieldAccessor&&) = default;

  /****************************************
   * @brief size of field.
   ****************************************/
  inline Index Size() const noexcept { return data.cols(); }

  /****************************************
   * Access internal data without indexer
   ****************************************/
  inline decltype(auto) operator[](Index i) noexcept { return data.col(i); }

  inline decltype(auto) operator[](Index i) const noexcept { return (*this)(i); }

  template <typename... Args> inline decltype(auto) operator()(Args... args) const noexcept {
    auto transformed = transform(data.col(indexer(std::forward<Args>(args)...)));
    return transformed;
  }

  template <typename... Args> inline decltype(auto) operator()(Args... args) noexcept {
    auto transformed = transform(data.col(indexer(std::forward<Args>(args)...)));
    return transformed;
  }

  inline decltype(auto) begin() const { return data.colwise().begin(); }

  inline decltype(auto) end() const { return data.colwise().end(); }
};

/****************************************
 * NOTE: Enumerator
 *
 ****************************************/
template <typename Access, typename It> struct Iterator {
  const Access& acc;
  It it;

  inline decltype(auto) operator*() const noexcept {
    auto index = *it;
    auto result = std::apply(acc, index);
    return std::tuple_cat(index, std::make_tuple(result));
  }

  inline Iterator& operator++() noexcept {
    ++it;
    return *this;
  }

  inline bool operator==(const Iterator& rhs) const noexcept { return it == rhs.it; }
  inline bool operator!=(const Iterator& rhs) const noexcept { return it != rhs.it; }

  explicit Iterator(const Access& a, It i) : acc(a), it(i) {}
};
template <typename Access> class FieldEnumerate {
public:
  Access acc_;

  inline decltype(auto) begin() const noexcept {
    return Iterator(acc_, acc_.indexer.Iterate().begin());
  }
  inline decltype(auto) end() const noexcept {
    return Iterator(acc_, acc_.indexer.Iterate().end());
  }

  explicit FieldEnumerate(Access a) : acc_(a) {}
};

}  // namespace details

}  // namespace acg
