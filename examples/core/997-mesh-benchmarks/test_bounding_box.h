// VCLib - Visual Computing Library
// Copyright (C) 2021-2026 Visual Computing Lab, ISTI - CNR.
//
// This Source Code Form is subject to the terms of the Mozilla Public License,
// v. 2.0. If a copy of the MPL was not distributed with this file, You can
// obtain one at https://mozilla.org/MPL/2.0/.

#ifndef TEST_BOUNDING_BOX_H
#define TEST_BOUNDING_BOX_H

#include "benchmark.h"
#include "types.h"

#include <vclib/space/core/box.h>

#include <algorithm>
#include <format>
#include <limits>
#include <print>

namespace bench {

/**
 * @brief Computes Axis-Aligned Bounding Box (AABB) for V1Mesh from scratch.
 *
 * Iterates directly over all vertex positions and computes coordinate-wise min/max.
 */
inline vcl::Box3d computeBoundingBoxV1(const V1Mesh& m)
{
    constexpr double inf = std::numeric_limits<double>::infinity();
    vcl::Point3d minP(inf, inf, inf);
    vcl::Point3d maxP(-inf, -inf, -inf);

    for (const auto& v : m.vertices()) {
        const auto& p = v.position();
        minP = minP.cwiseMin(p);
        maxP = maxP.cwiseMax(p);
    }

    return vcl::Box3d(minP, maxP);
}

/**
 * @brief Computes Axis-Aligned Bounding Box (AABB) for V2Mesh from scratch.
 *
 * Iterates directly over all vertex positions and computes coordinate-wise min/max.
 */
inline vcl::Box3d computeBoundingBoxV2(const V2Mesh& m)
{
    constexpr double inf = std::numeric_limits<double>::infinity();
    vcl::Point3d minP(inf, inf, inf);
    vcl::Point3d maxP(-inf, -inf, -inf);

    for (const auto v : m.vertices()) {
        const auto& p = v.position();
        minP = minP.cwiseMin(p);
        maxP = maxP.cwiseMax(p);
    }

    return vcl::Box3d(minP, maxP);
}

/**
 * @brief Computes Bounding Box for V1Mesh by traversing face vertex references.
 *
 * Measures topological traversal cost (Face -> Vertices dereferencing).
 */
inline vcl::Box3d computeFaceBoundingBoxV1(const V1Mesh& m)
{
    constexpr double inf = std::numeric_limits<double>::infinity();
    vcl::Point3d minP(inf, inf, inf);
    vcl::Point3d maxP(-inf, -inf, -inf);

    for (const auto& f : m.faces()) {
        minP = minP.cwiseMin(f.vertex(0)->position());
        minP = minP.cwiseMin(f.vertex(1)->position());
        minP = minP.cwiseMin(f.vertex(2)->position());

        maxP = maxP.cwiseMax(f.vertex(0)->position());
        maxP = maxP.cwiseMax(f.vertex(1)->position());
        maxP = maxP.cwiseMax(f.vertex(2)->position());
    }

    return vcl::Box3d(minP, maxP);
}

/**
 * @brief Computes Bounding Box for V2Mesh by traversing face vertex references.
 *
 * Measures topological traversal cost (Face -> Vertices proxy indexing).
 */
inline vcl::Box3d computeFaceBoundingBoxV2(const V2Mesh& m)
{
    constexpr double inf = std::numeric_limits<double>::infinity();
    vcl::Point3d minP(inf, inf, inf);
    vcl::Point3d maxP(-inf, -inf, -inf);

    for (const auto f : m.faces()) {
        minP = minP.cwiseMin(f.vertex(0).position());
        minP = minP.cwiseMin(f.vertex(1).position());
        minP = minP.cwiseMin(f.vertex(2).position());

        maxP = maxP.cwiseMax(f.vertex(0).position());
        maxP = maxP.cwiseMax(f.vertex(1).position());
        maxP = maxP.cwiseMax(f.vertex(2).position());
    }

    return vcl::Box3d(minP, maxP);
}

/**
 * @brief Benchmarks Bounding Box calculation for V1Mesh vs V2Mesh.
 */
inline void benchmarkBoundingBox(const V1Mesh& m1, const V2Mesh& m2, int iterations = 5)
{
    // Task 3a: Vertex-stream Bounding Box
    runBenchmark(
        std::format("Bounding Box via Vertices Stream (V={})", m1.vertexCount()),
        [&]() { return computeBoundingBoxV1(m1); },
        [&]() { return computeBoundingBoxV2(m2); },
        iterations);

    // Task 3b: Face-traversal Bounding Box
    runBenchmark(
        std::format("Bounding Box via Face Traversal (F={})", m1.faceCount()),
        [&]() { return computeFaceBoundingBoxV1(m1); },
        [&]() { return computeFaceBoundingBoxV2(m2); },
        iterations);

    // Verification
    auto b1 = computeBoundingBoxV1(m1);
    auto b2 = computeBoundingBoxV2(m2);

    double diffMin = (b1.min() - b2.min()).norm();
    double diffMax = (b1.max() - b2.max()).norm();

    std::println("\nBounding Box Verification (V1 vs V2):");
    std::println(
        "  Min corner: V1 = [{:.6f}, {:.6f}, {:.6f}], V2 = [{:.6f}, {:.6f}, {:.6f}] (diff: {:e})",
        b1.min().x(),
        b1.min().y(),
        b1.min().z(),
        b2.min().x(),
        b2.min().y(),
        b2.min().z(),
        diffMin);
    std::println(
        "  Max corner: V1 = [{:.6f}, {:.6f}, {:.6f}], V2 = [{:.6f}, {:.6f}, {:.6f}] (diff: {:e})",
        b1.max().x(),
        b1.max().y(),
        b1.max().z(),
        b2.max().x(),
        b2.max().y(),
        b2.max().z(),
        diffMax);
}

} // namespace bench

#endif // TEST_BOUNDING_BOX_H
