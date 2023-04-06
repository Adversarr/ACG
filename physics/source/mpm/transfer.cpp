#define ACG_INCLUDE_IMPL
#include <aphysics/mpm/transfer.hpp>

namespace acg::physics::mpm {

template class ApicRegular<Float32, 3, CubicBSplineKernel>;
template class ApicRegular<Float64, 3, CubicBSplineKernel>;

} // namespace acg::physics::mpm