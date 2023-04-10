//
// Created by Yang Jerry on 2023/4/9.
//

#pragma once

#include <acore/common.hpp>
#include <taskflow/taskflow.hpp>

namespace acg::parallel {
inline tf::Executor make_executor(size_t num_threads = std::thread::hardware_concurrency()) {
  return tf::Executor(num_threads);
}

template<typename Task, typename F>
void foreach_index(tf::Executor* executor, Index beg, Index end, Index step, F&& fu) {

}

}