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
#include <vclib/mesh_v2/mesh.h>
#include <vclib/mesh_v2/mesh_components.h>

using namespace vcl;

// =============================================================================
// Step 1: Definition of Vertex and Face component lists
// =============================================================================

using MyVertexComponents = meshv2::VertexComponents<
    meshv2::vert::BitFlags,
    meshv2::vert::Position3f,
    meshv2::vert::Normal3f,
    meshv2::vert::Color>;

using MyFaceComponents = meshv2::FaceComponents<
    meshv2::face::BitFlags,
    meshv2::face::TriangleVertexReferences<uint>,
    meshv2::face::Color>;

// =============================================================================
// Step 2 & 3: Mesh definition from Containers and Mesh-level components
// =============================================================================

using MyMesh = meshv2::Mesh<
    meshv2::VertexContainer<MyVertexComponents>,
    meshv2::FaceContainer<MyFaceComponents>,
    meshv2::mesh::Color>;

using SimpleVertexComponents = meshv2::VertexComponents<
    meshv2::vert::BitFlags,
    meshv2::vert::Position3f>;

using SimpleMesh = meshv2::Mesh<
    meshv2::VertexContainer<SimpleVertexComponents>,
    meshv2::FaceContainer<MyFaceComponents>>;

using PolyFaceComponents = meshv2::FaceComponents<
    meshv2::face::PolygonBitFlags,
    meshv2::face::PolygonVertexReferences<uint>>;

using PolyMesh = meshv2::Mesh<
    meshv2::VertexContainer<SimpleVertexComponents>,
    meshv2::FaceContainer<PolyFaceComponents>>;

template<typename T>
concept CanCallBitFlags = requires(T t) { t.bitFlags(); };

template<typename T>
concept CanCallPositions = requires(T t) { t.positions(); };

template<typename T>
concept CanCallNormals = requires(T t) { t.normals(); };

template<typename T>
concept CanCallVertexIndices = requires(T t) { t.vertexIndices(); };

