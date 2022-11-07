#pragma once
#include "acg_vis/world_controller.hpp"

#include <spdlog/spdlog.h>
#include <chrono>
#include <vector>

#include "co/co.h"
#include <co/time.h>
#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>

#include <acg_utils/singleton.hpp>



namespace acg::visualizer {

void WorldCtrlUiOnly::Run() {
  PreRun();
  is_running_ = true;
  while (is_running_) {
    auto start = now::us();
    // 1: Process basic keyboard input.
    ProcessInput();
    if (!is_running_) {
      break;
    }
    // 2. Draw
    RunDraw();
    if (!is_running_) {
      break;
    }
    // 3. (optional) Simulation
    if (is_physics_running_) {
      RunPhysics();
    }
    // 4. FPS Limitation
    if (fps_limit_ > 0) {
      using namespace std::chrono;
      loop_time_ = (now::us() - start) / 1000.0;
      auto ms_sleep
          = 1.0 / fps_limit_ - loop_time_;
      if (ms_sleep > 1) {
        co::sleep(ms_sleep);
      }
    }
  }
  PostRun();
}

void WorldCtrlUiOnly::ProcessInput() {
  glfwPollEvents();
  auto* window_ptr = get_vk_context().GetWindow()->GetWindow();
  if (get_vk_context().GetWindow()->ShouldClose()) {
    is_running_ = false;
    return;
  }
  for (const auto& [k, v] : keyboard_callbacks_) {
    // only process key press here.
    if (glfwGetKey(window_ptr, k) == GLFW_PRESS) {
      ACG_DEBUG_LOG("Key {} is pressed.", k);
      ACG_CHECK(v.callback(), "Callback failed.");
    }
  }
}

vk::CommandBuffer WorldCtrlUiOnly::DrawUI() {
  ImGui_ImplVulkan_NewFrame();
  ImGui_ImplGlfw_NewFrame();
  ImGui::NewFrame();
  RunUiImpl();
  ImGui::Render();
  auto* data = ImGui::GetDrawData();
  return ui_ppl_->Render(data);
}

void WorldCtrlUiOnly::RunDraw() {
  auto&& context = get_vk_context();
  if (!context.BeginDraw()) {
    RecreateSwapchain();
    return;
  }
  auto cbs = DrawScene();
  cbs.push_back(DrawUI());
  if (context.EndDrawSubmitPresent(cbs)) {
    RecreateSwapchain();
  }
}

void WorldCtrlUiOnly::RecreateSwapchain() {
  get_vk_context().RecreateSwapchain();
  RecreateSwapchainCallback();
  ui_ppl_->RecreateSwapchain();
}

void WorldCtrlUiOnly::CleanUp() {
  get_vk_context().GetDevice().waitIdle();
  // Cleanup the pipeline.
  ui_ppl_.reset();
  // NOTE: because vk context is not created by the world, these resources
  // are not required to release
  CleanUpCallback();
}

void WorldCtrlUiOnly::PreRun() {
  // DO NOTHING
}

void WorldCtrlUiOnly::PostRun() {
  // DO NOTHING
}

std::vector<vk::CommandBuffer> WorldCtrlUiOnly::DrawScene() {
  // Simply return a empty list.
  return {};
}

void WorldCtrlUiOnly::RecreateSwapchainCallback() {
  // DO NOTHING
}

void WorldCtrlUiOnly::CleanUpCallback() {
  // DO NOTHING
}

void WorldCtrlUiOnly::InitCallback() {
  // DO NOTHING
}

int WorldCtrlUiOnly::RunPhysicsImpl(F64 /*dt*/) { return 0; }

void WorldCtrlUiOnly::RunUiImpl() { 
  ImGui::ShowUserGuide(); 
  ImGui::Begin("Stats");
  ImGui::Text("Loop Time = %.3lf(us)", loop_time_ * 1000.0);
  ImGui::End();
}

void WorldCtrlUiOnly::Init() {
  InitCallback();
  // Initialize the Ui Pipeline
  auto ptr = details::UiPipeline::Builder().SetIsUIOnly(ui_only_mode_).Build();
  ui_ppl_.swap(ptr);
}

WorldCtrlUiOnly::WorldCtrlUiOnly() {
  // RAII, init everything.
  Init();
}

WorldCtrlUiOnly::~WorldCtrlUiOnly() { CleanUp(); }

void WorldCtrlUiOnly::RunPhysics() {}

}  // namespace acg::visualizer
