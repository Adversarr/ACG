#include <doctest/doctest.h>
#include <fmt/format.h>
#include <fmt/ostream.h>

#include <acore/math/decompositions/svd.hpp>
#include <acore/math/func.hpp>
#include <acore/math/ndrange.hpp>
#include <acore/math/ops/kronecker.hpp>
#include <iomanip>
#include <iostream>


#include <acore/math/utilities.hpp>
#include "acore/math/view.hpp"

TEST_CASE("Check Col copy status") {
  acg::Field<acg::Float32, 3> positions;
  positions.setRandom(3, 5);
  auto col0 = positions.col(0);
  col0(0) = 1;
  CHECK(positions(0, 0) == 1);
  auto col0copy = positions.col(0).eval();
  col0copy(0) = 2;
  CHECK(positions(0, 0) == 1);
}

TEST_CASE("Field Enumerate") {
  acg::Field<acg::Float32, 3> positions(3, 4);
  for (auto [i, v] : acg::enumerate(acg::view(positions))) {
    v.setConstant(i);
  }
  for (int i = 0; i < 4; ++i) {
    CHECK((positions.col(i).array() == i).all());
  }

  auto acc2d = acg::view(positions, acg::NdRangeIndexer<2>(2, 2));
  for (auto [i, j, v] : acg::enumerate(acc2d)) {
    v.setConstant(i);
  }

  for (int i = 0; i < 2; ++i) {
    for (int j = 0; j < 2; ++j) {
      CHECK((acc2d(i, j).array() == i).all());
    }
  }
}

TEST_CASE("Field Builder") {
  auto ones = acg::FieldBuilder<acg::Float32, 2>(2).Ones();
  CHECK(ones.cwiseEqual(1).all());
}

TEST_CASE("FieldGetter") {
  using namespace acg;
  Index p = 4, q = 3, r = 5;
  Index x = 1, y = 2, z = 4;
  NdRangeIndexer<3> getter(p, q, r);
  getter[3];
  CHECK(getter(x, y, z) == x * r * q + r * y + z);
  CHECK(getter[x * r * q + r * y + z]
        == std::tuple<Index, Index, Index>(x, y, z));
}

TEST_CASE("Field Rvalue") {
  using namespace acg;
  Index p = 4, q = 3, r = 5;
  NdRangeIndexer<3> getter(p, q, r);
  Field<float, 3> ones(3, 1);
  ones.setOnes();
  auto acc = view((ones + ones).eval());
  for (auto blk : acc) {
    CHECK(blk.array().sum() == 6);
  }
}
TEST_CASE("Field accessor") {
  using namespace acg;
  Index p = 4, q = 3, r = 5;
  Index x = 1, y = 2, z = 4;
  NdRangeIndexer<3> getter(p, q, r);
  Field<float, 3> field(3, p * q * r);
  field.setOnes();
  std::cout << field << std::endl;
  auto acc = view(field, getter);
  acc[x * r * q + r * y + z].setOnes();
  CHECK(acc(x, y, z).array().sum() == 3);
}

TEST_CASE("Row") {
  using namespace acg;
  Index p = 4, q = 3, r = 5;
  Index x = 1, y = 2, z = 4;
  NdRangeIndexer<3> getter(p, q, r);
  Field<float, 1> field(1, p * q * r);
  auto acc = view(field, getter);
  acc[x * r * q + r * y + z].setOnes();
  CHECK(acc(x, y, z) == 1);
}

TEST_CASE("Access standard iterate") {
  using namespace acg;
  Index p = 4, q = 3, r = 5;
  NdRangeIndexer<3> getter(p, q, r);
  Field<float, 3> field(3, p * q * r);
  field.setOnes();
  auto acc = view(field, getter);
  for (auto blk : acc) {
    blk.setZero();
  }
  CHECK(field.array().sum() == 0);
}

TEST_CASE("Default getter") {
  using namespace acg;
  Index p = 4, q = 3, r = 5;
  NdRangeIndexer<3> getter(p, q, r);
  Field<float, 3> field(3, p * q * r);
  field.setOnes();
  auto acc = view(field);
  for (auto blk : acc) {
    blk.setZero();
  }
  CHECK(field.array().sum() == 0);
}

TEST_CASE("Field Access Trasnform") {
  using namespace acg;
  auto field = FieldBuilder<float, 4>(3).Constant(0).eval();
  auto acc = view<NdRangeIndexer<1>, ReshapeTransform<2, 2>>(field);
  auto acc1 = acc[1];
  acc1.setOnes();
  CHECK_EQ(acc(1).trace(), 2);
  CHECK_EQ(acc(1).sum(), 4);
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

TEST_CASE("FieldReshape") {
  acg::Mat3x3f mat;
  acg::Field<float, 9> field(9, 1);
  auto acc = acg::view(field);

  for (int i = 0; i < 5; ++i) {
    mat.setRandom();
    acc(0) = mat.reshaped();

    CHECK_EQ(acc(0).reshaped(3, 3), mat);
  }
}

TEST_CASE("Field Init") {
  acg::Mat3x3f mat;
  mat << acg::Vec3f::Ones(), acg::Vec3f::Zero(), acg::Vec3f::Ones();
  CHECK_EQ(mat.col(1).array().square().sum(), 0);
}

struct Iter2D {
  constexpr Iter2D(int n, int m, int i, int j) : i(i), j(j), n(n), m(m) {}

  constexpr auto operator*() const { return std::make_tuple(i, j); }

  constexpr bool operator==(const Iter2D& another) const {
    return i == another.i && j == another.j && n == another.n && m == another.m;
  }

  constexpr bool operator!=(const Iter2D& another) const {
    return !(*this == another);
  }

  Iter2D& operator++() {
    if (j == m - 1) {
      i += 1;
      j = 0;
    } else {
      j += 1;
    }
    return *this;
  }

  int i, j;
  const int n;
  const int m;
};

struct Iter2DWrapper {
  Iter2D begin() const { return Iter2D(n, m, 0, 0); }

  Iter2D end() const { return Iter2D(n, m, n, 0); }

  int n, m;
  constexpr Iter2DWrapper(int n, int m) : n(n), m(m) {}
};

TEST_CASE("Iter2d") {
  Iter2DWrapper itw(3, 3);
  for (auto [i, j] : itw) {
    std::cout << i << j << std::endl;
  }
}

TEST_CASE("Sin") {
  acg::math::sin(10);
  acg::Field<float, 3> field(3, 3);
  field.setZero();
  auto sin_field = acg::math::sin(field);
  CHECK(sin_field.cwiseEqual(0).all());
}

TEST_CASE("Format Matrix") {
  acg::Mat3x3f mat;
  mat.setOnes();
  fmt::print(fmt::format("{}\n", mat.cwiseAbs2()));
  fmt::print(fmt::format("{}\n", mat));
}

TEST_CASE("SVD") {
  acg::Mat2x2f mat22;
  mat22.setZero();
  mat22.diagonal().setOnes();

  acg::math::Svd svd(mat22);

  std::cout << mat22 << std::endl;
  std::cout << svd.u_ << std::endl;
  std::cout << svd.sigma_ << std::endl;
  std::cout << svd.v_ << std::endl;

  acg::Mat3x3f mat33;
  mat33.setZero();
  mat33.diagonal().setOnes();

  acg::math::Svd svd33(mat33);

  std::cout << mat33 << std::endl;
  std::cout << svd33.u_ << std::endl;
  std::cout << svd33.sigma_ << std::endl;
  std::cout << svd33.v_ << std::endl;
  mat33.block<3, 3>(0, 0).setZero();
}
