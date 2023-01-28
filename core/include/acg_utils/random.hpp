#pragma once
#include <acg_core/common.hpp>

namespace acg::utils {

// Random Number Generator
class Random {
public:
  explicit Random(acg::Ui32 seed) : _seed(seed & 0x7fffffffu) {
    if (_seed == 0 || _seed == 2147483647L) _seed = 1;
  }
  Random() : Random(1u) {}

  acg::Ui32 next() {
    static const acg::Ui32 M = 2147483647L;  // 2^31-1
    static const acg::Ui64 A = 16385;        // 2^14+1

    // Computing _seed * A % M.
    const acg::Ui64 p = _seed * A;
    _seed = static_cast<acg::Ui32>((p >> 31) + (p & M));
    if (_seed > M) _seed -= M;

    return _seed;
  }

private:
  acg::Ui32 _seed;
};
}  // namespace acg::utils