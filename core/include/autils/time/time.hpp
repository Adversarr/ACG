#pragma once
#include <acore/common.hpp>
namespace acg::utils {

Int64 get_time_us();

Int64 get_time_ms();

void sleep_for_seconds(Int64 s);

void sleep_for_ms(Int64 ms);

}  // namespace acg::utils
