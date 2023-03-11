#pragma once

#include <acore/common.hpp>
#include <array>
#include <numeric>

namespace acg::utils {

template <typename T, Index N> class RotingRecord {
  static_assert(N > 0, "Array size N should be greater than zero.");

public:
  /****************************************
   * @brief Initialize default value in RR.
   ****************************************/
  explicit RotingRecord(T value);

  RotingRecord() = default;

  void Record(const T& value);

  T* Ptr();

  T Sum(T init = 0) const;

  T Mean(T init = 0) const;

  T Last() const { return data_[cur_index_]; }

  template <typename BinaryOp> T Reduce(T init, BinaryOp) const;

  void Reset();

private:
  std::array<T, N> data_;
  Index cur_index_{0};
};

}  // namespace acg::utils

#include "./details/roting_record-inl.hpp"
