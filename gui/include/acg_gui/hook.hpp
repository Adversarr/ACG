#pragma once

namespace acg::gui{

namespace details {

struct VkContextHooker {

  /**
  * @brief: Hook the vulkan backend context initializer and cleanuper
  *
  * TODO: use function to init instead of the struct
  */
  static void Hook() noexcept;
};
}

using VkContextHooker=details::VkContextHooker;

}
