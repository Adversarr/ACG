#include <acg_core/mesh.hpp>
#include <tiny_obj_loader.h>

namespace acg::port {



/**
 * @brief Use Tiny Obj Loader to load objfiles for scene.
 * 
 */
class Obj {
public:
  static Obj load_file(std::string_view file_name, 
    std::optional<std::string_view> mtl_basedir,
    bool triangulate=true) noexcept;


  // Raw API for tiny obj loader.
  std::shared_ptr<tinyobj::attrib_t> p_attrib;
  std::shared_ptr<std::vector<tinyobj::shape_t>> p_shapes;
  std::shared_ptr<std::vector<tinyobj::material_t>> p_materials;
};

}