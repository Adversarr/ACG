/****************************************
  Mass spring Simulator.
  algorithm: Fast Mass Spring (PD)
 ****************************************/
#include <acore/geometry/normal.hpp>
#include <acore/spatial/subdivision.hpp>
#include <agui/gui.hpp>
#include <agui/init.hpp>
#include <autils/init.hpp>
#include <autils/time/timer.hpp>

#include "mass_spring.hpp"
using namespace acg;

int main(int argc, char** argv) {
  using namespace gui;
  acg::utils::hook_default_utils_environment();
  acg::gui::hook_default_gui_environment("Mass Spring");
  acg::init(argc, argv);

  MassSpringApp app;
  app.n_grids_ = 10;
  app.Init();
  auto& gui = Gui::Instance();
  bool running = false;
  bool run_once = false;
  bool init_once = false;
  bool matfree = false;
  bool checking_mode = false;
  bool query_aabb = false;
  spatial::AABB<void, float, 3> aabb{{.2, .2, -.4}, {.6, .6, 0}};
  gui.SetUIDrawCallback([&]() {
    init_once = false;
    ImGui::Begin("GGui User Window");
    init_once |= ImGui::Button("Reset Scene.");
    run_once = ImGui::Button("Run Once");
    ImGui::Checkbox("Run", &running);

    ImGui::InputInt("Max Iterate Count", &app.steps_);
    ImGui::Checkbox("Matrix Free Solver", &matfree);
    ImGui::Checkbox("Check result of MF", &checking_mode);
    ImGui::Checkbox("Evaluate Error Term", &app.eval_error_);
    ImGui::PlotHistogram("Error Term", app.record_.Ptr(), app.steps_, 0, nullptr, FLT_MAX, FLT_MAX,
                         ImVec2(0, 80));
    ImGui::Text("Error Mean = %f, Max = %f", app.record_.Mean(),
                app.record_.Reduce(0, [](float a, float b) { return std::max(a, b); }));
    ImGui::InputFloat("Delta t for TimeStep", &app.dt_);

    ImGui::InputFloat3("lb", aabb.lower_bound.data());
    ImGui::InputFloat3("ub", aabb.upper_bound.data());
    query_aabb = ImGui::Button("Query");
    ImGui::Text("Error Last = %f", app.record_.Last());
    init_once |= ImGui::InputFloat("Spring K", &app.k_);
  });

  auto& render_mesh = gui.GetScene().AddMesh();
  auto& render_wf = gui.GetScene().AddWireframe();

  auto update_mesh = [&]() {
    auto indices = app.faces_;
    auto positions = app.position_;
    render_mesh.SetVertices(app.position_)
        .SetUniformColor(acg::types::Rgba{.7, .7, .3, 1})
        .ComputeDefaultNormal()
        .SetIndices(app.faces_)
        .SetEnableWireframe()
        .MarkUpdate();

    acg::spatial::SubDivisionAABB<Float, Index, 3, 8, 8> sd;
    for (auto [i, v] : acg::enumerate(acg::access(app.position_))) {
      acg::spatial::AABB<Index> aabb(v, (v.array() + 0.001).matrix(), i);
      sd.Insert(aabb);
    }

    auto [position, lines] = sd.Visualize();
    if (query_aabb) {
      auto colls = sd.QueryInternal();
      ACG_INFO("Count = {}", colls.size());
    }
    render_wf.SetPositions(position);
    render_wf.SetIndices(lines);
    render_wf.SetColors(types::Rgb{.7, 0, 0});
    render_wf.MarkUpdate();
  };

  update_mesh();

  while (!Window::Instance().ShouldClose()) {
    glfwPollEvents();
    gui.Tick();
    gui.RenderOnce();
    gui.UpdateScene();
    acg::utils::Timer timer;
    if (running || run_once) {
      timer.TickBegin();
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
      timer.TickEnd();
      update_mesh();
    }

    if (init_once) {
      app.Init();
    }
  }

  acg::clean_up();
  return 0;
}
