#define ACG_INCLUDE_IMPL
#include <aphysics/elastic/stvk.hpp>
using namespace acg::physics::elastic;
using namespace acg;
namespace acg::physics::elastic{

template class StVK<Float32, 3>;
template class StVK<Float64, 3>;

}