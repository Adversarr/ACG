/**
 * @file ASerializer.h
 * @author Zherui Yang (yangzherui2001@foxmail.com)
 * @brief JSON-based Object Serializer.
 * @version 0.0.1
 * @date 2022-08-28
 *
 * @copyright Copyright (c) 2022
 *
 */

#pragma once
#include <array>
#include <cctype>
#include <cstdio>
#include <deque>
#include <forward_list>
#include <istream>
#include <list>
#include <map>
#include <ostream>
#include <set>
#include <sstream>
#include <streambuf>
#include <string>
#include <string_view>
#include <type_traits>
#include <utility>
#include <vector>

#include "Utilities.h"

namespace AUtilities {
namespace details {

inline char extract_current(std::istream &is) {
  char c;
  is.get(c);
  is.putback(c);
  return c;
}

inline void consume_until(std::istream &is, char c) {
  char current_byte;
  is.get(current_byte);
  while (is.good() && !is.eof() && current_byte != c) {
    is.get(current_byte);
  }
}

inline void consume_blank(std::istream &is) {
  char current_byte;
  is.get(current_byte);
  while (is.good() && !is.eof() && std::isblank(current_byte)) {
    is.get(current_byte);
  }
  is.putback(current_byte);
}

template<typename T>
struct Serializer {
  void Forward(const T &, std::ostream &os) const noexcept;
  T Backward(std::istream &is) const noexcept;
};

}// namespace details

template<typename T, typename = typename T::_refl>
inline std::string serialize(const T &o) noexcept {
  return T::_refl::Serialize(o);
}

template<typename T>
inline std::enable_if_t<
  std::is_same<T, typename details::Serializer<T>::T>::value,
  std::string>
serialize(const T &o) noexcept {
  std::ostringstream output_stream;
  details::Serializer<T>{}.Forward(o, output_stream);
  return output_stream.str();
}

template<typename T, typename = typename T::_refl>
inline T deserialize(std::istream &is) noexcept {
  return T::_refl::Deserialize(is);
}

template<typename T, typename = typename T::_refl>
inline T deserialize(const std::string &s) noexcept {
  return T::_refl::Deserialize(s);
}

template<typename T>
inline std::enable_if_t<std::is_same<T, typename details::Serializer<T>::T>::value, T> deserialize(std::istream &is) noexcept {
  return details::Serializer<T>{}.Backward(is);
}

template<typename T>
inline std::enable_if_t<std::is_same<T, typename details::Serializer<T>::T>::value, T> deserialize(const std::string &s) noexcept {
  std::istringstream ss{s};
  return details::Serializer<T>{}.Backward(ss);
}

namespace details {

/* --- For Numeric Types. --- */
template<>
struct Serializer<int> {
  using T = int;
  std::ostream &Forward(const T &v, std::ostream &os) const noexcept {
    return (os << std::to_string(v));
  }

  T Backward(std::istream &is) const noexcept {
    consume_blank(is);
    T x;
    is >> x;
    return x;
  }
};

template<>
struct Serializer<long> {
  using T = long;
  std::ostream &Forward(const long &v, std::ostream &os) const noexcept {
    return (os << std::to_string(v));
  }
  T Backward(std::istream &is) const noexcept {
    consume_blank(is);
    T x;
    is >> x;
    return x;
  }
};

template<>
struct Serializer<long long> {
  using T = long long;
  std::ostream &Forward(const long long &v, std::ostream &os) const noexcept {
    return (os << std::to_string(v));
  }
  T Backward(std::istream &is) const noexcept {
    consume_blank(is);
    T x;
    is >> x;
    assert(!is.fail());
    return x;
  }
};

template<>
struct Serializer<unsigned> {
  using T = unsigned;
  std::ostream &Forward(const unsigned &v, std::ostream &os) const noexcept {
    return (os << std::to_string(v));
  }
  T Backward(std::istream &is) const noexcept {
    consume_blank(is);
    T x;
    is >> x;
    assert(!is.fail());
    return x;
  }
};

template<>
struct Serializer<unsigned long> {
  using T = unsigned long;
  std::ostream &Forward(const unsigned long &v,
                        std::ostream &os) const noexcept {
    return (os << std::to_string(v));
  }
  T Backward(std::istream &is) const noexcept {
    consume_blank(is);
    T x;
    is >> x;
    assert(!is.fail());
    return x;
  }
};

template<>
struct Serializer<unsigned long long> {
  using T = unsigned long long;
  std::ostream &Forward(const unsigned long long &v,
                        std::ostream &os) const noexcept {
    return (os << std::to_string(v));
  }
  T Backward(std::istream &is) const noexcept {
    consume_blank(is);
    T x;
    is >> x;
    assert(!is.fail());
    return x;
  }
};

template<>
struct Serializer<float> {
  using T = float;
  std::ostream &Forward(const float &v, std::ostream &os) const noexcept {
    return (os << std::to_string(v));
  }

