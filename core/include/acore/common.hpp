/**
 * @file acore.hpp
 * @author Zherui Yang (yangzherui2001@foxmail.com)
 * @brief include basic definition for the framework.
 * @version 0.0.1
 * @date 2022-10-23
 *
 * @copyright Copyright (c) 2022
 *
 */

#pragma once

#include <cstddef>
#include <cstdint>
#include <limits>

namespace acg {

/**
 * @brief Use size_t same to STL.
 *
 * @note Do not use this type if you are refer to the number of vertices, particles, etc. This type
 * is only designed for containers.
 */
using size_t = std::size_t;

/**
 * @brief 32-bit Float point number.
 *
 */
using Float32 = float;

/**
 * @brief 64-bit Float point number.
 *
 */
using Float64 = double;

/**
 * @brief Default floating-point type is Float 64, aka double.
 */
using Float = Float32;

/**
 * @brief Integer Types
 */
using Int32 = std::int32_t;
using UInt32 = std::uint32_t;
using Int64 = std::int64_t;
using UInt64 = std::uint64_t;

/**
 * @brief Index & Size type for mesh, particle system, rendering(index buffer), etc.
 */
using Index = std::ptrdiff_t;

/****************************************
 * NOTE: Special Flags may used multiple times
 ****************************************/

// @brief indicates resource is Invalid
constexpr Index InvalidIndex = std::numeric_limits<Index>::max();
constexpr size_t InvalidSubscript = std::numeric_limits<size_t>::max();

}  // namespace acg
