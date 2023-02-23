#include <acore/init.hpp>
#include <agui/init.hpp>
#include <autils/init.hpp>
int main(int argc, char** argv) {
  acg::utils::hook_default_utils_environment();
  acg::gui::hook_default_gui_environment("Fem-Implicit");
  acg::init(argc, argv);
  acg::clean_up();
  return 0;
}
