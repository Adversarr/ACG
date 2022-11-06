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
    // 4. FPS Limitation
    if (fps_limit_ > 0) {
      using namespace std::chrono;
      auto ms_sleep
          = 1.0 / fps_limit_ - duration_cast<milliseconds>(single_frame_watch_.elapsed()).count();
      if (ms_sleep > 1) {
        co::sleep(ms_sleep);
      }
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
      ACG_DEBUG_LOG("Key {} is pressed.", k);
      ACG_CHECK(v.callback(), "Callback failed.");
    }
  }
  // NOTE: more input methods.
}

vk::CommandBuffer TheWorld::DrawUI() {
  // TODO: Render the scene, ui.
  ImGui_ImplVulkan_NewFrame();
  ImGui_ImplGlfw_NewFrame();
  ImGui::NewFrame();
  RunUiImpl();
  ImGui::Render();
  auto* data = ImGui::GetDrawData();
  return ui_ppl_->Render(data);
}

void TheWorld::RunDraw() {
  DrawUI();
  DrawScene();
}

void TheWorld::RecreateSwapchain() {
  get_vk_context().RecreateSwapchain();
  RecreateSwapchainCallback();
}

void TheWorld::CleanUp() {
  get_vk_context().GetDevice().waitIdle();
  // Cleanup the pipeline.
  ui_ppl_.reset();

  // NOTE: because vk context is not created by the world, these resources
  //       are not required to release
  CleanUpCallback();
}

void TheWorld::PreRun() {
  // DO NOTHING
}

void TheWorld::PostRun() {
  // DO NOTHING
}

void TheWorld::DrawScene() {
  // DO NOTHING
}

void TheWorld::RecreateSwapchainCallback() {
  // DO NOTHING
}

void TheWorld::CleanUpCallback() {
  // DO NOTHING
}

void TheWorld::InitCallback() {
  // DO NOTHING
}

}  // namespace acg::visualizer::details
