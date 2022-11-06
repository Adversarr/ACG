#include <co/unitest.h>

#include <Eigen/Eigen>
#include <acg_core/geometry/mesh.hpp>
#include <acg_core/geometry/particlesystem.hpp>
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
    
  }
}

}  // namespace test
