// VCLib - Visual Computing Library
// Copyright (C) 2021-2026 Visual Computing Lab, ISTI - CNR.
//
// This Source Code Form is subject to the terms of the Mozilla Public License,
// v. 2.0. If a copy of the MPL was not distributed with this file, You can
// obtain one at https://mozilla.org/MPL/2.0/.

#ifndef TEST_BUILDING_H
#define TEST_BUILDING_H

#include "benchmark.h"
#include "types.h"

#include <vclib/algorithms.h>

#include <cassert>
#include <format>

namespace bench {

/**
 * @brief Builds a V1 Mesh from Eigen matrices using the official vclib algorithm.
 */
inline void buildMeshV1(
    V1Mesh&                 m,
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
        // v1 setVertices is slower due to std::list usage and consequent memory
        // heap allocation.
        m.face(i).setVertex(0, F(i, 0));
        m.face(i).setVertex(1, F(i, 1));
        m.face(i).setVertex(2, F(i, 2));
    }
}

/**
 * @brief Builds a V2 Mesh from Eigen matrices using SoA resize and element assignment.
 */
inline void buildMeshV2(
    V2Mesh&                 m,
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
    V1Mesh&                 outM1,
    V2Mesh&                 outM2,
    int                     iterations = 5)
{
    runBenchmark(
        std::format("Mesh Building from Eigen Matrices (V={}, F={})", V.rows(), F.rows()),
        [&]() {
            V1Mesh m;
            buildMeshV1(m, V, F);
        },
        [&]() {
            V2Mesh m;
            buildMeshV2(m, V, F);
        },
        iterations);

    // Final build for the output mesh instances
    buildMeshV1(outM1, V, F);
    buildMeshV2(outM2, V, F);

    // Sanity check
    assert(outM1.vertexCount() == outM2.vertexCount());
    assert(outM1.faceCount() == outM2.faceCount());
}

} // namespace bench

#endif // TEST_BUILDING_H
