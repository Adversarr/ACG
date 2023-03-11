#include <autils/time/time.hpp>
#include <ctime>
#include <thread>
namespace acg::utils {

Int64 get_time_ms() {
  return static_cast<Int64>(clock() * 1000 / CLOCKS_PER_SEC);
}

Int64 get_time_us() {
  return static_cast<Int64>(clock() * 1000000 / CLOCKS_PER_SEC);
}

void sleep_for_seconds(Int64 s) {
  std::this_thread::sleep_for(std::chrono::duration<Int64>(s));
}

void sleep_for_ms(Int64 ms) {
  std::this_thread::sleep_for(std::chrono::duration<Int64, std::milli>(ms));
}

}
