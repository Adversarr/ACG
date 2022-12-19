#include "acg_core/init.hpp"

#include <spdlog/spdlog.h>

#include <spdlog/spdlog.h>

namespace acg::details {

std::vector<InitHook> global_hooks;

void add_hook(const InitHook& hook) { global_hooks.push_back(hook); }

static void sort() {
  std::sort(global_hooks.begin(), global_hooks.end(),
            [](const InitHook& l, const InitHook& r) { return l.priority > r.priority; });
}
void init() {
  sort();
  for (const auto& cb : global_hooks) {
    if (cb.on_init) {
      if (cb.name) spdlog::info("Run initialize hook: {}", cb.name);
      cb.on_init.value()();
    }
  }
}

void cleanup() {
  for (const auto& cb : global_hooks) {
    if (cb.on_exit) {
      if (cb.name) spdlog::info("Run cleanup hook: {}", cb.name);
      cb.on_exit.value()();
    }
  }
}


}  // namespace acg::details

namespace acg {

void init(int argc, char** argv) { 
  details::init(); 
}

/**
  * @param
  */
void clean_up() { details::cleanup(); }
}  // namespace acg
