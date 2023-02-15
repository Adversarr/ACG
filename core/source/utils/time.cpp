#include <autils/time.hpp>
#include <ctime>
#include <thread>
namespace acg::utils {

I64 get_time_ms() {
  return static_cast<I64>(clock() * 1000 / CLOCKS_PER_SEC);
}

I64 get_time_us() {
  return static_cast<I64>(clock() * 1000000 / CLOCKS_PER_SEC);
}

void sleep_for_seconds(I64 s) {
  std::this_thread::sleep_for(std::chrono::duration<I64>(s));
}

void sleep_for_ms(I64 ms) {
  std::this_thread::sleep_for(std::chrono::duration<I64, std::milli>(ms));
}

}
