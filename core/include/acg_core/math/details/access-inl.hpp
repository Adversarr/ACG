#pragma once
#include <acg_utils/common.hpp>
#include <utility>

#include "acg_core/common.hpp"
#include "acg_utils/common.hpp"
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

// NOTE: standard field Indexer. An indexer can also be iterated.
template <Index dim> class MultiDimensionIndexer;

template <> class MultiDimensionIndexer<0> {
public:
  inline constexpr Index operator()() const { return 0; }

  static constexpr Index this_dim = 0;  // NOLINT
protected:
  static constexpr Index Run(Index result) { return result; }
};

template <> class MultiDimensionIndexer<1> {
public:
  inline Index operator()(Index id) const { return id; }

  explicit constexpr MultiDimensionIndexer(Index this_dim = 0) : this_dim(this_dim) {}

protected:
  constexpr Index Run(Index result, Index this_size) const { return result * this_dim + this_size; }

  const Index this_dim{0};
};

template <Index dim> class MultiDimensionIndexer : public MultiDimensionIndexer<dim - 1> {
public:
  template <typename... Args> constexpr explicit MultiDimensionIndexer(Index this_dim, Args... arg)
      : MultiDimensionIndexer<dim - 1>(std::forward<Args>(arg)...), this_dim(this_dim) {}

  template <typename... Args> constexpr Index operator()(Args... indices) const {
    return Run(0, indices...);
  }

protected:
  template <typename... Args>
  constexpr Index Run(Index result, Index this_size, Args... indices) const {
    return MultiDimensionIndexer<dim - 1>::Run(result * this_dim + this_size, indices...);
  }

  const Index this_dim{0};
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

  inline decltype(auto) operator[](Index i) noexcept { return data.col(i); }

  inline decltype(auto) operator[](Index i) const noexcept { return (*this)(i); }

  inline Index Size() const noexcept { return data.cols(); }

  template <typename... Args> inline decltype(auto) operator()(Args... args) const noexcept {
    return this->operator()(std::forward<Args>(args)...);
  }

  template <typename... Args> inline decltype(auto) operator()(Args... args) noexcept {
    // if constexpr (std::is_same_v<Transform, IdentityTransform>) {
    //   return data.col(indexer(std::forward<Args>(args)...));
    // } else {
    auto transformed = transform(data.col(indexer(std::forward<Args>(args)...)));
    return transformed;
  }

  inline decltype(auto) begin() const { return data.colwise().begin(); }

  inline decltype(auto) end() const { return data.colwise().end(); }
};

}  // namespace details

}  // namespace acg
