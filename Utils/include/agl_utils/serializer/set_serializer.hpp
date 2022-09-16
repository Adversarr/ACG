#pragma once

#include "serializer_decl.hpp"

#include <set>

namespace agl::utils::details {
template <typename K> struct Serializer<std::set<K>> {
  using T = std::set<K>;
  std::ostream &Forward(const std::set<K> &v, std::ostream &o) const noexcept {
    if (v.empty()) {
      o << "[]";
    } else {
      o << "[";
      auto it = v.cbegin();
      while (it != v.end()) {
        Serializer<K>{}.Forward(*it, o);
        ++it;
        if (it != v.end()) {
          o << ", ";
        }
      }
      o << "]";
    }
    return o;
  }
  std::optional<T> Backward(std::istream &is) const {
    consume_until(is, '[');
    T retval;
    while (extract_current(is) != ']') {
      std::optional<K> item = deserialize<K>(is);
      // cannot deserialize
      CHECK_ELSE_RETURN_NULLOPT(item.has_value());
      if constexpr (std::is_move_assignable_v<K> &&
                    std::is_move_constructible_v<K>) {
        retval.emplace(std::move(*item));
      } else {
        retval.emplace(*item);
      }
      auto current_ch = extract_current(is);
      CHECK_ELSE_RETURN_NULLOPT(current_ch.has_value());
      if (*current_ch == ']') {
        break;
      } else {
        CHECK_ELSE_RETURN_NULLOPT(consume_until(is, ','));
        CHECK_ELSE_RETURN_NULLOPT(consume_blank(is));
      }
    }
    consume_until(is, ']');
    return retval;
  }
};
}