#include <benchmark/benchmark.h>

#include <acore/math/view.hpp>

using namespace acg;
template <typename Scalar> struct Func {
  template <typename Derived> Scalar call_derived(const Eigen::MatrixBase<Derived>& field) {
    return field.norm();
  }

  BENCHMARK_ALWAYS_INLINE Scalar call_ref(const Eigen::Ref<const acg::Field<Scalar, 3>>& field) {
    return field.norm();
  }
  BENCHMARK_ALWAYS_INLINE Scalar call_direct(const acg::Field<Scalar, 3>& field) {
    return field.norm();
  }
};

void derived_test(benchmark::State& state) {
  Field<float, 3> field = FieldBuilder<float, 3>(1024).Random();
  for (auto _ : state) {
    benchmark::DoNotOptimize(Func<float>().call_derived(field));
  }
}

void direct_test(benchmark::State& state) {
  Field<float, 3> field = FieldBuilder<float, 3>(1024).Random();
  for (auto _ : state) {
    benchmark::DoNotOptimize(Func<float>().call_direct(field));
  }
}
void ref_test(benchmark::State& state) {
  Field<float, 3> field = FieldBuilder<float, 3>(1024).Random();

  for (auto _ : state) {
    benchmark::DoNotOptimize(Func<float>().call_ref(field));
  }
}

BENCHMARK(derived_test);
BENCHMARK(direct_test);
BENCHMARK(ref_test);
BENCHMARK_MAIN();
