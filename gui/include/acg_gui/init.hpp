#pragma once

namespace acg::gui{

namespace details {

struct VkContextHooker {

  /**
  * @brief: Hook the vulkan backend context initializer and cleanuper
  */
  void Hook() noexcept;
};
}

using VkContextHooker=details::VkContextHooker;

}
