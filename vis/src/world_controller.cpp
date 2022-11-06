#pragma once
#include "acg_vis/world_controller.hpp"

#include <spdlog/stopwatch.h>

#include "co/co.h"

#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>

#include <acg_utils/singleton.hpp>

namespace acg::visualizer::details {

void TheWorld::Run() {
  PreRun();
  is_running_ = true;
  while (is_running_) {
    single_frame_watch_.reset();
    // 1: Process IO
    ProcessInput();
    if (!is_running_) {
      break;
    }
    // 2. Draw
    RunDraw();
    // 3. (optional) Simulation
    if (is_physics_running_) {
      RunPhysics();
    }
    using namespace std::chrono;
    auto ms_sleep
        = 1.0 / fps_limit_ - duration_cast<milliseconds>(single_frame_watch_.elapsed()).count();
    if (ms_sleep > 1) {
      co::sleep(ms_sleep);
    }
  }
  PostRun();
}

void TheWorld::ProcessInput() {
  glfwPollEvents();
  auto* window_ptr = get_vk_context().GetWindow()->GetWindow();
  for (const auto& [k, v] : keyboard_callbacks_) {
    // only process key press here.
    if (glfwGetKey(window_ptr, k) == GLFW_PRESS) {
      ACG_CHECK(v(), "Callback failed.");
    }
  }
  // NOTE: more input methods.
}

void TheWorld::RunDraw() {
  RunUiImpl();
  // TODO: Render the scene, ui.
}

}  // namespace acg::visualizer::details
