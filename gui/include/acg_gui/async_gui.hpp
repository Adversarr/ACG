#pragma once
#include <imgui.h>

#include <acg_utils/result.hpp>
#include <mutex>

#include "camera.hpp"
#include "light.hpp"
#include "scene.hpp"

namespace acg::gui {

struct GuiEvent {
  enum class Kind {
    kShutdown,
    kSceneFullUpdate,
    kSceneMeshUpdate,
    kSceneParticleUpdate,
    kSceneMeshParticleUpdate,
    kSceneWireframeUpdate,
    kPlaneUpdate,
    kCameraUpdate,
    kLightUpdate,
  };

  enum Component : int { kVertex = 1, kIndex = 2, kInstance = 4, kModel = 8, kAll = 15 };

  Kind kind;
  int component;
  size_t object_id;

  // all update flag
  static constexpr size_t ALL_UPD_ = std::numeric_limits<size_t>::max();

  explicit GuiEvent(Kind k, size_t o, int component)
      : kind(k), component(component), object_id(o) {}

  explicit GuiEvent(Kind k, size_t o) : GuiEvent(k, o, kAll) {}

  explicit GuiEvent(Kind k) : GuiEvent(k, ALL_UPD_) {}

  bool IsAllUpdate() const { return object_id == ALL_UPD_; }
};

class AsyncGui {
public:
  ~AsyncGui();
  struct Config {
    std::thread::id host_id;
    Light light;
    Camera camera;
    Scene2 scene;
    bool ignore_unprocessed_device_event = false;
    uint32_t device_lock_try_duration = 100;
    uint32_t device_lock_try_rounds = 3;
  };


  /**
   * @brief register a gui event.
   */
  AsyncGui& RegisterHostEvent(const GuiEvent& event);

  /**
   * @brief register a device event.
   */
  AsyncGui& RegisterDeviceEvent(const GuiEvent& event);

  /**
   * @brief sync scene object to host
   */
  acg::Status SyncDeviceToHost();

  /**
   * @brief shutdown the device thread immediately, not use the event loop.
   */
  void ShutdownImmediately();

  /**
   * @brief send a shutdown signal to the thread
   */
  void SignalShutdown(bool need_lock = false);
  /**
   * @brief Enable x-y plane draw for debug.
   */
  acg::Status EnableXyPlaneDraw(acg::Vec4f color, acg::Vec2f range, acg::Vec2f density,
                                F32 height = 0.0f);
  void DisableXyPlaneDraw();

  /**
   * @brief set UI custom draw callback
   */
  AsyncGui& SetUIDrawCallback(std::function<void()> callback);

  // Accessor:
  Scene2& GetScene();
  Camera& GetCamera();
  Light& GetLight();

  // Lock immediately.
  [[nodiscard]] std::unique_lock<std::timed_mutex> Lock();
  [[nodiscard]] std::unique_lock<std::timed_mutex> Lock(std::defer_lock_t);
  [[nodiscard]] std::unique_lock<std::timed_mutex> Lock(std::try_to_lock_t);
  [[nodiscard]] std::unique_lock<std::timed_mutex> TryLock();

private:
  /**
   * @brief sync scene object to device. if event is not empty, will do additive
   *        update
   */
  acg::Status SyncHostToDevice();

  // Device thread start function
  void Run();

  Status ProcessEvents();

  // Constructor, for hooking.
  explicit AsyncGui(const Config& config);

  // p-impl mode for hiding vulkan based things
  class AsyncGui_Impl;

  std::unique_ptr<AsyncGui_Impl> impl_;
  // AsyncGui_Impl will have objects for backup and swapping.(double-buffering)
  Scene2 host_scene_;
  Light host_light_;
  Camera host_camera_;
  std::vector<GuiEvent> host_events_;
  std::vector<GuiEvent> device_events_;
  std::timed_mutex mutex_;
  std::thread::id host_id_;
  std::thread::id device_id_;

  Vec2f xy_plane_range_;
  Vec4f xy_plane_color_;
  Vec2f xy_plane_density_;
  float xy_plane_height_;
  bool xy_plane_enable_;
  std::atomic_bool should_shutdown_;

  const bool ignore_unprocessed_device_event;
  const uint32_t try_lock_duration_ = 100;
  const uint32_t try_lock_rounds_ = 3;

  // Callback for ui
  std::function<void()> ui_draw_callback_;

  std::unique_ptr<std::thread> child_thread_;
};

}  // namespace acg::gui
