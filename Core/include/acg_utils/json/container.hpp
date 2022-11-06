#pragma once

#include "decl.hpp"
#include <co/god.h>
#include <co/maybe.h>

namespace acg::utils {

using json::Json;

template <typename T> struct JsonSerializer<std::vector<T>> {
  inline Json Forward(std::vector<T>&& object) const noexcept {
    auto ret = json::array();
    for (const T& item : object) {
      ret.push_back(JsonSerializer<T>{}.Forward(item));
    }
    return ret;
  }

  inline Json Backward(Json&& obj) const noexcept {
    assert(obj.is_array());
    std::vector<T> ret;
    for (auto i = 0; i < obj.array_size(); ++i) {
      auto o = obj.get(i);
      assert(!o.is_null());
      ret.push_back(JsonSerializer<T>{}.Backward(o));
    }
    return ret;
  }
};

template <typename T> struct JsonSerializer<std::list<T>> {
  inline Json Forward(std::list<T>&& object) const noexcept {
    auto ret = json::array();
    for (const T& item : object) {
      ret.push_back(JsonSerializer<T>{}.Forward(item));
    }
    return ret;
  }

  inline Json Backward(Json&& obj) const noexcept {
    assert(obj.is_array());
    std::list<T> ret;
    for (auto i = 0; i < obj.array_size(); ++i) {
      auto o = obj.get(i);
      assert(!o.is_null());
      ret.push_back(JsonSerializer<T>{}.Backward(o));
    }
    return ret;
  }
};

template <typename V> struct JsonSerializer<std::map<std::string, V>> {
  inline Json Forward(std::map<std::string, V>&& object) const noexcept {
    auto ret = json::object();
    for (const auto& [k, v] : object) {
      ret.add_member(k.c_str(), JsonSerializer<V>{}.Forward(v));
    }
    return ret;
  }

  inline std::map<std::string, V> Backward(Json&& obj) const noexcept {
    assert(obj.is_object());
    std::map<std::string, V> ret;
    for (auto it = obj.begin(); it != obj.end(); ++it) {
      ret.emplace(it.key(), JsonSerializer<V>{}.Backward(it.value()));
    }
    return ret;
  }
};

template <typename V> struct JsonSerializer<std::map<fastring, V>> {
  inline Json Forward(std::map<fastring, V>&& object) const noexcept {
    auto ret = json::object();
    for (const auto& [k, v] : object) {
      ret.add_member(k.c_str(), JsonSerializer<V>{}.Forward(v));
    }
    return ret;
  }

  inline std::map<fastring, V> Backward(Json&& obj) const noexcept {
    assert(obj.is_object());
    std::map<fastring, V> ret;
    for (auto it = obj.begin(); it != obj.end(); ++it) {
      ret.emplace(it.key(), JsonSerializer<V>{}.Backward(it.value()));
    }
    return ret;
  }
};

}  // namespace acg::utils
