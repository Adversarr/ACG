#include <benchmark/benchmark.h>

#include <acore/math/coordinate/common.hpp>
#include <acore/math/ndrange.hpp>
#include <acore/math/view.hpp>
#include <iostream>

#include "iter.hpp"
using namespace acg;

static void origin_identity(benchmark::State &state) {
  Field<float, 4> field(4, 1024);
  for (auto _ : state) {
    for (auto value : field.colwise()) {
      value.setOnes();
      benchmark::DoNotOptimize(value.sum());
    }
  }
}

static void acc_identity(benchmark::State &state) {
  Field<float, 4> field(4, 1024);
  auto acc = view(field);
  for (auto _ : state) {
    for (auto value : acc) {
      value.setOnes();
      benchmark::DoNotOptimize(value.sum());
    }
  }
}

static void origin_transform(benchmark::State &state) {
  Field<float, 4> field(4, 1024);
  for (auto _ : state) {
    for (auto value : field.colwise()) {
      auto transformed = value.reshaped(2, 2);
      transformed.setOnes();
      benchmark::DoNotOptimize(transformed.sum());
    }
  }
}

static void acc_transform(benchmark::State &state) {
  Field<float, 4> field(4, 1024);
  auto acc = view<acg::DefaultIndexer, acg::ReshapeTransform<2, 2>>(field);
  for (auto _ : state) {
    for (auto value : acc) {
      value.setOnes();
      benchmark::DoNotOptimize(value.sum());
    }
  }
}

