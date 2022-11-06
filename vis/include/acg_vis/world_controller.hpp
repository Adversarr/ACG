#pragma once

#include "mesh_pipeline.hpp"
#include "renderer.hpp"
#include <spdlog/stopwatch.h>
#include "ui_pipeline.hpp"

namespace acg::visualizer::details {

class TheWorld {
  using KeyboardType = int;

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
  virtual void RunUiImpl() = 0;

  virtual void PreRun();

  virtual void PostRun();

public:
  TheWorld();

  ~TheWorld();

  void Run();

  void RunAsync();

protected:
  void UpdateCamera();
  void RefitBuffer();

  void SetupCameraKeyboardCallback();

  void RunPhysics();

  void RunDraw();

  void RunUI();

  void ProcessInput();

private:
  void Init();
  void CleanUp();

  Camera camera_;

  // Physical Time step in second.
  F32 time_step_{.01};
  uint32_t render_per_steps_{3};

  std::unique_ptr<details::MeshPipeline> mesh_ppl_;

  std::unique_ptr<details::UiPipeline> ui_ppl_;

  // TODO: Force use staging buffers.
  details::VkContext::BufMem staging_index_buffer_;

  details::VkContext::BufMem staging_vertice_buffer_;

  int frame_count_;

  // TODO: Support async physics run.
  bool is_async_{false};
  std::mutex scene_read_mtx_;
  std::mutex scene_write_mtx_;

  uint32_t fps_limit_{60};

  // Indicate the world is still running.
  bool is_running_{false};

  // Indicate whether physical simulation is running.
  bool is_physics_running_{false};

  std::map<KeyboardType, std::function<bool()>> keyboard_callbacks_;

  spdlog::stopwatch single_frame_watch_;
};

}  // namespace acg::visualizer
