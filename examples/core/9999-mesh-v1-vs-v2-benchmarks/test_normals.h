// VCLib - Visual Computing Library
// Copyright (C) 2021-2026 Visual Computing Lab, ISTI - CNR.
//
// This Source Code Form is subject to the terms of the Mozilla Public License,
// v. 2.0. If a copy of the MPL was not distributed with this file, You can
// obtain one at https://mozilla.org/MPL/2.0/.

#ifndef TEST_NORMALS_H
#define TEST_NORMALS_H

#include "benchmark.h"
#include "types.h"

#include <algorithm>
#include <cmath>
#include <format>
#include <print>

namespace bench {

/**
 * @brief Computes normalized face normals for V1 mesh.
 */
inline void computeFaceNormalsV1(V1Mesh& m)
{
    for (auto& f : m.faces()) {
        const auto& p0 = f.vertex(0)->position();
        const auto& p1 = f.vertex(1)->position();
        const auto& p2 = f.vertex(2)->position();
        vcl::Point3d fn = (p1 - p0).cross(p2 - p0);
        double n = fn.norm();
        if (n > 0.0) {
            f.normal() = fn / n;
        }
        else {
            f.normal() = vcl::Point3d::Zero();
        }
    }
}

/**
 * @brief Computes normalized face normals for V2 mesh.
 */
inline void computeFaceNormalsV2(V2Mesh& m)
{
    for (auto f : m.faces()) {
        const auto& p0 = f.vertex(0).position();
        const auto& p1 = f.vertex(1).position();
        const auto& p2 = f.vertex(2).position();
        vcl::Point3d fn = (p1 - p0).cross(p2 - p0);
        double n = fn.norm();
        if (n > 0.0) {
            f.normal() = fn / n;
        }
        else {
            f.normal() = vcl::Point3d::Zero();
        }
    }
}

/**
 * @brief Computes vertex normals by accumulating precomputed face normals for V1 mesh.
 */
inline void computeVertexNormalsFromFaceNormalsV1(V1Mesh& m)
{
    for (auto& v : m.vertices()) {
        v.normal() = vcl::Point3d::Zero();
    }

    for (auto& f : m.faces()) {
        const auto& fn = f.normal();
        f.vertex(0)->normal() += fn;
        f.vertex(1)->normal() += fn;
        f.vertex(2)->normal() += fn;
    }

    for (auto& v : m.vertices()) {
        double n = v.normal().norm();
        if (n > 0.0) {
            v.normal() /= n;
        }
    }
}

/**
 * @brief Computes vertex normals by accumulating precomputed face normals for V2 mesh.
 */
inline void computeVertexNormalsFromFaceNormalsV2(V2Mesh& m)
{
    for (auto v : m.vertices()) {
        v.normal() = vcl::Point3d::Zero();
    }

    for (auto f : m.faces()) {
        const auto& fn = f.normal();
        f.vertex(0).normal() += fn;
        f.vertex(1).normal() += fn;
        f.vertex(2).normal() += fn;
    }

    for (auto v : m.vertices()) {
        double n = v.normal().norm();
        if (n > 0.0) {
            v.normal() /= n;
        }
    }
}

/**
 * @brief Computes direct area-weighted vertex normals from face vertex positions for V1 mesh.
 */
inline void computeVertexNormalsV1(V1Mesh& m)
{
    for (auto& v : m.vertices()) {
        v.normal() = vcl::Point3d::Zero();
    }

    for (auto& f : m.faces()) {
        const auto& p0 = f.vertex(0)->position();
        const auto& p1 = f.vertex(1)->position();
        const auto& p2 = f.vertex(2)->position();
        vcl::Point3d fn = (p1 - p0).cross(p2 - p0);
        f.vertex(0)->normal() += fn;
        f.vertex(1)->normal() += fn;
        f.vertex(2)->normal() += fn;
    }

    for (auto& v : m.vertices()) {
        double n = v.normal().norm();
        if (n > 0.0) {
            v.normal() /= n;
        }
    }
}

/**
 * @brief Computes direct area-weighted vertex normals from face vertex positions for V2 mesh.
 */
inline void computeVertexNormalsV2(V2Mesh& m)
{
    for (auto v : m.vertices()) {
        v.normal() = vcl::Point3d::Zero();
    }

    for (auto f : m.faces()) {
        const auto& p0 = f.vertex(0).position();
        const auto& p1 = f.vertex(1).position();
        const auto& p2 = f.vertex(2).position();
        vcl::Point3d fn = (p1 - p0).cross(p2 - p0);
        f.vertex(0).normal() += fn;
        f.vertex(1).normal() += fn;
        f.vertex(2).normal() += fn;
    }

    for (auto v : m.vertices()) {
        double n = v.normal().norm();
        if (n > 0.0) {
            v.normal() /= n;
        }
    }
}

/**
 * @brief Benchmarks normals computation tasks for V1Mesh vs V2Mesh.
 */
inline void benchmarkNormals(V1Mesh& m1, V2Mesh& m2, int iterations = 5)
{
    // Task 2a: Per-Face Normals
    runBenchmark(
        std::format("Per-Face Normals (F={})", m1.faceCount()),
        m1,
        m2,
        [](V1Mesh& m) { computeFaceNormalsV1(m); },
        [](V2Mesh& m) { computeFaceNormalsV2(m); },
        iterations);

    // Task 2b: Per-Vertex Normals from Face Normals
    runBenchmark(
        std::format(
            "Per-Vertex Normals from Face Normals (V={}, F={})",
            m1.vertexCount(),
            m1.faceCount()),
        m1,
        m2,
        [](V1Mesh& m) { computeVertexNormalsFromFaceNormalsV1(m); },
        [](V2Mesh& m) { computeVertexNormalsFromFaceNormalsV2(m); },
        iterations);

    // Task 2c: Direct Area-Weighted Vertex Normals
    runBenchmark(
        std::format(
            "Per-Vertex Normals Direct Area-Weighted (V={}, F={})",
            m1.vertexCount(),
            m1.faceCount()),
        m1,
        m2,
        [](V1Mesh& m) { computeVertexNormalsV1(m); },
        [](V2Mesh& m) { computeVertexNormalsV2(m); },
        iterations);

    // Recompute face normals to make sure both face & vertex normals are populated
    computeFaceNormalsV1(m1);
    computeFaceNormalsV2(m2);

    // Verification of numerical equivalence
    double maxDiffFace = 0.0;
    const auto numF = m1.faceCount();
    for (std::size_t i = 0; i < numF; ++i) {
        double diff = (m1.face(i).normal() - m2.face(i).normal()).norm();
        maxDiffFace = std::max(maxDiffFace, diff);
    }

    double maxDiffVert = 0.0;
    const auto numV = m1.vertexCount();
    for (std::size_t i = 0; i < numV; ++i) {
        double diff = (m1.vertex(i).normal() - m2.vertex(i).normal()).norm();
        maxDiffVert = std::max(maxDiffVert, diff);
    }

    std::println("\nNormals Verification (max difference between V1 and V2):");
    std::println("  Face normals max diff:   {:e}", maxDiffFace);
    std::println("  Vertex normals max diff: {:e}", maxDiffVert);
}

} // namespace bench

#endif // TEST_NORMALS_H
