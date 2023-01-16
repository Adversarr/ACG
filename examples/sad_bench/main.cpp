#include <benchmark/benchmark.h>

#include <acg_core/sad/all.hpp>
#include <autodiff/forward/dual.hpp>
#include <autodiff/forward/real/eigen.hpp>

using namespace acg::sad;
Constant_expr(acg::Vec3f, d0, v.y() = v.z() = 0; v.x() = 1);
Constant_expr(acg::Vec3f, d1, v.x() = v.z() = 0; v.y() = 1);
Constant_expr(acg::Vec3f, d2, v.x() = v.y() = 0; v.z() = 1);
Constant_value(float, OriginalLength, 1.0f);
Variable(acg::Vec3f, X);
Variable(acg::Vec3f, Y);
using Distance = Norm<Sub<X, Y>>;
using Energy = Mul<Sub<Distance, OriginalLength>, Sub<Distance, OriginalLength>>;
using FinalExp = Energy;
using Dx0 = Simpliest_t<DirectionalDiff_t<FinalExp, X, d0>>;
using Dx1 = Simpliest_t<DirectionalDiff_t<FinalExp, X, d1>>;
using Dx2 = Simpliest_t<DirectionalDiff_t<FinalExp, X, d2>>;
using Dy0 = Simpliest_t<DirectionalDiff_t<FinalExp, Y, d0>>;
using Dy1 = Simpliest_t<DirectionalDiff_t<FinalExp, Y, d1>>;
using Dy2 = Simpliest_t<DirectionalDiff_t<FinalExp, Y, d2>>;
static void non_lazy(benchmark::State& state) {
  // Perform setup here
  Context<acg::utils::god::List<FinalExp, Dx0, Dx1, Dx2, Dy0, Dy1, Dy2>> context;
  context.Set<X>(acg::Vec3f{1, 2, 3});
  context.Set<Y>(acg::Vec3f{1, 2, 3});
  for (auto _ : state) {
    // This code gets timed
    acg::sad::run(context);
    benchmark::DoNotOptimize(context.Get<Dy1>());
    benchmark::DoNotOptimize(context.Get<Dy2>());
    benchmark::DoNotOptimize(context.Get<Dy0>());
    benchmark::DoNotOptimize(context.Get<Dx1>());
    benchmark::DoNotOptimize(context.Get<Dx2>());
    benchmark::DoNotOptimize(context.Get<Dx0>());
  }
}
static void lazy(benchmark::State& state) {
  // Perform setup here
  using namespace acg::sad;
  LazyContext<acg::utils::god::List<FinalExp, Dx0, Dx1, Dx2, Dy0, Dy1, Dy2>> context;
  context.Set<X>(acg::Vec3f{1, 2, 3});
  context.Set<Y>(acg::Vec3f{1, 2, 3});

  for (auto _ : state) {
    auto result = LazyResult(context);
    benchmark::DoNotOptimize(result.Get<Dy1>());
    benchmark::DoNotOptimize(result.Get<Dy2>());
    benchmark::DoNotOptimize(result.Get<Dy0>());
    benchmark::DoNotOptimize(result.Get<Dx1>());
    benchmark::DoNotOptimize(result.Get<Dx2>());
    benchmark::DoNotOptimize(result.Get<Dx0>());
  }
}
static void lazy2(benchmark::State& state) {
  // Perform setup here
  using namespace acg::sad;
  LazyContext<acg::utils::god::List<FinalExp, Dx0, Dx1, Dx2, Dy0, Dy1, Dy2>> context;
  context.Set<X>(acg::Vec3f{1, 2, 3});
  context.Set<Y>(acg::Vec3f{1, 2, 3});

  for (auto _ : state) {
    auto result = LazyResult2(context);
    benchmark::DoNotOptimize(result.Get<Dy1>());
    benchmark::DoNotOptimize(result.Get<Dy2>());
    benchmark::DoNotOptimize(result.Get<Dy0>());
    benchmark::DoNotOptimize(result.Get<Dx1>());
    benchmark::DoNotOptimize(result.Get<Dx2>());
    benchmark::DoNotOptimize(result.Get<Dx0>());
  }
}

using namespace autodiff;

// The scalar function for which the gradient is needed
real f(const ArrayXreal& x, const ArrayXreal& y) {
  auto dist2 = sqrt(((x - y) * (x - y)).sum());
  return (dist2 - 1) * (dist2 - 1);
}
static void ghad(benchmark::State& state) {
  // Perform setup here
  using namespace acg::sad;
  ArrayXreal x(3);  // the input vector x with 5 variables
  x << 1, 2, 3;     // x = [1, 2, 3, 4, 5]

  ArrayXreal p(3);  // the input parameter vector p with 3 variables
  p << 1, 2, 3;     // p = [1, 2, 3]
  for (auto _ : state) {
    Eigen::VectorXd gx = gradient(f, wrt(x), at(x, p));
    Eigen::VectorXd gp = gradient(f, wrt(p), at(x, p));
  }
  Eigen::VectorXd gx = gradient(f, wrt(p), at(x, p));
}

// Register the function as a benchmark
BENCHMARK(non_lazy);
BENCHMARK(lazy);
BENCHMARK(lazy2);
BENCHMARK(ghad);

// Run the benchmark
BENCHMARK_MAIN();
