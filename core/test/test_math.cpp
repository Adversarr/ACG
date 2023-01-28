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

TEST_CASE("Field Builder") {
  auto ones = acg::FieldBuilder<acg::F32, 2>(2).Ones();
  CHECK(ones.cwiseEqual(1).all());

  auto func = [](acg::Index i) { return acg::Vec2f::Constant(static_cast<float>(i)); };
  auto generated = acg::FieldBuilder<acg::F32, 2>(2).FromFunction(func);
  CHECK(acg::access(generated)[0](0) == 0);
  CHECK(acg::access(generated)[1](0) == 1);
}

TEST_CASE("FieldGetter") {
  using namespace acg::details;
  using namespace acg;
  Index p = 4, q = 3, r = 5;
  Index x = 1, y = 2, z = 4;
  MultiDimensionGetter<3> getter(p, q, r);
  CHECK(getter(x, y, z) == x * r * q + r * y + z);

  SUBCASE("Field Rvalue") {
    Field<float, 3> ones(3, 1);
    ones.setOnes();
    auto acc = access((ones + ones).eval());
    for (auto blk: acc) {
      CHECK(blk.array().sum() == 6);
    }
  }
  SUBCASE("Field accessor") {
    Field<float, 3> field(3, p * q * r);
    auto acc = access(field, getter);
    acc[x * r * q + r * y + z].setOnes();
    CHECK(acc(x, y, z).array().sum() == 3);
  }

  SUBCASE("Row") {
    Field<float, 1> field(1, p * q * r);
    auto acc = access(field, getter);
    acc[x * r * q + r * y + z].setOnes();
    CHECK(acc(x, y, z).array().sum() == 1);
  }

  SUBCASE("Access standard iterate") {
    Field<float, 3> field(3, p * q * r);
    field.setOnes();
    auto acc = access(field, getter);
    for (auto blk: acc) {
      blk.setZero();
    }
    CHECK(field.array().sum() == 0);
  }

  SUBCASE("Default getter") {
    Field<float, 3> field(3, p * q * r);
    field.setOnes();
    auto acc = access(field);
    for (auto blk: acc) {
      blk.setZero();
    }
    CHECK(field.array().sum() == 0);
  }
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
