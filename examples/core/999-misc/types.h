// VCLib - Visual Computing Library
// Copyright (C) 2021-2026 Visual Computing Lab, ISTI - CNR.
//
// This Source Code Form is subject to the terms of the Mozilla Public License,
// v. 2.0. If a copy of the MPL was not distributed with this file, You can
// obtain one at https://mozilla.org/MPL/2.0/.

#ifndef TEST_TYPES_H
#define TEST_TYPES_H

#include <array>
#include <cassert>
#include <limits>
#include <print>
#include <string>
#include <vector>

#include <vclib/space/core.h>

#include <vclib/mesh_v2/components/bit_flags.h>
#include <vclib/mesh_v2/components/custom_components.h>
#include <vclib/mesh_v2/components/normal.h>
#include <vclib/mesh_v2/components/position.h>
#include <vclib/mesh_v2/components/vertex_references.h>
#include <vclib/mesh_v2/containers/face_container.h>
#include <vclib/mesh_v2/containers/vertex_container.h>
#include <vclib/mesh_v2/mesh.h>
#include <vclib/mesh_v2/mesh_components.h>

template<typename CharT>
struct std::formatter<vcl::Color, CharT> :
        std::formatter<vcl::Point4<vcl::uint>, CharT>
{
    template<typename FormatContext>
    auto format(const vcl::Color& c, FormatContext& ctx) const
    {
        return std::formatter<vcl::Point4<vcl::uint>, CharT>::format(
            c.cast<vcl::uint>(), ctx);
    }
};

using namespace vcl;

// =============================================================================
// Mesh Type Definitions
// =============================================================================

using MyVertexComponents = meshv2::VertexComponents<
    meshv2::vert::BitFlags,
    meshv2::vert::Position3f,
    meshv2::vert::OptionalNormal3f,
    meshv2::vert::OptionalColor>;

using MyFaceComponents = meshv2::FaceComponents<
    meshv2::face::BitFlags,
    meshv2::face::TriangleVertexReferences<vcl::uint>,
    meshv2::face::OptionalColor>;

using MyMesh = meshv2::Mesh<
    meshv2::VertexContainer<MyVertexComponents>,
    meshv2::FaceContainer<MyFaceComponents>,
    meshv2::mesh::Color>;

using TransformedMesh = meshv2::Mesh<
    meshv2::VertexContainer<MyVertexComponents>,
    meshv2::FaceContainer<MyFaceComponents>,
    meshv2::mesh::Color,
    meshv2::mesh::TransformMatrixf>;

using BoundingBoxMesh = meshv2::Mesh<
    meshv2::VertexContainer<MyVertexComponents>,
    meshv2::FaceContainer<MyFaceComponents>,
    meshv2::mesh::BoundingBox3f>;

using TransformedBoundingBoxMesh = meshv2::Mesh<
    meshv2::VertexContainer<MyVertexComponents>,
    meshv2::FaceContainer<MyFaceComponents>,
    meshv2::mesh::BoundingBox3f,
    meshv2::mesh::TransformMatrixf>;

using NamedMesh = meshv2::Mesh<
    meshv2::VertexContainer<MyVertexComponents>,
    meshv2::FaceContainer<MyFaceComponents>,
    meshv2::mesh::Name>;

using CurvatureVertexComponents = meshv2::VertexComponents<
    meshv2::vert::BitFlags,
    meshv2::vert::Position3f,
    meshv2::vert::PrincipalCurvaturef>;

using OptionalCurvatureVertexComponents = meshv2::VertexComponents<
    meshv2::vert::BitFlags,
    meshv2::vert::Position3f,
    meshv2::vert::OptionalPrincipalCurvatured>;

using CurvatureFaceComponents = meshv2::FaceComponents<
    meshv2::face::BitFlags,
    meshv2::face::TriangleVertexReferences<vcl::uint>,
    meshv2::face::OptionalPrincipalCurvaturef>;

using CurvatureMesh = meshv2::Mesh<
    meshv2::VertexContainer<CurvatureVertexComponents>,
    meshv2::FaceContainer<CurvatureFaceComponents>,
    meshv2::mesh::Name>;

using OptionalCurvatureMesh = meshv2::Mesh<
    meshv2::VertexContainer<OptionalCurvatureVertexComponents>,
    meshv2::FaceContainer<CurvatureFaceComponents>,
    meshv2::mesh::Name>;

using FullVertexComponents = meshv2::VertexComponents<
    meshv2::vert::BitFlags,
    meshv2::vert::Position3f,
    meshv2::vert::Normal3f,
    meshv2::vert::Color>;

using FullFaceComponents = meshv2::FaceComponents<
    meshv2::face::BitFlags,
    meshv2::face::TriangleVertexReferences<vcl::uint>,
    meshv2::face::Color>;

using FullMesh = meshv2::Mesh<
    meshv2::VertexContainer<FullVertexComponents>,
    meshv2::FaceContainer<FullFaceComponents>,
    meshv2::mesh::Color>;

using SimpleVertexComponents =
    meshv2::VertexComponents<meshv2::vert::BitFlags, meshv2::vert::Position3f>;

