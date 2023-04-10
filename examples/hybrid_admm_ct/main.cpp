#include <acore/geometry/common_models.hpp>
#include <acore/geometry/normal.hpp>
#include <acore/geometry/tetface.hpp>
#include <acore/math/func.hpp>
#include <adata/common.hpp>
#include <adata/json/splash_surf_json.hpp>
#include <adata/triangle/ele_loader.hpp>
#include <adata/triangle/node_loader.hpp>
#include <agui/gui.hpp>
#include <agui/init.hpp>
#include <autils/init.hpp>
#include <autils/time/timer.hpp>

#include "adata/obj_export.hpp"
#include "agui/perfacenormal.hpp"
#include "app.hpp"

using namespace acg;

int main(int argc, char **argv) {
  using namespace gui;
  app::HybridAdmmCt app;
  acg::utils::hook_default_utils_environment();
  acg::gui::hook_default_gui_environment("Hybred Application");
  acg::init(argc, argv);

  acg::utils::set_default_log_level(spdlog::level::debug);

  auto &gui = Gui::Instance();

  app.ground_constraints_.z_value_ = -0.9;
  app.wall_x_low_.value_ = -.5;
  app.wall_y_low_.value_ = -.5;
  app.wall_x_high_.value_ = 1.5;
  app.wall_y_high_.value_ = 1.5;

  int export_every_x_frame = 1;
  auto *cloth = gui.GetScene().AddMesh();
  int plane_density = 7;
  auto plane = geometry::make_plane_xy(plane_density).Cast<app::HybridAdmmCt::Scalar>();
  auto position = plane.GetVertices();
  position *= 0.8;
  position.array().row(0) += 0.1;
  position.array().row(1) += 0.1;
  position.array().row(2) += .3;

  // Apply a transform
  for (auto v : view(position)) {
    v.z() += 0.3 * v.y();
  }

  app.AddCloth(position, plane.GetFaces(),
               1e-2 * Field<app::HybridAdmmCt::Scalar>::Ones(plane.GetVertices().cols()),
               10 * plane_density * plane_density);

  for (auto [i] : NdRange(plane_density)) {
    physics::PositionStaticConstraint<app::HybridAdmmCt::Scalar, 3> constraint(
        physics::PhysicsObject(physics::PhysicsObjectType::kCloth, 0, i), position.col(i));
    app.position_constraints_.push_back(constraint);
    physics::PositionStaticConstraint<app::HybridAdmmCt::Scalar, 3> constraint2(
        physics::PhysicsObject(physics::PhysicsObjectType::kCloth, 0,
                               plane_density * plane_density - 1 - i),
        position.col(plane_density * plane_density - 1 - i));
    app.position_constraints_.push_back(constraint2);
  }

  // for (auto [i] : NdRange(plane_density)) {
  //   physics::PositionStaticConstraint<app::HybridAdmmApp::Scalar, 3>
  //   constraint(
  //       physics::PhysicsObject(physics::PhysicsObjectType::kCloth, 0,
  //       plane_density * plane_density - i - 1), position.col(plane_density *
  //       plane_density - i - 1));
  //   app.position_constraints_.push_back(constraint);
  // }

  physics::LagrangeFluid<app::HybridAdmmCt::Scalar, 3> fluid;
  // 1 kg.
  Index fluid_particle_num = 5000;
  fluid.mass_.setConstant(fluid_particle_num, 1e-3);
  // 1 dm3
  fluid.position_.setRandom(Eigen::NoChange, fluid_particle_num);
  fluid.position_.array() *= .2;
  fluid.position_.array() += .5;
  fluid.position_.array().row(2) += 0.5;
  // fluid.position_.row(2).array() += .15;
  fluid.position_.row(2) = fluid.position_.row(2).array().cwiseMin(1.49);
  fluid.velocity_.setZero(Eigen::NoChange, fluid_particle_num);
  // Rho for water = 1e3
  fluid.rho_ = 1e3;
  app.SetFluid(fluid, 20);
  // 1cm3
  app.fluid_->pressure_scale_ = 3;

  auto *particles = gui.GetScene().AddMeshParticles();

  // auto data_path = acg::data::get_data_dir();
  // std::ifstream ele_file(data_path + "/house-ele-node/house.ele");
  // std::ifstream node_file(data_path + "/house-ele-node/house.node");
  // auto house_ele = acg::data::triangle::EleLoader(ele_file, false);
  // auto house_node = acg::data::triangle::NodeLoader(node_file);
  // house_ele.Load();
  // house_node.Load();
  // ACG_INFO("Ele load {} x {}, ", house_ele.GetData().rows(),
  //          house_ele.GetData().cols());
  // ACG_INFO("Node load {} x {}, ", house_node.GetData().rows(),
  //          house_node.GetData().cols());
  // auto tet = house_ele.GetData().cast<acg::Index>();
  // tet.array() -= house_node.GetOffset();
  // auto *mesh = gui.GetScene().AddMesh();
  // app.AddSoftbody(
  //     house_node.GetData().cast<app::HybridAdmmCt::Scalar>() * 0.1, tet,
  //     Field<app::HybridAdmmCt::Scalar>::Ones(house_node.GetData().cols()),
  //     0, 200);

  auto update_scene = [&]() {
    cloth->SetVertices(app.cloth_.front().data_.position_.cast<float>())
        .SetIndices(app.cloth_.front().data_.face_)
        .ComputeDefaultNormal()
        .SetUniformColor(types::Rgba{.7, .7, .7, 1})
        .SetEnableWireframe()
        .MarkUpdate();
    particles->SetPositions(app.fluid_->data_.position_.cast<float>())
        .SetUniformColor(types::Rgba{.7, .3, .3, 1})
        .SetRadius(.02)
        .MarkUpdate();

    // auto sposition = app.softbody_.front().data_.position_.cast<float>();
    // auto face = acg::geometry::Tet2Face<float>{
    //     sposition, app.softbody_.front().data_.tetras_};
    // face();
    // using namespace acg;
    // auto position = Field<Float32, 3>(3, face.faces_.cols() * 3);
    // auto faces = Field<Index, 3>(3, face.faces_.cols());
    // auto pnt = gui::PerfaceNormalTransform(
    //     app.softbody_.front().data_.position_.cast<Float32>(), face.faces_);
    // mesh->SetVertices(pnt.out_position_)
    //     .SetIndices(pnt.out_triangle_list_)
    //     .SetUniformColor(acg::types::Rgba{.7, .7, .7, 1})
    //     .ComputeDefaultNormal()
    //     .SetEnableWireframe(true)
    //     .MarkUpdate();
    gui.UpdateScene();
  };

  bool running = false;
  bool run_once = false;

  bool export_frame = false;
  gui.SetUIDrawCallback([&]() -> void {
    ImGui::Checkbox("Enable collision", &app.enable_collision_detect_);
    ImGui::Checkbox("Run", &running);
    run_once = ImGui::Button("Run Once");
    ImGui::Checkbox("Enable result check", &app.enable_result_check_);
    ImGui::Checkbox("Enable Subd", &app.enable_subd_);
    if (app.fluid_) {
      ImGui::InputDouble("Ground z", &app.fluid_->pressure_scale_);
    }
    ImGui::InputInt("Quasi Iteration Steps", &app.max_iteration_);
    ImGui::Checkbox("Export Frame", &export_frame);
    ImGui::Checkbox("Check subd result", &app.check_subdivision_result_);
    ImGui::InputInt("Export frame Per frame. ", &export_every_x_frame);
  });

  app.Init();
  update_scene();

  int c = 0;
  while (!Window::Instance().ShouldClose()) {
    gui.Tick(true);
    gui.RenderOnce();

    if (running || run_once) {
      app.Step();
      update_scene();
    }

    if (export_frame) {
      if (c % export_every_x_frame == 0) {
        acg::data::SplashSurfJson json_exportor(app.fluid_->data_.position_);
        std::ofstream of(
            fmt::format("./output/fluid/output_fluid_{}.json", c / export_every_x_frame),
            std::ios::out);
        ACG_CHECK(of.good(), "Failed to create/open output file.");
        json_exportor.ExportTo(of);

        acg::data::ObjExport obj;
        obj.position_ = app.cloth_.front().data_.position_;
        obj.triangle_ = app.cloth_.front().data_.face_;
        std::ofstream of2(
            fmt::format("./output/cloth/output_cloth_{}.obj", c / export_every_x_frame));
        ACG_CHECK(of2.good(), "Failed to open cloth.");
        obj.ExportTo(of2);
        ACG_INFO("Export done. frame = {}", c / export_every_x_frame);
      }
      c += 1;
    }
  }
  acg::clean_up();
  return 0;
}
