// VCLib - Visual Computing Library
// Copyright (C) 2021-2026 Visual Computing Lab, ISTI - CNR.
//
// This Source Code Form is subject to the terms of the Mozilla Public License,
// v. 2.0. If a copy of the MPL was not distributed with this file, You can
// obtain one at https://mozilla.org/MPL/2.0/.

#ifndef TEST_NORMALS_H
#define TEST_NORMALS_H

#include "types.h"

#include <benchmark_runner.h>

// TODO: re-enable once normal update algorithms are ported to Mesh V2
#if 0
#include <vclib/algorithms.h>

namespace bench {

inline void benchmarkNormals(
    BenchmarkRunner& runner,
    Mesh&            m,
    int              iterations = -1)
{
    runner.run(
        "update_face_normals",
        [&]() { vcl::updatePerFaceNormals(m); },
        iterations);

    runner.run(
        "update_vertex_normals",
        [&]() { vcl::updatePerVertexNormals(m); },
        iterations);

    runner.run(
        "update_vertex_normals_from_faces",
        [&]() { vcl::updatePerVertexNormalsFromFaceNormals(m); },
        iterations);
}

} // namespace bench
#endif

#endif // TEST_NORMALS_H
