#include <acore/init.hpp>
#include <agui/backend/context.hpp>
#include <agui/backend/graphics_context.hpp>
#include <agui/backend/window.hpp>
#include <agui/ggui.hpp>
#include <autils/init.hpp>
#include <autils/time.hpp>

#include "app.hpp"
using namespace acg::gui;

acg::geometry::topology::TetraList make_tetra() {
  acg::geometry::topology::TetraList tetra(4, 5);
  auto acc = acg::access(tetra);
  acc(0) = acg::Vec4Index{0, 4, 1, 3};
  acc(1) = acg::Vec4Index{2, 1, 6, 3};
  acc(2) = acg::Vec4Index{7, 6, 4, 3};
  acc(3) = acg::Vec4Index{5, 4, 6, 1};
  acc(4) = acg::Vec4Index{3, 4, 6, 1};
  return tetra;
}

acg::types::PositionField<acg::F64, 3> make_pos() {
  auto pos = acg::FieldBuilder<acg::F64, 3>(8).Placeholder();
  auto acc = acg::access(pos);
  acc(0) = acg::Vec3d(0, 0, 0);
  acc(1) = acg::Vec3d(0, 1, 0);
  acc(2) = acg::Vec3d(1, 1, 0);
  acc(3) = acg::Vec3d(1, 0, 0);
  acc(4) = acg::Vec3d(0, 0, 1);
  acc(5) = acg::Vec3d(0, 1, 1);
  acc(6) = acg::Vec3d(1, 1, 1);
  acc(7) = acg::Vec3d(1, 0, 1);
  return pos;
}

acg::geometry::topology::TriangleList make_face() {
  auto tri = acg::FieldBuilder<acg::Index, 3>(12).Placeholder();
  auto acc = acg::access(tri);
  acc(0) = acg::Vec3Index(0, 1, 3);
  acc(1) = acg::Vec3Index(1, 2, 3);
  acc(2) = acg::Vec3Index(4, 0, 3);
  acc(3) = acg::Vec3Index(7, 4, 3);
  acc(4) = acg::Vec3Index(3, 6, 7);
  acc(5) = acg::Vec3Index(2, 6, 3);
  acc(6) = acg::Vec3Index(1, 6, 2);
  acc(7) = acg::Vec3Index(1, 5, 6);
  acc(8) = acg::Vec3Index(1, 4, 5);
  acc(9) = acg::Vec3Index(0, 4, 1);
  acc(10) = acg::Vec3Index(5, 4, 6);
  acc(11) = acg::Vec3Index(6, 4, 7);
  return tri;
}

int main(int argc, char** argv) {
  // Setup all the global instances.
  auto utils_hooker = acg::utils::UtilsHooker();
  utils_hooker.default_logging_level = spdlog::level::debug;
  utils_hooker.Hook();
  acg::gui::Window::Hooker{"ggui"}.Hook();
  auto vkctx_hooker = acg::gui::VkContext2Hooker{};
  vkctx_hooker.app_name = "GGui";
  vkctx_hooker.enable_validation = true;
  vkctx_hooker.Hook();
  VkGraphicsContext::Hooker().Hook();
  acg::init(argc, argv);
  GGui::Config config;
  config.init_default_scene = false;
  config.enable_blending = true;
  {
    GGui gui(config);
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
  }
  acg::clean_up();
  return EXIT_SUCCESS;
}
