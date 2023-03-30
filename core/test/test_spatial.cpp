#include <doctest/doctest.h>

#include <acore/spatial/subdivision.hpp>
#include <iostream>

using namespace acg;
TEST_CASE("SpatialSubdivision") {
  acg::spatial::SubDivisionAABB<Float, Index, 3, 2, 7> sd;
  spatial::AABB<float, 3> data(Vec3f::Constant(0.3), Vec3f::Constant(0.31));
  spatial::AABB<float, 3> data2(Vec3f::Constant(0.2), Vec3f::Constant(0.21));
  spatial::AABB<float, 3> aabb(Vec3f::Zero(), Vec3f::Ones());
  auto ub_ceil = aabb.upper_bound.array().ceil().eval();
  auto lb_floor = aabb.lower_bound.array().floor().eval();
  CHECK((ub_ceil - 1 == lb_floor).all());
  auto idx = acg::make_tuple_from_vector(Vec3f(1, 2, 3));
  CHECK(std::get<0>(idx) == 1);
  CHECK(std::get<1>(idx) == 2);
  CHECK(std::get<2>(idx) == 3);
}
