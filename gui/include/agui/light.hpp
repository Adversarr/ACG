#pragma once
#include <acore/math/common.hpp>

namespace acg::gui {
struct Light {
  Vec3f light_position_{3.0, 3.0, 3.0};
  Vec3f light_color_{0.9, 0.9, 0.9};
  Float32 light_density_{0.0f};
  Vec3f ambient_light_color_{0.2, 0.2, 0.2};
  Float32 ambient_light_density_{.05f};

  // Parallel Lighting:
  Vec3f parallel_light_color_{0, 0, 0};
  Vec3f parallel_light_dir_{0, 0, 0};
  Float32 parallel_light_density_{0.0f};
};
}  // namespace acg::gui
