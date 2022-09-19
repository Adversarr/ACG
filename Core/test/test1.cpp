#include <doctest/doctest.h>

#include <Eigen/Eigen>
#include <acg_core/mesh.hpp>
#include <iostream>
#include <string_view>
#include <vector>
#define TO_TEXT(...) #__VA_ARGS__

TEST_CASE("test eigen") {
  auto mat = Eigen::MatrixX3f::Zero(12, 3);
  CHECK(mat.rows() == 12);
  auto arr = mat.array();
  acg::geometry::TriangleMesh<float> mesh{};
  mesh.vertices_.setRandom(3, 4);
  std::cout << mesh.vertices_ << std::endl;
  for (auto i = 0; i < 12; ++i) {
    std::cout << mesh.vertices_.data()[i] << std::endl;
  }
  std::string_view a{"123"};
  TO_TEXT(1, 2, 3);
  std::cout << a << std::endl;
}