using SimpleMesh = meshv2::Mesh<
    meshv2::VertexContainer<SimpleVertexComponents>,
    meshv2::FaceContainer<MyFaceComponents>>;

using PolyFaceComponents = meshv2::FaceComponents<
    meshv2::face::PolygonBitFlags,
    meshv2::face::PolygonVertexReferences<vcl::uint>>;

using PolyMesh = meshv2::Mesh<
    meshv2::VertexContainer<SimpleVertexComponents>,
    meshv2::FaceContainer<PolyFaceComponents>>;

using TriBitMesh = vcl::meshv2::Mesh<
    vcl::meshv2::VertexContainer<vcl::TypeWrapper<
        vcl::meshv2::comp::Position3f,
        vcl::meshv2::comp::BitFlags>>,
    vcl::meshv2::FaceContainer<vcl::TypeWrapper<
        vcl::meshv2::comp::TriangleBitFlags,
        vcl::meshv2::comp::VertexReferences<3>>>>;

using MeshDouble = vcl::meshv2::Mesh<
    vcl::meshv2::VertexContainer<vcl::TypeWrapper<
        vcl::meshv2::comp::Position3d,
        vcl::meshv2::comp::BitFlags>>,
    vcl::meshv2::FaceContainer<vcl::TypeWrapper<
        vcl::meshv2::comp::TriangleBitFlags,
        vcl::meshv2::comp::VertexReferences<3>>>>;

using CCMeshVertexComponents = meshv2::VertexComponents<
    meshv2::vert::BitFlags,
    meshv2::vert::Position3f,
    meshv2::vert::CustomComponents>;

using CCMeshFaceComponents = meshv2::FaceComponents<
    meshv2::face::BitFlags,
    meshv2::face::TriangleVertexReferences<vcl::uint>,
    meshv2::face::CustomComponents>;

using CCMesh = meshv2::Mesh<
    meshv2::VertexContainer<CCMeshVertexComponents>,
    meshv2::FaceContainer<CCMeshFaceComponents>,
    meshv2::mesh::CustomComponents>;

using VertexComponents64 = meshv2::VertexComponents<
    meshv2::vert::BitFlags,
    meshv2::vert::Position3d>;

using FaceComponents64 = meshv2::FaceComponents<
    meshv2::face::BitFlags,
    meshv2::face::TriangleVertexReferences<uint64_t>>;

using Mesh64 = meshv2::Mesh<
    meshv2::VertexContainer<VertexComponents64, uint64_t>,
    meshv2::FaceContainer<FaceComponents64, uint64_t>>;

using MaterialFaceComponents = meshv2::FaceComponents<
    meshv2::face::BitFlags,
    meshv2::face::TriangleVertexReferences<vcl::uint>,
    meshv2::face::MaterialIndex,
    meshv2::face::OptionalColor>;

using MaterialMesh = meshv2::Mesh<
    meshv2::VertexContainer<MyVertexComponents>,
    meshv2::FaceContainer<MaterialFaceComponents>>;

using OptionalMaterialFaceComponents = meshv2::FaceComponents<
    meshv2::face::BitFlags,
    meshv2::face::TriangleVertexReferences<vcl::uint>,
    meshv2::face::OptionalMaterialIndex>;

using OptionalMaterialMesh = meshv2::Mesh<
    meshv2::VertexContainer<MyVertexComponents>,
    meshv2::FaceContainer<OptionalMaterialFaceComponents>>;

using MaterialMeshWithMaterials = meshv2::Mesh<
    meshv2::VertexContainer<MyVertexComponents>,
    meshv2::FaceContainer<MaterialFaceComponents>,
    meshv2::mesh::Materials>;

using AdjVertexComponents = meshv2::VertexComponents<
    meshv2::vert::BitFlags,
    meshv2::vert::Position3f,
    meshv2::vert::AdjacentFaces,
    meshv2::vert::AdjacentVertices,
    meshv2::vert::OptionalAdjacentEdges>;

using AdjFaceComponents = meshv2::FaceComponents<
    meshv2::face::BitFlags,
    meshv2::face::TriangleVertexReferences<vcl::uint>,
    meshv2::face::TriangleAdjacentFaces,
    meshv2::face::OptionalTriangleAdjacentEdges,
    meshv2::face::OptionalColor>;

using AdjMesh = meshv2::Mesh<
    meshv2::VertexContainer<AdjVertexComponents>,
    meshv2::FaceContainer<AdjFaceComponents>>;

// Concept checking templates
template<typename T>
concept CanCallBitFlags = requires (T t) { t.bitFlags(); };

template<typename T>
concept CanCallPositions = requires (T t) { t.positions(); };

template<typename T>
concept CanCallNormals = requires (T t) { t.normals(); };

template<typename T>
concept CanCallVertexIndices = requires (T t) { t.vertexIndices(); };

