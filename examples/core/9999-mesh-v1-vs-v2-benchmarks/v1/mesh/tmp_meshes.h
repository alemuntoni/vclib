// VCLib - Visual Computing Library
// Copyright (C) 2021-2026 Visual Computing Lab, ISTI - CNR.
//
// This Source Code Form is subject to the terms of the Mozilla Public License,
// v. 2.0. If a copy of the MPL was not distributed with this file, You can
// obtain one at https://mozilla.org/MPL/2.0/.

#ifndef VCL_V1_MESH_TMP_MESHES_H
#define VCL_V1_MESH_TMP_MESHES_H

#include <v1/mesh/mesh.h>

namespace vcl::v1::detail {

class TMPSimpleTriMesh;
class TMPSimplePolyMesh;

namespace tmpMesh {

class TriVertex :
        public Vertex<TMPSimpleTriMesh, vert::BitFlags, vert::Position3d>
{
};

class PolyVertex :
        public Vertex<TMPSimplePolyMesh, vert::BitFlags, vert::Position3d>
{
};

class TriFace;
class PolyFace;

class TriFace :
        public Face<
            TMPSimpleTriMesh,
            face::TriangleBitFlags,
            face::TriangleVertexPtrs<TriVertex, TriFace>>
{
};

class PolyFace :
        public Face<
            TMPSimplePolyMesh,
            face::PolygonBitFlags,
            face::PolygonVertexPtrs<PolyVertex, PolyFace>>
{
};

} // namespace tmpMesh

class TMPSimpleTriMesh :
        public Mesh<
            mesh::VertexContainer<tmpMesh::TriVertex>,
            mesh::FaceContainer<tmpMesh::TriFace>>
{
};

class TMPSimplePolyMesh :
        public Mesh<
            mesh::VertexContainer<tmpMesh::PolyVertex>,
            mesh::FaceContainer<tmpMesh::PolyFace>>
{
};

} // namespace vcl::v1::detail

#endif // VCL_V1_MESH_TMP_MESHES_H
