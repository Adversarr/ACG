#include <doctest/doctest.h>
#include <aphysics/ccd/detect.hpp>
#include <fmt/core.h>
#include <Eigen/Geometry>
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