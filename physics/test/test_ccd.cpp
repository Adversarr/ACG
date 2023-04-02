#include <Eigen/Geometry>
#include <acore/math/utilities.hpp>
#include <aphysics/collision/detect.hpp>
#include <aphysics/collision/distance.hpp>
#include <doctest/doctest.h>
#include <fmt/core.h>
using namespace acg;

TEST_CASE("ccd") {
  physics::ccd::VertexTriangle<float> vt;
  Vec3f f0{0, 1, 1};
  Vec3f f1{1, 0, 1};
  Vec3f f2{1, 1, 1};
  Vec3f v{.5, 0, 1};
  Vec3f vend{.5, 2, 1};
  vt(v, f0, f1, f2, vend, f0, f1, f2);
  CHECK(vt.valid_);
  fmt::print("toi {}\n", vt.toi_);
}

TEST_CASE("distance") {
  Vec3f f0{0, 1, 1};
  Vec3f f1{1, 0, 1};
  Vec3f f2{1, 1, 1};
  Vec3f v{0, 0, 2};
  physics::ccd::VertexTriangleNormalDistance<float> vt(f0, f1, f2, v);
  fmt::print("value {}, grad {}\n", vt.Value(), vt.Grad());
}