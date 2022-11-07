#include "acg_core/init.hpp"

#include <spdlog/spdlog.h>

#include "acg_utils/log.hpp"
#include "co/flag.h"

namespace acg::details {

std::vector<InitHook> global_hooks;

void add_hook(const InitHook& hook) { global_hooks.push_back(hook); }

void init() {
  sort();
  for (const auto& cb : global_hooks) {
    if (cb.on_init) {
      if (cb.name) spdlog::info("Initialize hook: {}", cb.name);
      cb.on_init.value()();
    }
  }
}

void cleanup() {
  for (const auto& cb : global_hooks) {
    if (cb.on_exit) {
      if (cb.name) spdlog::info("Initialize hook: {}", cb.name);
      cb.on_exit.value()();
    }
  }
}

void sort() {
  std::sort(global_hooks.begin(), global_hooks.end(),
            [](const InitHook& l, const InitHook& r) { return l.priority > r.priority; });
}

}  // namespace acg::details

namespace acg {
void init(int argc, char** argv) { 
  details::init(); 
  flag::init(argc, argv);

}

void cleanup() { details::cleanup(); }
}  // namespace acg
