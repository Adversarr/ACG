#include <doctest/doctest.h>

#include <acore/spatial/subdivision.hpp>
#include <autils/log.hpp>
#include <iostream>

using namespace acg;
TEST_CASE("SpatialSubdivision") {
  acg::spatial::SubDivisionAABB<Float, Index, 3, 2, 7> sd;
  spatial::AABB<Index> data(Vec3f::Constant(0.3), Vec3f::Constant(0.31), 1);
  spatial::AABB<Index> data2(Vec3f::Constant(0.2), Vec3f::Constant(0.21), 2);
  auto print = [&]() {
    for (auto node : sd.nodes_) {
      INSPECT(node.depth_);
      INSPECT(node.box_.lower_bound.transpose());
      INSPECT(node.box_.upper_bound.transpose());
      for (auto subn : node.sub_nodes_) {
        if (subn != InvalidSubscript) {
          INSPECT(subn);
        }
      }
      INSPECT(node.leafs_.size());
      for (auto leaf : node.leafs_) {
        INSPECT(leaf);
      }
    }
  };

  sd.Insert(data);
  sd.Insert(data);
  sd.Insert(data2);
  print();
  spatial::AABB<void, float, 3> aabb(Vec3f::Zero(), Vec3f::Ones());
  auto ub_ceil = aabb.upper_bound.array().ceil().eval();
  auto lb_floor = aabb.lower_bound.array().floor().eval();
  CHECK((ub_ceil - 1 == lb_floor).all());

  auto idx = acg::make_tuple_from_vector(Vec3f(1, 2, 3));
  CHECK(std::get<0>(idx) == 1);
  CHECK(std::get<1>(idx) == 2);
  CHECK(std::get<2>(idx) == 3);
}

