/**
 * align.hpp: Support alignment and byte ops.
 */


#pragma once
#include <cstddef>

namespace acg::utils {
// Reference: https://github.com/idealvin/coost/blob/master/include/co/god.h

// @align must be power of 2
template <typename X, typename A>
inline X align_up(X x, A align) {
    const X o = static_cast<X>(align) - 1;
    return (x + o) & ~o;
}

template <size_t A, typename X>
inline X align_up(X x) {
    const X o = static_cast<X>(A) - 1;
    return (x + o) & ~o;
}

template <typename X, typename A>
inline X* align_up(X* x, A align) {
    return (X*) align_up((size_t)x, align);
}

template <size_t A, typename X>
inline X* align_up(X* x) {
    return (X*) align_up<A>((size_t)x);
}

// @align must be power of 2
template <typename X, typename A>
inline X align_down(X x, A align) {
    const X o = static_cast<X>(align) - 1;
    return x & ~o;
}

template <size_t A, typename X>
inline X align_down(X x) {
    const X o = static_cast<X>(A) - 1;
    return x & ~o;
}

template <typename X, typename A>
inline X* align_down(X* x, A align) {
    return (X*) align_down((size_t)x, align);
}

template <size_t A, typename X>
inline X* align_down(X* x) {
    return (X*) align_down<A>((size_t)x);
}

// b4(15) -> 4, b4(32) -> 8
template <typename T>
inline T b4(T n) {
    return (n >> 2) + !!(n & 3);
}

// b8(15) -> 2, b8(32) -> 4
template <typename T>
inline T b8(T n) {
    return (n >> 3) + !!(n & 7);
}

// b16(15) -> 1, b16(32) -> 2
template <typename T>
inline T b16(T n) {
    return (n >> 4) + !!(n & 15);
}

// b4k(4095) -> 1, b4k(8192) -> 2
template <typename T>
inline T b4k(T n) {
    return (n >> 12) + !!(n & 4095);
}

// whether the first sizeof(T) bytes are equal
template <typename T>
inline bool byte_eq(const void* p, const void* q) {
    return *(const T*)p == *(const T*)q;
}

template <int N>
inline void byte_cp(void* dst, const void* src) {
    byte_cp<N - 1>(dst, src);
    *((char*)dst + N - 1) = *(static_cast<const char*>(src) + N - 1);
}
}
