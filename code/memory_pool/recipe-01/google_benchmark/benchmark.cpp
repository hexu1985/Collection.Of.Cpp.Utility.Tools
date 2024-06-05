#include <memory>

#include "rational.hpp"
#include "benchmark/benchmark.h"

void new_delete_rational(Rational* array[], int times) {
    for (int i = 0; i < times; i++) {
        array[i] = new Rational(i);
    }
    for (int i = 0; i < times; i++) {
        delete array[i];
    }
}

void BM_memory_alloc(benchmark::State& state) {
    int N = state.range(0);
    std::unique_ptr<Rational* []> array(new Rational* [N]);
    for (auto _ : state) {
        new_delete_rational(array.get(), N);
    }
    state.SetItemsProcessed(N*state.iterations());
}

#define ARGS ->Arg(5000)

BENCHMARK(BM_memory_alloc) ARGS;

BENCHMARK_MAIN();

