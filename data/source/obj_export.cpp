#include <tiny_obj_loader.h>

#include <acore/geometry/normal.hpp>
#include <adata/obj_export.hpp>

#include "acore/math/view.hpp"
namespace acg::data {
void ObjExport::ExportTo(std::ostream& out) const {
  for (auto v : view(position_)) {
    out << "v " << v.x() << " " << v.z() << " " << v.y() << std::endl;
  }

  geometry::Normal<Float64> norm(triangle_, position_);

  auto result = norm.PerFace();

  for (auto f : view(triangle_)) {
    out << "f " << (f.array() + 1).transpose() << std::endl;
  }
}
}  // namespace acg::data
