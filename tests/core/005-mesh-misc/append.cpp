// VCLib - Visual Computing Library
// Copyright (C) 2021-2026 Visual Computing Lab, ISTI - CNR.
//
// This Source Code Form is subject to the terms of the Mozilla Public License,
// v. 2.0. If a copy of the MPL was not distributed with this file, You can
// obtain one at https://mozilla.org/MPL/2.0/.

#include "types.h"

#include <catch2/catch_test_macros.hpp>

TEST_CASE("Append - Basic Append")
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

    REQUIRE(m1.vertexCount() == 8);
    REQUIRE(m1.vertexContainerSize() == 8);
    REQUIRE(m1.faceCount() == 4);
    REQUIRE(m1.faceContainerSize() == 4);

    // Initial m1 faces: (0, 1, 2) and (1, 3, 2)
    REQUIRE(
        (m1.face(0).vertexIndex(0) == 0 && m1.face(0).vertexIndex(1) == 1 &&
         m1.face(0).vertexIndex(2) == 2));
    REQUIRE(
        (m1.face(1).vertexIndex(0) == 1 && m1.face(1).vertexIndex(1) == 3 &&
         m1.face(1).vertexIndex(2) == 2));

    // Appended m2 faces: shifted by 4 -> (4, 5, 6) and (5, 7, 6)
    REQUIRE(
        (m1.face(2).vertexIndex(0) == 4 && m1.face(2).vertexIndex(1) == 5 &&
         m1.face(2).vertexIndex(2) == 6));
    REQUIRE(
        (m1.face(3).vertexIndex(0) == 5 && m1.face(3).vertexIndex(1) == 7 &&
         m1.face(3).vertexIndex(2) == 6));

    // Check positions
    REQUIRE(m1.face(0).vertex(0).position() == Point3f(0.0f, 0.0f, 1.0f));
    REQUIRE(m1.face(2).vertex(0).position() == Point3f(10.0f, 0.0f, 1.0f));

    // Check colors
    REQUIRE(m1.face(0).color() == Color::DarkRed);
    REQUIRE(m1.face(1).color() == Color::DarkBlue);
    REQUIRE(m1.face(2).color() == Color::Green);
    REQUIRE(m1.face(3).color() == Color::Cyan);

    // Mesh color preserved
    REQUIRE(m1.color() == Color::Red);
}

TEST_CASE("Append - Append With Deleted Elements")
{
    MyMesh m1 = createSampleQuadMesh();
    MyMesh m2 = createSampleQuadMesh();

    // Delete vertex 1 and face 0 in m2
    m2.deleteVertex(1);
    m2.deleteFace(0);
    REQUIRE(m2.vertexCount() == 3);
    REQUIRE(m2.deletedVertexCount() == 1);
    REQUIRE(m2.faceCount() == 1);
    REQUIRE(m2.deletedFaceCount() == 1);

    m1.append(m2);

    REQUIRE(m1.vertexContainerSize() == 8);
    REQUIRE(m1.vertexCount() == 7);
    REQUIRE(m1.deletedVertexCount() == 1);

    REQUIRE(m1.faceContainerSize() == 4);
    REQUIRE(m1.faceCount() == 3);
    REQUIRE(m1.deletedFaceCount() == 1);

    // Deleted element in m2 (vertex index 1) is now at index 4 + 1 = 5
    REQUIRE(m1.vertex(5).deleted());
    REQUIRE(!m1.vertex(4).deleted());
    REQUIRE(!m1.vertex(6).deleted());

    // Deleted face in m2 (face index 0) is now at index 2 + 0 = 2
    REQUIRE(m1.face(2).deleted());
    REQUIRE(!m1.face(0).deleted());
    REQUIRE(!m1.face(1).deleted());
    REQUIRE(!m1.face(3).deleted());

    // Verify iteration skips deleted elements
    std::size_t activeVCount = 0;
    for (auto v : m1.vertices()) {
        REQUIRE(!v.deleted());
        ++activeVCount;
    }
    REQUIRE(activeVCount == 7);

    std::size_t activeFCount = 0;
    for (auto f : m1.faces()) {
        REQUIRE(!f.deleted());
        ++activeFCount;
    }
    REQUIRE(activeFCount == 3);
}

