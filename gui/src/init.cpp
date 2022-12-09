#include <acg_core/init.hpp>
#include <acg_utils/singleton.hpp>
#include "acg_gui/hook.hpp"
#include "acg_gui/backend/vkcontext.hpp"

namespace acg::gui::details {
void VkContextHooker::Hook() noexcept {
  static bool is_hooked{false};
  if (is_hooked) {
    return;
  }
  is_hooked = true;

  acg::details::InitHook hook;
  hook.priority = 10;
  hook.on_init = []() { acg::gui::details::VkContext::Builder().Build(); };
  hook.on_exit = []() { acg::utils::Singleton<acg::gui::details::VkContext>::Release(); };
  hook.name = "VkContext";
  acg::details::add_hook(hook);
};
}  // namespace acg::gui::details
