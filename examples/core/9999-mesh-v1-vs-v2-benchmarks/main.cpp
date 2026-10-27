// VCLib - Visual Computing Library
// Copyright (C) 2021-2026 Visual Computing Lab, ISTI - CNR.
//
// This Source Code Form is subject to the terms of the Mozilla Public License,
// v. 2.0. If a copy of the MPL was not distributed with this file, You can
// obtain one at https://mozilla.org/MPL/2.0/.

#include "load_matrices.h"
#include "test_bounding_box.h"
#include "test_building.h"
#include "test_compact.h"
#include "test_copy.h"
#include "test_normals.h"
#include "test_transform.h"
#include "test_traversal.h"
#include "types.h"

#include <iostream>
#include <print>

int main(int argc, char** argv)
{
    std::string path;
    if (argc > 1) {
        path = argv[1];
    }
    else {
#ifdef VCLIB_EXAMPLE_MESHES_PATH
        path = VCLIB_EXAMPLE_MESHES_PATH "/bimba.obj";
        // path = "/home/alessandro/devel/3DMeshes/3DMeshes/ESTE_PRINT.ply"; // LIN
        // path = "C:/Users/Alessandro/Desktop/ESTE_PRINT.ply"; // WIN
        // path = "/Users/alessandro/ESTE_PRINT.ply"; // MAC
#else
        std::println(stderr, "Usage: {} <mesh_path>", argv[0]);
        return 1;
#endif
    }

    std::println("==================================================");
    std::println("VCLib Mesh Benchmark: V1 vs V2");
    std::println("Loading mesh matrices from: {}", path);
    std::println("==================================================");

    try {
        auto [V, F] = bench::loadMeshMatrices(path);

        std::println("Successfully loaded matrices:");
        std::println("  Vertices: {} x {}", V.rows(), V.cols());
        std::println("  Faces:    {} x {}", F.rows(), F.cols());

        bench::V1Mesh m1;
        bench::V2Mesh m2;

        // Run Task 1: Building
        bench::benchmarkBuilding(V, F, m1, m2, 5);

        // Run Task 2: Bounding Box
        bench::benchmarkBoundingBox(m1, m2, 5);

        // Run Task 3: Face Traversal / Surface Area
        bench::benchmarkTraversal(m1, m2, 5);

        // Run Task 4: Normals Computation
        bench::benchmarkNormals(m1, m2, 5);

        // Run Task 5: Matrix Transform
        bench::benchmarkTransform(m1, m2, 5);

        // Run Task 6: Deep Copy / Cloning
        bench::benchmarkCopy(m1, m2, 5);

        // Run Task 7: Delete & Compact
        bench::benchmarkDeleteAndCompact(m1, m2, 5);

        std::println("\nFinal Verification:");
        std::println("  m1 vertices: {}, faces: {}", m1.vertexCount(), m1.faceCount());
        std::println("  m2 vertices: {}, faces: {}", m2.vertexCount(), m2.faceCount());
    }
    catch (const std::exception& e) {
        std::println(stderr, "Error: {}", e.what());
        return 1;
    }

    return 0;
}
