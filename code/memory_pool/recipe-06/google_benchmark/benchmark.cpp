#include <memory>
#include <iostream>

#include "rational.hpp"
#include "benchmark/benchmark.h"

static void DoSetup(const benchmark::State& state) {
	Rational::newMemPool();
}

static void DoTeardown(const benchmark::State& state) {
	Rational::deleteMemPool();
}

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

#define ARGS ->Arg(5000)->Setup(DoSetup)->Teardown(DoTeardown)

BENCHMARK(BM_memory_alloc) ARGS;

BENCHMARK_MAIN();

