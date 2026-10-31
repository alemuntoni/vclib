// VCLib - Visual Computing Library
// Copyright (C) 2021-2026 Visual Computing Lab, ISTI - CNR.
//
// This Source Code Form is subject to the terms of the Mozilla Public License,
// v. 2.0. If a copy of the MPL was not distributed with this file, You can
// obtain one at https://mozilla.org/MPL/2.0/.

#ifndef TEST_UPDATE_NORMALS_H
#define TEST_UPDATE_NORMALS_H

#include "types.h"

#include <vclib/algorithms/mesh/update/normal.h>

#include <cassert>
#include <cmath>
#include <print>

inline void testUpdateNormals()
{
    std::println("--- Testing Update Normals Algorithms ---");

    TangentMesh m;
    m.addVertices(3);
    m.vertex(0).position() = Point3f(0.0f, 0.0f, 0.0f);
    m.vertex(1).position() = Point3f(1.0f, 0.0f, 0.0f);
    m.vertex(2).position() = Point3f(0.0f, 1.0f, 0.0f);

    m.addFace(0, 1, 2);

    // 1. Update per-face normals
    vcl::updatePerFaceNormals(m);
    Point3f expectedNormal(0.0f, 0.0f, 1.0f);
    assert((m.face(0).normal() - expectedNormal).norm() < 1e-5f);

    // 2. Clear per-face normals
    vcl::clearPerFaceNormals(m);
    assert(m.face(0).normal() == Point3f(0.0f, 0.0f, 0.0f));

    // 3. Update per-vertex normals
    vcl::updatePerVertexNormals(m);
    for (uint i = 0; i < 3; ++i) {
        assert((m.vertex(i).normal() - expectedNormal).norm() < 1e-5f);
    }

    // 4. Clear per-vertex normals
    vcl::clearPerVertexNormals(m);
    for (uint i = 0; i < 3; ++i) {
        assert(m.vertex(i).normal() == Point3f(0.0f, 0.0f, 0.0f));
    }

    // 5. Update per-vertex normals from face normals
    vcl::updatePerFaceNormals(m);
    vcl::updatePerVertexNormalsFromFaceNormals(m);
    for (uint i = 0; i < 3; ++i) {
        assert((m.vertex(i).normal() - expectedNormal).norm() < 1e-5f);
    }

    // 6. Update per-vertex normals angle weighted
    vcl::clearPerVertexNormals(m);
    vcl::updatePerVertexNormalsAngleWeighted(m);
    for (uint i = 0; i < 3; ++i) {
        assert((m.vertex(i).normal() - expectedNormal).norm() < 1e-5f);
    }

    // 7. Update per-vertex normals Nelson-Max weighted
    vcl::clearPerVertexNormals(m);
    vcl::updatePerVertexNormalsNelsonMaxWeighted(m);
    for (uint i = 0; i < 3; ++i) {
        assert((m.vertex(i).normal() - expectedNormal).norm() < 1e-5f);
    }

    // 8. Update per-vertex and face normals together
    vcl::clearPerVertexNormals(m);
    vcl::clearPerFaceNormals(m);
    vcl::updatePerVertexAndFaceNormals(m);
    assert((m.face(0).normal() - expectedNormal).norm() < 1e-5f);
    for (uint i = 0; i < 3; ++i) {
        assert((m.vertex(i).normal() - expectedNormal).norm() < 1e-5f);
    }

    // 9. Multiply normals by matrix (rotate 90 deg around X axis: (0,0,1) -> (0,-1,0))
    Matrix33f rotX;
    rotX << 1.0f, 0.0f,  0.0f,
            0.0f, 0.0f, -1.0f,
            0.0f, 1.0f,  0.0f;
    vcl::multiplyPerFaceNormalsByMatrix(m, rotX);
    Point3f expectedRotated(0.0f, -1.0f, 0.0f);
    assert((m.face(0).normal() - expectedRotated).norm() < 1e-5f);

    vcl::multiplyPerVertexNormalsByMatrix(m, rotX);
    for (uint i = 0; i < 3; ++i) {
        assert((m.vertex(i).normal() - expectedRotated).norm() < 1e-5f);
    }

    std::println("Update Normals Algorithms verified successfully!");
}

#endif // TEST_UPDATE_NORMALS_H

