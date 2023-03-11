#pragma once
#include <acore/geometry/common.hpp>
#include <aphysics/elastic/shape_matching.hpp>

using namespace acg;
class ShapeMatchingApp {
public:
  ShapeMatchingApp() = default;

  void Step();

  void Init();

  physics::elastic::ShapeMatching<Float, 3> shape_matching_;
  Field<Float, 3> position_;
  Field<Float, 3> velocity_;
  geometry::topology::TetraList tetra_;
  geometry::topology::TriangleList faces_;

  Float dt_{0.01};

  Float alpha_{1};
};
