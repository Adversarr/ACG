#pragma once
#include "scene.hpp"
#include "light.hpp"
#include "camera.hpp"

namespace acg::gui {

class AsyncGui {
public:
  ~AsyncGui();
  struct Config {};

  

private:
  explicit AsyncGui(Config config);

  struct AsyncGui_Impl;

  AsyncGui_Impl* impl_;
};

}  // namespace acg::gui