  T Backward(std::istream &is) const noexcept {
    consume_blank(is);
    T x;
    is >> x;
    assert(!is.fail());
    return x;
  }
};

template<>
struct Serializer<double> {
  using T = double;
  std::ostream &Forward(const double &v, std::ostream &os) const noexcept {
    return (os << std::to_string(v));
  }

  T Backward(std::istream &is) const noexcept {
    consume_blank(is);
    T x;
    is >> x;
    assert(!is.fail());
    return x;
  }
};

template<>
struct Serializer<long double> {
  using T = long double;
  std::ostream &Forward(const long double &v, std::ostream &os) const noexcept {
    return (os << std::to_string(v));
  }

  T Backward(std::istream &is) const noexcept {
    consume_blank(is);
    T x;
    is >> x;
    assert(!is.fail());
    return x;
  }
};
/* --- For String. --- */
template<>
struct Serializer<std::string> {

  static inline std::size_t replace_all(std::string &inout,
                                        std::string_view what,
                                        std::string_view with) noexcept {
    std::size_t count{};
    for (std::string::size_type pos{};
         std::string::npos != (pos = inout.find(what.data(), pos, what.length()));
         pos += with.length(), ++count) {
      inout.replace(pos, what.length(), with.data(), with.length());
    }
    return count;
  }

  using T = std::string;
  std::ostream &Forward(const std::string &v, std::ostream &os) const noexcept {
    if (v.find('"') != std::string::npos) {
      auto backup = v;
      replace_all(backup, "\"", "\\\"");
      return os << '"' << backup << '"';
    }
    return (os << '"' << v << '"');
  }

  T Backward(std::istream &is) const {
    consume_until(is, '"');
    if (!is.good()) {
      RTE("cannot deserialize.");
    }
    char buffer;
    is.get(buffer);
    char last;
    std::ostringstream oss;
    do {
      oss << buffer;
      last = buffer;
      is.get(buffer);
    } while (!(last != '\\' && buffer == '"') && is.good() && !is.eof());
    if (is.bad()) {
      RTE("cannot deserialize.");
    }
    auto output = oss.str();
    replace_all(output, "\\\"", "\"");
    return output;
  }
};

template<>
struct Serializer<std::string_view> {
  using T = std::string_view;
  std::ostream &Forward(const T &v, std::ostream &os) const noexcept {
    return Serializer<std::string>{}.Forward(std::string(v), os);
  }
};

template<int N>
struct Serializer<char[N]> {
  using T = char[N];
  std::ostream &Forward(const char v[N], std::ostream &os) const noexcept {
    return Serializer<std::string>{}.Forward(std::string(std::string_view{v}),
                                             os);
  }
};

/* --- For Containers --- */
template<typename V>
struct Serializer<std::vector<V>> {
  using T = std::vector<V>;
  std::ostream &Forward(const std::vector<V> &v,
                        std::ostream &os) const noexcept {
    if (v.empty()) {
      os << "[]";
    } else {
      auto &o = os;
      o << "[";
      auto it = v.cbegin();
      while (it != v.end()) {
        Serializer<V>{}.Forward(*it, o);
        ++it;
        if (it != v.end()) {
          o << ", ";
        }
      }
      o << "]";
    }
    return os;
  }

  T Backward(std::istream &is) const {
    consume_until(is, '[');
    T retval;
    while (extract_current(is) != ']') {
      retval.emplace_back(Serializer<V>{}.Backward(is));
      if (extract_current(is) == ']') {
        break;
      } else {
        consume_until(is, ',');
        consume_blank(is);
      }
    }
    consume_until(is, ']');
    return retval;
  }
};

template<typename V, int N>
struct Serializer<std::array<V, N>> {

