#pragma once

#include "decl.hpp"

namespace acg::utils {

template <> struct JsonSerializer<int32_t> {
  inline Json Forward(int32_t x) const noexcept { return x; }

  inline int32_t Backward(const Json& o) const noexcept { 
    assert(o.is_int());
    return o.as_int32();
  }
};


template <> struct JsonSerializer<int64_t> {
  inline Json Forward(int64_t x) const noexcept { return x; }

  inline int64_t Backward(const Json& o) const noexcept { 
    assert(o.is_int());
    return o.as_int64(); 
  }
};

template <> struct JsonSerializer<uint32_t> {
  inline Json Forward(uint32_t x) const noexcept { return x; }

  inline int32_t Backward(const Json& o) const noexcept {
    assert(o.is_int());
    return o.as_int32();
  }
};

template <> struct JsonSerializer<uint64_t> {
  inline Json Forward(uint64_t x) const noexcept { return x; }

  inline int64_t Backward(const Json& o) const noexcept {
    assert(o.is_int());
    return o.as_int64();
  }
};

template <> struct JsonSerializer<float> {
  inline Json Forward(float x) const noexcept { return x; }

  inline int64_t Backward(const Json& o) const noexcept { 
    assert(o.is_double());
    return static_cast<float>(o.as_double());
  }
};

template <> struct JsonSerializer<double> {
  inline Json Forward(double x) const noexcept { return x; }

  inline double Backward(const Json& o) const noexcept { 
    assert(o.is_double());
    return o.as_double();
  }
};

template <> struct JsonSerializer<std::string> {
  inline Json Forward(const std::string& x) const noexcept { return x; }

  inline std::string Backward(const Json & o) const noexcept {
    assert(o.is_string());
    return o.as_string().c_str();
  }
};

template <> struct JsonSerializer<fastring> {
  inline Json Forward(const fastring& x) const noexcept { return x; }

  inline fastring Backward(const Json& o) const noexcept {
    assert(o.is_string());
    return o.as_string().c_str();
  }
};

template <> struct JsonSerializer<const char*> {
  inline Json Forward(const char* p) const noexcept { return p; }
};



}  // namespace acg::utils
