#pragma once
namespace acg::visualizer{

namespace details {
struct VkContextHooker {
  static void Hook() noexcept;
};
}

using VkContextHooker=details::VkContextHooker;

}
