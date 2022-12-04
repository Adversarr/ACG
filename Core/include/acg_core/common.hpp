/**
 * @file acg_core.hpp
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
using F32 = float;

/**
 * @brief 64-bit Float point number.
 *
 */
using F64 = double;

/**
 * @brief Default floating-point type is Float 64, aka double.
 */
using Float = F64;

/**
 * @brief Index & Size type for mesh, particle system, rendering(index buffer), etc.
 */
using Idx = int64_t;

using ULIdx = uint64_t;

constexpr char logo[]
    = "      ___           ___           ___     \n"
      "     /\\  \\         /\\__\\         /\\__\\    \n"
      "    /::\\  \\       /:/  /        /:/ _/_   \n"
      "   /:/\\:\\  \\     /:/  /        /:/ /\\  \\  \n"
      "  /:/ /::\\  \\   /:/  /  ___   /:/ /::\\  \\ \n"
      " /:/_/:/\\:\\__\\ /:/__/  /\\__\\ /:/__\\/\\:\\__\\\n"
      " \\:\\/:/  \\/__/ \\:\\  \\ /:/  / \\:\\  \\ /:/  /\n"
      "  \\::/__/       \\:\\  /:/  /   \\:\\  /:/  / \n"
      "   \\:\\  \\        \\:\\/:/  /     \\:\\/:/  /  \n"
      "    \\:\\__\\        \\::/  /       \\::/  /   \n"
      "     \\/__/         \\/__/         \\/__/    \n";

void init(int argc, char** argv);

void cleanup();

}  // namespace acg
