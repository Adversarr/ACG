#pragma once
#include <functional>
#include <optional>
#include <vector>
namespace acg {
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

void clean_up();
namespace details {

struct InitHook {
  std::optional<std::function<void()>> on_init;
  std::optional<std::function<void()>> on_exit;
  int priority;
  const char* name = nullptr;
};

void add_hook(const InitHook& hook);

/**
 * @brief Init all the hooks.
 */
void init();

/**
 * @brief Cleanup all the hooks.
 */
void cleanup();

}  // namespace details
}  // namespace acg