#pragma once
#include <autils/common.hpp>
#include <utility>

#include "acore/common.hpp"
#include "autils/common.hpp"
#include "autils/god/utilities.hpp"
#include "autils/meta.hpp"

namespace acg {

namespace details {

// Pre transform to field accessor.
struct IdentityTransform {
  template <typename T> inline decltype(auto) operator()(T&& in) const noexcept {
    return std::forward<T>(in);
  }
};

template <int rows, int cols> struct ReshapeTransform {
  template <typename T> inline decltype(auto) operator()(T&& in) const noexcept {
    return in.reshaped(rows, cols);
  }
};


/****************************************
 * NOTE: Foreach Indexer, it have:
 *  1. value_type: indicates the return value type
 *
 ****************************************/

// NOTE: standard field Indexer. An indexer can also be iterated.
template <Index dim> class NdRangeIndexer;

template <> class NdRangeIndexer<0> {
public:
  inline constexpr Index operator()() const { return 0; }

  static constexpr Index this_dim = 0;  // NOLINT
};

template <> class NdRangeIndexer<1> {
public:
  inline Index operator()(Index id) const { return id; }

  explicit constexpr NdRangeIndexer(Index this_dim = 0) : this_dim(this_dim) {}

protected:
  const Index this_dim{0};
};

template <Index dim> class NdRangeIndexer : public NdRangeIndexer<dim - 1> {
public:
  template <typename... Args> constexpr explicit NdRangeIndexer(Index this_dim, Args... arg)
      : NdRangeIndexer<dim - 1>(arg...),
        this_dim(this_dim),
        multiplier(acg::utils::product(static_cast<Index>(arg)...)) {}

  template <typename... Args>
  constexpr Index operator()(Index this_size, Args... indices) const noexcept {
    return this_size * multiplier
           + NdRangeIndexer<dim - 1>::operator()(static_cast<Index>(indices)...);
  }

protected:
  const Index this_dim{0};
  const Index multiplier{0};
};


// NOTE: Standard Field Accessor.
template <typename Type, typename Transform, typename Indexer> struct FieldAccessor {
  using type = Type;
  using origin_type = std::remove_cv_t<Type>;

  Type data;
  Indexer indexer;
  Transform transform;

  inline explicit FieldAccessor(origin_type& x, Indexer getter, utils::LvalueTag)
      : data(x), indexer(getter) {}

  inline explicit FieldAccessor(const origin_type& x, Indexer getter, utils::ConstLvalueTag)
      : data(x), indexer(getter) {}

  inline explicit FieldAccessor(origin_type&& x, Indexer getter, utils::RvalueTag)
      : data(std::forward<origin_type>(x)), indexer(getter) {}

  inline explicit FieldAccessor(const origin_type&& x, Indexer getter, utils::ConstRvalueTag)
      : data(x), indexer(getter) {}

  FieldAccessor(const FieldAccessor&) = delete;

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
    return this->operator()(std::forward<Args>(args)...);
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
 ****************************************/
template <typename DataType, typename IndexIterator>
class FieldEnumerate {
  using index_type = typename IndexIterator::value_type;
public:

};


}  // namespace details

}  // namespace acg
