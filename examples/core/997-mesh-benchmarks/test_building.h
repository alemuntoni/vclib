// VCLib - Visual Computing Library
// Copyright (C) 2021-2026 Visual Computing Lab, ISTI - CNR.
//
// This Source Code Form is subject to the terms of the Mozilla Public License,
// v. 2.0. If a copy of the MPL was not distributed with this file, You can
// obtain one at https://mozilla.org/MPL/2.0/.

#ifndef TEST_BUILDING_H
#define TEST_BUILDING_H

#include "types.h"

#include <vclib/algorithms.h>
#include <vclib/base/timer.h>

#include <algorithm>
#include <numeric>
#include <print>
#include <vector>

namespace bench {

/**
 * @brief Builds a V1 Mesh from Eigen matrices using the official vclib algorithm.
 */
inline void buildMeshV1(
    V1Mesh& m,
    const Eigen::MatrixX3d& V,
    const Eigen::MatrixX3i& F)
{
    vcl::meshFromMatrices(m, V, F);
}

/**
 * @brief Builds a V2 Mesh from Eigen matrices using SoA resize and element assignment.
 */
inline void buildMeshV2(
    V2Mesh& m,
    const Eigen::MatrixX3d& V,
    const Eigen::MatrixX3i& F)
{
    m.clear();
    const auto numV = V.rows();
    m.resizeVertices(numV);
    for (int i = 0; i < numV; ++i) {
        m.vertex(i).position() = {V(i, 0), V(i, 1), V(i, 2)};
    }

    const auto numF = F.rows();
    m.resizeFaces(numF);
    for (int i = 0; i < numF; ++i) {
        m.face(i).setVertices(F(i, 0), F(i, 1), F(i, 2));
    }
}

/**
 * @brief Benchmarks building V1Mesh vs V2Mesh from Eigen matrices.
 *
 * @param V The vertex positions matrix (V x 3).
 * @param F The face vertex indices matrix (F x 3).
 * @param[out] outM1 Output populated V1 mesh.
 * @param[out] outM2 Output populated V2 mesh.
 * @param iterations Number of benchmark runs.
 */
inline void benchmarkBuilding(
    const Eigen::MatrixX3d& V,
    const Eigen::MatrixX3i& F,
    V1Mesh& outM1,
    V2Mesh& outM2,
    int iterations = 5)
{
    std::println("\n==================================================");
    std::println("Task: Mesh Building from Eigen Matrices (V={}, F={})", V.rows(), F.rows());
    std::println("==================================================");

    // Warmup
    {
        V1Mesh m1;
        buildMeshV1(m1, V, F);
        V2Mesh m2;
        buildMeshV2(m2, V, F);
    }

    // Benchmark V1
    vcl::Timer timerV1("Build V1", false);
    std::vector<double> runsV1;
    runsV1.reserve(iterations);
    for (int i = 0; i < iterations; ++i) {
        V1Mesh m1;
        timerV1.start();
        buildMeshV1(m1, V, F);
        timerV1.stop();
        runsV1.push_back(timerV1.delay() * 1000.0); // ms
    }

    // Benchmark V2
    vcl::Timer timerV2("Build V2", false);
    std::vector<double> runsV2;
    runsV2.reserve(iterations);
    for (int i = 0; i < iterations; ++i) {
        V2Mesh m2;
        timerV2.start();
        buildMeshV2(m2, V, F);
        timerV2.stop();
        runsV2.push_back(timerV2.delay() * 1000.0); // ms
    }

    // Final build for the output mesh instances
    buildMeshV1(outM1, V, F);
    buildMeshV2(outM2, V, F);

    auto avg = [](const std::vector<double>& v) {
        return std::accumulate(v.begin(), v.end(), 0.0) / static_cast<double>(v.size());
    };
    auto min = [](const std::vector<double>& v) {
        return *std::min_element(v.begin(), v.end());
    };

    double avgV1 = avg(runsV1);
    double minV1 = min(runsV1);
    double avgV2 = avg(runsV2);
    double minV2 = min(runsV2);
    double speedup = avgV1 / avgV2;

    std::println("Results (over {} iterations):", iterations);
    std::println("  Mesh V1: min = {:8.3f} ms, avg = {:8.3f} ms", minV1, avgV1);
    std::println("  Mesh V2: min = {:8.3f} ms, avg = {:8.3f} ms", minV2, avgV2);
    if (speedup >= 1.0) {
        std::println("  -> Mesh V2 is {:.2f}x FASTER than Mesh V1", speedup);
    }
    else {
        std::println("  -> Mesh V1 is {:.2f}x FASTER than Mesh V2 ({:.2f}x)", 1.0 / speedup, speedup);
    }

    // Sanity check
    assert(outM1.vertexCount() == outM2.vertexCount());
    assert(outM1.faceCount() == outM2.faceCount());
}

} // namespace bench

#endif // TEST_BUILDING_H
