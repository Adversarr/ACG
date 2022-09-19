#pragma once
#include "acg_utils/acg_utils.hpp"
#include "serializer_decl.hpp"
#include <sstream>
#include <string>

/* --- For String. --- */

namespace acg::utils::details {

static inline std::size_t replace_all(std::string &inout, std::string_view what,
                                      std::string_view with) noexcept {
  std::size_t count{};
  for (std::string::size_type pos{};
       std::string::npos != (pos = inout.find(what.data(), pos, what.length()));
       pos += with.length(), ++count) {
    inout.replace(pos, what.length(), with.data(), with.length());
  }
  return count;
}

template <> struct Serializer<std::string> {

  using T = std::string;
  std::ostream &Forward(const std::string &v, std::ostream &os) const noexcept {
    if (v.find('"') != std::string::npos) {
      auto backup = v;
      replace_all(backup, "\"", "\\\"");
      return os << '"' << backup << '"';
    }
    return (os << '"' << v << '"');
  }

  std::optional<T> Backward(std::istream &is) const {
    // consume first "
    CHECK_ELSE_RETURN_NULLOPT(consume_until(is, '"'));
    CHECK_ELSE_RETURN_NULLOPT(is.good());
    std::ostringstream oss;
    
    // now read buffer.
    while (is.good()) {
      char current_ch= 0; is.get(current_ch);
      if (current_ch == '"') {
        // done.
        break;
      } else if (current_ch == '\\') {
        CHECK_ELSE_RETURN_NULLOPT(is.good());
        char translated = 0; is.get(translated);
        if (translated != '"') {
          oss << "\\";
        }
        oss << translated;
      } else if (current_ch == '"') {
        // not translated " should be marked the end of string.
        break;
      } else {
        // normal char should be put into the buffer.
        oss << current_ch;
      }
    }
    return {oss.str()};
  }
};

template <> struct Serializer<std::string_view> {
  using T = std::string_view;
  std::ostream &Forward(const T &v, std::ostream &os) const noexcept {
    return Serializer<std::string>{}.Forward(std::string(v), os);
  }
};


template <int N> struct Serializer<char[N]> {
  using T = char[N];
  std::ostream &Forward(const char v[N], std::ostream &os) const noexcept {
    return Serializer<std::string>{}.Forward(std::string(std::string_view{v}),
                                             os);
  }
};
} // namespace acg::utils::details