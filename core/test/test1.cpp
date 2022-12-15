#include <co/fastream.h>
#include <co/fastring.h>
#include <co/unitest.h>

#include <Eigen/Eigen>
#include <acg_core/geometry/mesh.hpp>
#include <acg_core/geometry/particlesystem.hpp>
#include <acg_core/geometry/smoother.hpp>
#include <acg_core/math/all.hpp>
#include <iostream>
#include <string_view>
#include <vector>

namespace test {

DEF_test(test1) {
  DEF_case(eig) {
    acg::geometry::ParticleSystem<acg::F32> particlesystem{
        {{1.0, 0.0, 3.0}, 2},
        {{1.0, 0.0, 3.0}, 2},
        {{1.0, 0.0, 3.0}, 2},
        {{1.0, 0.0, 3.0}, 2},
    };
    std::cout << particlesystem.to_string();
    EXPECT((acg::details::TensorTrait<Eigen::Vector<acg::F32, 4>>::is_col_major));
    EXPECT(!(acg::details::TensorTrait<Eigen::Vector<acg::F32, 4>>::is_row_major));
    EXPECT((acg::details::TensorTrait<Eigen::RowVector<acg::F32, 4>>::is_row_major));
    EXPECT((acg::details::TensorTrait<Eigen::RowVector<acg::F32, 4>>::rows == 1));
    EXPECT((acg::details::TensorTrait<Eigen::Vector<acg::F32, 4>>::rows == 4));
  }
  DEF_case(TypeTranspose) {
    using namespace acg;
    EXPECT((std::is_same_v<TransposeType<Attr<acg::F32, 3>>, AttrTrans<acg::F32, 3>>));
  }

  DEF_case(cofastream) {
    fastream fs;
    fs.append('\n');
  }

  DEF_case(uniform_laplacian_smoother) {
    // acg::Attr<float, 3> attr;
    // acg::geometry::topology::TriangleList tl;
    // tl.resize(3, 1);
    // tl(0, 0) = 0;
    // tl(1, 0) = 1;
    // tl(2, 0) = 2;
    // attr.setOnes(Eigen::NoChange, 3);
    // std::cout << attr << std::endl;
    // std::cout << acg::geometry::UniformLaplacianSmoother(tl).Compute(attr) << std::endl;
  }
}

DEF_test(coost) {
  DEF_case(align) { std::cout << god::align_up(10000, 1024) << std::endl; }
}

}  // namespace test
