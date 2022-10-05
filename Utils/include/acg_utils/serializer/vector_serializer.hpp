#pragma once
#include "acg_utils/acg_utils.hpp"
#include "serializer_decl.hpp"
#include <vector>

namespace acg::utils::details{
template <typename V> struct Serializer<std::vector<V>> {
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

  std::optional<T> Backward(std::istream &is) const {
    CHECK_ELSE_RETURN_NULLOPT(consume_until(is, '['));
    T retval;
    auto current_ch = extract_current(is);
    CHECK_ELSE_RETURN_NULLOPT(current_ch.has_value());
    while (*current_ch != ']') {
      std::optional<V> item = deserialize<V>(is);
      // cannot deserialize
      CHECK_ELSE_RETURN_NULLOPT(item.has_value());
      if constexpr (std::is_move_assignable_v<V> &&
                    std::is_move_constructible_v<V>) {
        retval.emplace_back(std::move(*item));
      } else {
        retval.emplace_back(*item);
      }
      current_ch = extract_current(is);
      CHECK_ELSE_RETURN_NULLOPT(current_ch.has_value());
      if (*current_ch == ']') {
        break;
      } else {
        CHECK_ELSE_RETURN_NULLOPT(consume_until(is, ','));
        CHECK_ELSE_RETURN_NULLOPT(consume_blank(is));
      }
    }
    CHECK_ELSE_RETURN_NULLOPT(consume_until(is, ']'));
    return retval;
  }
};
}