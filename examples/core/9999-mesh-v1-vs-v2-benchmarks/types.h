// VCLib - Visual Computing Library
// Copyright (C) 2021-2026 Visual Computing Lab, ISTI - CNR.
//
// This Source Code Form is subject to the terms of the Mozilla Public License,
// v. 2.0. If a copy of the MPL was not distributed with this file, You can
// obtain one at https://mozilla.org/MPL/2.0/.

#ifndef BENCH_TYPES_H
#define BENCH_TYPES_H

#include <v1/meshes/tri_mesh.h>
#include <vclib/mesh.h>

namespace bench {

// V1 Mesh: legacy vclib TriMesh (isolated under v1)
using V1Mesh = vcl::v1::TriMesh;

// V2 Mesh: canonical vclib Mesh (double precision, equivalent core components)
using V2VertexComponents = vcl::vert::VertexComponents<
    vcl::vert::BitFlags,
    vcl::vert::Position3d,
    vcl::vert::Normal3d>;

using V2FaceComponents = vcl::face::FaceComponents<
    vcl::face::TriangleBitFlags,
    vcl::face::TriangleVertexReferences<vcl::uint>,
    vcl::face::Normal3d>;

using V2Mesh = vcl::Mesh<
    vcl::mesh::VertexContainer<V2VertexComponents>,
    vcl::mesh::FaceContainer<V2FaceComponents>>;

} // namespace bench

#endif // BENCH_TYPES_H
