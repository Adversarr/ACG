#pragma once

#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>

#include <acg_core/math/common.hpp>
#include <string>
#include <tuple>
#include <vulkan/vulkan.hpp>

namespace acg::gui {

class Window {
public:
  static constexpr int DEFAULT_WIDTH_ = 800;

  static constexpr int DEFAULT_HEIGHT_ = 600;

  explicit Window(std::string_view title) noexcept;

  Window(Window&&) = delete;

  Window(const Window&) = delete;

  ~Window() noexcept;

  inline bool IsResized() const noexcept;

  inline void ResetResizeFlag() noexcept;

  inline std::pair<uint32_t, uint32_t> GetWindowSize() const noexcept;

  inline GLFWwindow* GetWindow() const noexcept;

  inline bool ShouldClose() const;

  vk::SurfaceKHR CreateWindowSurface(vk::Instance& instance);

  bool IsKeyPressed(int glfw_key) const;

  void UpdateWindowSize();

  inline Vec2d GetMousePosition() const;

private:
  static void WindowResizeCallback(GLFWwindow* window, int width, int height);

  static void CursurCallback(GLFWwindow* window, double xpos, double ypos);

  double mouse_x_position_;
  double mouse_y_position_;

  GLFWwindow* window_{nullptr};

  std::string title_{"Vulkan Gui"};

  int width_{DEFAULT_WIDTH_};

  int height_{DEFAULT_HEIGHT_};

  bool resized_{false};

public:
  static Window& Instance() noexcept;

  static void Init(const std::string& title);

  static void Destroy();
  struct Hooker {
    std::string name_;

    Hooker() = default;

    inline explicit Hooker(std::string name) : name_(std::move(name)) {}

    void Hook() const;
  };
};

inline Vec2d Window::GetMousePosition() const { return {mouse_x_position_, mouse_y_position_}; }

inline bool Window::IsResized() const noexcept { return resized_; }

void Window::ResetResizeFlag() noexcept { resized_ = false; }

inline std::pair<uint32_t, uint32_t> Window::GetWindowSize() const noexcept {
  return {width_, height_};
}

inline GLFWwindow* Window::GetWindow() const noexcept { return window_; }

inline bool Window::ShouldClose() const { return glfwWindowShouldClose(window_); }

using WindowHooker = Window::Hooker;

}  // namespace acg::gui
