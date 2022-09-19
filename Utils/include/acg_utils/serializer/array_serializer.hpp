#include <array>
#include <optional>
#include <type_traits>
#include "serializer_decl.hpp"

namespace acg::utils::details {

template <typename V, int N> struct Serializer<std::array<V, N>> {

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

  std::optional<T> Backward(std::istream &is) const {
    consume_until(is, '[');
    T retval;
    while (extract_current(is) != ']') {
      std::optional<V> item = deserialize<V>(is);
      // cannot deserialize
      CHECK_ELSE_RETURN_NULLOPT(item.has_value());
      if constexpr (std::is_move_assignable_v<V> && std::is_move_constructible_v<V>) {
        retval.emplace_back(std::move(*item));
      } else {
        retval.emplace_back(*item);
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
