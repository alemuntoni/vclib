// VCLib - Visual Computing Library
// Copyright (C) 2021-2026 Visual Computing Lab, ISTI - CNR.
//
// This Source Code Form is subject to the terms of the Mozilla Public License,
// v. 2.0. If a copy of the MPL was not distributed with this file, You can
// obtain one at https://mozilla.org/MPL/2.0/.

#ifndef TEST_TRAVERSAL_H
#define TEST_TRAVERSAL_H

#include "benchmark.h"
#include "types.h"

#include <cmath>
#include <format>
#include <print>

namespace bench {

/**
 * @brief Computes total surface area of V1Mesh by traversing face vertex references.
 *
 * Pure read-only benchmark measuring topological traversal and cross-product norm.
 */
inline double computeTotalAreaV1(const V1Mesh& m)
{
    double totalArea = 0.0;
    for (const auto& f : m.faces()) {
        const auto& p0 = f.vertex(0)->position();
        const auto& p1 = f.vertex(1)->position();
        const auto& p2 = f.vertex(2)->position();
        totalArea += 0.5 * (p1 - p0).cross(p2 - p0).norm();
    }
    return totalArea;
}

/**
 * @brief Computes total surface area of V2Mesh by traversing face vertex references.
 *
 * Pure read-only benchmark measuring topological traversal and cross-product norm.
 */
inline double computeTotalAreaV2(const V2Mesh& m)
{
    double totalArea = 0.0;
    for (const auto f : m.faces()) {
        const auto& p0 = f.vertex(0).position();
        const auto& p1 = f.vertex(1).position();
        const auto& p2 = f.vertex(2).position();
        totalArea += 0.5 * (p1 - p0).cross(p2 - p0).norm();
    }
    return totalArea;
}

/**
 * @brief Computes sum of all face barycenters of V1Mesh.
 *
 * Pure read-only benchmark measuring topological traversal and 3D vector addition.
 */
inline vcl::Point3d computeBarycentersSumV1(const V1Mesh& m)
{
    vcl::Point3d sum = vcl::Point3d::Zero();
    constexpr double oneThird = 1.0 / 3.0;
    for (const auto& f : m.faces()) {
        const auto& p0 = f.vertex(0)->position();
        const auto& p1 = f.vertex(1)->position();
        const auto& p2 = f.vertex(2)->position();
        sum += (p0 + p1 + p2) * oneThird;
    }
    return sum;
}

/**
 * @brief Computes sum of all face barycenters of V2Mesh.
 *
 * Pure read-only benchmark measuring topological traversal and 3D vector addition.
 */
inline vcl::Point3d computeBarycentersSumV2(const V2Mesh& m)
{
    vcl::Point3d sum = vcl::Point3d::Zero();
    constexpr double oneThird = 1.0 / 3.0;
    for (const auto f : m.faces()) {
        const auto& p0 = f.vertex(0).position();
        const auto& p1 = f.vertex(1).position();
        const auto& p2 = f.vertex(2).position();
        sum += (p0 + p1 + p2) * oneThird;
    }
    return sum;
}

/**
 * @brief Benchmarks topological face-to-vertex traversal tasks for V1Mesh vs V2Mesh.
 */
inline void benchmarkTraversal(const V1Mesh& m1, const V2Mesh& m2, int iterations = 5)
{
    // Task 5a: Total Surface Area via Face Traversal
    runBenchmark(
        std::format("Total Surface Area via Faces (F={})", m1.faceCount()),
        [&]() { return computeTotalAreaV1(m1); },
        [&]() { return computeTotalAreaV2(m2); },
        iterations);

    // Task 5b: Sum of Face Barycenters via Face Traversal
    runBenchmark(
        std::format("Face Barycenters Sum via Faces (F={})", m1.faceCount()),
        [&]() { return computeBarycentersSumV1(m1); },
        [&]() { return computeBarycentersSumV2(m2); },
        iterations);

    // Verification
    double a1 = computeTotalAreaV1(m1);
    double a2 = computeTotalAreaV2(m2);
    double diffArea = std::abs(a1 - a2);

    vcl::Point3d b1 = computeBarycentersSumV1(m1);
    vcl::Point3d b2 = computeBarycentersSumV2(m2);
    double diffBary = (b1 - b2).norm();

    std::println("\nTraversal Verification (V1 vs V2):");
    std::println("  Total Area:      V1 = {:14.6f}, V2 = {:14.6f} (diff: {:e})", a1, a2, diffArea);
    std::println(
        "  Barycenters Sum: V1 = [{:.4f}, {:.4f}, {:.4f}], V2 = [{:.4f}, {:.4f}, {:.4f}] (diff: {:e})",
        b1.x(),
        b1.y(),
        b1.z(),
        b2.x(),
        b2.y(),
        b2.z(),
        diffBary);
}

} // namespace bench

#endif // TEST_TRAVERSAL_H
