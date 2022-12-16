#include <doctest/doctest.h>
#include "acg_core/math/iter.hpp"

TEST_CASE("Random Attr") {
  acg::Field<acg::F32, 3> positions;
  positions.setRandom(3, 5);
}
