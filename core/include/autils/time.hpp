#pragma once
#include <acore/common.hpp>
namespace acg::utils {

I64 get_time_us();
I64 get_time_ms();

void sleep_for_seconds(I64 s);

void sleep_for_ms(I64 ms);

}  // namespace acg::utils
