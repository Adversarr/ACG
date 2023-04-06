#define ACG_INCLUDE_IMPL
#include <Eigen/Dense>
#include <acore/geometry/tetface.hpp>


namespace acg::geometry {

template class Tet2Face<Float32>;
template class Tet2Face<Float64>;

} // namespace acg::geometry