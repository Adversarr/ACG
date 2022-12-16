#include <acg_utils/time.hpp>
#include <ctime>
#include <thread>
namespace acg::utils {

I64 GetTimeMs() {
  return static_cast<I64>(clock() * 1000 / CLOCKS_PER_SEC);
}

I64 GetTimeUs() {
  return static_cast<I64>(clock() * 1000000 / CLOCKS_PER_SEC);
}

void SleepSec(I64 s) {
  std::this_thread::sleep_for(std::chrono::duration<I64>(s));
}

void SleepMs(I64 ms) {
  std::this_thread::sleep_for(std::chrono::duration<I64, std::milli>(ms));
}

}