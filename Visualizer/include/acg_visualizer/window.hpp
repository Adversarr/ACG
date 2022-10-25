#pragma once

#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>

#include <string>
#include <tuple>

#include "avk.hpp"

namespace acg::visualizer::details {

class Window {
public:
  static constexpr int DEFAULT_WIDTH = 800;

  static constexpr int DEFAULT_HEIGHT = 600;

  explicit Window(std::string_view title) noexcept;

  Window(Window&& ) = delete;
  Window(const Window& ) = delete;

  ~Window() noexcept;

  inline bool IsResized() const noexcept;

  inline void ResetResizeFlag() noexcept;

  inline std::pair<uint32_t, uint32_t> GetWindowSize() const noexcept;

  inline GLFWwindow* GetWindow() const noexcept;

  inline bool ShouldClose() const;

  vk::SurfaceKHR CreateWindowSurface(vk::Instance& instance);

  bool IsKeyPressed(int glfw_key) const;

  void UpdateWindowSize();

private:
  static void WindowResizeCallback(GLFWwindow* window, int width, int height);

  GLFWwindow* window_{nullptr};

  std::string title_{"Vulkan Visualizer"};

  int width_{DEFAULT_WIDTH};

  int height_{DEFAULT_HEIGHT};

  bool resized_{false};
};

inline bool Window::IsResized() const noexcept { return resized_; }

void Window::ResetResizeFlag() noexcept { resized_ = false; }

inline std::pair<uint32_t, uint32_t> Window::GetWindowSize() const noexcept { return {width_, height_}; }

inline GLFWwindow* Window::GetWindow() const noexcept { return window_; }

inline bool Window::ShouldClose() const { return glfwWindowShouldClose(window_); }

}  // namespace acg::visualizer