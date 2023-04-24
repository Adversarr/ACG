#pragma once

#include "acore/math/common.hpp"

namespace acg::data {

class ObjExport {
public:
  ObjExport() = default;

  void ExportTo(std::ostream& out) const;

  Field<Float64, 3> position_;

  types::topology::TriangleList triangle_;
};

}  // namespace acg::data
