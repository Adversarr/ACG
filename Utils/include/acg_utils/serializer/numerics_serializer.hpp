#pragma once
#include "serializer_decl.hpp"

namespace acg::utils::details {
/* --- For Numeric Types. --- */
template <> struct Serializer<int> {
  using T = int;
  std::ostream &Forward(const T &v, std::ostream &os) const noexcept {
    return (os << std::to_string(v));
  }

  std::optional<T> Backward(std::istream &is) const noexcept {
    CHECK_ELSE_RETURN_NULLOPT(consume_blank(is));
    T x;
    is >> x;

    CHECK_ELSE_RETURN_NULLOPT(is.good() || is.eof());
    return {x};
  }
};

template <> struct Serializer<long> {
  using T = long;
  std::ostream &Forward(const long &v, std::ostream &os) const noexcept {
    return (os << std::to_string(v));
  }

  std::optional<T> Backward(std::istream &is) const noexcept {
    CHECK_ELSE_RETURN_NULLOPT(consume_blank(is));
    T x;
    is >> x;

    CHECK_ELSE_RETURN_NULLOPT(is.good() || is.eof());
    return {x};
  }
};

template <> struct Serializer<long long> {
  using T = long long;
  std::ostream &Forward(const long long &v, std::ostream &os) const noexcept {
    return (os << std::to_string(v));
  }

  std::optional<T> Backward(std::istream &is) const noexcept {
    CHECK_ELSE_RETURN_NULLOPT(consume_blank(is));
    T x;
    is >> x;

    CHECK_ELSE_RETURN_NULLOPT(is.good() || is.eof());
    return {x};
  }
};

template <> struct Serializer<unsigned> {
  using T = unsigned;
  std::ostream &Forward(const unsigned &v, std::ostream &os) const noexcept {
    return (os << std::to_string(v));
  }

  std::optional<T> Backward(std::istream &is) const noexcept {
    CHECK_ELSE_RETURN_NULLOPT(consume_blank(is));
    T x;
    is >> x;

    CHECK_ELSE_RETURN_NULLOPT(is.good() || is.eof());
    return {x};
  }
};

template <> struct Serializer<unsigned long> {
  using T = unsigned long;
  std::ostream &Forward(const unsigned long &v,
                        std::ostream &os) const noexcept {
    return (os << std::to_string(v));
  }

  std::optional<T> Backward(std::istream &is) const noexcept {
    CHECK_ELSE_RETURN_NULLOPT(consume_blank(is));
    T x;
    is >> x;

    CHECK_ELSE_RETURN_NULLOPT(is.good() || is.eof());
    return {x};
  }
};

template <> struct Serializer<unsigned long long> {
  using T = unsigned long long;
  std::ostream &Forward(const unsigned long long &v,
                        std::ostream &os) const noexcept {
    return (os << std::to_string(v));
  }

  std::optional<T> Backward(std::istream &is) const noexcept {
    CHECK_ELSE_RETURN_NULLOPT(consume_blank(is));
    T x;
    is >> x;

    CHECK_ELSE_RETURN_NULLOPT(is.good() || is.eof());
    return {x};
  }
};

template <> struct Serializer<float> {
  using T = float;
  std::ostream &Forward(const float &v, std::ostream &os) const noexcept {
    return (os << std::to_string(v));
  }

  std::optional<T> Backward(std::istream &is) const noexcept {
    CHECK_ELSE_RETURN_NULLOPT(consume_blank(is));
    T x;
    is >> x;

    CHECK_ELSE_RETURN_NULLOPT(is.good() || is.eof());
    return {x};
  }
};

template <> struct Serializer<double> {
  using T = double;
  std::ostream &Forward(const double &v, std::ostream &os) const noexcept {
    return (os << std::to_string(v));
  }
  std::optional<T> Backward(std::istream &is) const noexcept {
    CHECK_ELSE_RETURN_NULLOPT(consume_blank(is));
    T x;
    is >> x;

    CHECK_ELSE_RETURN_NULLOPT(is.good() || is.eof());
    return {x};
  }
};

template <> struct Serializer<long double> {
  using T = long double;
  std::ostream &Forward(const long double &v, std::ostream &os) const noexcept {
    return (os << std::to_string(v));
  }

  std::optional<T> Backward(std::istream &is) const noexcept {
    CHECK_ELSE_RETURN_NULLOPT(consume_blank(is));
    T x;
    is >> x;

    CHECK_ELSE_RETURN_NULLOPT(is.good() || is.eof());
    return {x};
  }
};
} // namespace acg::utils::details