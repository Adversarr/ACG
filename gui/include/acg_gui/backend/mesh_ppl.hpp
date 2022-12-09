#pragma once

#include "graphics_pass.hpp"

namespace acg::gui {
namespace details {

class MeshPipeline2 {
public:

  void Init();
 
  void Cleanup();
  
  void CreateDescriptorSetLayout();
 
  void CreateUniformBuffers();

  void CreateGraphicsPipeline();

  


private:

  bool is_inited_{false};



};

}
}  // namespace acg::gui
