#include <benchmark/benchmark.h>
#include "iter.hpp"
#include <acore/access.hpp>
#include <iostream>
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


std::tuple<int, int> rc(2000, 2000);

void iter1d_origin(benchmark::State& state) {
  Field<float, 2> pos(2, std::get<0>(rc));
  for (auto _ : state) {
    auto acc = access(pos);
    for (int i = 0; i < std::get<0>(rc); ++i) {
      benchmark::DoNotOptimize(acc(i).norm());
    }
  }
}

void iter1d_iter(benchmark::State& state) {
  Field<float, 2> pos(2, std::get<0>(rc));
  for (auto _ : state) {
    auto acc = access(pos);
    for (auto i : Iter1DWrapper(std::get<0>(rc))) {
      benchmark::DoNotOptimize(acc(i).norm());
    }
  }
}

void iter2d_origin(benchmark::State& state) {
  Field<float, 4> pos(4, std::get<0>(rc) * std::get<1>(rc));
  pos.setRandom();
  for (auto _ : state) {
    benchmark::DoNotOptimize(std::get<0>(rc));
    benchmark::DoNotOptimize(std::get<1>(rc));
    float csum = 0;
    auto acc = access(pos, acg::MultiDimensionIndexer<2>{std::get<0>(rc), std::get<1>(rc)});
    const auto rows = std::get<0>(rc), cols = std::get<1>(rc);
    for (int i = 0; i < rows; i++) {
      for (int j = 0; j < cols; ++j) {
        csum += acc(i, j).norm();
      }
    }
    benchmark::DoNotOptimize(csum);
  }
}

void iter2d_iter(benchmark::State& state) {
  Field<float, 4> pos(4, std::get<0>(rc) * std::get<1>(rc));
  pos.setRandom();
  auto acc = access(pos, acg::MultiDimensionIndexer<2>{std::get<0>(rc), std::get<1>(rc)});
  for (auto _ : state) {
    float csum = 0;
    Iter2DWrapper wrap(std::get<0>(rc), std::get<1>(rc));
    for (auto&& [i, j] : wrap) {
       csum += acc(i, j).norm();
    }
    benchmark::DoNotOptimize(csum);
  }
}
// Register the function as a benchmark
BENCHMARK(iter1d_origin);
BENCHMARK(iter1d_iter);
BENCHMARK(iter2d_origin);
BENCHMARK(iter2d_iter);
BENCHMARK(origin_identity);
BENCHMARK(acc_identity);

BENCHMARK(origin_transform);
BENCHMARK(acc_transform);

BENCHMARK(origin_transform_2d);
BENCHMARK(acc_transform_2d);
// Run the benchmark
BENCHMARK_MAIN();

