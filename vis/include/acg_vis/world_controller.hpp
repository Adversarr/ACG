#pragma once

#include "mesh_pipeline.hpp"
#include "renderer.hpp"
#include <spdlog/stopwatch.h>
#include "ui_pipeline.hpp"

namespace acg::visualizer::details {

class TheWorld {
  using KeyType = int;

  struct KeyboardCallback {
    const char* description;
    std::function<bool()> callback;
  };

private:
  /**
   * @brief Run Physics Simulation
   *
   * @param dt world time change.
   */
  virtual int RunPhysicsImpl(F32 dt) = 0;

  /**
   * @brief UI Interactor
   *
   * @param dt world time change
   * @return true no interruption is detected.
   * @return false interruption occurs.
   */
  virtual void RunUiImpl();

  /**
   * @brief this function is called before world loop start.
   * 
   */
  virtual void PreRun();

  /**
   * @brief this function is called, after world loop ends.
   * 
   */
  virtual void PostRun();

  /**
   * @brief This function render the scene use the mesh pipeline
   * 
   */
  virtual void DrawScene();

  /**
   * @brief This function is called whever swapchain is recreated.
   * 
   */
  virtual void RecreateSwapchainCallback();

  virtual void CleanUpCallback();
  virtual void InitCallback();

public:
  TheWorld();

  ~TheWorld();

  void Run();

  void RunAsync();

protected:
  /**
   * @brief Recreate Swapchain.
   * 
   */
  void RecreateSwapchain();

  void SetupCameraKeyboardCallback();

  void RunPhysics();

  void RunDraw();

  vk::CommandBuffer DrawUI();

  void ProcessInput();

private:
  void Init();

  void CleanUp();

  Camera camera_;

  // Physical Time step in second.
  F64 time_step_{.01};

  // NOTE: Ui Pipeline is the last pipeline.
  std::unique_ptr<details::UiPipeline> ui_ppl_{nullptr};

  // TODO: Force use staging buffers.
  details::VkContext::BufMem staging_index_buffer_;

  details::VkContext::BufMem staging_vertice_buffer_;

  int frame_count_;

  // TODO: Support async physics run.
  bool is_async_{false};

  uint32_t fps_limit_{60};

  // Indicate the world is still running.
  bool is_running_{false};

  // Indicate whether physical simulation is running.
  bool is_physics_running_{false};

  // Hooks for keyboard callback.
  std::map<KeyType, KeyboardCallback> keyboard_callbacks_;

  spdlog::stopwatch single_frame_watch_;
};

}  // namespace acg::visualizer
