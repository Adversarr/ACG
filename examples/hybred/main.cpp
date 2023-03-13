#include <acore/geometry/normal.hpp>
#include <acore/geometry/common_models.hpp>
#include <acore/spatial/subdivision.hpp>
#include <agui/gui.hpp>
#include <agui/init.hpp>
#include <autils/init.hpp>
#include <autils/time/timer.hpp>

#include "app.hpp"
using namespace acg;

int main(int argc, char** argv) {
  using namespace gui;
  acg::utils::hook_default_utils_environment();
  acg::gui::hook_default_gui_environment("Hybred Application");
  acg::init(argc, argv);

  auto& gui = Gui::Instance();
  app::HybredApp app;
  {
    auto c = geometry::make_plane_xy(10);
    app.cloth_.face_ = c.GetFaces();
    app.cloth_.position_ = c.GetVertices().cast<float>();
  }
  app.Init();

  while (!Window::Instance().ShouldClose()) {
    gui.Tick(true);
    gui.RenderOnce();
  }
  acg::clean_up();
  return 0;
}
