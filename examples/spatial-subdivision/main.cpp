#include <acore/init.hpp>
#include <acore/spatial/subdivision.hpp>
#include <agui/gui.hpp>
#include <agui/init.hpp>
#include <autils/init.hpp>
#include <autils/time/time.hpp>

using namespace acg;
using namespace acg::gui;

int main(int argc, char** argv) {
  // Setup all the global instances.
  acg::utils::hook_default_utils_environment();
  acg::gui::hook_default_gui_environment("Spatial Subdivision");
  acg::init(argc, argv);
  spatial::SubDivisionAABB<Float, Index, 3, 2, 7> ss;
  auto& gui = acg::gui::Gui::Instance();
  auto& wireframe = gui.GetScene().AddWireframe();
  auto& particles_render = gui.GetScene().AddMeshParticles();

  Field<Float, 3> particles;
  Float particles_size = 0.01;

  bool regenerate = false;
  int seed_count = 10;
  gui.SetUIDrawCallback([&]() -> void {
    ImGui::InputInt("Seed count", &seed_count);
    regenerate = ImGui::Button("Regenerate");
    ImGui::InputFloat("Particle Size", &particles_size);
  });

  auto update = [&]() {
    particles_render.SetRadius(particles_size)
        .SetUseInstanceRendering(true)
        .SetUniformColor(types::Rgba{0.7, 0, 0, 1})
        .SetPositions(particles)
        .MarkUpdate();
    auto [position, lines] = ss.Visualize();
    wireframe.SetPositions(position);
    wireframe.SetIndices(lines);
    wireframe.SetColors(types::Rgb{.7, .7, .7});
    wireframe.MarkUpdate();
    gui.UpdateScene();
  };
  while (!Window::Instance().ShouldClose()) {
    gui.Tick(true);
    gui.RenderOnce();

    if (regenerate) {
      particles.resize(3, seed_count);
      particles.setRandom();
      ss.Clear();
      for (auto [i, v]: acg::enumerate(acg::access(particles))) {
        ss.Insert({v - Vec3f::Constant(particles_size), v + Vec3f::Constant(particles_size), 
            1});
      }
      update();
    }
  }

  acg::clean_up();
  return 0;
}
