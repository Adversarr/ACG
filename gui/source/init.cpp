#include <agui/init.hpp>
#include <agui/backend/context.hpp>
#include <agui/backend/graphics_context.hpp>
#include <agui/backend/window.hpp>
#include <agui/gui.hpp>

namespace acg::gui {
void hook_default_gui_environment(std::string app_name) {
  Window::Hooker(app_name).Hook();
  auto vkctx_hooker = acg::gui::VkContext2Hooker{};
  vkctx_hooker.app_name = app_name;
  vkctx_hooker.enable_validation = true;
  vkctx_hooker.Hook();
  VkGraphicsContext::Hooker().Hook();
  Gui::Config config;
  config.enable_ui_editor = true;
  config.Hook();
}

}  // namespace acg::gui
