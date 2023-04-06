#define ACG_INCLUDE_IMPL
#include <aphysics/collision/detect.hpp>
#include <Eigen/Dense>

namespace acg::physics::collision {

template struct VertexTriangleDynamic<Float32>;
template struct VertexTriangleDynamic<Float64>;

template struct EdgeEdgeDynamic<Float32>;
template struct EdgeEdgeDynamic<Float64>;

template struct VertexVertexDynamic<Float32>;
template struct VertexVertexDynamic<Float64>;

template struct VertexTriangleStatic<Float32>;
template struct VertexTriangleStatic<Float64>;
}