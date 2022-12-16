#pragma once
#include <nlohmann/json.hpp>
#include "../acg_utils.hpp"


namespace acg::utils {

using Json = nlohmann::json;

// TODO
// template<typename T, typename = void>
// struct JsonSerializer {
//   inline Json Forward(T&& object) const noexcept;

//   inline Json Backward(T&& object) const noexcept;
// };
}
