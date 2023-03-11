#pragma once

namespace acg::math {

/**
 * NOTE: What is a operator?
 * 1. have zero, one, or multiple `Input`, each input should be exactly one column of some field;
 * 2. have one, or multiple `Output`, each output should be exactly one column of some field;
 * 3. have zero, one, or multiple `Constants`, each constants are shared, and not changed during the computation;
 * 4. The computation should not change the input buffer, then parallel map is supported.
 * Several Constrants:
 * 1. Variables in one kernel MUST share one floating point type, i.e. Scalar;
 * 2. Each variable should be indexed EXACTLY using type `acg::Index`
 */

template <typename Derived> struct Operator {

};

/**
 */
template <typename Derived> struct Kernel {};

}  // namespace acg::math
