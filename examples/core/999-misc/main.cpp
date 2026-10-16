// VCLib - Visual Computing Library
// Copyright (C) 2021-2026 Visual Computing Lab, ISTI - CNR.
//
// This Source Code Form is subject to the terms of the Mozilla Public License,
// v. 2.0. If a copy of the MPL was not distributed with this file, You can
// obtain one at https://mozilla.org/MPL/2.0/.

#include <iostream>
#include <vclib/space/core.h>

#include <vclib/mesh_v2/components/bit_flags.h>
#include <vclib/mesh_v2/components/normal.h>
#include <vclib/mesh_v2/components/position.h>
#include <vclib/mesh_v2/components/vertex_references.h>
#include <vclib/mesh_v2/containers/face_container.h>
#include <vclib/mesh_v2/containers/vertex_container.h>
#include <vclib/mesh_v2/elements.h>

using namespace vcl;

// =============================================================================
// Step 1: Definition of Vertex and Face component lists
// =============================================================================

using MyVertexComponents = meshv2::VertexComponents<
    meshv2::comp::BitFlags,
    meshv2::comp::Position3f,
    meshv2::comp::Normal3f>;

using MyFaceComponents = meshv2::FaceComponents<
    meshv2::comp::BitFlags,
    meshv2::comp::TriangleVertexReferences<uint>>;

// =============================================================================
// Step 2 & 3: ProtoMesh inherits directly from VertexContainer and FaceContainer
// =============================================================================

class ProtoMesh :
    public meshv2::VertexContainer<MyVertexComponents, uint, ProtoMesh>,
    public meshv2::FaceContainer<MyFaceComponents, uint, ProtoMesh>
{
public:
    using VertexContainer = meshv2::VertexContainer<MyVertexComponents, uint, ProtoMesh>;
    using FaceContainer   = meshv2::FaceContainer<MyFaceComponents, uint, ProtoMesh>;

    using Vertex      = typename VertexContainer::Vertex;
    using ConstVertex = typename VertexContainer::ConstVertex;
    using Face        = typename FaceContainer::Face;
    using ConstFace   = typename FaceContainer::ConstFace;

    ProtoMesh()
    {
        VertexContainer::setMesh(this);
        FaceContainer::setMesh(this);

        VertexContainer::template enable<meshv2::CompId::POSITION>();
        VertexContainer::template enable<meshv2::CompId::NORMAL>();
        FaceContainer::template enable<meshv2::CompId::VERTEX_REFERENCES>();
    }
};

