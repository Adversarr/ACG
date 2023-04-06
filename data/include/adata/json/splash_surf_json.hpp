#pragma once
#include <acore/math/common.hpp>

namespace acg::data {
class SplashSurfJson {
public:
  using Scalar = Float64;

  SplashSurfJson() = default;

  explicit SplashSurfJson(Field<Scalar, 3> position) : position_(position) {}

  void ExportTo(std::ostream& out) const;

  Field<Scalar, 3> position_;
};
}  // namespace acg::data