static void origin_transform_2d(benchmark::State &state) {
  Field<float, 4> field(4, 1024);
  auto acc = view<DefaultIndexer, acg::ReshapeTransform<2, 2>>(field);
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

static void acc_transform_2d(benchmark::State &state) {
  Field<float, 4> field(4, 1024);
  auto acc = view<acg::NdRangeIndexer<2>, acg::ReshapeTransform<2, 2>>(
      field, NdRangeIndexer<2>(32, 32));
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

void iter1d_origin(benchmark::State &state) {
  Field<float, 2> pos(2, std::get<0>(rc));
  for (auto _ : state) {
    auto acc = view(pos);
    for (int i = 0; i < std::get<0>(rc); ++i) {
      benchmark::DoNotOptimize(acc(i).norm());
    }
  }
}

void iter1d_iter(benchmark::State &state) {
  Field<float, 2> pos(2, std::get<0>(rc));
  for (auto _ : state) {
    auto acc = view(pos);
    for (auto i : Iter1DWrapper(std::get<0>(rc))) {
      benchmark::DoNotOptimize(acc(i).norm());
    }
  }
}

void iter2d_origin(benchmark::State &state) {
  Field<float, 4> pos(4, std::get<0>(rc) * std::get<1>(rc));
  pos.setRandom();
  for (auto _ : state) {
    benchmark::DoNotOptimize(std::get<0>(rc));
    benchmark::DoNotOptimize(std::get<1>(rc));
    float csum = 0;
    auto acc =
        view(pos, acg::NdRangeIndexer<2>{std::get<0>(rc), std::get<1>(rc)});
    const auto rows = std::get<0>(rc), cols = std::get<1>(rc);
    for (int i = 0; i < rows; i++) {
      for (int j = 0; j < cols; ++j) {
        csum += acc(i, j).norm();
      }
    }
    benchmark::DoNotOptimize(csum);
  }
}

void iter2d_iter(benchmark::State &state) {
  Field<float, 4> pos(4, std::get<0>(rc) * std::get<1>(rc));
  pos.setRandom();
  auto acc = view(pos, DiscreteStorageSequentialTransform<2>(
                           {std::get<0>(rc), std::get<1>(rc)}));
  for (auto _ : state) {
    float csum = 0;
    for (auto [i, j, v] :enumerate(acc)) {
      csum += v.norm();
    }
    benchmark::DoNotOptimize(csum);
  }
}

volatile int size_i = 1024;
volatile int size_j = 1024;

void enumerate_origin_1d(benchmark::State &state) {
  auto size_local = size_i;
  Field<float, 4> pos(4, size_local);
  pos.setRandom();
  for (auto _ : state) {
    for (int i = 0; i < size_local; ++i) {
      benchmark::DoNotOptimize(pos.col(i).norm());
      benchmark::DoNotOptimize(i);
    }
  }
}

void enumerate_origin_2d(benchmark::State &state) {
  auto size_i_local = size_i;
  auto size_j_local = size_j;
  Field<float, 4> pos(4, size_i_local * size_j_local);
  pos.setRandom();
  auto acc = view(pos, acg::NdRangeIndexer<2>(size_i_local, size_j_local));
  for (auto _ : state) {
    for (Index i = 0; i < size_i_local; ++i) {
      for (Index j = 0; j < size_j_local; ++j) {
        benchmark::DoNotOptimize(acc(i, j).norm());
        benchmark::DoNotOptimize(i);
        benchmark::DoNotOptimize(j);
      }
    }
  }
}

void enumerate_enum_1d(benchmark::State &state) {
  auto size_local = size_i;
  Field<float, 4> pos(4, size_local);
  pos.setRandom();
  for (auto _ : state) {
    auto en = enumerate(view(pos));
    for (auto [i, value] : en) {
      benchmark::DoNotOptimize(value.norm());
      benchmark::DoNotOptimize(i);
    }
  }
}

void enumerate_enum_2d(benchmark::State &state) {
  auto size_i_local = size_i;
  auto size_j_local = size_j;
  Field<float, 4> pos(4, size_i_local * size_j_local);
  pos.setRandom();
  auto acc = view(pos, acg::NdRangeIndexer<2>(size_i_local, size_j_local));
  for (auto _ : state) {
    for (auto [i, j] : acg::NdRange<2>({size_i_local, size_j_local})) {
      benchmark::DoNotOptimize(acc(i, j).norm());
      benchmark::DoNotOptimize(i);
      benchmark::DoNotOptimize(j);
    }
  }
}

void non_template_indexer1(benchmark::State &state) {
  auto size_i_local = size_i;
  for (auto _ : state) {
    acg::DiscreteStorageSequentialTransform<1> trans(
        make_vector<Index>(size_i_local));
    for (Index i = 0; i < size_i_local; ++i) {
      benchmark::DoNotOptimize(trans(i));
    }
  }
}

void template_indexer1(benchmark::State &state) {
  auto size_i_local = size_i;
  for (auto _ : state) {
    acg::NdRangeIndexer<1> trans(size_i_local);
    for (Index i = 0; i < size_i_local; ++i) {
      benchmark::DoNotOptimize(trans(i));
    }
  }
}

void non_template_indexer2(benchmark::State &state) {
  auto size_i_local = size_i;
  auto size_j_local = size_j;
  for (auto _ : state) {
    acg::DiscreteStorageSequentialTransform<2> trans(
        make_vector<Index>(size_i_local, size_j_local));
    for (Index i = 0; i < size_i_local; ++i) {
      for (Index j = 0; j < size_j_local; ++j) {
        benchmark::DoNotOptimize(trans(i, j));
      }
    }
  }
}

void template_indexer2(benchmark::State &state) {
  auto size_i_local = size_i;
  auto size_j_local = size_j;
  for (auto _ : state) {
    acg::NdRangeIndexer<2> trans(size_i_local, size_j_local);
    for (Index i = 0; i < size_i_local; ++i) {
      for (Index j = 0; j < size_j_local; ++j) {
        benchmark::DoNotOptimize(trans(i, j));
      }
    }
  }
}

BENCHMARK(enumerate_enum_1d);
BENCHMARK(enumerate_enum_2d);
BENCHMARK(enumerate_origin_1d);
BENCHMARK(enumerate_origin_2d);
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

BENCHMARK(template_indexer1);
BENCHMARK(non_template_indexer1);
BENCHMARK(template_indexer2);
BENCHMARK(non_template_indexer2);
// Run the benchmark
BENCHMARK_MAIN();
