// VCLib - Visual Computing Library
// Copyright (C) 2021-2026 Visual Computing Lab, ISTI - CNR.
//
// This Source Code Form is subject to the terms of the Mozilla Public License,
// v. 2.0. If a copy of the MPL was not distributed with this file, You can
// obtain one at https://mozilla.org/MPL/2.0/.

#ifndef TEST_APPEND_H
#define TEST_APPEND_H

#include "types.h"

inline void testBasicAppend()
{
    MyMesh m1 = createSampleQuadMesh(); // 4 vertices, 2 faces
    MyMesh m2 = createSampleQuadMesh(); // 4 vertices, 2 faces

    // Offset positions and change face colors of m2
    for (auto v : m2.vertices()) {
        v.position() += Point3f(10.0f, 0.0f, 0.0f);
    }
    m2.face(0).color() = Color::Green;
    m2.face(1).color() = Color::Cyan;

    // Append m2 to m1
    m1.append(m2);

    assert(m1.vertexCount() == 8);
    assert(m1.vertexContainerSize() == 8);
    assert(m1.faceCount() == 4);
    assert(m1.faceContainerSize() == 4);

    // Initial m1 faces: (0, 1, 2) and (1, 3, 2)
    assert(m1.face(0).vertexIndex(0) == 0 && m1.face(0).vertexIndex(1) == 1 && m1.face(0).vertexIndex(2) == 2);
    assert(m1.face(1).vertexIndex(0) == 1 && m1.face(1).vertexIndex(1) == 3 && m1.face(1).vertexIndex(2) == 2);

    // Appended m2 faces: shifted by 4 -> (4, 5, 6) and (5, 7, 6)
    assert(m1.face(2).vertexIndex(0) == 4 && m1.face(2).vertexIndex(1) == 5 && m1.face(2).vertexIndex(2) == 6);
    assert(m1.face(3).vertexIndex(0) == 5 && m1.face(3).vertexIndex(1) == 7 && m1.face(3).vertexIndex(2) == 6);

    // Check positions
    assert(m1.face(0).vertex(0).position() == Point3f(0.0f, 0.0f, 1.0f));
    assert(m1.face(2).vertex(0).position() == Point3f(10.0f, 0.0f, 1.0f));

    // Check colors
    assert(m1.face(0).color() == Color::DarkRed);
    assert(m1.face(1).color() == Color::DarkBlue);
    assert(m1.face(2).color() == Color::Green);
    assert(m1.face(3).color() == Color::Cyan);

    // Mesh color preserved
    assert(m1.color() == Color::Red);
}

inline void testAppendWithDeletedElements()
{
    MyMesh m1 = createSampleQuadMesh();
    MyMesh m2 = createSampleQuadMesh();

    // Delete vertex 1 and face 0 in m2
    m2.deleteVertex(1);
    m2.deleteFace(0);
    assert(m2.vertexCount() == 3);
    assert(m2.deletedVertexCount() == 1);
    assert(m2.faceCount() == 1);
    assert(m2.deletedFaceCount() == 1);

    m1.append(m2);

    assert(m1.vertexContainerSize() == 8);
    assert(m1.vertexCount() == 7);
    assert(m1.deletedVertexCount() == 1);

    assert(m1.faceContainerSize() == 4);
    assert(m1.faceCount() == 3);
    assert(m1.deletedFaceCount() == 1);

    // Deleted element in m2 (vertex index 1) is now at index 4 + 1 = 5
    assert(m1.vertex(5).deleted());
    assert(!m1.vertex(4).deleted());
    assert(!m1.vertex(6).deleted());

    // Deleted face in m2 (face index 0) is now at index 2 + 0 = 2
    assert(m1.face(2).deleted());
    assert(!m1.face(0).deleted());
    assert(!m1.face(1).deleted());
    assert(!m1.face(3).deleted());

    // Verify iteration skips deleted elements
    std::size_t activeVCount = 0;
    for (auto v : m1.vertices()) {
        assert(!v.deleted());
        ++activeVCount;
    }
    assert(activeVCount == 7);

    std::size_t activeFCount = 0;
    for (auto f : m1.faces()) {
        assert(!f.deleted());
        ++activeFCount;
    }
    assert(activeFCount == 3);
}

inline void testSelfAppend()
{
    MyMesh m = createSampleQuadMesh();
    m.append(m); // Self-append

    assert(m.vertexCount() == 8);
    assert(m.vertexContainerSize() == 8);
    assert(m.faceCount() == 4);
    assert(m.faceContainerSize() == 4);

    assert(m.face(2).vertexIndex(0) == 4 && m.face(2).vertexIndex(1) == 5 && m.face(2).vertexIndex(2) == 6);
    assert(m.face(3).vertexIndex(0) == 5 && m.face(3).vertexIndex(1) == 7 && m.face(3).vertexIndex(2) == 6);
}

