#include <acore/geometry/tetface.hpp>
#include <acore/init.hpp>
#include <adata/common.hpp>
#include <adata/triangle/ele_loader.hpp>
#include <adata/triangle/node_loader.hpp>
#include <agui/gui.hpp>
#include <agui/init.hpp>
#include <autils/init.hpp>
#include <fstream>

#include "app.hpp"
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

acg::types::PositionField<acg::Float32, 3> make_pos() {
  auto pos = acg::FieldBuilder<acg::Float32, 3>(8).Placeholder();
  auto acc = acg::access(pos);
  acc(0) = acg::Vec3f(0, 0, 0);
  acc(1) = acg::Vec3f(0, 1, 0);
  acc(2) = acg::Vec3f(1, 1, 0);
  acc(3) = acg::Vec3f(1, 0, 0);
  acc(4) = acg::Vec3f(0, 0, 1);
  acc(5) = acg::Vec3f(0, 1, 1);
  acc(6) = acg::Vec3f(1, 1, 1);
  acc(7) = acg::Vec3f(1, 0, 1);
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

auto make_tet_simple() {
  acg::geometry::topology::TetraList tetra(4, 1);
  auto acc = acg::access(tetra);
  acc(0) = acg::Vec4Index{0, 1, 2, 3};
  return tetra;
}

auto make_face_simple() {
  acg::geometry::topology::TriangleList faces(3, 4);
  auto acc = acg::access(faces);

  acc(0) = acg::Vec3Index(0, 2, 1);
  acc(1) = acg::Vec3Index(0, 1, 3);
  acc(2) = acg::Vec3Index(3, 1, 2);
  acc(3) = acg::Vec3Index(0, 2, 3);
  return faces;
}

auto make_position_simple() {
  acg::types::PositionField<float, 3> pos(3, 4);
  auto acc = acg::access(pos);
  acc(0) = acg::Vec3f{0, 0, 0};
  acc(1) = acg::Vec3f{1, 0, 0};
  acc(2) = acg::Vec3f{0, 1, 0};
  acc(3) = acg::Vec3f{0, 0, 1};
  return pos;
}

int main(int argc, char** argv) {
  acg::utils::hook_default_utils_environment();
  acg::gui::hook_default_gui_environment("Fem-Implicit");
  acg::init(argc, argv);
  auto& gui = acg::gui::Gui::Instance();
  auto& window = acg::gui::Window::Instance();
  auto& mesh_render = gui.GetScene().AddMesh();
  auto data_path = acg::data::get_data_dir();
  std::ifstream ele_file(data_path + "/house-ele-node/house.ele");
  std::ifstream node_file(data_path + "/house-ele-node/house.node");
  std::cout << data_path << std::endl;
  std::cout << ele_file.good() << std::endl;
  auto house_ele = acg::data::triangle::EleLoader(ele_file, false);
  auto house_node = acg::data::triangle::NodeLoader(node_file);
  house_ele.Load();
  house_node.Load();

  ACG_INFO("Ele load {} x {}, ", house_ele.GetData().rows(), house_ele.GetData().cols());
  ACG_INFO("Node load {} x {}, ", house_node.GetData().rows(), house_node.GetData().cols());
  std::cout << house_node.GetData().array().maxCoeff() << std::endl;
  std::cout << house_node.GetData().array().minCoeff() << std::endl;
  app::FemImplicitApp app;

  // app.position_ = make_pos();
  // app.tetras_ = make_tetra();
  app.position_ = house_node.GetData().cast<float>();
  app.tetras_ = house_ele.GetData().cast<acg::Index>();
  app.tetras_.array() -= 1;
  app.position_ *= 0.1;
  app.rest_position_ = app.position_;
  app.Init();
  bool reset = false;
  auto update_scene = [&]() {
    auto face = acg::geometry::Tet2Face<float>{app.position_, app.tetras_};
    face();
    mesh_render.SetVertices(app.position_)
        .SetIndices(face.faces_)
        .SetUniformColor(acg::types::Rgba{.7, .3, .3, 1})
        .ComputeDefaultNormal()
        .SetEnableWireframe(true)
        .MarkUpdate();
    gui.UpdateScene();
  };

  bool run_once, run;
  gui.SetUIDrawCallback([&]() -> void {
    run_once = ImGui::Button("Run Once");
    ImGui::Checkbox("Run", &run);

    reset = false;
    reset |= ImGui::InputFloat3("x0", app.position_.col(0).data());
    reset |= ImGui::InputFloat3("x1", app.position_.col(1).data());
    reset |= ImGui::InputFloat3("x2", app.position_.col(2).data());
    reset |= ImGui::InputFloat3("x3", app.position_.col(3).data());
    reset |= ImGui::InputFloat("Lambda", &app.lambda_);
    reset |= ImGui::InputFloat("mu", &app.mu_);
    reset |= ImGui::Checkbox("Explicit solver", &app.explicit_);
    reset |= ImGui::InputInt("Maximum Iterate Steps", &app.steps_);
    if (ImGui::Button("Reset Rest Pose.")) {
      app.rest_position_ = app.position_;
    }
    ImGui::Checkbox("Stvk", &app.use_stvk_);
  });

  update_scene();
  while (!window.ShouldClose()) {
    gui.Tick(true);
    gui.RenderOnce();
    if ((run_once | run) && (!reset)) {
      app.Step();
      update_scene();
    }

    if (reset) {
      ACG_INFO("Reset...");
      app.Init();
      update_scene();
    }
  }
  acg::clean_up();
  return EXIT_SUCCESS;
}
