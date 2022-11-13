#include <co/unitest.h>

#include <Eigen/Eigen>
#include <acg_core/geometry/mesh.hpp>
#include <acg_core/geometry/particlesystem.hpp>
#include <iostream>
#include <string_view>
#include <vector>
#include <co/fastream.h>
#include <co/fastring.h>

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
    
  }
  DEF_case(TypeTranspose) {
    using namespace acg;
    EXPECT((std::is_same_v<TransposeType<AttrVec<acg::F32, 3>>, AttrVecTrans<acg::F32, 3>>));
  }

  DEF_case(cofastream) {
    fastream fs;
    fs.append('\n');
  }
}

DEF_test(coost) {
  DEF_case(align) {
    std::cout << god::align_up(10000, 1024) << std::endl;
  }
}

}  // namespace test
