#include <acore/init.hpp>
#include <adata/json/splash_surf_json.hpp>
#include <agui/backend/context.hpp>
#include <agui/backend/graphics_context.hpp>
#include <agui/backend/window.hpp>
#include <agui/gui.hpp>
#include <autils/init.hpp>
#include <autils/time/time.hpp>
#include <fstream>

#include "app.hpp"

using namespace acg::gui;

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
  Gui::Config config;
  config.init_default_scene = false;
  config.enable_blending = true;
  {
    Gui gui(config);
    bool clear = false;
    bool running = false;
    bool run_once = false;
    bool export_frame = false;
    MpmExplictApp app;
    app.Init();
    gui.SetUIDrawCallback([&]() {
      clear = ImGui::Button("Reset Scene.");
      // running = ImGui::Button("Run Once");
      ImGui::Checkbox("Run", &running);
      run_once = ImGui::Button("Run once");
      acg::Float64 vel_eng = app.lag_.velocity_.array().square().sum();
      Vec3f pos = app.lag_.position_.rowwise().mean().cast<Float32>();
      ImGui::Text("Velocity Energy = %lf, Weight %lf", vel_eng, app.weight_sum_);
      ImGui::InputFloat3("Position", pos.data());
      ImGui::DragFloat3("Gravity", app.grav_.data(), 0.1, -3, 3);
      ImGui::InputDouble("Scaling", &app.E_);
      ImGui::InputDouble("Rho 0", & app.rho0_);

      export_frame = ImGui::Button("Export this frame");
    });

    auto* mp = gui.GetScene().AddMeshParticles();
    mp->SetPositions(app.particle_position_.cast<Float32>())
        .SetUniformColor(types::Rgba(1, 0, 0, 1))
        .SetRadius(0.01)
        .MarkUpdate();
    while (!Window::Instance().ShouldClose()) {
      glfwPollEvents();
      gui.Tick();
      gui.RenderOnce();
      gui.UpdateScene();
      if (clear) {
        app.Init();
        mp->SetPositions(app.lag_.position_.cast<Float32>()).MarkUpdate();
      }

      if (export_frame) {
        acg::data::SplashSurfJson json_exportor(app.lag_.position_);
        std::ofstream of("./output.json", std::ios::out);
        ACG_CHECK(of.good(), "Failed to create/open output file.");
        json_exportor.ExportTo(of);

        ACG_INFO("Export done.");
      }

      if (running || run_once) {
        app.Step();
        mp->SetPositions(app.lag_.position_.cast<Float32>()).MarkUpdate();
        // running = false;
      }
    }
  }
  acg::clean_up();
  return EXIT_SUCCESS;
}
