#include <optional>
#include <functional>

namespace acg::details {
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


struct InitHook {
  // @brief: Callbacks for initialize call.
  std::optional<std::function<void()>> on_init;
  // @brief: Callbacks for cleanup call.
  std::optional<std::function<void()>> on_exit;

  // Higher priority is inited first.
  int priority;

  // @brief: Name for the hooker.
  const char* name = nullptr;
};

void add_hook(const InitHook& hook);
/**
 * @brief Init all the hooks.
 */
void init_hooks();

/**
 * @brief Cleanup all the hooks.
 */
void cleanup_hooks();
}