inline void testContainerRangeViews()
{
    MyMesh m = createSampleQuadMesh(); // 4 vertices, 2 faces

    // Test elements(begin, end) on mesh
    std::size_t count = 0;
    for (auto v : m.elements<meshv2::ElemId::VERTEX>(1, 3)) {
        assert(v.index() >= 1 && v.index() < 3);
        ++count;
    }
    assert(count == 2);

    // Test vertices(begin, end) on mesh
    count = 0;
    for (auto v : m.vertices(1, 3)) {
        assert(v.index() >= 1 && v.index() < 3);
        ++count;
    }
    assert(count == 2);

    // Test faces(begin, end) on mesh
    count = 0;
    for (auto f : m.faces(0, 1)) {
        assert(f.index() == 0);
        ++count;
    }
    assert(count == 1);
}

inline void testAppendWithTransformMatrix()
{
    TransformedMesh m1 = createSampleTransformedMesh();
    TransformedMesh m2 = createSampleTransformedMesh();

    // Verify initial transform matrix is identity
    assert(m1.transformMatrix() == Matrix44f::Identity());
    assert(m2.transformMatrix() == Matrix44f::Identity());

    // 1. Identity transform append: vertex positions and normals are identical to original
    m1.append(m2);
    assert(m1.vertexCount() == 8);
    assert(m1.transformMatrix() == Matrix44f::Identity());
    // m1 original vertex 0 position (0, 0, 1), normal (0, 0, 1)
    assert(m1.vertex(0).position() == Point3f(0.0f, 0.0f, 1.0f));
    assert(m1.vertex(0).normal() == Point3f(0.0f, 0.0f, 1.0f));
    // m2 appended vertex 0 -> index 4: same position and normal
    assert(m1.vertex(4).position() == Point3f(0.0f, 0.0f, 1.0f));
    assert(m1.vertex(4).normal() == Point3f(0.0f, 0.0f, 1.0f));

    // 2. Relative translation append:
    TransformedMesh m3 = createSampleTransformedMesh();
    TransformedMesh m4 = createSampleTransformedMesh();

    Matrix44f t4 = Matrix44f::Identity();
    t4(0, 3) = 10.0f;
    t4(1, 3) = 20.0f;
    m4.transformMatrix() = t4;

    m3.append(m4);
    assert(m3.vertexCount() == 8);
    // m3's own transform matrix remains unchanged (Identity)
    assert(m3.transformMatrix() == Matrix44f::Identity());

    // Original vertices 0..3 of m3 unchanged
    assert(m3.vertex(0).position() == Point3f(0.0f, 0.0f, 1.0f));
    assert(m3.vertex(1).position() == Point3f(1.0f, 0.0f, 0.0f));

    // Appended vertices 4..7 from m4: position shifted by relative transform (T3^-1 * T4 = I * T4 = T4)
    // Vertex 0 of m4 was (0, 0, 1) -> now (10, 20, 1)
    assert((m3.vertex(4).position() == Point3f(10.0f, 20.0f, 1.0f)));
    // Vertex 1 of m4 was (1, 0, 0) -> now (11, 20, 0)
    assert((m3.vertex(5).position() == Point3f(11.0f, 20.0f, 0.0f)));
    // Normals are unchanged because translation does not alter normals
    assert((m3.vertex(4).normal() == Point3f(0.0f, 0.0f, 1.0f)));

    // 3. Relative translation with non-identity source transform:
    // m5 has translation (5, 0, 0)
    // m6 has translation (15, 0, 0)
    // Relative transform: T5^-1 * T6 = (-5) + 15 = (+10)
    TransformedMesh m5 = createSampleTransformedMesh();
    TransformedMesh m6 = createSampleTransformedMesh();
    Matrix44f t5 = Matrix44f::Identity();
    t5(0, 3) = 5.0f;
    m5.transformMatrix() = t5;

    Matrix44f t6 = Matrix44f::Identity();
    t6(0, 3) = 15.0f;
    m6.transformMatrix() = t6;

    m5.append(m6);
    assert(m5.transformMatrix() == t5);
    // m5 vertex 0 original was (0, 0, 1)
    assert((m5.vertex(0).position() == Point3f(0.0f, 0.0f, 1.0f)));
    // m6 vertex 0 was (0, 0, 1) -> in m5's frame: (0 + 10, 0, 1) = (10, 0, 1)
    assert((m5.vertex(4).position() == Point3f(10.0f, 0.0f, 1.0f)));

    // 4. Relative rotation (normals rotation check):
    // Rotate m8 by 90 degrees around X axis: (x, y, z) -> (x, -z, y)
    TransformedMesh m7 = createSampleTransformedMesh();
    TransformedMesh m8 = createSampleTransformedMesh();
    Matrix44f r8 = Matrix44f::Identity();
    r8(1, 1) = 0.0f;
    r8(1, 2) = -1.0f;
    r8(2, 1) = 1.0f;
    r8(2, 2) = 0.0f;
    m8.transformMatrix() = r8;

    m7.append(m8);
    // Vertex 0 normal was (0, 0, 1) in m8
    // Rotated by 90 deg around X: (0, -1, 0)
    assert(std::abs(m7.vertex(4).normal().x() - 0.0f) < 1e-5f);
    assert(std::abs(m7.vertex(4).normal().y() - (-1.0f)) < 1e-5f);
    assert(std::abs(m7.vertex(4).normal().z() - 0.0f) < 1e-5f);

    // 5. Self-append on a transformed mesh
    TransformedMesh mSelf = createSampleTransformedMesh();
    mSelf.transformMatrix() = t5;
    mSelf.append(mSelf);
    assert(mSelf.vertexCount() == 8);
    assert(mSelf.transformMatrix() == t5);
    assert(mSelf.vertex(4).position() == mSelf.vertex(0).position());
    assert(mSelf.vertex(5).position() == mSelf.vertex(1).position());
}

