#include <doctest/doctest.h>
#include <fmt/format.h>

#include <aphysics/mpm/transfer.hpp>
using namespace acg;
TEST_CASE("apic") {
  physics::LagrangeFluid<float, 3> lag;
  physics::EulerFluidRegular<float, 3> euler;
  lag.mass_.resize(1, 1);
  lag.mass_.setOnes();
  lag.position_.resize(3, 1);
  lag.position_.setZero();
  fmt::print("position = {}\n", lag.position_.transpose());
  lag.velocity_.resizeLike(lag.position_);
  lag.velocity_.setOnes();
  lag.volumn_.resizeLike(lag.mass_);
  euler.lower_bound_ = Vec3f(-2, -2, -2);
  euler.upper_bound_ = Vec3f(2, 2, 2);
  euler.div_count_ = Vec3Index(4, 4, 4);
  euler.grid_size_ = 1;
  euler.mass_.resize(1, 64);
  euler.velocity_.resize(3, 64);

  physics::mpm::ApicRegular<float, 3>
      apic(lag, euler);
  apic.Forward();
  // fmt::print("{}\n", euler.velocity_);
  apic.Backward();
  fmt::print("{}\n", lag.velocity_);
  apic.Forward();
  // fmt::print("{}\n", euler.velocity_);
  apic.Backward();
  fmt::print("{}\n", lag.velocity_);
  apic.Forward();
  // fmt::print("{}\n", euler.velocity_);
  apic.Backward();
  fmt::print("{}\n", lag.velocity_);
}
