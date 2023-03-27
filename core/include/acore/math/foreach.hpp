/****************************************
 *  TODO: Under construction
 ****************************************/

#pragma once

#include <acore/math/view.hpp>

namespace acg {

template <typename A, typename F> void for_each(A& access, F&& function);
template <typename A, typename F> void for_each(const A& access, F&& function);

template <typename A, typename F>
void for_each_indexed(const A& access, F&& function);
template <typename A, typename F>
void for_each_indexed(A&& access, F&& function);

template <typename A, typename F> void transform(A& access, F&& function);
template <typename A, typename F> void transform(const A& access, F&& function);

template <typename A, typename F>
void transform_indexed(A& access, F&& function);
template <typename A, typename F>
void transform_indexed(const A& access, F&& function);

}  // namespace acg

#include "./details/foreach-inl.hpp"
