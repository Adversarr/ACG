#include <acore/init.hpp>
#include <agui/init.hpp>
#include <autils/init.hpp>


#include <agui/gui.hpp>
int main(int argc, char** argv) {
  acg::utils::hook_default_utils_environment();
  acg::gui::hook_default_gui_environment("Fem-Implicit");
  acg::init(argc, argv);
  auto& gui = acg::gui::Gui::Instance();
  auto& window = acg::gui::Window::Instance();


  while (!window.ShouldClose()) {
    gui.Tick(true);
    gui.RenderOnce();
  }
  acg::clean_up();
  return EXIT_SUCCESS;
}
