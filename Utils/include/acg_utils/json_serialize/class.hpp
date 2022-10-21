#pragma once

#include "decl.hpp"

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
#define SERIALIZE_MEMBER(m) js.add_member(#m, acg::utils::JsonSerializer<__tp_##m>{}.Forward(ref.m))
#define GENERATE_SERIALIZE(...)                                            \
  static inline acg::utils::Json Serialize(const __base_t& ref) noexcept { \
    Json js = json::object();                                                \
    FOR_EACH(SERIALIZE_MEMBER, ;, __VA_ARGS__);                            \
    return js;                                                             \
  }

#define DESERIALIZE_MEMBER(m)                                           \
  if (k == #m) {                                                        \
    auto _result = acg::utils::JsonSerializer<__tp_##m>{}.Backward(js); \
    object.m = _result;                                                 \
    return;

#define GENERATE_STOREATTR(...)                                         \
  static inline void StoreAttribute(__base_t& object,  const char* k,   \
    const Json& js) {                                                   \
    FOR_EACH(DESERIALIZE_MEMBER, }, __VA_ARGS__) }                      \
    assert(false);                                                      \
  }

#define ENABLE_JSON_SERIALIZE_FOR(base, ...)                        \
public:                                                             \
  struct _refl {                                                    \
    using __base_t = base;                                          \
    EXTRACT_TYPE_ALL(__VA_ARGS__);                                  \
    GENERATE_SERIALIZE(__VA_ARGS__)                                 \
    GENERATE_STOREATTR(__VA_ARGS__)                                 \
    static inline __base_t Deserialize(const Json& json) noexcept { \
      __base_t ret;                                                 \
      assert(json.is_object());                                     \
      for (auto it = json.begin(); it != json.end(); ++it) {        \
        StoreAttribute(ret, it.key(), it.value());                  \
      }                                                             \
      return ret;                                                   \
    }                                                               \
  }


namespace acg::utils {


}