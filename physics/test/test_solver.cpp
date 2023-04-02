#include <aphysics/elastic/neohookean.hpp>
#include <aphysics/solver/admm/hyperelastic.hpp>
#include <aphysics/solver/admm/spring.hpp>
#include <doctest/doctest.h>
#include <acore/math/utilities.hpp>
TEST_CASE("he-admm") {
  using namespace acg;
  using Model = physics::elastic::OgdenNeoHookean<float, 3>;
  physics::admm::HyperElasticConstraint<float, Model> constraint;
  Field<float, 3> position;
  position.resize(3, 4);
  position.col(0).setZero();
  position.col(1).setUnit(0);
  position.col(2).setUnit(1);
  position.col(3).setUnit(2);
  auto rest = position.eval();
  position *= 2;
  Field<float, 9> rinv;
  rinv.resize(9, 1);
  auto racc = view(rest);
  auto ri_a = view<DefaultIndexer, ReshapeTransform<3, 3>>(rinv);
  types::topology::TetraList tet;
  tet.resize(4, 1);
  tet.col(0).setLinSpaced(0, 3);
  for (auto [i, te] : enumerate(view(tet))) {
    // Foreach tetra. gather the weights.
    Mat3x3<float> rest;
    rest.col(0) = racc(te.y()) - racc(te.x());
    rest.col(1) = racc(te.z()) - racc(te.x());
    rest.col(2) = racc(te.w()) - racc(te.x());

    ri_a(i) = rest.inverse();
  }
  constraint.iteration_count_ = 20;
  Field<float, 9> u;
  u.resize(9, 1);
  ReshapeTransform<3, 3>{}(u.col(0)).setIdentity();
  constraint.Compute(position, rest, rinv, tet, u, 1,
                     1, 3);
  fmt::print("{}", constraint.slack_);
}

TEST_CASE("sp-admm") {
  using namespace acg;
  physics::admm::SpringConstraint<float> constraint;
  Field<float, 3> position;
  position.resize(3, 4);
  position.col(0).setZero();
  position.col(1).setUnit(0);
  Field<float> orig;
  orig.setConstant(1, .5);
  Field<float, 3> u;
  u.resize(3, 1);
  u.setZero();
  types::topology::LineList line;
  line.resize(2, 1);
  line(0, 0) = 1;
  line(1, 0) = 0;
  Field<float> stiffness = Field<float>::Ones(1);
  constraint.Compute(position, u, line, orig, 1000*stiffness, 1 * stiffness);
  fmt::print("{}", constraint.slack_);
}