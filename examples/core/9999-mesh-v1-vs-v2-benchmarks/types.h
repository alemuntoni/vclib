// VCLib - Visual Computing Library
// Copyright (C) 2021-2026 Visual Computing Lab, ISTI - CNR.
//
// This Source Code Form is subject to the terms of the Mozilla Public License,
// v. 2.0. If a copy of the MPL was not distributed with this file, You can
// obtain one at https://mozilla.org/MPL/2.0/.

#ifndef BENCH_TYPES_H
#define BENCH_TYPES_H

#include <vclib/meshes.h>

#include <vclib/mesh_v2/components/bit_flags.h>
#include <vclib/mesh_v2/components/normal.h>
#include <vclib/mesh_v2/components/position.h>
#include <vclib/mesh_v2/components/vertex_references.h>
#include <vclib/mesh_v2/containers/face_container.h>
#include <vclib/mesh_v2/containers/vertex_container.h>
#include <vclib/mesh_v2/mesh.h>

namespace bench {

// V1 Mesh: standard vclib TriMesh (double precision)
using V1Mesh = vcl::TriMesh;

// V2 Mesh: vclib mesh_v2 (double precision, equivalent core components)
using V2VertexComponents = vcl::meshv2::VertexComponents<
    vcl::meshv2::vert::BitFlags,
    vcl::meshv2::vert::Position3d,
    vcl::meshv2::vert::Normal3d>;

using V2FaceComponents = vcl::meshv2::FaceComponents<
    vcl::meshv2::face::BitFlags,
    vcl::meshv2::face::TriangleVertexReferences<vcl::uint>,
    vcl::meshv2::face::Normal3d>;

using V2Mesh = vcl::meshv2::Mesh<
    vcl::meshv2::VertexContainer<V2VertexComponents>,
    vcl::meshv2::FaceContainer<V2FaceComponents>>;

} // namespace bench

#endif // BENCH_TYPES_H
