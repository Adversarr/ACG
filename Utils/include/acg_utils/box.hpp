// Follow optional, but with error message.

#include <optional>
#include <variant>
#include <string>

#include <exception>
namespace acg::utils {

template<typename _Enum>
class Error {
  // static_assert(std::, message);
public:
  using enum_type = _Enum;
private:
  const std::string message_;
  const std::optional<enum_type> kind_ = -1;
public:
  inline explicit Error(std::string_view msg, _Enum kind) noexcept: 
    message_(msg), kind_(kind) {}

  inline explicit Error(std::string_view msg) noexcept:
    message_(msg), kind_(std::nullopt) {}

  // Get the error message.
  inline std::string_view GetMessage() { return message_; }

  // Get the error kind.
  inline enum_type GetKind() { return kind_; } 

};

template<typename _Tp, typename _Enum = int>
using Box = std::variant<_Tp, Error<_Enum> >;

template<typename _Tp, typename _Enum = int>
Box<std::decay_t<_Tp>> make_box(_Tp&& v) {
  return Box<std::decay_t<_Tp>, _Enum>{std::forward<_Tp>(v)};
}
// TODO:

}