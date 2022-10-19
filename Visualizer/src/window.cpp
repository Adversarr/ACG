#include <co/log.h>

#include <acg_utils/log.hpp>

#include "acg_utils/debugger.hpp"
#include "acg_visualizer/window.hpp"

namespace acg::visualizer::details {

Window::Window(std::string_view title) noexcept : title_(title) {
  ACG_CHECK(glfwInit() == GLFW_TRUE, "Failed to init GLFW.");
  glfwInit();
  glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);
  glfwWindowHint(GLFW_RESIZABLE, GLFW_TRUE);
  window_ = glfwCreateWindow(width_, height_, title_.c_str(), nullptr, nullptr);
  ACG_CHECK(window_ != nullptr, "Failed to create window.");
  glfwSetWindowUserPointer(window_, this);
  glfwSetFramebufferSizeCallback(window_, WindowResizeCallback);
}

Window::~Window() noexcept {
  glfwDestroyWindow(window_);
  window_ = nullptr;
  glfwTerminate();
}

vk::SurfaceKHR Window::CreateWindowSurface(vk::Instance& instance) {
  VkSurfaceKHR surface;
  ACG_CHECK(glfwVulkanSupported(), "Glfw has no vulkan support");
  VK_CHECK_SUCCESS(glfwCreateWindowSurface(instance, window_, nullptr, &surface));
  return surface;
}

void Window::WindowResizeCallback(GLFWwindow* window, int width, int height) {
  auto w = reinterpret_cast<Window*>(glfwGetWindowUserPointer(window));
  w->resized_ = true;
  w->height_ = height;
  w->width_ = width;
  ACG_DEBUG_LOG("Window resized. (w: {}, h: {})", width, height);
}

void Window::UpdateWindowSize() {
  width_ = height_ = 0;
  while (width_ == 0 || height_ == 0) {
    glfwGetFramebufferSize(window_, &width_, &height_);
    glfwWaitEvents();
  }
}

}  // namespace acg::visualizer
