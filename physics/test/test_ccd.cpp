#include <Eigen/Geometry>
#include <acore/math/utilities.hpp>
#include <aphysics/collision/detect.hpp>
#include <aphysics/collision/distance.hpp>
#include <doctest/doctest.h>
#include <fmt/core.h>
using namespace acg;

TEST_CASE("ccd") {
  physics::ccd::VertexTriangleDynamic<float> vt;
  Vec3f f0{0, 1, 1};
  Vec3f f1{1, 0, 1};
  Vec3f f2{1, 1, 1};
  Vec3f v{.7, .7, 0};
  Vec3f vend{.7, 0.7, 10};
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

TEST_CASE("distance-pp") {
  Vec3f f0{0, 0, 1};
  Vec3f f1{1, 0, 1};
  Vec3f e0{.5, 0, 1};
  Vec3f e1{.5, 0, 1};
  physics::ccd::VertexVertexDynamic<float> vv;
  vv.min_distance_ = 0.00;
  vv(f0, f1, e0, e1);
  fmt::print("valid {}, toi {}\n", int(vv.valid_), vv.toi_);
}


TEST_CASE("distance-vt-static") {
  Vec3f f0{0, 1, 1};
  Vec3f f1{1, 0, 1};
  Vec3f f2{1, 1, 1};
  Vec3f e1{.5, .7, 1.001};
  physics::ccd::VertexTriangleStatic<float> vt;
  vt(f0, f1, f2, e1);
  fmt::print("valid {}\n", int(vt.valid_));
}