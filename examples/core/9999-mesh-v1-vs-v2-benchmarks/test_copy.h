// VCLib - Visual Computing Library
// Copyright (C) 2021-2026 Visual Computing Lab, ISTI - CNR.
//
// This Source Code Form is subject to the terms of the Mozilla Public License,
// v. 2.0. If a copy of the MPL was not distributed with this file, You can
// obtain one at https://mozilla.org/MPL/2.0/.

#ifndef TEST_COPY_H
#define TEST_COPY_H

#include "benchmark.h"
#include "types.h"

#include <cassert>
#include <format>
#include <print>

namespace bench {

/**
 * @brief Benchmarks deep copying / cloning of V1Mesh vs V2Mesh.
 *
 * In V1, copying requires allocating AoS element vectors and running pointer-fixing
 * to re-point all face-vertex pointers to the newly allocated vertices.
 * In V2, copying is a sequence of direct memory copies (memcpy) of the contiguous SoA
 * component buffers with zero pointer-fixing required.
 */
inline void benchmarkCopy(const V1Mesh& m1, const V2Mesh& m2, int iterations = 5)
{
    runBenchmark(
        std::format(
            "Mesh Deep Copy / Cloning (V={}, F={})",
            m1.vertexCount(),
            m1.faceCount()),
        [&]() {
            V1Mesh copy1 = m1;
            doNotOptimize(copy1);
        },
        [&]() {
            V2Mesh copy2 = m2;
            doNotOptimize(copy2);
        },
        iterations);

    // Verification
    V1Mesh v1Copy = m1;
    V2Mesh v2Copy = m2;

    assert(v1Copy.vertexCount() == m1.vertexCount());
    assert(v1Copy.faceCount() == m1.faceCount());
    assert(v2Copy.vertexCount() == m2.vertexCount());
    assert(v2Copy.faceCount() == m2.faceCount());

    std::println("\nCopy Verification:");
    std::println(
        "  V1 original V={}, F={} -> copy V={}, F={}",
        m1.vertexCount(),
        m1.faceCount(),
        v1Copy.vertexCount(),
        v1Copy.faceCount());
    std::println(
        "  V2 original V={}, F={} -> copy V={}, F={}",
        m2.vertexCount(),
        m2.faceCount(),
        v2Copy.vertexCount(),
        v2Copy.faceCount());
}

} // namespace bench

#endif // TEST_COPY_H
