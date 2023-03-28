#include <acore/geometry/common_models.hpp>
#include <acore/geometry/normal.hpp>
#include <acore/geometry/tetface.hpp>
#include <acore/math/func.hpp>
#include <acore/spatial/subdivision.hpp>
#include <adata/common.hpp>
#include <adata/triangle/ele_loader.hpp>
#include <adata/triangle/node_loader.hpp>
#include <agui/gui.hpp>
#include <agui/init.hpp>
#include <autils/init.hpp>
#include <autils/time/timer.hpp>

#include "app.hpp"
using namespace acg;

int main(int argc, char **argv) {
  using namespace gui;
  app::HybredApp app;
  acg::utils::hook_default_utils_environment();
  acg::gui::hook_default_gui_environment("Hybred Application");
  acg::init(argc, argv);

  auto &gui = Gui::Instance();
  auto *mesh = gui.GetScene().AddMesh();

  auto *cloth = gui.GetScene().AddMesh();
  int plane_density = 10;
  auto plane =
      geometry::make_plane_xy(plane_density).Cast<app::HybredApp::Scalar>();
  app.AddCloth(
      plane.GetVertices(), plane.GetFaces(),
      1.0 / (math::square(plane_density)) *
          Field<app::HybredApp::Scalar>::Ones(plane.GetVertices().cols()),
      10.0 * plane_density);


  auto data_path = acg::data::get_data_dir();
  std::ifstream ele_file(data_path + "/house-ele-node/house.ele");
  std::ifstream node_file(data_path + "/house-ele-node/house.node");
  auto house_ele = acg::data::triangle::EleLoader(ele_file, false);
  auto house_node = acg::data::triangle::NodeLoader(node_file);
  house_ele.Load();
  house_node.Load();
  ACG_INFO("Ele load {} x {}, ", house_ele.GetData().rows(),
           house_ele.GetData().cols());
  ACG_INFO("Node load {} x {}, ", house_node.GetData().rows(),
           house_node.GetData().cols());
  auto tet = house_ele.GetData().cast<acg::Index>();
  tet.array() -= house_node.GetOffset();
  app.AddSoftbody(
      house_node.GetData().cast<app::HybredApp::Scalar>(), tet,
      Field<app::HybredApp::Scalar>::Ones(house_node.GetData().cols()), 0.5, 2000);

  physics::PositionStaticConstraint<app::HybredApp::Scalar, 3> constraint(
      physics::PhysicsObject(physics::PhysicsObjectType::kCloth, 0, 0),
      plane.GetVertices().col(0));
  app.constraints_.push_back(constraint);
  constraint.object_.id_ = plane_density - 1;
  constraint.value_ = plane.GetVertices().col(constraint.object_.id_);
  app.constraints_.push_back(constraint);

  constraint.object_.id_ = plane_density * plane_density - 1;
  constraint.value_ = plane.GetVertices().col(constraint.object_.id_);
  app.constraints_.push_back(constraint);
  constraint.object_.id_ = plane_density * plane_density - plane_density;
  constraint.value_ = plane.GetVertices().col(constraint.object_.id_);
  app.constraints_.push_back(constraint);

  auto update_scene = [&]() {
    cloth->SetVertices(app.cloth_.front().data_.position_)
        .SetIndices(app.cloth_.front().data_.face_)
        .ComputeDefaultNormal()
        .SetUniformColor(types::Rgba{.7, .7, .7, 1})
        .SetEnableWireframe()
        .MarkUpdate();

    auto& sposition = app.softbody_.front().data_.position_;
    auto face = acg::geometry::Tet2Face<app::HybredApp::Scalar>{
        app.softbody_.front().data_.position_,
        app.softbody_.front().data_.tetras_};
    face();
    using namespace acg;
    auto position = Field<Float32, 3>(3, face.faces_.cols() * 3);
    auto faces = Field<Index, 3>(3, face.faces_.cols());
    for (auto [i, f] : enumerate(view(face.faces_))) {
      faces.col(i) = Vec3Index{3 * i, 3 * i + 1, 3 * i + 2};
      position.col(3 * i) = view(sposition)(f.x()).cast<Float32>();
      position.col(3 * i + 1) = view(sposition)(f.y()).cast<Float32>();
      position.col(3 * i + 2) = view(sposition)(f.z()).cast<Float32>();
    }
    mesh->SetVertices(position)
        .SetIndices(faces)
        .SetUniformColor(acg::types::Rgba{.7, .7, .7, 1})
        .ComputeDefaultNormal()
        .SetEnableWireframe(true)
        .MarkUpdate();
    
    gui.UpdateScene();
  };

  bool running = false;
  bool run_once = false;
  gui.SetUIDrawCallback([&]() -> void {
    ImGui::Checkbox("Run", &running);
    run_once = ImGui::Button("Run Once");
  });

  update_scene();
  while (!Window::Instance().ShouldClose()) {
    gui.Tick(true);
    gui.RenderOnce();

    if (running || run_once) {
      app.Step();
      update_scene();
    }
  }
  acg::clean_up();
  return 0;
}
