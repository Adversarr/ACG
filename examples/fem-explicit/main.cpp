#include <acore/init.hpp>
#include <agui/gui.hpp>
#include <agui/init.hpp>
#include <autils/init.hpp>
#include <autils/time.hpp>

#include "app.hpp"
using namespace acg::gui;

acg::geometry::topology::TetraList make_tetra() {
  acg::geometry::topology::TetraList tetra(4, 1);
  auto acc = acg::access(tetra);
  acc(0) = acg::Vec4Index{0, 1, 2, 3};
  return tetra;
}

acg::types::PositionField<acg::F64, 3> make_pos() {
  auto pos = acg::FieldBuilder<acg::F64, 3>(4).Placeholder();
  auto acc = acg::access(pos);
  acc(0) = acg::Vec3d(0, 0, 0);
  acc(1) = acg::Vec3d(1, 0, 0);
  acc(2) = acg::Vec3d(0, 1, 0);
  acc(3) = acg::Vec3d(0, 0, 1);
  return pos;
}

acg::geometry::topology::TriangleList make_face() {
  auto tri = acg::FieldBuilder<acg::Index, 3>(4).Placeholder();
  auto acc = acg::access(tri);
  acc(0) = acg::Vec3Index(0, 1, 3);
  acc(1) = acg::Vec3Index(0, 2, 1);
  acc(2) = acg::Vec3Index(0, 3, 2);
  acc(3) = acg::Vec3Index(1, 2, 3);
  return tri;
}

int main(int argc, char** argv) {
  // Setup all the global instances.
  acg::utils::hook_default_utils_environment();
  acg::gui::hook_default_gui_environment("FEM");
  acg::init(argc, argv);
  Gui& gui = Gui::Instance();
  bool clear = false;
  bool running = false;
  FemExplicitApp app;
  app.position_ = make_pos();
  app.tetra_ = make_tetra();
  app.Init();

  bool step_once = false;
  bool reset_once = false;
  int sub_steps = 32;
  gui.SetUIDrawCallback([&clear, &running, &app, &step_once, &reset_once, &sub_steps]() {
    ImGui::Begin("GGui User Window");
    clear = ImGui::Button("Reset Scene.");
    // running = ImGui::Button("Run Once");
    ImGui::Checkbox("Run", &running);
    float avg_speed = app.velocity_.colwise().norm().mean();

    ImGui::InputFloat("Avg Speed", &avg_speed);
    ImGui::InputDouble("Mu", &app.mu_);
    ImGui::InputDouble("Lambda", &app.lambda_);

    ImGui::InputInt("SubStep Count", &sub_steps);
    step_once = ImGui::Button("Step");
    reset_once = ImGui::Button("Reset");
  });

  auto indices = make_face();
  auto& mesh = gui.GetScene()
                   .AddMesh(indices, app.position_.cast<acg::F32>())
                   .SetUniformColor(acg::types::Rgba(1, 0, 0, 1))
                   .SetEnableWireframe();
  mesh.MarkUpdate();
  while (!Window::Instance().ShouldClose()) {
    glfwPollEvents();
    gui.Tick();
    gui.RenderOnce();
    gui.UpdateScene();
    if (step_once || running) {
      for (int i = 0; i < sub_steps; ++i) {
        app.Step();
      }
      mesh.SetVertices(app.position_.cast<acg::F32>()).MarkUpdate();
    }

    if (reset_once) {
      app.position_ = make_pos();
      app.Init();
      mesh.SetVertices(app.position_.cast<acg::F32>()).MarkUpdate();
    }
  }

  acg::clean_up();
  return EXIT_SUCCESS;
}