TEST_CASE("Append - Self Append")
{
    MyMesh m = createSampleQuadMesh();
    m.append(m); // Self-append

    REQUIRE(m.vertexCount() == 8);
    REQUIRE(m.vertexContainerSize() == 8);
    REQUIRE(m.faceCount() == 4);
    REQUIRE(m.faceContainerSize() == 4);

    REQUIRE(
        (m.face(2).vertexIndex(0) == 4 && m.face(2).vertexIndex(1) == 5 &&
         m.face(2).vertexIndex(2) == 6));
    REQUIRE(
        (m.face(3).vertexIndex(0) == 5 && m.face(3).vertexIndex(1) == 7 &&
         m.face(3).vertexIndex(2) == 6));
}

TEST_CASE("Append - Container Range Views")
{
    MyMesh m = createSampleQuadMesh(); // 4 vertices, 2 faces

    // Test elements(begin, end) on mesh
    std::size_t count = 0;
    for (auto v : m.elements<ElemId::VERTEX>(1, 3)) {
        REQUIRE((v.index() >= 1 && v.index() < 3));
        ++count;
    }
    REQUIRE(count == 2);

    // Test vertices(begin, end) on mesh
    count = 0;
    for (auto v : m.vertices(1, 3)) {
        REQUIRE((v.index() >= 1 && v.index() < 3));
        ++count;
    }
    REQUIRE(count == 2);

    // Test faces(begin, end) on mesh
    count = 0;
    for (auto f : m.faces(0, 1)) {
        REQUIRE(f.index() == 0);
        ++count;
    }
    REQUIRE(count == 1);
}

TEST_CASE("Append - Append With Transform Matrix")
{
    TransformedMesh m1 = createSampleTransformedMesh();
    TransformedMesh m2 = createSampleTransformedMesh();

    // Verify initial transform matrix is identity
    REQUIRE(m1.transformMatrix() == Matrix44f::Identity());
    REQUIRE(m2.transformMatrix() == Matrix44f::Identity());

    // 1. Identity transform append: vertex positions and normals are identical
    // to original
    m1.append(m2);
    REQUIRE(m1.vertexCount() == 8);
    REQUIRE(m1.transformMatrix() == Matrix44f::Identity());
    // m1 original vertex 0 position (0, 0, 1), normal (0, 0, 1)
    REQUIRE(m1.vertex(0).position() == Point3f(0.0f, 0.0f, 1.0f));
    REQUIRE(m1.vertex(0).normal() == Point3f(0.0f, 0.0f, 1.0f));
    // m2 appended vertex 0 -> index 4: same position and normal
    REQUIRE(m1.vertex(4).position() == Point3f(0.0f, 0.0f, 1.0f));
    REQUIRE(m1.vertex(4).normal() == Point3f(0.0f, 0.0f, 1.0f));

    // 2. Relative translation append:
    TransformedMesh m3 = createSampleTransformedMesh();
    TransformedMesh m4 = createSampleTransformedMesh();

    Matrix44f t4         = Matrix44f::Identity();
    t4(0, 3)             = 10.0f;
    t4(1, 3)             = 20.0f;
    m4.transformMatrix() = t4;

    m3.append(m4);
    REQUIRE(m3.vertexCount() == 8);
    // m3's own transform matrix remains unchanged (Identity)
    REQUIRE(m3.transformMatrix() == Matrix44f::Identity());

    // Original vertices 0..3 of m3 unchanged
    REQUIRE(m3.vertex(0).position() == Point3f(0.0f, 0.0f, 1.0f));
    REQUIRE(m3.vertex(1).position() == Point3f(1.0f, 0.0f, 0.0f));

    // Appended vertices 4..7 from m4: position shifted by relative transform
    // (T3^-1 * T4 = I * T4 = T4) Vertex 0 of m4 was (0, 0, 1) -> now (10, 20,
    // 1)
    REQUIRE((m3.vertex(4).position() == Point3f(10.0f, 20.0f, 1.0f)));
    // Vertex 1 of m4 was (1, 0, 0) -> now (11, 20, 0)
    REQUIRE((m3.vertex(5).position() == Point3f(11.0f, 20.0f, 0.0f)));
    // Normals are unchanged because translation does not alter normals
    REQUIRE((m3.vertex(4).normal() == Point3f(0.0f, 0.0f, 1.0f)));

    // 3. Relative translation with non-identity source transform:
    // m5 has translation (5, 0, 0)
    // m6 has translation (15, 0, 0)
    // Relative transform: T5^-1 * T6 = (-5) + 15 = (+10)
    TransformedMesh m5   = createSampleTransformedMesh();
    TransformedMesh m6   = createSampleTransformedMesh();
    Matrix44f       t5   = Matrix44f::Identity();
    t5(0, 3)             = 5.0f;
    m5.transformMatrix() = t5;

    Matrix44f t6         = Matrix44f::Identity();
    t6(0, 3)             = 15.0f;
    m6.transformMatrix() = t6;

    m5.append(m6);
    REQUIRE(m5.transformMatrix() == t5);
    // m5 vertex 0 original was (0, 0, 1)
    REQUIRE((m5.vertex(0).position() == Point3f(0.0f, 0.0f, 1.0f)));
    // m6 vertex 0 was (0, 0, 1) -> in m5's frame: (0 + 10, 0, 1) = (10, 0, 1)
    REQUIRE((m5.vertex(4).position() == Point3f(10.0f, 0.0f, 1.0f)));

    // 4. Relative rotation (normals rotation check):
    // Rotate m8 by 90 degrees around X axis: (x, y, z) -> (x, -z, y)
    TransformedMesh m7   = createSampleTransformedMesh();
    TransformedMesh m8   = createSampleTransformedMesh();
    Matrix44f       r8   = Matrix44f::Identity();
    r8(1, 1)             = 0.0f;
    r8(1, 2)             = -1.0f;
    r8(2, 1)             = 1.0f;
    r8(2, 2)             = 0.0f;
    m8.transformMatrix() = r8;

    m7.append(m8);
    // Vertex 0 normal was (0, 0, 1) in m8
    // Rotated by 90 deg around X: (0, -1, 0)
    REQUIRE(std::abs(m7.vertex(4).normal().x() - 0.0f) < 1e-5f);
    REQUIRE(std::abs(m7.vertex(4).normal().y() - (-1.0f)) < 1e-5f);
    REQUIRE(std::abs(m7.vertex(4).normal().z() - 0.0f) < 1e-5f);

    // 5. Self-append on a transformed mesh
    TransformedMesh mSelf   = createSampleTransformedMesh();
    mSelf.transformMatrix() = t5;
    mSelf.append(mSelf);
    REQUIRE(mSelf.vertexCount() == 8);
    REQUIRE(mSelf.transformMatrix() == t5);
    REQUIRE(mSelf.vertex(4).position() == mSelf.vertex(0).position());
    REQUIRE(mSelf.vertex(5).position() == mSelf.vertex(1).position());
}

