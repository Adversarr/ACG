#pragma once
#include <functional>
#include <optional>
#include <vector>
namespace acg {

/**
 * @brief: Initialize the acg library.
 */
void init(int argc, char** argv);


bool is_inited();

/**
 * @brief Cleanup for the system.
 */
void clean_up();

}  // namespace acg

// Import implemtations
#include "./details/init-inl.hpp"

namespace acg {
using InitHook = details::InitHook;
}
