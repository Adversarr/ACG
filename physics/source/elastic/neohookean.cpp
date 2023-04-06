#define ACG_INCLUDE_IMPL
#include <aphysics/elastic/neohookean.hpp>

namespace acg {
template class acg::physics::elastic::OgdenNeoHookean<Float32, 3>;
template class acg::physics::elastic::OgdenNeoHookean<Float64, 3>;
template class acg::physics::elastic::SNHNeoHookean<Float32, 3>;
template class acg::physics::elastic::SNHNeoHookean<Float64, 3>;
} // namespace acg