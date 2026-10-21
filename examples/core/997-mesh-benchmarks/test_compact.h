// VCLib - Visual Computing Library
// Copyright (C) 2021-2026 Visual Computing Lab, ISTI - CNR.
//
// This Source Code Form is subject to the terms of the Mozilla Public License,
// v. 2.0. If a copy of the MPL was not distributed with this file, You can
// obtain one at https://mozilla.org/MPL/2.0/.

#ifndef TEST_COMPACT_H
#define TEST_COMPACT_H

#include "benchmark.h"
#include "types.h"

#include <cassert>
#include <format>
#include <print>
#include <vector>

namespace bench {

inline void printComparison(
    std::string_view taskName,
    const std::vector<double>& runsV1,
    const std::vector<double>& runsV2,
    int iterations)
{
    auto s1 = computeStats(runsV1);
    auto s2 = computeStats(runsV2);
    double speedup = (s2.avgMs > 0.0) ? (s1.avgMs / s2.avgMs) : 0.0;

    std::println("\n==================================================");
    std::println("Task: {}", taskName);
    std::println("==================================================");
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
 * @brief Benchmarks element deletion (marking bit flags) and mesh compaction.
 *
 * Each iteration clones the input mesh outside the timer, marks 25% of faces as deleted
 * (step 4), and then compacts the mesh container, measuring both steps and their total.
 */
inline void benchmarkDeleteAndCompact(
    const V1Mesh& m1,
    const V2Mesh& m2,
    int           iterations = 5)
{
    const auto numF = m1.faceCount();
    const auto expectedDeleted = (numF + 3) / 4;

    std::vector<double> deleteRunsV1;
    std::vector<double> compactRunsV1;
    std::vector<double> totalRunsV1;
    deleteRunsV1.reserve(iterations);
    compactRunsV1.reserve(iterations);
    totalRunsV1.reserve(iterations);

    vcl::Timer timer("Timer", false);

    // Warmup
    {
        V1Mesh warmM = m1;
        for (std::size_t i = 0; i < warmM.faceCount(); i += 4) {
            warmM.deleteFace(i);
        }
        warmM.compact();
    }

    for (int it = 0; it < iterations; ++it) {
        V1Mesh testM = m1; // Cloned outside the timer

        // Measure deletion
        timer.start();
        for (std::size_t i = 0; i < testM.faceCount(); i += 4) {
            testM.deleteFace(i);
        }
        timer.stop();
        double delMs = timer.delay() * 1000.0;
        deleteRunsV1.push_back(delMs);

        // Measure compaction
        timer.start();
        testM.compact();
        timer.stop();
        double compMs = timer.delay() * 1000.0;
        compactRunsV1.push_back(compMs);

        totalRunsV1.push_back(delMs + compMs);
    }

    std::vector<double> deleteRunsV2;
    std::vector<double> compactRunsV2;
    std::vector<double> totalRunsV2;
    deleteRunsV2.reserve(iterations);
    compactRunsV2.reserve(iterations);
    totalRunsV2.reserve(iterations);

    // Warmup
    {
        V2Mesh warmM = m2;
        for (std::size_t i = 0; i < warmM.faceCount(); i += 4) {
            warmM.deleteFace(i);
        }
        warmM.compact();
    }

    for (int it = 0; it < iterations; ++it) {
        V2Mesh testM = m2; // Cloned outside the timer

        // Measure deletion
        timer.start();
        for (std::size_t i = 0; i < testM.faceCount(); i += 4) {
            testM.deleteFace(i);
        }
        timer.stop();
        double delMs = timer.delay() * 1000.0;
        deleteRunsV2.push_back(delMs);

        // Measure compaction
        timer.start();
        testM.compact();
        timer.stop();
        double compMs = timer.delay() * 1000.0;
        compactRunsV2.push_back(compMs);

        totalRunsV2.push_back(delMs + compMs);
    }

    printComparison(
        std::format("Mark Deleted Elements (25% faces deleted, N={})", expectedDeleted),
        deleteRunsV1,
        deleteRunsV2,
        iterations);

    printComparison(
        std::format("Compact Mesh Containers (Removed N={})", expectedDeleted),
        compactRunsV1,
        compactRunsV2,
        iterations);

    printComparison(
        "Total Delete + Compact Combined",
        totalRunsV1,
        totalRunsV2,
        iterations);

    // Verification
    V1Mesh v1 = m1;
    for (std::size_t i = 0; i < v1.faceCount(); i += 4) {
        v1.deleteFace(i);
    }
    v1.compact();

    V2Mesh v2 = m2;
    for (std::size_t i = 0; i < v2.faceCount(); i += 4) {
        v2.deleteFace(i);
    }
    v2.compact();

    assert(v1.faceCount() == v2.faceCount());
    assert(v1.vertexCount() == v2.vertexCount());

    std::println("\nCompaction Verification:");
    std::println("  V1 final faces: {}, compact: {}", v1.faceCount(), v1.isCompact());
    std::println("  V2 final faces: {}, compact: {}", v2.faceCount(), v2.isCompact());
}

} // namespace bench

#endif // TEST_COMPACT_H
