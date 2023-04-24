#define ACG_INCLUDE_IMPL
#include <aphysics/elastic/invariants.hpp>

namespace acg::physics::elastic {

template class CauchyGreenInvariants<Float32, 3>;
template class CauchyGreenInvariants<Float64, 3>;
template class SmithInvariants<Float32, 3>;
template class SmithInvariants<Float64, 3>;

} // namespace acg::physics::elastic