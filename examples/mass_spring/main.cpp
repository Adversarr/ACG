/****************************************
  Mass spring Simulator. 
  algorithm: Fast Mass Spring (PD)
 ****************************************/



#include <acore/geometry/normal.hpp>
#include <acore/init.hpp>
#include <agui/backend/context.hpp>
#include <agui/backend/graphics_context.hpp>
#include <agui/backend/window.hpp>
#include <agui/ggui.hpp>
#include <agui/init.hpp>
#include <autils/init.hpp>

#include "mass_spring.hpp"
using namespace acg;

int main(int argc, char** argv) {
  using namespace gui;
  auto utils_hooker = acg::utils::UtilsHooker();
  utils_hooker.Hook();
  Window::Hooker{"MassSpring"}.Hook();
  auto vkctx_hooker = acg::gui::VkContext2Hooker{};
  vkctx_hooker.app_name = "MassSpring";
  vkctx_hooker.enable_validation = false;
  vkctx_hooker.Hook();
  VkGraphicsContext::Hooker().Hook();
  Gui::Config config;
  config.Hook();
  acg::init(argc, argv);
  App app;
  app.n_grids_ = 100;
  app.Init();
  auto& gui = Gui::Instance();
  bool running = false;
  bool run_once = false;
  bool init_once = false;
  gui.SetUIDrawCallback([&init_once, &running, &run_once, &app]() {
    init_once = false;
    ImGui::Begin("GGui User Window");
    init_once |= ImGui::Button("Reset Scene.");
    run_once = ImGui::Button("Run Once");
    ImGui::Checkbox("Run", &running);
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
      app.StepImplicit();
      update_mesh();
    }

    if (init_once) {
      app.Init();
    }
  }

  acg::clean_up();
  return 0;
}
