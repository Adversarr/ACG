#include <doctest/doctest.h>

#include <acg_core/math/kronecker.hpp>
#include <iostream>

#include "acg_core/math/access.hpp"

TEST_CASE("Check Col copy status") {
  acg::Field<acg::F32, 3> positions;
  positions.setRandom(3, 5);
  auto col0 = positions.col(0);
  col0(0) = 1;
  CHECK(positions(0, 0) == 1);
  auto col0copy = positions.col(0).eval();
  col0copy(0) = 2;
  CHECK(positions(0, 0) == 1);
}

TEST_CASE("Field Enumerate") {
  acg::Field<acg::F32, 3> positions(3, 4);
  acg::FieldEnumerate enumerator(positions);
  positions.setOnes();
  for (auto v : enumerator) {
    v.second(0) = v.first;
  }
  for (auto v : enumerator) {
    CHECK(v.second(0) == v.first);
  }

  for (auto v : enumerator) {
    CHECK(v.second(1) == positions(1, v.first));
  }
}

TEST_CASE("Field Access") {
  acg::Field<acg::F32, 3> positions(3, 2);
  positions << 1, 2, 3, 4, 5, 6;
  acg::FieldAccess accessor(positions);

  CHECK(accessor[0](0) == 1);
  CHECK(accessor[0](1) == 3);
  CHECK(accessor[0](2) == 5);

  CHECK(accessor.Size() == 2);
}

TEST_CASE("Field Builder") {
  auto ones = acg::FieldBuilder<acg::F32, 2>(2).Ones();
  CHECK(ones.cwiseEqual(1).all());

  auto func = [](acg::Idx i) { return acg::Vec2f::Constant((float)i); };
  auto generated = acg::FieldBuilder<acg::F32, 2>(2).FromFunction(func);
  CHECK(acg::FieldAccess(generated)[0](0) == 0);
  CHECK(acg::FieldAccess(generated)[1](0) == 1);
}

TEST_CASE("Kronecker") {
  acg::Mat2x2f mata;
  mata << 2, 0, 1, 3;
  acg::Mat2x2f matb;
  matb << 5, -1, -1, 4;

  acg::math::KroneckerDenseByDense kronecker_compute(mata, matb);
  auto result = kronecker_compute();
  CHECK(result(0, 0) == 10);
  CHECK(result(2, 3) == -3);

  acg::MatNxXf<3> matc(3, 4);
  matc.setOnes();
  acg::math::KroneckerDenseByDense kronecker_bc(matb, matc);
  auto bc = kronecker_bc();
  CHECK(bc.rows() == matb.rows() * matc.rows());
  CHECK(bc.cols() == matb.cols() * matc.cols());
  CHECK(bc(5, 7) == 4);
}
