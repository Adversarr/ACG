#include <benchmark/benchmark.h>

#include <acore/math/access.hpp>
using namespace acg;

static void origin_identity(benchmark::State& state) {
  Field<float, 4> field(4, 1024);
  for (auto _ : state) {
    for (auto value : field.colwise()) {
      value.setOnes();
      benchmark::DoNotOptimize(value.sum());
    }
  }
}

static void acc_identity(benchmark::State& state) {
  Field<float, 4> field(4, 1024);
  auto acc = access(field);
  for (auto _ : state) {
    for (auto value : acc) {
      value.setOnes();
      benchmark::DoNotOptimize(value.sum());
    }
  }
}

static void origin_transform(benchmark::State& state) {
  Field<float, 4> field(4, 1024);
  for (auto _ : state) {
    for (auto value : field.colwise()) {
      auto transformed = value.reshaped(2, 2);
      transformed.setOnes();
      benchmark::DoNotOptimize(transformed.sum());
    }
  }
}

static void acc_transform(benchmark::State& state) {
  Field<float, 4> field(4, 1024);
  auto acc = access<acg::DefaultIndexer, acg::ReshapeTransform<2, 2>>(field);
  for (auto _ : state) {
    for (auto value : acc) {
      value.setOnes();
      benchmark::DoNotOptimize(value.sum());
    }
  }
}

static void origin_transform_2d(benchmark::State& state) {
  Field<float, 4> field(4, 1024);
  auto acc = access<DefaultIndexer, acg::ReshapeTransform<2, 2>>(field);
  for (auto _ : state) {
    for (Index i = 0; i < 32; ++i) {
      for (Index j = 0; j < 32; ++j) {
        auto blk = acc(i * 32 + j);
        blk.setOnes();
        benchmark::DoNotOptimize(blk.sum());
      }
    }
  }
}

static void acc_transform_2d(benchmark::State& state) {
  Field<float, 4> field(4, 1024);
  auto acc = access<acg::MultiDimensionIndexer<2>, acg::ReshapeTransform<2, 2>>(
      field, MultiDimensionIndexer<2>(32, 32));
  for (auto _ : state) {
    for (Index i = 0; i < 32; ++i) {
      for (Index j = 0; j < 32; ++j) {
        auto blk = acc(i, j);
        blk.setOnes();
        benchmark::DoNotOptimize(blk.sum());
      }
    }
  }
}
// Register the function as a benchmark
BENCHMARK(origin_identity);
BENCHMARK(acc_identity);

BENCHMARK(origin_transform);
BENCHMARK(acc_transform);

BENCHMARK(origin_transform_2d);
BENCHMARK(acc_transform_2d);
// Run the benchmark
BENCHMARK_MAIN();
