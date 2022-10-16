#include "acg_visualizer/vkwindow.hpp"

#include <co/log.h>
#include <acg_utils/log.hpp>
#include "acg_utils/debugger.hpp"

namespace acg::visualizer::details {

VkWindow::VkWindow(std::string_view title) noexcept : title_(title) {
  ACG_CHECK(glfwInit() == GLFW_TRUE, "Failed to init GLFW.");
  glfwInit();
  glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);
  glfwWindowHint(GLFW_RESIZABLE, GLFW_TRUE);
  window_ = glfwCreateWindow(width_, height_, title_.c_str(), nullptr, nullptr);
  ACG_CHECK(window_ != nullptr, "Failed to create window.");
  glfwSetWindowUserPointer(window_, this);
  glfwSetFramebufferSizeCallback(window_, WindowResizeCallback);
}

VkWindow::~VkWindow() noexcept {
  glfwDestroyWindow(window_);
  window_ = nullptr;
  glfwTerminate();
}

vk::SurfaceKHR VkWindow::CreateWindowSurface(vk::Instance& instance) {
  VkSurfaceKHR surface;
  VK_CHECK_SUCCESS(glfwCreateWindowSurface(instance, window_, nullptr, &surface));
  return surface;
}

void VkWindow::WindowResizeCallback(GLFWwindow* window, int width, int height) {
  auto w = reinterpret_cast<VkWindow*>(glfwGetWindowUserPointer(window));
  w->resized_ = true;
  w->height_ = height;
  w->width_ = width;
  ACG_DEBUG_LOG("Window resized. (w: {}, h: {})", width, height);
}

}  // namespace acg::visualizer
