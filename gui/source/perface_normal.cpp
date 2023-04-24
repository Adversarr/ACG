#include <agui/perfacenormal.hpp>

#include <acore/math/view.hpp>
namespace acg::gui {
PerfaceNormalTransform::PerfaceNormalTransform(
    const Field<Float32, 3> &position,
    const types::topology::TriangleList &triangles) noexcept {
  auto vp = view(position);
  out_position_.resize(Eigen::NoChange, 3 * triangles.cols());
  out_triangle_list_.resize(Eigen::NoChange, triangles.cols());
  for (auto [i, f] : enumerate(view(triangles))) {
    out_triangle_list_.col(i) = Vec3Index{3 * i, 3 * i + 1, 3 * i + 2};
    out_position_.col(3 * i) = vp(f.x());
    out_position_.col(3 * i + 1) = vp(f.y());
    out_position_.col(3 * i + 2) = vp(f.z());
  }
}
} // namespace acg::gui