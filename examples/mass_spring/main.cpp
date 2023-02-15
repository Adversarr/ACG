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
  utils_hooker.default_logging_level = spdlog::level::debug;
  utils_hooker.Hook();
  Window::Hooker{"Mass Spring"}.Hook();
  auto vkctx_hooker = acg::gui::VkContext2Hooker{};
  vkctx_hooker.app_name = "GGui";
  vkctx_hooker.enable_validation = true;
  vkctx_hooker.Hook();
  VkGraphicsContext::Hooker().Hook();
  acg::init(argc, argv);
  GGui::Config config;

  {
    App app;
    app.Init();
    GGui gui(config);
    for (auto p: app.springs_) {
      std::cout << p.first << "\t" << p.second << std::endl;
    }
    bool clear = false;
    bool running = false;
    gui.SetUIDrawCallback([&clear, &running, &app]() {
      ImGui::Begin("GGui User Window");
      clear = ImGui::Button("Reset Scene.");
      // running = ImGui::Button("Run Once");
      ImGui::Checkbox("Run", &running);
      ImGui::InputFloat("Spring K", &app.k_);
    });

    auto& render_mesh = gui.GetScene().AddMesh();

    auto update_mesh = [&render_mesh, &app]() {
      auto indices = app.faces_;
      auto positions = app.position_;
      acg::geometry::Normal<F32> kern_normal(indices, positions);
      auto normal = kern_normal.PerVertex(geometry::NormalPerVertexMode::kArea);
      render_mesh.SetVertices(app.position_)
          .SetUniformColor(acg::types::Rgba{.7, .7, .7, 1})
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
      if (running) {
        app.Step();
        update_mesh();
      }
    }
  }

  acg::clean_up();
  return 0;
}
