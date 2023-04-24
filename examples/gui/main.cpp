#include <acore/geometry/common_models.hpp>
#include <acore/init.hpp>
#include <agui/backend/context.hpp>
#include <agui/backend/graphics_context.hpp>
#include <agui/backend/window.hpp>
#include <agui/gui.hpp>
#include <agui/imnodes/imnodes.hpp>
#include <autils/init.hpp>
#include <autils/time/time.hpp>

using namespace acg::gui;

int main(int argc, char **argv) {
  // Setup all the global instances.
  auto utils_hooker = acg::utils::UtilsHooker();
  utils_hooker.default_logging_level = spdlog::level::debug;
  utils_hooker.Hook();
  acg::gui::Window::Hooker{"GGui"}.Hook();
  auto vkctx_hooker = acg::gui::VkContext2Hooker{};
  vkctx_hooker.app_name = "GGui";
  vkctx_hooker.enable_validation = true;
  vkctx_hooker.Hook();
  VkGraphicsContext::Hooker().Hook();
  Gui::Config config;
  config.init_default_scene = false;
  config.enable_blending = true;
  config.Hook();
  acg::init(argc, argv);
  auto &gui = Gui::Instance();
  bool clear = false;
  gui.SetUIDrawCallback([&clear]() {
    ImGui::Text("Hello world!");
    clear = ImGui::Button("Clear Scene!");
  });

  gui.SetNodeDrawCallback([]() -> void {
    ImGui::Begin("node editor");
    ImNodes::BeginNodeEditor();
    ImNodes::BeginNode(1);
    const int output_attr_id = 1;
    ImGui::Dummy(ImVec2(80.0f, 45.0f));
    ImNodes::BeginOutputAttribute(output_attr_id);
    ImGui::Text("output pin");
    ImNodes::EndOutputAttribute();
    ImNodes::EndNode();
    ImNodes::BeginNode(2);
    ImGui::Dummy(ImVec2(80.0f, 45.0f));
    ImGui::Text("input pin");
    ImNodes::BeginInputAttribute(2);
    ImNodes::EndInputAttribute();
    ImNodes::EndNode();

    ImNodes::BeginNode(3);
    ImGui::Dummy(ImVec2(80.0f, 45.0f));
    ImGui::Text("input pin");
    ImNodes::BeginInputAttribute(3);
    ImNodes::EndInputAttribute();
    ImNodes::EndNode();
    ImNodes::Link(0, 1, 2);
    ImNodes::Link(1, 1, 3);
    ImNodes::EndNodeEditor();
    ImGui::End();
  });

  auto *cloth = gui.GetScene().AddMesh();
  auto plane = acg::geometry::make_plane_xy(10).Cast<float>();
  cloth->SetIndices(plane.GetFaces())
      .SetVertices(plane.GetVertices())
      .ComputeDefaultNormal()
      .SetUniformColor(acg::types::Rgba(.7, .7, .7, 1))
      .SetEnableWireframe()
      .MarkUpdate();
  auto v = plane.GetVertices().eval();
  v.array().row(2) += 1.0;

  auto *cloth2 = gui.GetScene().AddMesh();
  cloth2->SetIndices(plane.GetFaces())
      .SetVertices(v)
      .ComputeDefaultNormal()
      .SetUniformColor(acg::types::Rgba(.7, .7, .7, 1))
      .SetEnableWireframe()
      .MarkUpdate();
  gui.UpdateScene();
  while (!Window::Instance().ShouldClose()) {
    glfwPollEvents();
    gui.Tick();
    gui.RenderOnce();
    if (clear) {
      gui.ClearScene();
    }
  }
  acg::clean_up();
  return EXIT_SUCCESS;
}
