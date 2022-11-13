#pragma once
#include <functional>
#include <optional>
#include <vector>

namespace acg::details {

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

}  // namespace acg::details
