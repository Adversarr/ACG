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

  auto p = ss.subdivision_count_total_;
  while (!Window::Instance().ShouldClose()) {
    gui.Tick(true);
    gui.RenderOnce();

    if (regenerate) {
      particles.resize(3, seed_count);
      particles.setRandom();
      ss.Clear();
      ACG_INFO("{}", fmt::streamed(particles));
      for (auto [i, v] : acg::enumerate(acg::access(particles))) {
        spatial::AABB<Index, Float, 3> aabb(v - Vec3f::Constant(particles_size),
                                            v + Vec3f::Constant(particles_size), i);
        ss.Insert(aabb);
        ACG_INFO("Particle {}", i);
        ACG_INFO("Lower bound = {}", fmt::streamed(aabb.lower_bound.transpose()));
        ACG_INFO("Upper bound = {}", fmt::streamed(aabb.upper_bound.transpose()));

        auto seq = ss.FindVisitSequence(aabb);
        auto cur = seq.front();
        ACG_INFO("Entry = {}", cur);
        for (int i = 1; i < 7; ++i) {
          auto id = seq[i];
          if (id != InvalidSubscript) {
            ACG_INFO("For depth = {}, visit subnode #{}, subnode id = {}", i - 1, id, ss.nodes_[cur].sub_nodes_[id]);
            cur = ss.nodes_[cur].sub_nodes_[id];
            std::cout << "\t" << ss.nodes_[cur].box_.lower_bound.transpose() << std::endl;
            std::cout << "\t" << ss.nodes_[cur].box_.upper_bound.transpose() << std::endl;
          }
        }
      }
      update();
    }
  }

  acg::clean_up();
  return 0;
}
