// VCLib - Visual Computing Library
// Copyright (C) 2021-2026 Visual Computing Lab, ISTI - CNR.
//
// This Source Code Form is subject to the terms of the Mozilla Public License,
// v. 2.0. If a copy of the MPL was not distributed with this file, You can
// obtain one at https://mozilla.org/MPL/2.0/.

#ifndef TEST_TRANSFORM_H
#define TEST_TRANSFORM_H

#include "benchmark.h"
#include "types.h"

#include <algorithm>
#include <cmath>
#include <format>
#include <print>

namespace bench {

/**
 * @brief Creates a sample affine 4x4 matrix (rotation + translation).
 */
inline Eigen::Matrix4d createSampleTransformMatrix()
{
    Eigen::Matrix4d m = Eigen::Matrix4d::Identity();
    vcl::Point3d    axis(1.0, 1.0, 1.0);
    axis.normalize();

    constexpr double pi    = 3.14159265358979323846;
    double           angle = 30.0 * pi / 180.0; // 30 degrees
    double           c     = std::cos(angle);
    double           s     = std::sin(angle);
    double           q     = 1.0 - c;

    m(0, 0) = axis[0] * axis[0] * q + c;
    m(0, 1) = axis[0] * axis[1] * q - axis[2] * s;
    m(0, 2) = axis[0] * axis[2] * q + axis[1] * s;
    m(0, 3) = 10.0;

    m(1, 0) = axis[1] * axis[0] * q + axis[2] * s;
    m(1, 1) = axis[1] * axis[1] * q + c;
    m(1, 2) = axis[1] * axis[2] * q - axis[0] * s;
    m(1, 3) = 20.0;

    m(2, 0) = axis[2] * axis[0] * q - axis[1] * s;
    m(2, 1) = axis[2] * axis[1] * q + axis[0] * s;
    m(2, 2) = axis[2] * axis[2] * q + c;
    m(2, 3) = 30.0;

    return m;
}

/**
 * @brief Transforms all vertex positions of V1Mesh by a 4x4 matrix.
 */
inline void transformPositionsV1(V1Mesh& m, const Eigen::Matrix4d& mat)
{
    for (auto& v : m.vertices()) {
        v.position() *= mat;
    }
}

/**
 * @brief Transforms all vertex positions of V2Mesh by a 4x4 matrix.
 */
inline void transformPositionsV2(V2Mesh& m, const Eigen::Matrix4d& mat)
{
    for (auto v : m.vertices()) {
        v.position() *= mat;
    }
}

/**
 * @brief Transforms all vertex normals of V1Mesh by a 3x3 rotation matrix.
 */
inline void transformNormalsV1(V1Mesh& m, const Eigen::Matrix3d& mat)
{
    for (auto& v : m.vertices()) {
        v.normal() = mat * v.normal();
    }
}

/**
 * @brief Transforms all vertex normals of V2Mesh by a 3x3 rotation matrix.
 */
inline void transformNormalsV2(V2Mesh& m, const Eigen::Matrix3d& mat)
{
    for (auto v : m.vertices()) {
        v.normal() = mat * v.normal();
    }
}

/**
 * @brief Transforms both vertex positions (4x4) and normals (3x3) of V1Mesh.
 */
inline void transformVerticesV1(
    V1Mesh&                m,
    const Eigen::Matrix4d& m44,
    const Eigen::Matrix3d& m33)
{
    for (auto& v : m.vertices()) {
        v.position() *= m44;
        v.normal() = m33 * v.normal();
    }
}

/**
 * @brief Transforms both vertex positions (4x4) and normals (3x3) of V2Mesh.
 */
inline void transformVerticesV2(
    V2Mesh&                m,
    const Eigen::Matrix4d& m44,
    const Eigen::Matrix3d& m33)
{
    for (auto v : m.vertices()) {
        v.position() *= m44;
        v.normal() = m33 * v.normal();
    }
}

/**
 * @brief Benchmarks matrix transformation tasks for V1Mesh vs V2Mesh.
 */
inline void benchmarkTransform(V1Mesh& m1, V2Mesh& m2, int iterations = 5)
{
    const Eigen::Matrix4d m44 = createSampleTransformMatrix();
    const Eigen::Matrix3d m33 = m44.block<3, 3>(0, 0);

    // Task 4a: Transform Positions only (4x4)
    runBenchmark(
        std::format("Transform Positions 4x4 (V={})", m1.vertexCount()),
        m1,
        m2,
        [&](V1Mesh& m) { transformPositionsV1(m, m44); },
        [&](V2Mesh& m) { transformPositionsV2(m, m44); },
        iterations);

    // Task 4b: Transform Normals only (3x3)
    runBenchmark(
        std::format("Transform Normals 3x3 (V={})", m1.vertexCount()),
        m1,
        m2,
        [&](V1Mesh& m) { transformNormalsV1(m, m33); },
        [&](V2Mesh& m) { transformNormalsV2(m, m33); },
        iterations);

    // Task 4c: Transform Positions (4x4) + Normals (3x3) together
    runBenchmark(
        std::format("Transform Positions + Normals (V={})", m1.vertexCount()),
        m1,
        m2,
        [&](V1Mesh& m) { transformVerticesV1(m, m44, m33); },
        [&](V2Mesh& m) { transformVerticesV2(m, m44, m33); },
        iterations);

    // Verification
    double maxDiffPos  = 0.0;
    double maxDiffNorm = 0.0;
    const auto numV    = m1.vertexCount();
    for (std::size_t i = 0; i < numV; ++i) {
        double diffP = (m1.vertex(i).position() - m2.vertex(i).position()).norm();
        maxDiffPos   = std::max(maxDiffPos, diffP);

        double diffN = (m1.vertex(i).normal() - m2.vertex(i).normal()).norm();
        maxDiffNorm  = std::max(maxDiffNorm, diffN);
    }

    std::println("\nTransform Verification (max difference between V1 and V2):");
    std::println("  Positions max diff: {:e}", maxDiffPos);
    std::println("  Normals max diff:   {:e}", maxDiffNorm);
}

} // namespace bench

#endif // TEST_TRANSFORM_H
