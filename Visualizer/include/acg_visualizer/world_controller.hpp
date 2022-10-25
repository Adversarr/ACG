#pragma once

#include "renderer.hpp"
#include "ui_pipeline.hpp"
#include "mesh_pipeline.hpp"

namespace acg::visualizer {

class TheWorld {
public:
  using KeyboardType = int;

  /**
   * @brief Run Physics Simulation
   * 
   * @param dt world time change
   * @return true 
   * @return false 
   */
  virtual bool RunPhysicsImpl(F32 dt) = 0;

  /**
   * @brief UI Interactor
   * 
   * @param dt world time change
   * @return true no error is detected.
   * @return false error occurs.
   */
  virtual bool RunUiImpl(F32 dt) = 0;

  virtual bool PreRun();

  virtual bool PostRun();
  
  TheWorld();

  ~TheWorld();

  void Run();

  void RunAsync();


protected:
  void RunOnce();
  void UpdateCamera();
  void RefitBuffer();


  void SetupCameraKeyboardCallback();

  void RunPhysics();
  void RunDraw();

private:
  void Init();
  void CleanUp();
  // TODO: Last Edit
  Camera camera_;
  
  // Physical Time step in second.
  F32 time_step_{.01};
  uint32_t render_per_steps_{3};

  std::unique_ptr<details::Renderer> renderer_;

  std::unique_ptr<details::MeshPipeline> mesh_ppl_;

  std::unique_ptr<details::UiPipeline> ui_ppl_;

  // TODO: Force use staging buffers.
  details::Renderer::BufMem staging_index_buffer_;

  details::Renderer::BufMem staging_vertice_buffer_;

  details::Scene scene_;
  int frame_count_;
  std::mutex scene_read_mtx_;
  std::mutex scene_write_mtx_;

  uint32_t fps_limit_{60};

  bool is_running_{false};

  std::map<KeyboardType, std::function<bool()>> keyboard_callbacks_;
};

}
