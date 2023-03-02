#include <iterator>
#include <tuple>
#include <array>

struct Iter2D {
  constexpr Iter2D(int n, int m, int i, int j) : ij({i, j}), n(n), m(m) {}

  constexpr decltype(auto) operator*() const { return ij; }

  constexpr decltype(auto) operator->() const { return &ij; }

  constexpr bool operator==(const Iter2D& another) const { return ij == another.ij; }

  constexpr bool operator!=(const Iter2D& another) const { return ij != another.ij; }

  Iter2D& operator++() {
    auto& i = std::get<0>(ij);
    auto& j = std::get<1>(ij);
    j += 1;
    // This cost lots of time...
    if (j >= m) [[unlikely]] {
      j = 0;
      i += 1;
    }
    return *this;
  }

  Iter2D operator++(int) {
    auto clone = *this;
    auto& i = std::get<0>(ij);
    auto& j = std::get<1>(ij);
    j += 1;
    if (j == m) [[unlikely]] {
      j = 0;
      i += 1;
    }
    return clone;
  }

  using reference = std::tuple<int, int>&;
  using value_type = std::tuple<int, int>;
  using difference_type = int;
  using pointer = std::tuple<int, int>*;
  using iterator_category = std::random_access_iterator_tag;

  // std::array<int, 2> ij;
  std::tuple<int, int> ij;
  const int n;
  const int m;
};

struct Iter2DWrapper {
  Iter2D begin() const { return Iter2D(n, m, 0, 0); }

  Iter2D end() const { return Iter2D(n, m, n, 0); }

  int n, m;
  constexpr Iter2DWrapper(int n, int m) : n(n), m(m) {}
};

struct Iter1D {
  Iter1D& operator++() {
    i++;
    return *this;
  }

  constexpr bool operator!=(const Iter1D& another) const { return i != another.i; }

  int operator*() const { return i; }

  explicit Iter1D(int i) : i(i) {}

  int i = 0;
};

template <typename S, typename Si, typename Ti> struct CrossProductIterator {
  S itgen;
  Si it1;
  Ti it2;
};

struct Iter1DWrapper {
  Iter1D begin() const { return Iter1D(0); }

  Iter1D end() const { return Iter1D(n); }

  Iter1DWrapper(int n) : n(n) {}

  int n;
};