  using T = std::array<V, N>;
  std::ostream &Forward(const std::array<V, N> &v,
                        std::ostream &os) const noexcept {
    if (v.empty()) {
      os << "[]";
    } else {
      auto &o = os;
      o << "[";
      auto it = v.cbegin();
      while (it != v.end()) {
        Serializer<V>{}.Forward(*it, o);
        ++it;
        if (it != v.end()) {
          o << ", ";
        }
      }
      o << "]";
    }
    return os;
  }

  T Backward(std::istream &is) const {
    consume_until(is, '[');
    T retval;
    while (extract_current(is) != ']') {
      retval.emplace_back(Serializer<V>{}.Backward(is));
      if (extract_current(is) == ']') {
        break;
      } else {
        consume_until(is, ',');
        consume_blank(is);
      }
    }
    consume_until(is, ']');
    return retval;
  }
};

template<typename V>
struct Serializer<std::deque<V>> {
  using T = std::deque<V>;
  std::ostream &Forward(const std::deque<V> &v,
                        std::ostream &os) const noexcept {
    if (v.empty()) {
      os << "[]";
    } else {
      auto &o = os;
      o << "[";
      auto it = v.cbegin();
      while (it != v.end()) {
        Serializer<V>{}.Forward(*it, o);
        ++it;
        if (it != v.end()) {
          o << ", ";
        }
      }
      o << "]";
    }
    return os;
  }
  T Backward(std::istream &is) const {
    consume_until(is, '[');
    T retval;
    while (extract_current(is) != ']') {
      retval.emplace_back(Serializer<V>{}.Backward(is));
      if (extract_current(is) == ']') {
        break;
      } else {
        consume_until(is, ',');
        consume_blank(is);
      }
    }
    consume_until(is, ']');
    return retval;
  }
};

template<typename V>
struct Serializer<std::list<V>> {
  using T = std::list<V>;
  std::ostream &Forward(const std::list<V> &v,
                        std::ostream &os) const noexcept {
    if (v.empty()) {
      os << "[]";
    } else {
      auto &o = os;
      o << "[";
      auto it = v.cbegin();
      while (it != v.end()) {
        Serializer<V>{}.Forward(*it, o);
        ++it;
        if (it != v.end()) {
          o << ", ";
        }
      }
      o << "]";
    }
    return os;
  }
  T Backward(std::istream &is) const {
    consume_until(is, '[');
    T retval;
    while (extract_current(is) != ']') {
      retval.emplace_back(Serializer<V>{}.Backward(is));
      if (extract_current(is) == ']') {
        break;
      } else {
        consume_until(is, ',');
        consume_blank(is);
      }
    }
    consume_until(is, ']');
    return retval;
  }
};

/* --- map --- */

template<typename K, typename V>
struct Serializer<std::map<K, V>> {
  using T = std::map<K, V>;
  std::ostream &Forward(const std::map<K, V> &v,
                        std::ostream &o) const noexcept {
    if (v.empty()) {
      o << "{}";
    } else {
      o << "{";
      auto it = v.cbegin();
      while (it != v.end()) {
        Serializer<K>{}.Forward(it->first, o);
        o << ": ";
        Serializer<V>{}.Forward(it->second, o);
        ++it;
        if (it != v.end()) {
          o << ", ";
        }
      }
      o << "}";
    }
    return o;
  }

  T Backward(std::istream &is) const {
    T retval;
    consume_blank(is);
    consume_until(is, '{');
    while (extract_current(is) != '}') {
      // key:
      K k = Serializer<K>{}.Backward(is);
      consume_until(is, ':');
      consume_blank(is);
      V v = Serializer<V>{}.Backward(is);
      retval.emplace(k, v);
      consume_blank(is);
      if (extract_current(is) == ',') {
        consume_until(is, ',');
      }
    }
    return retval;
  }
};

template<typename K>
struct Serializer<std::set<K>> {
  using T = std::set<K>;
  std::ostream &Forward(const std::set<K> &v, std::ostream &o) const noexcept {
    if (v.empty()) {
      o << "{}";
    } else {
      o << "{";
      auto it = v.cbegin();
      while (it != v.end()) {
        Serializer<K>{}.Forward(*it, o);
        ++it;
        if (it != v.end()) {
          o << ", ";
        }
      }
      o << "}";
    }
    return o;
  }
};

}// namespace details

// TODO: Last Edit.

}// namespace AUtilities

