#define ACG_INCLUDE_IMPL
#include <aphysics/elastic/shape_matching.hpp>

namespace acg::physics::elastic {
template class ShapeMatching<Float32, 3>;
template class ShapeMatching<Float64, 3>;
}