#pragma once

#include <map>
#include <optional>
#include "serializer_decl.hpp"

namespace agl::utils::details {

/* --- map --- */

template <typename K, typename V> struct Serializer<std::map<K, V>> {
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

  std::optional<T> Backward(std::istream &is) const {
    T retval;
    CHECK_ELSE_RETURN_NULLOPT(consume_blank(is));
    CHECK_ELSE_RETURN_NULLOPT(consume_until(is, '{'));
    auto current_ch = extract_current(is);
    CHECK_ELSE_RETURN_NULLOPT(current_ch.has_value());
    while (*current_ch != '}' && is.good() && !is.eof()) {
      // key:
      std::optional<K> k = deserialize<K>(is);
      CHECK_ELSE_RETURN_NULLOPT(k.has_value());
      // "k : v"
      CHECK_ELSE_RETURN_NULLOPT(consume_until(is, ':'));
      CHECK_ELSE_RETURN_NULLOPT(consume_blank(is));
      // value:
      std::optional<V> v = deserialize<V>(is);
      CHECK_ELSE_RETURN_NULLOPT(v);

      // I hope the compiler can optimize this...
      retval.emplace(*k, *v);
      CHECK_ELSE_RETURN_NULLOPT(consume_blank(is));
      // Extract current char.
      current_ch = extract_current(is);
      CHECK_ELSE_RETURN_NULLOPT(current_ch.has_value());
      if (*current_ch == ',') {
        CHECK_ELSE_RETURN_NULLOPT(consume_until(is, ','));
      } else if (*current_ch == '}') { 
        // end of map, just exit.
        break;
      } else {
        // Opps, illegal.
        return std::nullopt;
      }
    }
    return retval;
  }
};

}
