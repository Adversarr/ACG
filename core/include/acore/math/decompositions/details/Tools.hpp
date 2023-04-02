/**
Copyright (c) 2016 Theodore Gast, Chuyuan Fu, Chenfanfu Jiang, Joseph Teran

Permission is hereby granted, free of charge, to any person obtaining a copy of
this software and associated documentation files (the "Software"), to deal in
the Software without restriction, including without limitation the rights to
use, copy, modify, merge, publish, distribute, sublicense, and/or sell copies
of the Software, and to permit persons to whom the Software is furnished to do
so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in all
copies or substantial portions of the Software.

If the code is used in an article, the following paper shall be cited:
@techreport{qrsvd:2016,
  title={Implicit-shifted Symmetric QR Singular Value Decomposition of 3x3
Matrices}, author={Gast, Theodore and Fu, Chuyuan and Jiang, Chenfanfu and
Teran, Joseph}, year={2016}, institution={University of California Los Angeles}
}

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
SOFTWARE.

################################################################################
This file provides a random number generator and a timer.
Sample usage:
    RandomNumber<float> rand;
    float x = randReal(-0.5, 0.8);

    Timer timer;
    timer.start();
    SOME CODE A
    std::cout<<"CODE A took "<<timer.click()<<" seconds"<<std::endl;
    SOME CODE B
    std::cout<<"CODE B took "<<timer.click()<<" seconds"<<std::endl;
################################################################################
*/

#ifndef JIXIE_SVD_TOOLS_H
#define JIXIE_SVD_TOOLS_H

#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wdeprecated-declarations"
#include <Eigen/Core>
#include <Eigen/Dense>
#include <Eigen/SVD>
#pragma GCC diagnostic pop

#if defined(__i386__) || defined(__x86_64__)
#  include <immintrin.h>
#  define JIXIE_RSQRT32(x) _mm_cvtss_f32(_mm_rsqrt_ss(_mm_set_ss((x))))
#endif

#if defined(__arm64__)
#  include <arm_neon.h>
#  define JIXIE_RSQRT32(x) vrsqrtes_f32(x)
#  define JIXIE_RSQRT64(x) vrsqrted_f64(x)
#endif

// #include <mmintrin.h>
// #include <xmmintrin.h>
#include <chrono>
#include <cmath>
#include <iomanip>
#include <iostream>
#include <random>

namespace third_party {
namespace JIXIE {

template <bool B, class T = void> using enable_if_t =
    typename std::enable_if<B, T>::type;

/**
    Random number generator.
*/
template <class T> class RandomNumber {
public:
  std::mt19937 generator;

  explicit RandomNumber(unsigned s = 123) : generator(s) {}

  ~RandomNumber() = default;

  /**
     Random real number from an interval
  */
  T randReal(T a, T b) {
    std::uniform_real_distribution<T> distribution(a, b);
    return distribution(generator);
  }

  /**
    Fill with uniform random numbers
  */
  template <class Derived> void fill(Eigen::DenseBase<Derived> &x, T a, T b) {
    for (typename Derived::Index i = 0; i < x.size(); i++)
      x(i) = randReal(a, b);
  }
};

namespace MATH_TOOLS {

/**
\brief Inverse square root
computed from approx_rsqrt and one newton step
*/
inline float rsqrt(float x) {
#ifdef JIXIE_RSQRT32
  return JIXIE_RSQRT32(x);
#else
  return static_cast<float>(1) / std::sqrt(x);
#endif
}

/**
\brief Approximate inverse square root

A fast approximation to the inverse sqrt
The relative error is less than  1.5*2^-12
*/
inline float approx_rsqrt(float a) {
#ifdef JIXIE_RSQRT32
  return JIXIE_RSQRT32(a);
#else
  float x = a;
  float xhalf = 0.5f * x;
  int i = *reinterpret_cast<int *>(&x);  // View x as an int.
  i = 0x5f37599e - (i >> 1);             // Initial guess.
  x = *reinterpret_cast<float *>(&i);    // View i as F.
  x = x * (1.5f - xhalf * x * x);        // Newton step.
  x = x * (1.5f - xhalf * x * x);        // Newton step again.
  return x;
#endif
}

/**
\brief Inverse square root
computed from 1/std::sqrt
*/
inline double rsqrt(double a) {
#ifdef JIXIE_RSQRT64
  return JIXIE_RSQRT64(a);
#else
  using std::sqrt;
  return 1 / sqrt(a);
#endif
}
}  // namespace MATH_TOOLS

namespace INTERNAL {
using namespace std;
template <class T, class Enable = void> struct ScalarTypeHelper {
  using type = typename T::Scalar;
};
template <class T>
struct ScalarTypeHelper<T, enable_if_t<is_arithmetic<T>::value>> {
  using type = T;
};
}  // namespace INTERNAL

template <class T> using ScalarType =
    typename INTERNAL::ScalarTypeHelper<T>::type;

template <class MatrixType> constexpr bool isSize(int m, int n) {
  return MatrixType::RowsAtCompileTime == m
         && MatrixType::ColsAtCompileTime == n;
}
}  // namespace JIXIE
}  // namespace third_party
#endif
