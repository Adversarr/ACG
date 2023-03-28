#include <acore/geometry/common_models.hpp>
#include <acore/init.hpp>
#include <agui/gui.hpp>
#include <agui/init.hpp>
#include <autils/init.hpp>

#include "app.hpp"

int main(int argc, char** argv) {
  acg::utils::hook_default_utils_environment();
  acg::gui::hook_default_gui_environment("Shape Matching");
  acg::init(argc, argv);
  auto& gui = acg::gui::Gui::Instance();
  auto& window = acg::gui::Window::Instance();

  ShapeMatchingApp app;
  auto reset_scene = [&]() {
    auto sphere = geometry::sphere_uv({0, 0, 1}, 0.7, 16, 32);
    app.position_ = sphere.GetVertices();
    app.faces_ = sphere.GetFaces();
    app.Init();
  };
  auto* mesh_render = gui.GetScene().AddMesh();
  auto update_scene = [&]() {
    mesh_render->SetEnableWireframe()
        .SetUniformColor(types::Rgba{.8, .3, .3, 1})
        .SetIndices(app.faces_)
        .SetVertices(app.position_)
        .ComputeDefaultNormal()
        .MarkUpdate();
    gui.UpdateScene();
  };

  reset_scene();
  update_scene();

  bool reset = false;
  bool run_once = false;
  bool run = false;
  gui.SetUIDrawCallback([&]() -> void {
    reset = ImGui::Button("Reset");
    run_once = ImGui::Button("Run once");
    ImGui::Checkbox("Run", &run);
    ImGui::InputFloat("Alpha", &app.alpha_);
    ImGui::InputFloat("dt", &app.dt_);
  });
  while (!window.ShouldClose()) {
    gui.Tick(true);
    gui.RenderOnce();
    if (reset) {
      reset_scene();
      update_scene();
    } else {
      if (run_once || run) {
        app.Step();
        update_scene();
      }
    }
  }
  acg::clean_up();
  return EXIT_SUCCESS;
}