int main()
{
    // =========================================================================
    // Compile-time checks (Concepts & Strong Typing)
    // =========================================================================

    // Component lists satisfy component concepts
    static_assert(meshv2::HasRequiredVertexComponents<MyVertexComponents>);
    static_assert(meshv2::HasRequiredFaceComponents<MyFaceComponents>);

    // The single element classes (Vertex and Face) satisfy concepts
    static_assert(meshv2::ElementConcept<MyMesh::Vertex>);
    static_assert(meshv2::ElementConcept<MyMesh::ConstVertex>);
    static_assert(meshv2::ElementConcept<MyMesh::Face>);
    static_assert(meshv2::ElementConcept<MyMesh::ConstFace>);

    static_assert(meshv2::VertexConcept<MyMesh::Vertex>);
    static_assert(meshv2::VertexConcept<MyMesh::ConstVertex>);
    static_assert(meshv2::FaceConcept<MyMesh::Face>);
    static_assert(meshv2::FaceConcept<MyMesh::ConstFace>);
    static_assert(meshv2::TriangleFaceConcept<MyMesh::Face>);
    static_assert(meshv2::TriangleFaceConcept<MyMesh::ConstFace>);
    static_assert(meshv2::face::HasTriangleVertexReferences<MyMesh::Face>);
    static_assert(meshv2::face::HasTriangleVertexReferences<MyMesh::ConstFace>);
    static_assert(meshv2::face::HasTriangleVertexReferences<MyFaceComponents>);

    // Strong typing: Vertex and Face cannot be confused
    static_assert(!meshv2::VertexConcept<MyMesh::Face>);
    static_assert(!meshv2::FaceConcept<MyMesh::Vertex>);

    // Mesh Concepts
    static_assert(meshv2::HasVertices<MyMesh>);
    static_assert(meshv2::HasFaces<MyMesh>);
    static_assert(meshv2::HasTriangles<MyMesh>);
    static_assert(meshv2::MeshConcept<MyMesh>);

    // Color Component Concepts
    static_assert(meshv2::mesh::HasColor<MyMesh>);
    static_assert(meshv2::mesh::HasColor<meshv2::mesh::Color>);
    static_assert(!meshv2::mesh::HasColor<SimpleMesh>);

    static_assert(meshv2::vert::HasColor<MyMesh::Vertex>);
    static_assert(meshv2::vert::HasColor<MyMesh::ConstVertex>);
    static_assert(meshv2::vert::HasColor<MyVertexComponents>);
    static_assert(!meshv2::vert::HasColor<SimpleMesh::Vertex>);

    static_assert(meshv2::face::HasColor<MyMesh::Face>);
    static_assert(meshv2::face::HasColor<MyMesh::ConstFace>);
    static_assert(meshv2::face::HasColor<MyFaceComponents>);

    // Container and Component Concepts
    static_assert(meshv2::ElementContainerConcept<
                  MyMesh::ContainerType<meshv2::ElemId::VERTEX>>);
    static_assert(meshv2::ElementContainerConcept<
                  MyMesh::ContainerType<meshv2::ElemId::FACE>>);
    static_assert(!meshv2::ElementContainerConcept<meshv2::mesh::Color>);
    static_assert(meshv2::ComponentConcept<meshv2::mesh::Color>);
    static_assert(!meshv2::ComponentConcept<
                  MyMesh::ContainerType<meshv2::ElemId::VERTEX>>);
    static_assert(!meshv2::ComponentConcept<
                  MyMesh::ContainerType<meshv2::ElemId::FACE>>);
    static_assert(std::is_same_v<MyMesh::Components, vcl::TypeWrapper<meshv2::mesh::Color>>);

    // =========================================================================
    // Create and Populate MyMesh
    // =========================================================================

    MyMesh mesh;
    // Position and VertexReferences are mandatory and auto-enabled!
    // Optional components (like Normal) can be enabled explicitly:
    mesh.enable<meshv2::ElemId::VERTEX, meshv2::CompId::NORMAL>();

    // Methods directly inherited from the containers!
    mesh.resizeVertices(4);
    mesh.resizeFaces(2);

    // 1. Setting vertex data via Vertex
    mesh.vertex(0).position() = Point3f(0.0f, 0.0f, 1.0f);
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

    // 3. Setting mesh-level Color
    mesh.color() = Color::Red;
    assert(mesh.color() == Color::Red);
    assert(meshv2::mesh::isColorAvailableOn(mesh));
    assert(meshv2::comp::isColorAvailableOn(mesh));

    // 4. Setting Vertex and Face Colors
    assert((!mesh.isEnabled<meshv2::ElemId::VERTEX, meshv2::CompId::COLOR>()));
    assert(!meshv2::comp::isColorAvailableOn(mesh.vertex(0)));
    mesh.enable<meshv2::ElemId::VERTEX, meshv2::CompId::COLOR>();
    assert((mesh.isEnabled<meshv2::ElemId::VERTEX, meshv2::CompId::COLOR>()));
    assert(meshv2::comp::isColorAvailableOn(mesh.vertex(0)));

    mesh.vertex(0).color() = Color::Blue;
    mesh.vertex(1).color() = Color::Green;
    mesh.vertex(2).color() = Color::Yellow;
    mesh.vertex(3).color() = Color::Cyan;
    assert(mesh.vertex(0).color() == Color::Blue);
    assert(mesh.vertex(1).color() == Color::Green);

    assert((!mesh.isEnabled<meshv2::ElemId::FACE, meshv2::CompId::COLOR>()));
    assert(!meshv2::comp::isColorAvailableOn(mesh.face(0)));
    mesh.enable<meshv2::ElemId::FACE, meshv2::CompId::COLOR>();
    assert((mesh.isEnabled<meshv2::ElemId::FACE, meshv2::CompId::COLOR>()));
    assert(meshv2::comp::isColorAvailableOn(mesh.face(0)));

    mesh.face(0).color() = Color::DarkRed;
    mesh.face(1).color() = Color::DarkBlue;
    assert(mesh.face(0).color() == Color::DarkRed);
    assert(mesh.face(1).color() == Color::DarkBlue);

    std::cout << "--- MyMesh Created ---\n";
    std::cout << "Vertices: " << mesh.vertexCount() << ", Faces: " << mesh.faceCount() << "\n";
    std::cout << "Mesh color: (" << (int)mesh.color().red() << ", "
              << (int)mesh.color().green() << ", " << (int)mesh.color().blue() << ", "
              << (int)mesh.color().alpha() << ")\n";
    std::cout << "Vertex 0 color: (" << (int)mesh.vertex(0).color().red() << ", "
              << (int)mesh.vertex(0).color().green() << ", " << (int)mesh.vertex(0).color().blue() << ")\n";
    std::cout << "Face 0 color: (" << (int)mesh.face(0).color().red() << ", "
              << (int)mesh.face(0).color().green() << ", " << (int)mesh.face(0).color().blue() << ")\n\n";

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
    for (MyMesh::Vertex v : mesh.face(1).vertices()) {
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
    // ElementContainerProxy & ComponentView Tests
    // =========================================================================

    std::cout << "\n--- ElementContainerProxy & ComponentView Tests ---\n";

    // 1. Direct indexing on ElementContainerProxy
    assert(mesh.vertices().size() == 4);
    assert(mesh.vertices()[0].position() == Point3f(0.0f, 0.0f, 1.0f));
    assert(mesh.faces().size() == 2);
    assert(mesh.faces()[0].vertexIndex(0) == 0);

    // 2. Positions ComponentView
    auto posView = mesh.vertices().positions();
    assert(posView.size() == 4);
    assert(posView.isEnabled());
    assert(posView[0] == Point3f(0.0f, 0.0f, 1.0f));
    assert(posView.data() != nullptr);
    assert(posView.data()[0] == Point3f(0.0f, 0.0f, 1.0f));

    // Test mutation via positions operator[]
    posView[0] = Point3f(0.1f, 0.2f, 1.3f);
    assert(mesh.vertex(0).position() == Point3f(0.1f, 0.2f, 1.3f));
    posView[0] = Point3f(0.0f, 0.0f, 1.0f); // restore

    // Iterate directly over positions
    std::size_t posCount = 0;
    for (Point3f& p : mesh.vertices().positions()) {
        static_assert(!std::is_const_v<std::remove_reference_t<decltype(p)>>);
        std::cout << "  Iterated position: " << p << "\n";
        ++posCount;
    }
    assert(posCount == 4);

    // 3. Normals & Colors ComponentView
    auto normView = mesh.vertices().normals();
    assert(normView.size() == 4);
    assert(normView[0] == Point3f(0.0f, 0.0f, 1.0f));

    auto colView = mesh.vertices().colors();
    assert(colView.size() == 4);
    assert(colView[0] == Color::Blue);
    assert(colView[1] == Color::Green);

    // Face colors and vertex references
    auto faceColView = mesh.faces().colors();
    assert(faceColView.size() == 2);
    assert(faceColView[0] == Color::DarkRed);

    auto faceIndicesView = mesh.faces().vertexIndices();
    assert(faceIndicesView.size() == 2);
    assert(faceIndicesView[0][0] == 0 && faceIndicesView[0][1] == 1 && faceIndicesView[0][2] == 2);
    assert(faceIndicesView.data() != nullptr);

    // 4. Compile-time constraints on ElementContainerProxy
    static_assert(!CanCallBitFlags<decltype(mesh.vertices())>);
    static_assert(!CanCallPositions<decltype(mesh.faces())>);
    static_assert(!CanCallNormals<decltype(SimpleMesh{}.vertices())>);
    static_assert(CanCallPositions<decltype(mesh.vertices())>);
    static_assert(CanCallNormals<decltype(mesh.vertices())>);
    static_assert(CanCallVertexIndices<decltype(mesh.faces())>);
    static_assert(!CanCallVertexIndices<decltype(PolyMesh{}.faces())>);

    std::cout << "ElementContainerProxy and ComponentView verified successfully!\n";

    // =========================================================================
    // Const Correctness
    // =========================================================================

    std::cout << "\n--- Const Correctness Test ---\n";
    const MyMesh& constMesh = mesh;

    // Mesh color const correctness
    assert(constMesh.color() == Color::Red);
    static_assert(std::is_same_v<decltype(constMesh.color()), const vcl::Color&>);

    // Vertex & Face color const correctness
    assert(constMesh.vertex(0).color() == Color::Blue);
    static_assert(std::is_same_v<decltype(constMesh.vertex(0).color()), const vcl::Color&>);
    assert(constMesh.face(0).color() == Color::DarkRed);
    static_assert(std::is_same_v<decltype(constMesh.face(0).color()), const vcl::Color&>);

    // Const ComponentView tests
    auto constPosView = constMesh.vertices().positions();
    assert(constPosView.size() == 4);
    assert(constPosView[0] == Point3f(0.0f, 0.0f, 1.0f));
    static_assert(std::is_same_v<decltype(constPosView[0]), const Point3f&>);
    static_assert(std::is_same_v<decltype(constPosView.data()), const Point3f*>);
    for (const Point3f& p : constMesh.vertices().positions()) {
        static_assert(std::is_const_v<std::remove_reference_t<decltype(p)>>);
    }

    // Access returns ConstVertex / ConstFace proxies
    MyMesh::ConstVertex cv0 = constMesh.vertex(0);
    std::cout << "Const Vertex 0 pos: " << cv0.position() << "\n";
    // cv0.position() = Point3f(0.0f, 0.0f, 0.0f); // Compile error as expected!

    for (MyMesh::ConstVertex v : constMesh.face(0).vertices()) {
        std::cout << "  Const face vertex: " << v.index()
                  << " pos: " << v.position() << "\n";
    }

    // Conversion from mutable Vertex to ConstVertex
    MyMesh::ConstVertex cvFromMutable = mesh.vertex(2);
    std::cout << "ConstVertex converted from mutable Vertex: index "
              << cvFromMutable.index() << "\n";

    // Test vertexIndices() view (mutable vs const)
    auto f0 = mesh.face(0);
    f0.vertexIndices()[1] = 1; // Mutate via view operator[]
    for (auto& vi : f0.vertexIndices()) {
        static_assert(!std::is_const_v<std::remove_reference_t<decltype(vi)>>);
        vi = vi; // Mutate via reference in range-for
    }
    std::cout << "VertexIndices view (const face):\n";
    for (const auto& vi : constMesh.face(0).vertexIndices()) {
        static_assert(std::is_const_v<std::remove_reference_t<decltype(vi)>>);
        std::cout << "  Const face vertexIndex via view: " << vi << "\n";
    }
    std::cout << "\n";

    // =========================================================================
    // VertexReferences Component API Tests
    // =========================================================================
    std::cout << "--- VertexReferences Component API Tests ---\n";
    auto testF = mesh.face(0);
    // testF indices are (0, 1, 2)
    assert(testF.vertexIndexMod(0) == 0);
    assert(testF.vertexIndexMod(3) == 0);
    assert(testF.vertexIndexMod(-1) == 2);
    assert(testF.vertexIndexMod(-3) == 0);

    assert(testF.containsVertex(0));
    assert(testF.containsVertex(1));
    assert(!testF.containsVertex(3));
    assert(testF.containsVertex(mesh.vertex(2)));

    assert(testF.indexOfVertex(0) == 0);
    assert(testF.indexOfVertex(1) == 1);
    assert(testF.indexOfVertex(2) == 2);
    assert(testF.indexOfVertex(3) == UINT_NULL);

    // Edge (0, 1) -> edge 0
    // Edge (1, 2) -> edge 1
    // Edge (2, 0) -> edge 2
    assert(testF.indexOfEdge(0, 1) == 0);
    assert(testF.indexOfEdge(1, 0) == 0);
    assert(testF.indexOfEdge(1, 2) == 1);
    assert(testF.indexOfEdge(2, 1) == 1);
    assert(testF.indexOfEdge(2, 0) == 2);
    assert(testF.indexOfEdge(0, 2) == 2);
    assert(testF.indexOfEdge(0, 3) == UINT_NULL);

    // Test setVertexMod
    testF.setVertexMod(-1, 2);

    // Test setVertices range
    std::array<uint, 3> newIdxs = {0, 1, 2};
    testF.setVertices(newIdxs);
    assert(testF.vertexIndex(0) == 0 && testF.vertexIndex(1) == 1 && testF.vertexIndex(2) == 2);

    // Test setVertices variadic
    testF.setVertices(0, 1, 2);
    assert(testF.vertexIndex(0) == 0 && testF.vertexIndex(1) == 1 && testF.vertexIndex(2) == 2);
    std::cout << "All VertexReferences methods verified successfully!\n\n";

    // =========================================================================
    // Element Validity
    // =========================================================================

    std::cout << "--- Element Validity Test ---\n";
    MyMesh::Vertex validV = mesh.vertex(0);
    MyMesh::Vertex invalidV;

    std::cout << "validV isValid: " << (validV ? "valid" : "invalid") << "\n";
    std::cout << "invalidV isValid: " << (invalidV ? "valid" : "invalid") << "\n";

    // =========================================================================
    // Copy, Move, Swap Tests
    // =========================================================================

    std::cout << "\n--- Mesh Copy & Swap Tests ---\n";
    MyMesh meshCopy = mesh;
    assert(meshCopy.color() == Color::Red);
    std::cout << "Copied mesh face 0 vertex 0 pos: "
              << meshCopy.face(0).vertex(0).position() << "\n";

    MyMesh meshMoved = std::move(meshCopy);
    assert(meshMoved.color() == Color::Red);
    std::cout << "Moved mesh face 0 vertex 0 pos: "
              << meshMoved.face(0).vertex(0).position() << "\n";

    MyMesh meshSwapped;
    swap(meshMoved, meshSwapped);
    assert(meshSwapped.color() == Color::Red);
    std::cout << "Swapped mesh vertices: " << meshSwapped.vertexCount() << "\n";

    // Copy & Swap assignment tests
    MyMesh meshAssigned;
    meshAssigned = mesh; // Copy assignment
    assert(meshAssigned.color() == Color::Red);
    std::cout << "Copy-assigned mesh vertices: " << meshAssigned.vertexCount() << "\n";

    MyMesh meshMoveAssigned;
    meshMoveAssigned = std::move(meshAssigned); // Move assignment
    assert(meshMoveAssigned.color() == Color::Red);
    std::cout << "Move-assigned mesh vertices: " << meshMoveAssigned.vertexCount() << "\n";

    // =========================================================================
    // Deletion and Automatic Skipping
    // =========================================================================

    std::cout << "\n--- Deletion and Skip Test ---\n";
    mesh.deleteVertex(1);

    std::cout << "Marked vertex 1 as deleted.\n";
    std::cout << "  Physical container size: " << mesh.vertexContainerSize() << "\n";
    std::cout << "  Active (non-deleted) count: " << mesh.vertexCount() << "\n";
    std::cout << "  Deleted count: " << mesh.deletedVertexCount() << "\n";
    std::cout << "  isCompact(): " << (mesh.isCompact() ? "yes" : "no") << "\n";

    std::cout << "Iterating over active vertices (vertex 1 automatically skipped):\n";
    for (auto v : mesh.vertices()) {
        std::cout << "  Visible vertex: " << v.index() << " pos = " << v.position() << "\n";
    }

    std::cout << "Iterating over ALL vertices (including deleted, jumpDeleted = false):\n";
    for (auto v : mesh.vertices(false)) {
        std::cout << "  Vertex " << v.index()
                  << " (deleted: " << (v.deleted() ? "yes" : "no") << ")\n";
    }

    // Test ComponentView jumping deleted elements
    assert(mesh.vertices().positions().size() == 3);
    assert(mesh.vertices(false).positions().size() == 4);
    std::size_t activePosCount = 0;
    for (auto& p : mesh.vertices().positions()) {
        std::cout << "  Active vertex position via view: " << p << "\n";
        ++activePosCount;
    }
    assert(activePosCount == 3);

    std::size_t allPosCount = 0;
    for (auto& p : mesh.vertices(false).positions()) {
        ++allPosCount;
    }
    assert(allPosCount == 4);

    // =========================================================================
    // Mesh Compaction & Reference Remapping
    // =========================================================================

    std::cout << "\n--- Mesh Compaction Test ---\n";
    std::cout << "Before compact():\n";
    std::cout << "  isCompact(): " << (mesh.isCompact() ? "yes" : "no") << "\n";
    std::cout << "  Vertex container size: " << mesh.vertexContainerSize()
              << ", active: " << mesh.vertexCount() << "\n";
    std::cout << "  Face 0 vertex indices: (" << mesh.face(0).vertexIndex(0) << ", "
              << mesh.face(0).vertexIndex(1) << ", " << mesh.face(0).vertexIndex(2) << ")\n";
    std::cout << "  Face 1 vertex indices: (" << mesh.face(1).vertexIndex(0) << ", "
              << mesh.face(1).vertexIndex(1) << ", " << mesh.face(1).vertexIndex(2) << ")\n";

    mesh.compact();

    using VertexIndex = MyMesh::Vertex::IndexType;
    auto formatVIdx = [](VertexIndex idx) {
        return idx == std::numeric_limits<VertexIndex>::max() ? "NULL" : std::to_string(idx);
    };

    std::cout << "After compact():\n";
    std::cout << "  isCompact(): " << (mesh.isCompact() ? "yes" : "no") << "\n";
    std::cout << "  Vertex container size: " << mesh.vertexContainerSize()
              << ", active: " << mesh.vertexCount() << "\n";
    std::cout << "  Face 0 vertex indices: (" << formatVIdx(mesh.face(0).vertexIndex(0)) << ", "
              << formatVIdx(mesh.face(0).vertexIndex(1)) << ", "
              << formatVIdx(mesh.face(0).vertexIndex(2)) << ")\n";
    std::cout << "  Face 1 vertex indices: (" << formatVIdx(mesh.face(1).vertexIndex(0)) << ", "
              << formatVIdx(mesh.face(1).vertexIndex(1)) << ", "
              << formatVIdx(mesh.face(1).vertexIndex(2)) << ")\n";

    std::cout << "  Face 0 corner 0 pos: " << mesh.face(0).vertex(0).position() << "\n";
    std::cout << "  Face 0 corner 1 isValid: " << (mesh.face(0).vertex(1).isValid() ? "valid" : "invalid (deleted)") << "\n";
    std::cout << "  Face 0 corner 2 pos: " << mesh.face(0).vertex(2).position() << "\n";

    // Test face deletion and compactFaces
    mesh.deleteFace(0);
    std::cout << "Marked face 0 as deleted:\n";
    std::cout << "  isCompact(): " << (mesh.isCompact() ? "yes" : "no") << "\n";
    std::cout << "  Face count: " << mesh.faceCount() << ", container size: " << mesh.faceContainerSize() << "\n";

    mesh.compactFaces();
    std::cout << "After compactFaces():\n";
    std::cout << "  isCompact(): " << (mesh.isCompact() ? "yes" : "no") << "\n";
    std::cout << "  Face count: " << mesh.faceCount() << ", container size: " << mesh.faceContainerSize() << "\n";
    std::cout << "  Remaining face 0 indices: ("
              << formatVIdx(mesh.face(0).vertexIndex(0)) << ", "
              << formatVIdx(mesh.face(0).vertexIndex(1)) << ", "
              << formatVIdx(mesh.face(0).vertexIndex(2)) << ")\n";

    // =========================================================================
    // enableSameOptionalComponentsOf Tests
    // =========================================================================

    std::cout << "\n--- enableSameOptionalComponentsOf Tests ---\n";
    MyMesh meshWithNormals;
    meshWithNormals.enable<meshv2::ElemId::VERTEX, meshv2::CompId::NORMAL>();
    assert((meshWithNormals.isEnabled<meshv2::ElemId::VERTEX, meshv2::CompId::NORMAL>()));

    MyMesh meshWithoutNormals;
    assert((!meshWithoutNormals.isEnabled<meshv2::ElemId::VERTEX, meshv2::CompId::NORMAL>()));

    // 1. Mesh-level enableSameOptionalComponentsOf
    meshWithoutNormals.enableSameOptionalComponentsOf(meshWithNormals);
    assert((meshWithoutNormals.isEnabled<meshv2::ElemId::VERTEX, meshv2::CompId::NORMAL>()));
    std::cout << "Successfully enabled Normal component on mesh matching other mesh!\n";

    // 2. Test with a mesh that does not have the component (remains disabled)
    SimpleMesh simpleMesh;
    MyMesh meshTestedAgainstSimple;
    assert((!meshTestedAgainstSimple.isEnabled<meshv2::ElemId::VERTEX, meshv2::CompId::NORMAL>()));
    meshTestedAgainstSimple.enableSameOptionalComponentsOf(simpleMesh);
    assert((!meshTestedAgainstSimple.isEnabled<meshv2::ElemId::VERTEX, meshv2::CompId::NORMAL>()));
    std::cout << "Verified that components absent in source mesh remain disabled!\n";

    // =========================================================================
    // addVertex / addVertices & addFace / addFaces Tests
    // =========================================================================

    std::cout << "\n--- addVertex / addVertices & addFace / addFaces Tests ---\n";
    MyMesh addMesh;

    // 1. addVertex() and addVertex(pos)
    uint v0 = addMesh.addVertex();
    addMesh.vertex(v0).position() = Point3f(0.0f, 0.0f, 0.0f);
    assert(addMesh.vertexCount() == 1);
    assert(addMesh.vertex(v0).position() == Point3f(0.0f, 0.0f, 0.0f));

    uint v1 = addMesh.addVertex(Point3f(1.0f, 0.0f, 0.0f));
    assert(addMesh.vertexCount() == 2);
    assert(addMesh.vertex(v1).position() == Point3f(1.0f, 0.0f, 0.0f));

    // 2. addVertices(count)
    uint baseV = addMesh.addVertices(2);
    assert(baseV == 2);
    assert(addMesh.vertexCount() == 4);
    addMesh.vertex(baseV).position() = Point3f(0.0f, 1.0f, 0.0f);
    addMesh.vertex(baseV + 1).position() = Point3f(1.0f, 1.0f, 0.0f);

    // 3. addVertices(variadic positions)
    uint baseVVar = addMesh.addVertices(Point3f(2.0f, 0.0f, 0.0f), Point3f(2.0f, 1.0f, 0.0f));
    assert(baseVVar == 4);
    assert(addMesh.vertexCount() == 6);
    assert(addMesh.vertex(4).position() == Point3f(2.0f, 0.0f, 0.0f));
    assert(addMesh.vertex(5).position() == Point3f(2.0f, 1.0f, 0.0f));

    // 4. addVertices(range of positions)
    std::vector<Point3f> newPos = {Point3f(3.0f, 0.0f, 0.0f), Point3f(3.0f, 1.0f, 0.0f)};
    uint baseVRng = addMesh.addVertices(newPos);
    assert(baseVRng == 6);
    assert(addMesh.vertexCount() == 8);
    assert(addMesh.vertex(6).position() == Point3f(3.0f, 0.0f, 0.0f));
    assert(addMesh.vertex(7).position() == Point3f(3.0f, 1.0f, 0.0f));

    // 5. addFace() and addFace(indices)
    uint addF0 = addMesh.addFace();
    addMesh.face(addF0).setVertices(0, 1, 2);
    assert(addMesh.faceCount() == 1);
    assert(addMesh.face(addF0).vertexIndex(0) == 0 && addMesh.face(addF0).vertexIndex(1) == 1 && addMesh.face(addF0).vertexIndex(2) == 2);

    uint addF1 = addMesh.addFace(1, 3, 2);
    assert(addMesh.faceCount() == 2);
    assert(addMesh.face(addF1).vertexIndex(0) == 1 && addMesh.face(addF1).vertexIndex(1) == 3 && addMesh.face(addF1).vertexIndex(2) == 2);

    // 6. addFace(vertex proxies)
    uint addF2 = addMesh.addFace(addMesh.vertex(0), addMesh.vertex(1), addMesh.vertex(3));
    assert(addMesh.faceCount() == 3);
    assert(addMesh.face(addF2).vertexIndex(0) == 0 && addMesh.face(addF2).vertexIndex(1) == 1 && addMesh.face(addF2).vertexIndex(2) == 3);

    // 7. addFace(range of indices)
    std::vector<uint> faceIndices = {4, 5, 2};
    uint addF3 = addMesh.addFace(faceIndices);
    assert(addMesh.faceCount() == 4);
    assert(addMesh.face(addF3).vertexIndex(0) == 4 && addMesh.face(addF3).vertexIndex(1) == 5 && addMesh.face(addF3).vertexIndex(2) == 2);

    // 8. addFaces(count)
    uint baseF = addMesh.addFaces(2);
    assert(baseF == 4);
    assert(addMesh.faceCount() == 6);

    // 9. addFaces(range of ranges)
    std::vector<std::vector<uint>> multiFaces = {{6, 7, 4}, {7, 5, 4}};
    uint baseFMulti = addMesh.addFaces(multiFaces);
    assert(baseFMulti == 6);
    assert(addMesh.faceCount() == 8);
    assert(addMesh.face(6).vertexIndex(0) == 6 && addMesh.face(6).vertexIndex(1) == 7 && addMesh.face(6).vertexIndex(2) == 4);
    assert(addMesh.face(7).vertexIndex(0) == 7 && addMesh.face(7).vertexIndex(1) == 5 && addMesh.face(7).vertexIndex(2) == 4);

    // 11. Polygon mesh tests: addFace with arbitrary number of vertices
    PolyMesh polyMesh;
    polyMesh.addVertices(Point3f(0.0f, 0.0f, 0.0f), Point3f(1.0f, 0.0f, 0.0f),
                         Point3f(1.0f, 1.0f, 0.0f), Point3f(0.0f, 1.0f, 0.0f),
                         Point3f(0.5f, 1.5f, 0.0f));
    assert(polyMesh.vertexCount() == 5);

    // Quad face
    uint polyF0 = polyMesh.addFace(0, 1, 2, 3);
    assert(polyMesh.face(polyF0).vertexCount() == 4);
    assert(polyMesh.face(polyF0).vertexIndex(0) == 0);
    assert(polyMesh.face(polyF0).vertexIndex(3) == 3);

    // Pentagon face from range
    std::vector<uint> pentagon = {0, 1, 2, 4, 3};
    uint polyF1 = polyMesh.addFace(pentagon);
    assert(polyMesh.face(polyF1).vertexCount() == 5);
    assert(polyMesh.face(polyF1).vertexIndex(3) == 4);

    std::cout << "addVertex, addVertices, addFace, addFaces verified successfully!\n";

    // --- Phase 1: Component-level importFrom Tests ---
    std::cout << "\n--- Component-level importFrom Tests ---\n";
    {
        // 1. Position importFrom (same type and cross scalar cast)
        using MeshDouble = vcl::meshv2::Mesh<
            vcl::meshv2::VertexContainer<vcl::TypeWrapper<
                vcl::meshv2::comp::Position3d,
                vcl::meshv2::comp::BitFlags
            >>,
            vcl::meshv2::FaceContainer<vcl::TypeWrapper<
                vcl::meshv2::comp::TriangleBitFlags,
                vcl::meshv2::comp::VertexReferences<3>
            >>
        >;
        MeshDouble mDouble;
        mDouble.addVertex(Point3d(1.5, 2.5, 3.5));

        MyMesh mFloat;
        mFloat.addVertex(Point3f(0.0f, 0.0f, 0.0f));

        // Call Position::importFrom directly through element proxy:
        mFloat.vertex(0).vcl::meshv2::comp::Position3f::importFrom(mDouble.vertex(0));
        assert(mFloat.vertex(0).position() == Point3f(1.5f, 2.5f, 3.5f));

        // 2. Normal importFrom
        mFloat.enableAll();
        // importing from element without Normal should do nothing
        mFloat.vertex(0).normal() = Point3f(0, 1, 0);
        mFloat.vertex(0).vcl::meshv2::comp::Normal3f::importFrom(mDouble.vertex(0));
        assert(mFloat.vertex(0).normal() == Point3f(0, 1, 0));

        // import from element with Normal
        MyMesh mSource;
        mSource.enableAll();
        mSource.addVertex(Point3f(0, 0, 0));
        mSource.vertex(0).normal() = Point3f(1, 0, 0);
        mFloat.vertex(0).vcl::meshv2::comp::Normal3f::importFrom(mSource.vertex(0));
        assert(mFloat.vertex(0).normal() == Point3f(1, 0, 0));

        // 3. Color importFrom
        mSource.vertex(0).color() = vcl::Color::Green;
        mFloat.vertex(0).vcl::meshv2::comp::Color::importFrom(mSource.vertex(0));
        assert(mFloat.vertex(0).color() == vcl::Color::Green);

        // 4. BitFlags importFrom
        mSource.vertex(0).selected() = true;
        mSource.vertex(0).visited() = true;
        mSource.vertex(0).onBorder() = true;
        mSource.vertex(0).userBit(3) = true;
        mFloat.vertex(0).vcl::meshv2::comp::BitFlags::importFrom(mSource.vertex(0));
        assert(mFloat.vertex(0).selected() == true);
        assert(mFloat.vertex(0).visited() == true);
        assert(mFloat.vertex(0).onBorder() == true);
        assert(mFloat.vertex(0).userBit(3) == true);
        assert(mFloat.vertex(0).userBit(2) == false);

        // 5. TriangleBitFlags & PolygonBitFlags importFrom
        using TriBitMesh = vcl::meshv2::Mesh<
            vcl::meshv2::VertexContainer<vcl::TypeWrapper<
                vcl::meshv2::comp::Position3f,
                vcl::meshv2::comp::BitFlags
            >>,
            vcl::meshv2::FaceContainer<vcl::TypeWrapper<
                vcl::meshv2::comp::TriangleBitFlags,
                vcl::meshv2::comp::VertexReferences<3>
            >>
        >;
        TriBitMesh mSourceTri;
        mSourceTri.addVertices(3);
        mSourceTri.addFace(0, 1, 2);
        mSourceTri.face(0).edgeOnBorder(1) = true;
        mSourceTri.face(0).edgeSelected(2) = true;
        mSourceTri.face(0).edgeFaux(0) = true;

        polyMesh.face(0).vcl::meshv2::comp::PolygonBitFlags<>::importFrom(mSourceTri.face(0));
        assert(polyMesh.face(0).edgeOnBorder(1) == true);
        assert(polyMesh.face(0).edgeSelected(2) == true);
        assert(polyMesh.face(0).edgeFaux(0) == true);

        // PolygonBitFlags back to TriangleBitFlags
        TriBitMesh mDestTri;
        mDestTri.addVertices(3);
        mDestTri.addFace(0, 1, 2);
        mDestTri.face(0).vcl::meshv2::comp::TriangleBitFlags::importFrom(polyMesh.face(0));
        assert(mDestTri.face(0).edgeOnBorder(1) == true);
        assert(mDestTri.face(0).edgeSelected(2) == true);
        assert(mDestTri.face(0).edgeFaux(0) == true);
        assert(mDestTri.face(0).edgeOnBorder(0) == false);

        // Test VertexReferences::importFrom
        // 1. Fixed Tri to Tri
        TriBitMesh mTriA;
        mTriA.addVertices(4);
        mTriA.addFace(0, 1, 2);
        TriBitMesh mTriB;
        mTriB.addVertices(4);
        mTriB.addFace(3, 2, 1);

        mTriA.face(0).vcl::meshv2::comp::VertexReferences<3>::importFrom(mTriB.face(0));
        assert(mTriA.face(0).vertexIndex(0) == 3);
        assert(mTriA.face(0).vertexIndex(1) == 2);
        assert(mTriA.face(0).vertexIndex(2) == 1);

        // 2. importRefs = false: does nothing
        mTriA.face(0).vcl::meshv2::comp::VertexReferences<3>::importFrom(mSourceTri.face(0), false);
        assert(mTriA.face(0).vertexIndex(0) == 3); // unchanged

        // 3. Tri to Poly
        polyMesh.face(0).vcl::meshv2::comp::VertexReferences<-1>::importFrom(mTriB.face(0));
        assert(polyMesh.face(0).vertexCount() == 3);
        assert(polyMesh.face(0).vertexIndex(0) == 3);
        assert(polyMesh.face(0).vertexIndex(1) == 2);
        assert(polyMesh.face(0).vertexIndex(2) == 1);

        // 4. Poly (3 vertices) to Tri
        mTriA.face(0).setVertices(99, 99, 99);
        mTriA.face(0).vcl::meshv2::comp::VertexReferences<3>::importFrom(polyMesh.face(0));
        assert(mTriA.face(0).vertexIndex(0) == 3);
        assert(mTriA.face(0).vertexIndex(1) == 2);
        assert(mTriA.face(0).vertexIndex(2) == 1);

        // 5. Poly (4 vertices) to Tri (should not import, size mismatch)
        polyMesh.face(0).resizeVertices(4);
        polyMesh.face(0).setVertices(4, 5, 6, 7);
        mTriA.face(0).vcl::meshv2::comp::VertexReferences<3>::importFrom(polyMesh.face(0));
        // mTriA remains 3, 2, 1
        assert(mTriA.face(0).vertexIndex(0) == 3);
        assert(mTriA.face(0).vertexIndex(1) == 2);
        assert(mTriA.face(0).vertexIndex(2) == 1);

        std::cout << "Component-level importFrom verified successfully!\n";
    }

    std::cout << "\nAll mesh tests passed successfully!\n";
    return 0;
}
