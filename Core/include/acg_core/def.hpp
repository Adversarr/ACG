#pragma once
#include <cstddef>
#include <cstdint>

namespace acg {

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
 * @brief Index & Size type for mesh, particle system, rendering(index buffer), etc.
 *
 */
using Idx = uint32_t;

/**
 * @brief Use size_t same to STL.
 *
 * @note Do not use this type if you are refer to the number of vertices, particles, etc. This type
 * is only designed for containers.
 */
using size_t = std::size_t;

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

}  // namespace acg
