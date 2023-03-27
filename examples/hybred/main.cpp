#include <acore/geometry/common_models.hpp>
#include <acore/geometry/normal.hpp>
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
  auto& cloth = gui.GetScene().AddMesh();

  app::HybredApp app;
  app.Init();
  auto plane = geometry::make_plane_xy(4);
  app.AddCloth(plane.GetVertices().cast<app::HybredApp::Scalar>(),
               plane.GetFaces(), 100);

  auto update_scene = [&]() {
    cloth.SetVertices(app.cloth_.front().data_.position_)
        .SetIndices(app.cloth_.front().data_.face_)
        .ComputeDefaultNormal()
        .SetUniformColor(types::Rgba{.7, .7, .7, 1})
        .SetEnableWireframe()
        .MarkUpdate();

    gui.UpdateScene();
  };

  bool running = false;
  gui.SetUIDrawCallback([&]()->void{
      ImGui::Checkbox("Run", &running);
      });

  update_scene();
  while (!Window::Instance().ShouldClose()) {
    gui.Tick(true);
    gui.RenderOnce();

    if (running) {
      app.Step();
    }
  }
  acg::clean_up();
  return 0;
}
