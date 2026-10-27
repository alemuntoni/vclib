// VCLib - Visual Computing Library
// Copyright (C) 2021-2026 Visual Computing Lab, ISTI - CNR.
//
// This Source Code Form is subject to the terms of the Mozilla Public License,
// v. 2.0. If a copy of the MPL was not distributed with this file, You can
// obtain one at https://mozilla.org/MPL/2.0/.

#ifndef BENCHMARK_H
#define BENCHMARK_H

#include <vclib/base/timer.h>

#include <algorithm>
#include <numeric>
#include <print>
#include <string_view>
#include <vector>

namespace bench {

struct BenchmarkStats
{
    double minMs = 0.0;
    double maxMs = 0.0;
    double avgMs = 0.0;
};

inline BenchmarkStats computeStats(const std::vector<double>& runs)
{
    BenchmarkStats stats;
    if (runs.empty()) {
        return stats;
    }
    stats.minMs = *std::min_element(runs.begin(), runs.end());
    stats.maxMs = *std::max_element(runs.begin(), runs.end());
    stats.avgMs = std::accumulate(runs.begin(), runs.end(), 0.0) /
                  static_cast<double>(runs.size());
    return stats;
}

// do not optimize function to prevent compiler optimizations from removing
// unused results
template<typename T>
inline void doNotOptimize(const T& val)
{
#if defined(__GNUC__) || defined(__clang__)
    asm volatile("" : : "r,m"(val) : "memory");
#else
    volatile const auto* p = &val;
    (void)p;
#endif
}

template<typename F>
inline void invokeBenchmarkFunc(F&& f)
{
    if constexpr (!std::is_void_v<std::invoke_result_t<F>>) {
        auto res = f();
        doNotOptimize(res);
    }
    else {
        f();
    }
}

/**
 * @brief Generic benchmark runner executing two arbitrary callables (V1 vs V2).
 *
 * @param taskName Name and brief description of the benchmark task.
 * @param funcV1 Callable executing the V1 task.
 * @param funcV2 Callable executing the V2 task.
 * @param iterations Number of runs to average (default: 5).
 * @param warmup If true, executes each callable once before measuring.
 */
template<typename Func1, typename Func2>
inline void runBenchmark(
    std::string_view taskName,
    Func1&&          funcV1,
    Func2&&          funcV2,
    int              iterations = 5,
    bool             warmup     = true)
{
    std::println("\n==================================================");
    std::println("Task: {}", taskName);
    std::println("==================================================");

    if (warmup) {
        invokeBenchmarkFunc(funcV1);
        invokeBenchmarkFunc(funcV2);
    }

    // Benchmark V1
    vcl::Timer          timerV1("V1", false);
    std::vector<double> runsV1;
    runsV1.reserve(iterations);
    for (int i = 0; i < iterations; ++i) {
        timerV1.start();
        invokeBenchmarkFunc(funcV1);
        timerV1.stop();
        runsV1.push_back(timerV1.delay() * 1000.0); // ms
    }

    // Benchmark V2
    vcl::Timer          timerV2("V2", false);
    std::vector<double> runsV2;
    runsV2.reserve(iterations);
    for (int i = 0; i < iterations; ++i) {
        timerV2.start();
        invokeBenchmarkFunc(funcV2);
        timerV2.stop();
        runsV2.push_back(timerV2.delay() * 1000.0); // ms
    }

    auto   s1      = computeStats(runsV1);
    auto   s2      = computeStats(runsV2);
    double speedup = (s2.avgMs > 0.0) ? (s1.avgMs / s2.avgMs) : 0.0;

    std::println("Results (over {} iterations):", iterations);
    std::println("  Mesh V1: min = {:8.3f} ms, avg = {:8.3f} ms", s1.minMs, s1.avgMs);
    std::println("  Mesh V2: min = {:8.3f} ms, avg = {:8.3f} ms", s2.minMs, s2.avgMs);
    if (speedup >= 1.0) {
        std::println("  -> Mesh V2 is {:.2f}x FASTER than Mesh V1", speedup);
    }
    else {
        std::println(
            "  -> Mesh V1 is {:.2f}x FASTER than Mesh V2 ({:.2f}x)",
            1.0 / speedup,
            speedup);
    }
}

/**
 * @brief Overload that accepts two mesh references and passes them to the callables.
 *
 * @param taskName Name of the task.
 * @param m1 Mesh instance for V1.
 * @param m2 Mesh instance for V2.
 * @param funcV1 Callable taking (M1&).
 * @param funcV2 Callable taking (M2&).
 * @param iterations Number of runs (default: 5).
 * @param warmup If true, runs a warmup iteration (default: true).
 */
template<typename M1, typename M2, typename Func1, typename Func2>
inline void runBenchmark(
    std::string_view taskName,
    M1&              m1,
    M2&              m2,
    Func1&&          funcV1,
    Func2&&          funcV2,
    int              iterations = 5,
    bool             warmup     = true)
{
    runBenchmark(
        taskName,
        [&]() { funcV1(m1); },
        [&]() { funcV2(m2); },
        iterations,
        warmup);
}

} // namespace bench

#endif // BENCHMARK_H
