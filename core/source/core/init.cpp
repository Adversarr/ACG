#include "acore/init.hpp"
#include "autils/common.hpp"
#include <autils/log.hpp>
#include <spdlog/spdlog.h>
#include <iostream>

namespace acg::details {

std::vector<InitHook> global_hooks;

bool acg_initialized;

void add_hook(const InitHook& hook) { 
  global_hooks.push_back(hook);

}

static void sort() {
  std::sort(global_hooks.begin(), global_hooks.end(),
            [](const InitHook& l, const InitHook& r) { return l.priority > r.priority; });
}

void init_hooks() {
  sort();
  for (const auto& hook : global_hooks) {
    if (hook.on_init) {
      if constexpr (acg::utils::is_debug_mode) {
        if (hook.name)
          std::cout << fmt::format("Execute initialize hook: {}", hook.name) << std::endl;
      }
      hook.on_init.value()();
    }
  }
}

void cleanup_hooks() {
  auto run_clean = [](const InitHook& hook) {
    if (hook.on_exit) {
      if (hook.name) std::cout << fmt::format("Execute cleanup hook: {}", hook.name) << std::endl;
      hook.on_exit.value()();
    }
  };
  std::for_each(std::make_reverse_iterator(global_hooks.end()),
                std::make_reverse_iterator(global_hooks.begin()), run_clean);
}

}  // namespace acg::details

namespace acg {

void init(int argc, char** argv) { 
  ACG_CHECK(!details::acg_initialized, "ACG has been initialized.");
  utils::details::do_nothing(argc, argv);
  details::init_hooks();
  details::acg_initialized = true;
}

bool is_inited() {
  return details::acg_initialized;
}

/**
 * @param
 */
void clean_up() { 
  ACG_CHECK(acg::is_inited(), "ACG has not been initialize.");
  details::cleanup_hooks(); 
  details::acg_initialized = false;
}
}  // namespace acg
