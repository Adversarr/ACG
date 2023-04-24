#include <doctest/doctest.h>

#include <acore/math/kernels/linear.hpp>
#include <acore/math/utilities.hpp>
#include <iostream>

#include <acore/math/kernels/bsplines.hpp>

TEST_CASE("LinearInterpolation") {
  acg::math::Lerp<double> interp;
  using namespace acg;
  auto result = interp(Vec2d{.3, .3});
  std::cout << result << std::endl;
  fmt::print("Interp .5 = {}\n", interp(.5).transpose());
  fmt::print("Interp [.5 .5] = {}\n", interp(Vec3d{.3, .3, .3}).transpose());
}

using namespace acg::math;
using namespace acg;
TEST_CASE("CubicBSpline") {
  acg::math::CubicBSpline<float> bsp;
  std::cout << bsp(constant<acg::Float32>(1)) << std::endl;
  std::cout << bsp((Vec2f(0, 0) * 2).eval()) << std::endl;
}
