#pragma once
#include <cstddef>
#include <cstdint>
#include <memory>
namespace acg::core {

// acg use standard int32_t for indexing.
using Index = std::int32_t;


template<typename T>
using Box = std::shared_ptr<T>;





}
