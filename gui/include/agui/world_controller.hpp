#pragma once

#include <acore/common.hpp>
#include <functional>
#include <map>

#include "backend/ui_pipeline.hpp"

namespace acg::gui {

/****************************************
 *
 * WARN: Deprecated.
 *
 ****************************************/
class WorldCtrlUiOnly {
  using KeyType = int;

  using KeyboardCallback = std::function<bool()>;

protected:
  /**
   * @brief Run Physics Simulation
   *
   * @param dt world time change.
   */
  virtual int RunPhysicsImpl(Float64 dt);

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
  [[deprecated("This class has deprecated. Use acg::gui::Gui instead!")]] WorldCtrlUiOnly();

  ~WorldCtrlUiOnly();

  WorldCtrlUiOnly& Run();

  void CleanUp();

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

  virtual void ProcessInputImpl();

private:
protected:
  // Ui Pipeline is the only pipeline.
  bool ui_only_mode_{true};

  // FPS Limitation, 0 indicates no limit.
  uint32_t fps_limit_{30};

  // Physical Time step in second.
  Float64 time_step_{.01};

  std::unique_ptr<details::UiPipeline> ui_ppl_{nullptr};

  int frame_count_;

  bool is_async_{false};

  // Indicate the world is still running.
  bool is_running_{false};

  // Indicate whether physical simulation is running.
  bool is_physics_running_{false};

  // Hooks for keyboard callback.
  std::map<KeyType, KeyboardCallback> keyboard_callbacks_;

  Float64 loop_time_{0};
};

}  // namespace acg::gui
