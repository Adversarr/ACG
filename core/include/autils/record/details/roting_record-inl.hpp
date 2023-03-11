#pragma once

#include "../roting_record.hpp"

namespace acg::utils {

template <typename T, Index N> void RotingRecord<T, N>::Record(const T& value) {
  data_[cur_index_ += 1] = value;
  cur_index_ %= N;
}

template <typename T, Index N> RotingRecord<T, N>::RotingRecord(T value) {
  std::fill(data_.begin(), data_.end(), value);
}

template <typename T, Index N> T RotingRecord<T, N>::Sum(T init) const {
  return Reduce(init, std::plus<T>{});
}

template <typename T, Index N> T RotingRecord<T, N>::Mean(T init) const {
  return Sum(init) / N;
}

template <typename T, Index N> template <typename BinaryOp>
T RotingRecord<T, N>::Reduce(T init, BinaryOp op) const {
  return std::reduce(data_.begin(), data_.end(), init, op);
}
template <typename T, Index N> T* RotingRecord<T, N>::Ptr() {
  return data_.data();
}
template <typename T, Index N> void RotingRecord<T, N>::Reset() {
  cur_index_ = 0;
}

}  // namespace acg::utils
