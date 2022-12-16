#pragma once
#include <acg_core/common.hpp>
namespace acg::utils {

I64 GetTimeUs();
I64 GetTimeMs();

void SleepSec(I64 s);

void SleepMs(I64 ms);

}  // namespace acg::utils