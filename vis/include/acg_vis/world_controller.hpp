#pragma once

#include <acg_core/core.hpp>
#include <map>
#include <functional>

#include "ui_pipeline.hpp"

namespace acg::visualizer {

class WorldCtrlUiOnly {
  using KeyType = int;

  struct KeyboardCallback {
    const char* description;
    std::function<bool()> callback;
  };

protected:
  /**
   * @brief Run Physics Simulation
   *
   * @param dt world time change.
   */
  virtual int RunPhysicsImpl(F64 dt);

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
  virtual std::vector<vk::CommandBuffer> DrawScene();

  /**
   * @brief This function is called whever swapchain is recreated.
   *
   */
  virtual void RecreateSwapchainCallback();

  virtual void CleanUpCallback();

  virtual void InitCallback();

public:
  WorldCtrlUiOnly();

  ~WorldCtrlUiOnly();

  void Run();

  void RunAsync();

  WorldCtrlUiOnly& Init();
protected:
  /**
   * @brief Recreate Swapchain.
   *
   */
  void RecreateSwapchain();

  void RunPhysics();

  void RunDraw();

  vk::CommandBuffer DrawUI();

  void ProcessInput();
private:

  void CleanUp();

protected:
  bool ui_only_mode_{true};

  // Physical Time step in second.
  F64 time_step_{.01};

  std::unique_ptr<details::UiPipeline> ui_ppl_{nullptr};

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

  F64 loop_time_{0};
};

}  // namespace acg::visualizer
