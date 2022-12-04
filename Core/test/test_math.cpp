#include <co/fastream.h>
#include <co/fastring.h>
#include <co/unitest.h>

#include "acg_core/math/iter.hpp"

namespace test {

DEF_test(math_iter) {
  DEF_case(cenum) {
    acg::Attr<acg::F32, 3> positions;
    positions.setRandom(3, 5);
  }
}
}  // namespace test
