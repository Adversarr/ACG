#pragma once
#include <cctype>
#include <istream>
#include <ostream>
#include <sstream>
#include <co/json.h>
#include <optional>
#include "../acg_utils.hpp"

namespace acg::utils{
namespace details {

inline std::optional<char> extract_current(std::istream &is) {
  char c;
  if (is.good()) {
    is.get(c);
    is.putback(c);
    return {c};
  }
  return std::nullopt;
}

inline bool consume_until(std::istream &is, char c) {
  // consume and found the dilem c
  is.ignore(std::numeric_limits<std::streamsize>::max(), c);
  // the stream is not good (both bad & eof impiles the 
  // dilem is not found.)
  if (! is.good())
    return false;
  return true;
}

inline bool consume_blank(std::istream &is) {
  char current_byte = 0;
  while (! is.eof() && is.good()) {
    // end of file, just return true.
    if (is.eof()) { return true; }
    // invalid operation, return false.
    if (!is.good()) { return false; }
    // continue...
    is.get(current_byte);
    if (!std::isblank(current_byte)) {
      is.putback(current_byte);
      break;
    }
  }
  return is.good() || is.eof();
}



template <typename T> struct Serializer {
  void Forward(const T &, std::ostream &os) const noexcept;
  std::optional<T> Backward(std::istream &is) const noexcept;
};

} // namespace details




template <typename T, typename = typename T::_refl>
inline std::string serialize(const T &o) noexcept {
  return T::_refl::Serialize(o);
}

template <typename T>
inline std::enable_if_t<
    std::is_same<T, typename details::Serializer<T>::T>::value, std::string>
serialize(const T &o) noexcept {
  std::ostringstream output_stream;
  details::Serializer<T>{}.Forward(o, output_stream);
  return output_stream.str();
}



template <typename T, typename = typename T::_refl>
inline std::optional<T> deserialize(std::istream &is) noexcept {
  return T::_refl::Deserialize(is);
}

template <typename T, typename = typename T::_refl>
inline std::optional<T> deserialize(const std::string &s) noexcept {
  return T::_refl::Deserialize(s);
}

template <typename T>
inline std::enable_if_t<
    std::is_same<T, typename details::Serializer<T>::T>::value, std::optional<T>>
deserialize(std::istream &is) noexcept {
  return details::Serializer<T>{}.Backward(is);
}

template <typename T>
inline std::enable_if_t<
    std::is_same<T, typename details::Serializer<T>::T>::value, std::optional<T>>
deserialize(const std::string &s) noexcept {
  std::istringstream ss{s};
  return details::Serializer<T>{}.Backward(ss);
}


}