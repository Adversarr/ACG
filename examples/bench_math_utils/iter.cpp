#include "./iter.hpp"
Iter2D& Iter2D::operator++() {
  auto& i = std::get<0>(ij);
  auto& j = std::get<1>(ij);
  j += 1;
  // This cost lots of time...
  if (j == m) [[unlikely]] {

    j = 0;
    i += 1;
  }
  return *this;
}
