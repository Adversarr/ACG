#pragma once
#include "imgui.h"
#include "imgui_impl_glfw.h"
#include "imgui_impl_vulkan.h"
#define GLFW_INCLUDE_NONE
#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>
#include <vulkan/vulkan.h>

namespace acg::visual::vulkan {


class Visualizer {
public:
  void init();
};


}  // namespace acg::visual::vulkan
