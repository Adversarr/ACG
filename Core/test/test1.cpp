#include <doctest/doctest.h>

#include <Eigen/Eigen>
#include <acg_core/geometry/mesh.hpp>
#include <iostream>
#include <string_view>
#include <vector>
#define TO_TEXT(...) #__VA_ARGS__

TEST_CASE("test eigen") {
  auto mat = Eigen::MatrixX3f::Zero(12, 3);
  CHECK(mat.rows() == 12);
  auto arr = mat.array();
  std::string_view a{"123"};
  std::cout << a << std::endl;
}