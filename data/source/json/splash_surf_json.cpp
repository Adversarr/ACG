#include <adata/json/splash_surf_json.hpp>
#include <iostream>

#include "acore/math/view.hpp"
namespace acg::data {

void SplashSurfJson::ExportTo(std::ostream& out) const {
  out << "[" << std::endl;
  for (auto [i, v] : enumerate(view(position_))) {
    out << "[" << v.x() << ", " << v.z() << ", " << v.y() << "]";
    if (i == position_.cols() - 1) {
      out << std::endl;
    } else {
      out << "," << std::endl;
    }
  }
  out << "]" << std::endl;
}

}  // namespace acg::data
