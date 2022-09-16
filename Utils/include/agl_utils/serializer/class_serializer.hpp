#pragma once
#define PARENS ()

#define EXPAND(...) __VA_OPT__(EXPAND4(EXPAND4(EXPAND4(EXPAND4(__VA_ARGS__)))))
#define EXPAND4(...) __VA_OPT__(EXPAND3(EXPAND3(EXPAND3(EXPAND3(__VA_ARGS__)))))
#define EXPAND3(...) __VA_OPT__(EXPAND2(EXPAND2(EXPAND2(EXPAND2(__VA_ARGS__)))))
#define EXPAND2(...) __VA_OPT__(EXPAND1(EXPAND1(EXPAND1(EXPAND1(__VA_ARGS__)))))
#define EXPAND1(...) __VA_ARGS__
#define FOR_EACH(macro, mid, ...)                                              \
  __VA_OPT__(EXPAND(FOR_EACH_HELPER(macro, mid, __VA_ARGS__)))
#define FOR_EACH_HELPER(macro, mid, a1, ...)                                   \
  macro(a1) __VA_OPT__(mid FOR_EACH_AGAIN PARENS(macro, mid, __VA_ARGS__))
#define FOR_EACH_AGAIN() FOR_EACH_HELPER
#define EXTRACT_TYPE(m) using __tp_##m = decltype(m)
#define EXTRACT_TYPE_ALL(...) FOR_EACH(EXTRACT_TYPE, ;, __VA_ARGS__)

#define SERIALIZE_MEMBER(m) o << "\"" #m "\": " << agl::utils::serialize(ref.m)

#define GENERATE_SERIALIZE(...)                                                \
  static inline std::string Serialize(const __base_t &ref) noexcept {          \
    std::ostringstream o;                                                      \
    o << "{";                                                                  \
    FOR_EACH(SERIALIZE_MEMBER, ; o << ", ";, __VA_ARGS__);                     \
    o << "}";                                                                  \
    return o.str();                                                            \
  }

#define DESERIALIZE_MEMBER(m)                                                  \
  if (k == #m) {                                                               \
    auto _result = agl::utils::details::Serializer<__tp_##m>{}.Backward(is);   \
    if (!(_result.has_value()))                                                \
      return false;                                                            \
    object.m = *_result;                                                       \
    return true;

#define GENERATE_STOREATTR(...) static inline bool StoreAttribute(__base_t& object, \
          const std::string& k, std::istringstream& is) {                           \
  FOR_EACH(DESERIALIZE_MEMBER, }, __VA_ARGS__)                                      \
  }                                                                                 \
  return false;                                                                     \
  }

#define ENABLE_SERIALIZE_FOR(base, ...)                                        \
public:                                                                        \
  struct _refl {                                                               \
    using __base_t = base;                                                     \
    EXTRACT_TYPE_ALL(__VA_ARGS__);                                             \
    GENERATE_SERIALIZE(__VA_ARGS__)                                            \
    GENERATE_STOREATTR(__VA_ARGS__)                                            \
    static inline std::optional<__base_t>                                      \
    Deserialize(const std::string &s) noexcept {                               \
      std::istringstream ss{s};                                                \
      return Deserialize(ss);                                                  \
    }                                                                          \
    static inline std::optional<__base_t>                                      \
    Deserialize(std::istringstream &ss) noexcept {                             \
      __base_t ret;                                                            \
      CHECK_ELSE_RETURN_NULLOPT(agl::utils::details::consume_blank(ss));       \
      CHECK_ELSE_RETURN_NULLOPT(agl::utils::details::consume_until(ss, '{'));  \
      CHECK_ELSE_RETURN_NULLOPT(agl::utils::details::consume_blank(ss));       \
      while (agl::utils::details::extract_current(ss) != '}') {                \
        auto k = agl::utils::details::Serializer<std::string>{}.Backward(ss);  \
        CHECK_ELSE_RETURN_NULLOPT(k.has_value());                              \
        CHECK_ELSE_RETURN_NULLOPT(agl::utils::details::consume_blank(ss));     \
        CHECK_ELSE_RETURN_NULLOPT(                                             \
            agl::utils::details::consume_until(ss, ':'));                      \
        CHECK_ELSE_RETURN_NULLOPT(StoreAttribute(ret, *k, ss));                \
        CHECK_ELSE_RETURN_NULLOPT(agl::utils::details::consume_blank(ss));     \
        auto current_ch = agl::utils::details::extract_current(ss);            \
        CHECK_ELSE_RETURN_NULLOPT(current_ch.has_value());                     \
        if (*current_ch == '}') {                                              \
          break;                                                               \
        }                                                                      \
      }                                                                        \
      return ret;                                                              \
    }                                                                          \
  }
