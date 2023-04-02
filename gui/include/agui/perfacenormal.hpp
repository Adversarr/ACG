#pragma once
#include <acore/math/common.hpp>

namespace acg::gui {
struct PerfaceNormalTransform {

  PerfaceNormalTransform(
      const Field<Float32, 3> &position,
      const types::topology::TriangleList &triangles) noexcept;

  Field<Float32, 3> out_position_;
  types::topology::TriangleList out_triangle_list_;
};
} // namespace acg::gui