int main()
{
    // =========================================================================
    // Compile-time checks (Concepts & Strong Typing)
    // =========================================================================

    // Component lists satisfy component concepts
    static_assert(meshv2::HasRequiredVertexComponents<MyVertexComponents>);
    static_assert(meshv2::HasRequiredFaceComponents<MyFaceComponents>);

    // The single element classes (Vertex and Face) satisfy concepts
    static_assert(meshv2::ElementConcept<ProtoMesh::Vertex>);
    static_assert(meshv2::ElementConcept<ProtoMesh::ConstVertex>);
    static_assert(meshv2::ElementConcept<ProtoMesh::Face>);
    static_assert(meshv2::ElementConcept<ProtoMesh::ConstFace>);

    static_assert(meshv2::VertexConcept<ProtoMesh::Vertex>);
    static_assert(meshv2::VertexConcept<ProtoMesh::ConstVertex>);
    static_assert(meshv2::FaceConcept<ProtoMesh::Face>);
    static_assert(meshv2::FaceConcept<ProtoMesh::ConstFace>);
    static_assert(meshv2::TriangleFaceConcept<ProtoMesh::Face>);
    static_assert(meshv2::TriangleFaceConcept<ProtoMesh::ConstFace>);

    // Strong typing: Vertex and Face cannot be confused
    static_assert(!meshv2::VertexConcept<ProtoMesh::Face>);
    static_assert(!meshv2::FaceConcept<ProtoMesh::Vertex>);

    // =========================================================================
    // Create and Populate ProtoMesh
    // =========================================================================

    ProtoMesh mesh;
    // Methods directly inherited from the containers!
    mesh.resizeVertices(4);
    mesh.resizeFaces(2);

    // 1. Setting vertex data via Vertex
    mesh.vertex(0).position() = Point3f(0.0f, 0.0f, 0.0f);
    mesh.vertex(0).normal()   = Point3f(0.0f, 0.0f, 1.0f);

    mesh.vertex(1).position() = Point3f(1.0f, 0.0f, 0.0f);
    mesh.vertex(1).normal()   = Point3f(0.0f, 0.0f, 1.0f);

    mesh.vertex(2).position() = Point3f(0.0f, 1.0f, 0.0f);
    mesh.vertex(2).normal()   = Point3f(0.0f, 0.0f, 1.0f);

    mesh.vertex(3).position() = Point3f(1.0f, 1.0f, 0.0f);
    mesh.vertex(3).normal()   = Point3f(0.0f, 0.0f, 1.0f);

    // 2. Setting face references via Face
    // Face 0: set using indices directly
    mesh.face(0).setVertices(0, 1, 2);

    // Face 1: set using vertices
    mesh.face(1).setVertices(mesh.vertex(1), mesh.vertex(3), mesh.vertex(2));

    std::cout << "--- ProtoMesh Created ---\n";
    std::cout << "Vertices: " << mesh.vertexCount() << ", Faces: " << mesh.faceCount() << "\n\n";

    // =========================================================================
    // Topological Navigation (Face -> Vertex)
    // =========================================================================

    std::cout << "--- Topological Navigation from Face 0 ---\n";
    for (uint i = 0; i < mesh.face(0).vertexCount(); ++i) {
        std::cout << "  Face 0 corner " << i
                  << ": vertex index = " << mesh.face(0).vertexIndex(i)
                  << ", position = " << mesh.face(0).vertex(i).position() << "\n";
    }

    std::cout << "\nIterating over Face 1 vertices with face.vertices():\n";
    for (ProtoMesh::Vertex v : mesh.face(1).vertices()) {
        std::cout << "  Vertex " << v.index()
                  << " pos = " << v.position()
                  << " norm = " << v.normal() << "\n";
    }

    // =========================================================================
    // Iterating over Mesh Elements
    // =========================================================================

    std::cout << "\n--- Iterating over all Vertices ---\n";
    for (auto v : mesh.vertices()) {
        std::cout << "  Vertex " << v.index()
                  << " pos = " << v.position()
                  << " norm = " << v.normal() << "\n";
    }

    std::cout << "\n--- Iterating over all Faces ---\n";
    for (auto f : mesh.faces()) {
        std::cout << "  Face " << f.index() << " with vertex indices: ("
                  << f.vertexIndex(0) << ", "
                  << f.vertexIndex(1) << ", "
                  << f.vertexIndex(2) << ")\n";
    }

    // =========================================================================
    // Deletion and Automatic Skipping
    // =========================================================================

    std::cout << "\n--- Deletion and Skip Test ---\n";
    mesh.vertex(1).setDeleted(true);

    std::cout << "Marked vertex 1 as deleted.\n";
    std::cout << "  Physical container size: " << mesh.vertexContainerSize() << "\n";
    std::cout << "  Active (non-deleted) count: " << mesh.vertexCount() << "\n";
    std::cout << "  Deleted count: " << mesh.deletedVertexCount() << "\n";

    std::cout << "Iterating over active vertices (vertex 1 automatically skipped):\n";
    for (auto v : mesh.vertices()) {
        std::cout << "  Visible vertex: " << v.index() << " pos = " << v.position() << "\n";
    }

    std::cout << "Iterating over ALL vertices (including deleted, jumpDeleted = false):\n";
    for (auto v : mesh.vertices(false)) {
        std::cout << "  Vertex " << v.index()
                  << " (deleted: " << (v.deleted() ? "yes" : "no") << ")\n";
    }

    // Undelete vertex 1
    mesh.vertex(1).setDeleted(false);
    std::cout << "Undeleted vertex 1, active count: " << mesh.vertexCount() << "\n\n";

    // =========================================================================
    // Const Correctness
    // =========================================================================

    std::cout << "--- Const Correctness Test ---\n";
    const ProtoMesh& constMesh = mesh;

    // Access returns ConstVertex / ConstFace proxies
    ProtoMesh::ConstVertex cv0 = constMesh.vertex(0);
    std::cout << "Const Vertex 0 pos: " << cv0.position() << "\n";
    // cv0.position() = Point3f(0.0f, 0.0f, 0.0f); // Compile error as expected!

    for (ProtoMesh::ConstVertex v : constMesh.face(0).vertices()) {
        std::cout << "  Const face vertex: " << v.index()
                  << " pos: " << v.position() << "\n";
    }

    // Conversion from mutable Vertex to ConstVertex
    ProtoMesh::ConstVertex cvFromMutable = mesh.vertex(2);
    std::cout << "ConstVertex converted from mutable Vertex: index "
              << cvFromMutable.index() << "\n\n";

    // =========================================================================
    // Element Validity
    // =========================================================================

    std::cout << "--- Element Validity Test ---\n";
    ProtoMesh::Vertex validV = mesh.vertex(0);
    ProtoMesh::Vertex invalidV;

    std::cout << "validV isValid: " << (validV ? "valid" : "invalid") << "\n";
    std::cout << "invalidV isValid: " << (invalidV ? "valid" : "invalid") << "\n";

    return 0;
}
