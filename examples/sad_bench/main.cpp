#include <benchmark/benchmark.h>

#include <acg_core/sad/la.hpp>
#include <acg_core/sad/lazy.hpp>
#include <iostream>
static void non_lazy(benchmark::State& state) {
  // Perform setup here
  using namespace acg::sad;
  Constant_expr(acg::Vec3f, d0, v.y() = v.z() = 0; v.x() = 1);
  Constant_expr(acg::Vec3f, d1, v.x() = v.z() = 0; v.y() = 1);
  Constant_expr(acg::Vec3f, d2, v.x() = v.y() = 0; v.z() = 1);
  Variable(acg::Vec3f, X);
  Variable(acg::Vec3f, Y);
  using FinalExp = Dot<Sub<X, Y>, Sub<X, Y>>;
  using Dx0 = Simpliest_t<DirectionalDiff_t<FinalExp, X, d0>>;
  using Dx1 = Simpliest_t<DirectionalDiff_t<FinalExp, X, d1>>;
  using Dx2 = Simpliest_t<DirectionalDiff_t<FinalExp, X, d2>>;
  using Dy0 = Simpliest_t<DirectionalDiff_t<FinalExp, Y, d0>>;
  using Dy1 = Simpliest_t<DirectionalDiff_t<FinalExp, Y, d1>>;
  using Dy2 = Simpliest_t<DirectionalDiff_t<FinalExp, Y, d2>>;
  for (auto _ : state) {
    // This code gets timed
    Context<acg::utils::god::List<FinalExp, Dx0, Dx1, Dx2, Dy0, Dy1, Dy2>> context;
    context.Set<X>(acg::Vec3f{1, 2, 3});
    context.Set<Y>(acg::Vec3f{3, 2, 1});
    acg::sad::run(context);
  }
}
static void lazy(benchmark::State& state) {
  // Perform setup here
  using namespace acg::sad;
  Constant_expr(acg::Vec3f, d0, v.y() = v.z() = 0; v.x() = 1);
  Constant_expr(acg::Vec3f, d1, v.x() = v.z() = 0; v.y() = 1);
  Constant_expr(acg::Vec3f, d2, v.x() = v.y() = 0; v.z() = 1);
  Variable(acg::Vec3f, X);
  Variable(acg::Vec3f, Y);
  using FinalExp = Dot<Sub<X, Y>, Sub<X, Y>>;
  using Dx0 = Simpliest_t<DirectionalDiff_t<FinalExp, X, d0>>;
  using Dx1 = Simpliest_t<DirectionalDiff_t<FinalExp, X, d1>>;
  using Dx2 = Simpliest_t<DirectionalDiff_t<FinalExp, X, d2>>;
  using Dy0 = Simpliest_t<DirectionalDiff_t<FinalExp, Y, d0>>;
  using Dy1 = Simpliest_t<DirectionalDiff_t<FinalExp, Y, d1>>;
  using Dy2 = Simpliest_t<DirectionalDiff_t<FinalExp, Y, d2>>;
  LazyContext<acg::utils::god::List<FinalExp, Dx0, Dx1, Dx2, Dy0, Dy1, Dy2>> context;
  for (auto _ : state) {
    context.Set<X>(acg::Vec3f{1, 2, 3});
    context.Set<Y>(acg::Vec3f{3, 2, 1});
    auto result = LazyResult(context);
    result.Get<Dy2>();
  }
}
// Register the function as a benchmark
BENCHMARK(non_lazy);
BENCHMARK(lazy);
// Run the benchmark
BENCHMARK_MAIN();
