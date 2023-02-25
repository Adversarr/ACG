/****************************************
  Mass spring Simulator.
  algorithm: Fast Mass Spring (PD)
 ****************************************/

#include <acore/geometry/normal.hpp>
#include <agui/gui.hpp>
#include <agui/init.hpp>
#include <autils/init.hpp>

#include "mass_spring.hpp"
using namespace acg;

int main(int argc, char** argv) {
  using namespace gui;
  acg::utils::hook_default_utils_environment();
  acg::gui::hook_default_gui_environment("MASS");
  acg::init(argc, argv);

  App app;
  app.n_grids_ = 160;
  app.Init();
  auto& gui = Gui::Instance();
  bool running = false;
  bool run_once = false;
  bool init_once = false;
  bool matfree = false;
  bool checking_mode = false;
  gui.SetUIDrawCallback([&init_once, &running, &run_once, &app, &matfree, &checking_mode]() {
    init_once = false;
    ImGui::Begin("GGui User Window");
    init_once |= ImGui::Button("Reset Scene.");
    run_once = ImGui::Button("Run Once");
    ImGui::Checkbox("Run", &running);
    ImGui::InputInt("Max Iterate Count", &app.steps_);
    ImGui::Checkbox("Matrix Free Solver", &matfree);
    ImGui::Checkbox("Check result of MF", &checking_mode);
    init_once |= ImGui::InputFloat("Spring K", &app.k_);
  });

  auto& render_mesh = gui.GetScene().AddMesh();

  auto update_mesh = [&render_mesh, &app]() {
    auto indices = app.faces_;
    auto positions = app.position_;
    acg::geometry::Normal<F32> kern_normal(indices, positions);
    auto normal = kern_normal.PerVertex(geometry::NormalPerVertexMode::kArea);
    render_mesh.SetVertices(app.position_)
        .SetUniformColor(acg::types::Rgba{.7, .7, .3, 1})
        .SetNormals(normal)
        .SetIndices(app.faces_)
        .SetEnableWireframe()
        .MarkUpdate();
  };

  update_mesh();

  while (!Window::Instance().ShouldClose()) {
    glfwPollEvents();
    gui.Tick();
    gui.RenderOnce();
    gui.UpdateScene();
    if (running || run_once) {
      if (!checking_mode) {
        if (matfree) {
          app.StepProjDynMf();
        } else {
          app.StepProjDyn();
        }
      } else {
        auto backup_position = app.position_.eval();
        auto backup_velocity = app.velocity_.eval();
        app.StepProjDyn();
        auto position_pd = app.position_.eval();
        auto velocity_pd = app.velocity_.eval();
        auto d_pd = app.d_.eval();

        app.position_ = backup_position;
        app.velocity_ = backup_velocity;
        app.StepProjDynMf();

        auto err_position = (app.position_ - position_pd).array().abs().maxCoeff();
        auto err_velocity = (app.velocity_ - velocity_pd).array().abs().maxCoeff();
        auto err_d = (app.d_ - d_pd).array().abs().maxCoeff();
        ACG_INFO("Error of Position = {}", err_position);
        ACG_INFO("Error of Velocity = {}", err_velocity);
        ACG_INFO("Error of d = {}", err_d);
      }
      update_mesh();
    }

    if (init_once) {
      app.Init();
    }
  }

  acg::clean_up();
  return 0;
}
