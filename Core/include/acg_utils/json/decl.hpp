#pragma once

#include <co/json.h>
#include "../acg_utils.hpp"


namespace acg::utils {

using json::Json;

template<typename T>
struct JsonSerializer {
  inline Json Forward(T&& object) const noexcept;

  inline Json Backward(T&& object) const noexcept;
};



// shortcut for JsonSerializer::operator().
//
// Notice: exit when fail.
template<typename T>
inline Json to_json(T&& object) noexcept {
  return JsonSerializer<T>{}.Forward(std::forward<T>(object));
}

template<typename T>
inline Json from_json(Json&& object) noexcept {
  return JsonSerializer<T>{}.Backward(std::forward<Json>(object));
}



}
