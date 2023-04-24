#include <doctest/doctest.h>
#include <acore/math/utilities.hpp>
#include <acore/spatial/subdivision.hpp>
#include <acore/spatial/octree.hpp>
#include <iostream>

using namespace acg;

TEST_CASE("SpatialSubdivision") {
  acg::spatial::SubDivisionAABB<Float, Index, 3, 2, 7> sd;
  spatial::AABB<float, 3> data(Vec3f::Constant(0.3), Vec3f::Constant(0.31));
  spatial::AABB<float, 3> data2(Vec3f::Constant(0.2), Vec3f::Constant(0.21));
  spatial::AABB<float, 3> aabb(Vec3f::Zero(), Vec3f::Ones());
  auto ub_ceil = aabb.upper_bound_.array().ceil().eval();
  auto lb_floor = aabb.lower_bound_.array().floor().eval();
  CHECK((ub_ceil - 1 == lb_floor).all());
  auto idx = acg::make_tuple_from_vector(Vec3f(1, 2, 3));
  CHECK(std::get<0>(idx) == 1);
  CHECK(std::get<1>(idx) == 2);
  CHECK(std::get<2>(idx) == 3);
}

TEST_CASE("Octree") {
  acg::spatial::AlignedBox<float, 3> box(Vec3f::Zero(), Vec3f::Ones());
  acg::spatial::BoundedOctree<float, 3> bo(box);
  bo.Insert({Vec3f::Zero(), Vec3f::Ones() * 0.1}, 0);
  bo.Insert({Vec3f::Ones() * 0.5, Vec3f::Ones() * 0.6}, 1);
  auto ret = bo.Query({Vec3f::Zero(), Vec3f::Ones()});
  for (auto id: ret) {
    fmt::print("Result= {}\n", id);
  }

  ret = bo.Query({Vec3f::Ones() * 0.5, Vec3f::Ones() * 0.6});
  for (auto id: ret) {
    fmt::print("Result= {}\n", id);
  }
  using Pmo = 
  acg::spatial::ParallelMultiOctree<acg::spatial::BoundedOctree<float, 3, 2, 2>>;
  Pmo pmo(1);
  using BB = Pmo::BoundingBox;
  using ST = Pmo::StorageType;
  std::vector<ST> data;
  for (auto [i]: NdRange<1>(10)) {
    auto p = Vec3f::Random().eval();
    data.push_back(std::make_pair(BB(p, p + Vec3f::Constant(.1)), i));
    fmt::print("Point {}: {}\n", i, p.transpose());
  }
  pmo.SetFromPairs(data);
  for (auto i: pmo.Query(BB(-Vec3f::Ones(), Vec3f::Ones()))) {
    fmt::print("i = {}\n", i);
  }
}