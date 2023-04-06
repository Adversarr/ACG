#include "acore/math/view.hpp"
#include <acore/math/coordinate/common.hpp>
#include <acore/math/coordinate/continuous_discrete.hpp>
#include <doctest/doctest.h>

using namespace acg;

TEST_CASE("Coordinate") {
  SUBCASE("C2D") {
    ContinuousDiscreteTransformRegular<float, 3> trans(.1);
    CHECK(trans.Forward({0, 0, 0}) == IndexVec<3>(0, 0, 0));
    CHECK(trans.Forward({1, 1, 1}) == IndexVec<3>(10, 10, 10));
    CHECK(trans.Forward({.05, .05, .05}) == IndexVec<3>(0, 0, 0));
  }

  SUBCASE("DiscreteStorage") {
    DiscreteStorageSequentialTransform<3> trans({5, 4, 3});
    CHECK_EQ(trans.Forward({0, 0, 0}), 0);
    CHECK_EQ(trans.Forward({0, 0, 1}), 1);
    CHECK_EQ(trans.Forward({1, 1, 1}), 16);
  }

  SUBCASE("Operator call") {
    DiscreteStorageSequentialTransform<3> trans({5, 4, 3});
    CHECK_EQ(trans(0, 0, 1), 1);
    CHECK_EQ(trans(1, 1, 1), 16);
    CHECK_EQ(trans({1, 1, 1}), 16);
    CHECK_EQ(std::apply(trans, std::make_tuple(1, 1, 1)), 16);
  }

  SUBCASE("Combined") {
    auto comb =
        combine_transform(ContinuousDiscreteTransformRegular<float, 3>{.1},
                          DiscreteStorageSequentialTransform<3>({5, 4, 3}));
    CHECK_EQ(comb.Forward({0, 0, 0}), 0);
    CHECK_EQ(comb.Forward({.1, .1, .1}), 16);
    CHECK_EQ(comb.Forward({.14, .1, .1}), 16);
  }

  SUBCASE("View") {
    Field<float, 3> x;
    x.setConstant(3, 3, 3);
    auto v = view(x, DiscreteStorageSequentialTransform<1>(make_vector<Index>(3)));
    for (auto val: v) {
      CHECK_EQ(val.sum(), 9);
    }
  }

  SUBCASE("C-D") {
    ContinuousDiscreteTransformBoundedRegular<float, 3> transf(Vec3f::Ones(), 0.5);
    CHECK(transf(1, 1, 1) == Vec3Index::Zero());
    CHECK(transf(2, 1, 1) == Vec3Index({2, 0, 0}));
    CHECK(transf.Backward({1, 1, 1}) == Vec3f::Constant(1.5));
  }
}

TEST_CASE("make vec") {
  CHECK(make_vector(10).sum() == 10);
  CHECK(make_vector(1, 1).sum() == 2);
  CHECK(make_vector(1, 2, 3).sum() == 6);

  CHECK(to_vector(std::make_tuple(1, 1)).sum() == 2);
  CHECK(to_vector(std::make_tuple(1, 2, 3)).sum() == 6);
  CHECK(to_vector(std::make_tuple(1)).sum() == 1);

  CHECK(to_vector(std::array<float, 3>{.1, .2, .2}).sum() == .5);
}