#include <co/unitest.h>

#include <Eigen/Eigen>
#include <acg_core/geometry/mesh.hpp>
#include <iostream>
#include <string_view>
#include <vector>

namespace test {

DEF_test(test1) {
  DEF_case(eig) {
    auto mat = Eigen::MatrixX3f::Zero(12, 3);
    EXPECT(mat.rows() == 12);
  }
}

}  // namespace test