inline void testAppendWithMaterials()
{
    // 1. Basic material append and remapping
    MaterialMeshWithMaterials m1 = createSampleMaterialMesh();
    MaterialMeshWithMaterials m2;
    m2.resizeVertices(4);
    m2.resizeFaces(3);

    m2.vertex(0).position() = Point3f(10.0f, 0.0f, 0.0f);
    m2.vertex(1).position() = Point3f(11.0f, 0.0f, 0.0f);
    m2.vertex(2).position() = Point3f(10.0f, 1.0f, 0.0f);
    m2.vertex(3).position() = Point3f(11.0f, 1.0f, 0.0f);

    m2.face(0).setVertices(0, 1, 2);
    m2.face(1).setVertices(1, 3, 2);
    m2.face(2).setVertices(0, 2, 3);

    Material matDuplicate;
    matDuplicate.name() = "Material_1"; // duplicate of m1's material 1
    Material matNew;
    matNew.name() = "Material_2"; // new material

    m2.pushMaterial(matDuplicate);
    m2.pushMaterial(matNew);

    m2.face(0).materialIndex() = 0; // "Material_1" (should remap to index 1 of m1)
    m2.face(1).materialIndex() = 1; // "Material_2" (should become index 2 of m1)
    m2.face(2).materialIndex() =
        MaterialMeshWithMaterials::Face::NULL_MATERIAL_INDEX; // unassigned

    m2.pushTextureImage("textures/wood.png", Image());

    m1.append(m2);

    // Verify vertices and faces
    assert(m1.vertexCount() == 8);
    assert(m1.faceCount() == 5);

    // Verify materials count: Material_0, Material_1, Material_2 (duplicate was merged)
    assert(m1.materialCount() == 3);
    assert(m1.material(0).name() == "Material_0");
    assert(m1.material(1).name() == "Material_1");
    assert(m1.material(2).name() == "Material_2");

    // Verify texture images
    assert(m1.textureImageCount() == 1);

    // Verify face material indices:
    // Original faces of m1:
    assert(m1.face(0).materialIndex() == 0);
    assert(m1.face(1).materialIndex() == 1);

    // Appended faces from m2:
    // face 2 was m2 face 0 (had mat 0 = Material_1 -> remapped to m1's index 1)
    assert(m1.face(2).materialIndex() == 1);
    // face 3 was m2 face 1 (had mat 1 = Material_2 -> remapped to m1's index 2)
    assert(m1.face(3).materialIndex() == 2);
    // face 4 was m2 face 2 (NULL_MATERIAL_INDEX -> remained NULL_MATERIAL_INDEX)
    assert(m1.face(4).materialIndex() ==
           MaterialMeshWithMaterials::Face::NULL_MATERIAL_INDEX);

    // 2. Self-append on a mesh with materials
    MaterialMeshWithMaterials mSelf = createSampleMaterialMesh();
    mSelf.append(mSelf);
    assert(mSelf.vertexCount() == 8);
    assert(mSelf.faceCount() == 4);
    assert(mSelf.materialCount() == 2); // all materials already exist, count unchanged
    assert(mSelf.face(0).materialIndex() == 0);
    assert(mSelf.face(1).materialIndex() == 1);
    assert(mSelf.face(2).materialIndex() == 0);
    assert(mSelf.face(3).materialIndex() == 1);
}

inline void testAppend()
{
    std::println("--- Testing Mesh::append ---");
    testBasicAppend();
    testAppendWithDeletedElements();
    testSelfAppend();
    testContainerRangeViews();
    testAppendWithTransformMatrix();
    testAppendWithMaterials();
    std::println("Mesh::append verified successfully!\n");
}

#endif // TEST_APPEND_H