#define PARENS ()

#define EXPAND(...) __VA_OPT__(EXPAND4(EXPAND4(EXPAND4(EXPAND4(__VA_ARGS__)))))
#define EXPAND4(...) __VA_OPT__(EXPAND3(EXPAND3(EXPAND3(EXPAND3(__VA_ARGS__)))))
#define EXPAND3(...) __VA_OPT__(EXPAND2(EXPAND2(EXPAND2(EXPAND2(__VA_ARGS__)))))
#define EXPAND2(...) __VA_OPT__(EXPAND1(EXPAND1(EXPAND1(EXPAND1(__VA_ARGS__)))))
#define EXPAND1(...) __VA_ARGS__
#define FOR_EACH(macro, mid, ...) __VA_OPT__(EXPAND(FOR_EACH_HELPER(macro, mid, __VA_ARGS__)))
#define FOR_EACH_HELPER(macro, mid, a1, ...) macro(a1) __VA_OPT__(mid FOR_EACH_AGAIN PARENS(macro, mid, __VA_ARGS__))
#define FOR_EACH_AGAIN() FOR_EACH_HELPER
#define EXTRACT_TYPE(m) using __tp_##m = decltype(m)
#define EXTRACT_TYPE_ALL(...) FOR_EACH(EXTRACT_TYPE, ;, __VA_ARGS__)

#define SERIALIZE_MEMBER(m) o << "\"" #m "\": " << AUtilities::serialize(ref.m)

#define GENERATE_SERIALIZE(...)                                       \
  static inline std::string Serialize(const __base_t &ref) noexcept { \
    std::ostringstream o;                                             \
    o << "{";                                                         \
    FOR_EACH(SERIALIZE_MEMBER, ; o << ", ";, __VA_ARGS__);            \
    o << "}";                                                         \
    return o.str();                                                   \
  }

#define DESERIALIZE_MEMBER(m)                                            \
  if (k == #m) {                                                         \
    object.m = AUtilities::details::Serializer<__tp_##m>{}.Backward(is); \
    return;

#define GENERATE_STOREATTR(...) static inline void StoreAttribute(__base_t& object, \
          const std::string& k, std::istringstream& is) {                           \
  FOR_EACH(DESERIALIZE_MEMBER, }, __VA_ARGS__)                                      \
  }                                                                                 \
  RTE("Cannot Deserialize...");                                                     \
  }

#define ENABLE_SERIALIZE_FOR(base, ...)                                       \
 public:                                                                      \
  struct _refl {                                                              \
    using __base_t = base;                                                    \
    EXTRACT_TYPE_ALL(__VA_ARGS__);                                            \
    GENERATE_SERIALIZE(__VA_ARGS__)                                           \
    GENERATE_STOREATTR(__VA_ARGS__)                                           \
    static inline __base_t Deserialize(const std::string &s) noexcept {       \
      std::istringstream ss{s};                                               \
      return Deserialize(ss);                                                 \
    }                                                                         \
    static inline __base_t Deserialize(std::istringstream &ss) noexcept {     \
      __base_t ret;                                                           \
      AUtilities::details::consume_blank(ss);                                 \
      AUtilities::details::consume_until(ss, '{');                            \
      AUtilities::details::consume_blank(ss);                                 \
      while (AUtilities::details::extract_current(ss) != '}') {               \
        auto k = AUtilities::details::Serializer<std::string>{}.Backward(ss); \
        AUtilities::details::consume_blank(ss);                               \
        AUtilities::details::consume_until(ss, ':');                          \
        StoreAttribute(ret, k, ss);                                           \
        AUtilities::details::consume_blank(ss);                               \
        if (AUtilities::details::extract_current(ss) == '}') {                \
          break;                                                              \
        }                                                                     \
      }                                                                       \
      return ret;                                                             \
    }                                                                         \
  }
