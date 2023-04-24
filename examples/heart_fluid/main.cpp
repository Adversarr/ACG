#include <acore/geometry/common_models.hpp>
#include <acore/geometry/normal.hpp>
#include <acore/geometry/tetface.hpp>
#include <acore/math/func.hpp>
#include <acore/math/utilities.hpp>
#include <adata/common.hpp>
#include <adata/json/splash_surf_json.hpp>
#include <agui/gui.hpp>
#include <agui/init.hpp>
#include <aphysics/objects/fluid.hpp>
#include <autils/init.hpp>
#include <autils/time/timer.hpp>

#include "acore/math/coordinate/continuous_discrete.hpp"
#include "aphysics/mpm/transfer.hpp"

using namespace acg;
using namespace acg::gui;

using Scalar = Float32;
int density = 40;
Field<Scalar, 3> grad_field;
DiscreteStorageSequentialTransform<3> ds_tran({density, density, density});
BiasTransform<Scalar, 3> bias({-2, -2, -2});
ContinuousDiscreteTransformRegular<Scalar, 3> cd_tran(4.0 / density);

using APIC = physics::mpm::ApicRegular<Scalar, 3>;
physics::LagrangeFluid<Scalar, 3> lag;
physics::EulerFluidRegular<Scalar, 3> euler;
std::unique_ptr<APIC> apic;

// Prepare the gradient field.
void make_grad_field();

// Prepare the fluid.
void prepare_fluid();

// Step the simulation.
void step();

int main(int argc, char** argv) {
  acg::utils::hook_default_utils_environment();
  acg::gui::hook_default_gui_environment("Heart");
  acg::init(argc, argv);
  make_grad_field();
  prepare_fluid();

  auto& g = gui::Gui::Instance();
  auto* p_particles = g.GetScene().AddParticles();
  auto update_scene = [&]() {
    p_particles->SetPositions(lag.position_)
        .SetUniformColor({.7, .7, .7, 1})
        .SetRadius(3)
        .MarkUpdate();
    g.UpdateScene();
  };

  bool run = false;

  bool export_frame = false;
  g.SetUIDrawCallback([&]() -> void {
    ImGui::Checkbox("Run", &run);
    ImGui::Text("1");

    ImGui::Checkbox("Export", &export_frame);
  });

  update_scene();

  int frame = 0;
  while (!gui::Window::Instance().ShouldClose()) {
    g.Tick(true);
    g.RenderOnce();

    if (run) {
      step();
      update_scene();

      if (export_frame) {
        if (frame % 3 == 0) {
          std::ofstream out(fmt::format("heart/h_{}.json", frame / 3));
          data::SplashSurfJson jsonexp(lag.position_.cast<Float64>());
          jsonexp.ExportTo(out);
        }
        frame += 1;
      }
    }
  }

  acg::clean_up();
  return 0;
}
inline float func(float x, float y, float z) {
  float x2 = x * x, y2 = 2.25f * y * y, z2 = z * z;
  float t = (x2 + y2 + z2 - 1);
  t = t * t * t;
  return t - (x2 + 0.025f * y2) * z2 * z;
}

// Gradient field, to form the Heart Shape.
void make_grad_field() {
  grad_field.resize(3, math::pow<3>(density));
  auto gfview = view(grad_field, ds_tran);
  for (auto [i, j, k] : NdRange(density, density, density)) {
    Vec3<Scalar> p = bias.Forward(cd_tran.Backward({i, j, k}));
    auto cse = math::square(-1 + p.x() * p.x() + 2.25 * p.y() * p.y() + p.z() * p.z());
    Vec3<Scalar> grad = {-2 * p.x() * math::pow<3>(p.z()) + 6 * p.x() * cse,
                         -p.y() * p.z() * 0.05 + 13.5 * p.y() * cse,
                         -3 * (math::square(p.x()) + .025 * math::square(p.y())) + 6 * p.z() * cse};
    Scalar min_leng = 5;
    grad.normalize();
    grad *= min_leng;
    auto val = func(p.x(), p.y(), p.z());

    if (val > 0) {
      gfview(i, j, k) = -grad;
    } else {
      gfview(i, j, k) = .1 * grad;
    }
  }
}

void prepare_fluid() {
  int n_part = density * density * 10;
  lag.mass_.resize(n_part);
  lag.position_.resize(3, n_part);
  lag.position_.setRandom();
  lag.position_ *= 1.9;
  lag.velocity_.resize(3, n_part);
  lag.velocity_.setRandom();

  euler.velocity_.resize(3, density * density * density);
  euler.div_count_.setConstant(density);
  euler.grid_size_ = 4.0 / density;
  euler.lower_bound_.setConstant(-2);
  euler.upper_bound_.setConstant(2);
  euler.mass_.resize(density * density * density);
  apic = std::make_unique<APIC>(lag, euler);
}

void step() {
  lag.position_ += lag.velocity_ * 0.001;
  for (auto v : view(lag.position_)) {
    v.x() = std::clamp<float>(v.x(), -1.9, 1.9);
    v.y() = std::clamp<float>(v.y(), -1.9, 1.9);
    v.z() = std::clamp<float>(v.z(), -1.9, 1.9);
  }
  apic->Forward();
  euler.velocity_ += grad_field * .4;
  euler.velocity_ *= 0.99;
  apic->Backward();
}