TEST_CASE("Append - Append With Materials")
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

    m2.face(0).materialIndex() =
        0; // "Material_1" (should remap to index 1 of m1)
    m2.face(1).materialIndex() =
        1; // "Material_2" (should become index 2 of m1)
    m2.face(2).materialIndex() =
        MaterialMeshWithMaterials::Face::NULL_MATERIAL_INDEX; // unassigned

    m2.pushTextureImage("textures/wood.png", Image());

    m1.append(m2);

    // Verify vertices and faces
    REQUIRE(m1.vertexCount() == 8);
    REQUIRE(m1.faceCount() == 5);

    // Verify materials count: Material_0, Material_1, Material_2 (duplicate was
    // merged)
    REQUIRE(m1.materialCount() == 3);
    REQUIRE(m1.material(0).name() == "Material_0");
    REQUIRE(m1.material(1).name() == "Material_1");
    REQUIRE(m1.material(2).name() == "Material_2");

    // Verify texture images
    REQUIRE(m1.textureImageCount() == 1);

    // Verify face material indices:
    // Original faces of m1:
    REQUIRE(m1.face(0).materialIndex() == 0);
    REQUIRE(m1.face(1).materialIndex() == 1);

    // Appended faces from m2:
    // face 2 was m2 face 0 (had mat 0 = Material_1 -> remapped to m1's index 1)
    REQUIRE(m1.face(2).materialIndex() == 1);
    // face 3 was m2 face 1 (had mat 1 = Material_2 -> remapped to m1's index 2)
    REQUIRE(m1.face(3).materialIndex() == 2);
    // face 4 was m2 face 2 (NULL_MATERIAL_INDEX -> remained
    // NULL_MATERIAL_INDEX)
    REQUIRE(
        m1.face(4).materialIndex() ==
        MaterialMeshWithMaterials::Face::NULL_MATERIAL_INDEX);

    // 2. Self-append on a mesh with materials
    MaterialMeshWithMaterials mSelf = createSampleMaterialMesh();
    mSelf.append(mSelf);
    REQUIRE(mSelf.vertexCount() == 8);
    REQUIRE(mSelf.faceCount() == 4);
    REQUIRE(
        mSelf.materialCount() ==
        2); // all materials already exist, count unchanged
    REQUIRE(mSelf.face(0).materialIndex() == 0);
    REQUIRE(mSelf.face(1).materialIndex() == 1);
    REQUIRE(mSelf.face(2).materialIndex() == 0);
    REQUIRE(mSelf.face(3).materialIndex() == 1);
}
