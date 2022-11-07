#include "acg_vis/init.hpp"

#include <acg_core/init.hpp>
#include <acg_utils/singleton.hpp>

#include "acg_vis/renderer.hpp"

namespace acg::visualizer::details {
void VkContextHooker::Hook() noexcept {
  static bool is_hooked{false};
  if (is_hooked) {
    return;
  }
  is_hooked = true;

  acg::details::InitHook hook;
  hook.priority = 10;
  hook.on_init = []() { acg::visualizer::details::VkContext::Builder().Build(); };
  hook.on_exit = []() { acg::utils::Singleton<acg::visualizer::details::VkContext>::Release(); };
  hook.name = "VkContext";
  acg::details::add_hook(hook);
};
}  // namespace acg::visualizer::details