/**
 * @brief Helper to create a standard sample mesh with 4 vertices and 2 faces
 * forming a quad.
 *
 * Vertex 0: (0, 0, 1), normal (0, 0, 1), color Blue
 * Vertex 1: (1, 0, 0), normal (0, 0, 1), color Green
 * Vertex 2: (0, 1, 0), normal (0, 0, 1), color Yellow
 * Vertex 3: (1, 1, 0), normal (0, 0, 1), color Cyan
 *
 * Face 0: (0, 1, 2), color DarkRed
 * Face 1: (1, 3, 2), color DarkBlue
 *
 * Mesh color: Red
 */
inline MyMesh createSampleQuadMesh()
{
    MyMesh mesh;
    mesh.enablePerElementComponent<meshv2::ElemId::VERTEX, meshv2::CompId::NORMAL>();
    mesh.resizeVertices(4);
    mesh.resizeFaces(2);

    mesh.vertex(0).position() = Point3f(0.0f, 0.0f, 1.0f);
    mesh.vertex(0).normal()   = Point3f(0.0f, 0.0f, 1.0f);

    mesh.vertex(1).position() = Point3f(1.0f, 0.0f, 0.0f);
    mesh.vertex(1).normal()   = Point3f(0.0f, 0.0f, 1.0f);

    mesh.vertex(2).position() = Point3f(0.0f, 1.0f, 0.0f);
    mesh.vertex(2).normal()   = Point3f(0.0f, 0.0f, 1.0f);

    mesh.vertex(3).position() = Point3f(1.0f, 1.0f, 0.0f);
    mesh.vertex(3).normal()   = Point3f(0.0f, 0.0f, 1.0f);

    mesh.face(0).setVertices(0, 1, 2);
    mesh.face(1).setVertices(mesh.vertex(1), mesh.vertex(3), mesh.vertex(2));

    mesh.color() = Color::Red;

    mesh.enablePerElementComponent<meshv2::ElemId::VERTEX, meshv2::CompId::COLOR>();
    mesh.vertex(0).color() = Color::Blue;
    mesh.vertex(1).color() = Color::Green;
    mesh.vertex(2).color() = Color::Yellow;
    mesh.vertex(3).color() = Color::Cyan;

    mesh.enablePerElementComponent<meshv2::ElemId::FACE, meshv2::CompId::COLOR>();
    mesh.face(0).color() = Color::DarkRed;
    mesh.face(1).color() = Color::DarkBlue;

    return mesh;
}

inline TransformedMesh createSampleTransformedMesh()
{
    TransformedMesh mesh;
    mesh.enablePerElementComponent<meshv2::ElemId::VERTEX, meshv2::CompId::NORMAL>();
    mesh.resizeVertices(4);
    mesh.resizeFaces(2);

    mesh.vertex(0).position() = Point3f(0.0f, 0.0f, 1.0f);
    mesh.vertex(0).normal()   = Point3f(0.0f, 0.0f, 1.0f);

    mesh.vertex(1).position() = Point3f(1.0f, 0.0f, 0.0f);
    mesh.vertex(1).normal()   = Point3f(0.0f, 0.0f, 1.0f);

    mesh.vertex(2).position() = Point3f(0.0f, 1.0f, 0.0f);
    mesh.vertex(2).normal()   = Point3f(0.0f, 0.0f, 1.0f);

    mesh.vertex(3).position() = Point3f(1.0f, 1.0f, 0.0f);
    mesh.vertex(3).normal()   = Point3f(0.0f, 0.0f, 1.0f);

    mesh.face(0).setVertices(0, 1, 2);
    mesh.face(1).setVertices(mesh.vertex(1), mesh.vertex(3), mesh.vertex(2));

    mesh.color() = Color::Red;

    mesh.enablePerElementComponent<meshv2::ElemId::VERTEX, meshv2::CompId::COLOR>();
    mesh.vertex(0).color() = Color::Blue;
    mesh.vertex(1).color() = Color::Green;
    mesh.vertex(2).color() = Color::Yellow;
    mesh.vertex(3).color() = Color::Cyan;

    mesh.enablePerElementComponent<meshv2::ElemId::FACE, meshv2::CompId::COLOR>();
    mesh.face(0).color() = Color::DarkRed;
    mesh.face(1).color() = Color::DarkBlue;

    return mesh;
}

inline MaterialMeshWithMaterials createSampleMaterialMesh()
{
    MaterialMeshWithMaterials mesh;
    mesh.resizeVertices(4);
    mesh.resizeFaces(2);

    mesh.vertex(0).position() = Point3f(0.0f, 0.0f, 1.0f);
    mesh.vertex(1).position() = Point3f(1.0f, 0.0f, 0.0f);
    mesh.vertex(2).position() = Point3f(0.0f, 1.0f, 0.0f);
    mesh.vertex(3).position() = Point3f(1.0f, 1.0f, 0.0f);

    mesh.face(0).setVertices(0, 1, 2);
    mesh.face(1).setVertices(1, 3, 2);

    Material mat0;
    mat0.name() = "Material_0";
    Material mat1;
    mat1.name() = "Material_1";

    mesh.pushMaterial(mat0);
    mesh.pushMaterial(mat1);

    mesh.face(0).materialIndex() = 0;
    mesh.face(1).materialIndex() = 1;

    return mesh;
}

#endif // TEST_TYPES_H
