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
  auto plane = geometry::make_plane_xy(10).Cast<app::HybredApp::Scalar>();
  app.AddCloth(plane.GetVertices(),
               plane.GetFaces(), 1000);

  physics::PositionStaticConstraint<app::HybredApp::Scalar, 3> constraint(
    physics::PhysicsObject(physics::PhysicsObjectType::kCloth, 0, 0)
    , plane.GetVertices().col(0));
  app.constraints_.push_back(constraint);

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
  bool run_once = false;
  gui.SetUIDrawCallback([&]()->void{
      ImGui::Checkbox("Run", &running);
      run_once = ImGui::Button("Run Once");
      });

  update_scene();
  while (!Window::Instance().ShouldClose()) {
    gui.Tick(true);
    gui.RenderOnce();

    if (running || run_once) {
      app.Step();
      update_scene();
    }
  }
  acg::clean_up();
  return 0;
}
