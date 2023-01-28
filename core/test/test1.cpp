#include <doctest/doctest.h>

#include <Eigen/Eigen>
#include <acg_core/geometry/mesh.hpp>
#include <acg_core/geometry/particlesystem.hpp>
#include <acg_core/geometry/smoother.hpp>
#include <acg_core/math/all.hpp>
#include <iostream>
#include <string_view>
#include <vector>

TEST_CASE("test1") {
  acg::geometry::ParticleSystem<acg::F32> particlesystem{
      {{1.0, 0.0, 3.0}, 2},
      {{1.0, 0.0, 3.0}, 2},
      {{1.0, 0.0, 3.0}, 2},
      {{1.0, 0.0, 3.0}, 2},
  };
  std::cout << particlesystem.to_string();
  CHECK((acg::details::Trait<Eigen::Vector<acg::F32, 4>>::is_col_major));
  CHECK(!(acg::details::Trait<Eigen::Vector<acg::F32, 4>>::is_row_major));
  CHECK((acg::details::Trait<Eigen::RowVector<acg::F32, 4>>::is_row_major));
  CHECK((acg::details::Trait<Eigen::RowVector<acg::F32, 4>>::rows == 1));
  CHECK((acg::details::Trait<Eigen::Vector<acg::F32, 4>>::rows == 4));
}

