// VCLib - Visual Computing Library
// Copyright (C) 2021-2026 Visual Computing Lab, ISTI - CNR.
//
// This Source Code Form is subject to the terms of the Mozilla Public License,
// v. 2.0. If a copy of the MPL was not distributed with this file, You can
// obtain one at https://mozilla.org/MPL/2.0/.

#ifndef VCL_MESH_V2_COMPONENTS_VERTEX_REFERENCES_H
#define VCL_MESH_V2_COMPONENTS_VERTEX_REFERENCES_H

#include <vclib/mesh_v2/components/base/component_predicates.h>
#include <vclib/mesh_v2/components/base/container_references_component.h>
#include <vclib/mesh_v2/elements/base/base.h>

#include <cassert>
#include <concepts>
#include <ranges>

namespace vcl::meshv2::comp {

/**
 * @brief Component that stores indices to the 3 vertices of a triangular face.
 *
 * It provides both direct access to the stored indices (vertexIndex,
 * vertexIndices) and high-level access to the referenced vertex proxies (vertex,
 * vertices) through the parent mesh.
 *
 * @tparam Index The unsigned integral type used to store indices (default uint).
 */
template<std::unsigned_integral Index = uint>
class TriangleVertexReferences :
        public ContainerReferencesComponent<
            CompId::VERTEX_REFERENCES,
            Index,
            ElemId::VERTEX,
            3,
            false>
{
    using Base = ContainerReferencesComponent<
        CompId::VERTEX_REFERENCES,
        Index,
        ElemId::VERTEX,
        3,
        false>;

public:
    using IndexType                    = Index;
    static constexpr uint VERTEX_COUNT = 3;

    uint vertexCount(this auto&&)
    {
        return VERTEX_COUNT;
    }

    // --- Direct Index Access (works standalone without parent Mesh) ---

    decltype(auto) vertexIndices(this auto&& self)
    {
        return Base::containerData(self);
    }

    IndexType vertexIndex(this auto&& self, uint i)
    {
        assert(i < 3);
        return self.vertexIndices()[i];
    }

    void setVertexIndex(this auto&& self, uint i, IndexType vIdx)
    {
        assert(i < 3);
        self.vertexIndices()[i] = vIdx;
    }

    void setVertexIndices(
        this auto&& self,
        IndexType v0,
        IndexType v1,
        IndexType v2)
    {
        auto& refs = self.vertexIndices();
        refs[0]    = v0;
        refs[1]    = v1;
        refs[2]    = v2;
    }

    // --- Element Proxy Access (via Parent Mesh) ---

    decltype(auto) vertex(this auto&& self, uint i)
    {
        assert(i < 3);
        return self.referencedElement(i);
    }

    decltype(auto) vertices(this auto&& self)
    {
        return self.referencedElements();
    }
};

/**
 * @brief Concept that checks whether a type T (which can be a TypeWrapper of
 * components, an ElementProxy, an ElementContainer, or a Mesh) has the
 * VertexReferences component (COMP_ID = CompId::VERTEX_REFERENCES).
 */
template<typename T>
concept HasVertexReferences = HasComponent<CompId::VERTEX_REFERENCES, T>;

} // namespace vcl::meshv2::comp

#endif // VCL_MESH_V2_COMPONENTS_VERTEX_REFERENCES_H
