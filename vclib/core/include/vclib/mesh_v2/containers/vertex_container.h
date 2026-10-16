// VCLib - Visual Computing Library
// Copyright (C) 2021-2026 Visual Computing Lab, ISTI - CNR.
//
// This Source Code Form is subject to the terms of the Mozilla Public License,
// v. 2.0. If a copy of the MPL was not distributed with this file, You can
// obtain one at https://mozilla.org/MPL/2.0/.

#ifndef VCL_MESH_V2_CONTAINERS_VERTEX_CONTAINER_H
#define VCL_MESH_V2_CONTAINERS_VERTEX_CONTAINER_H

#include <vclib/base/views/view.h>
#include <vclib/mesh_v2/components/base/component_predicates.h>
#include <vclib/mesh_v2/containers/base/element_container.h>
#include <vclib/mesh_v2/elements/vertex.h>

namespace vcl::meshv2 {

/**
 * @brief VertexContainer stores the per-vertex data in a Structure of Arrays (SoA).
 *
 * It accepts the Vertex component list (satisfying VertexComponentsConcept),
 * instantiates the underlying storage and exposes the Vertex element view.
 *
 * @tparam CompList The vertex components type (e.g. VertexComponents<BitFlags, Position3f, Normal3f>).
 * @tparam Index The index type (default uint).
 * @tparam ParentMesh The parent mesh type (default void).
 *
 * @ingroup containers
 */
template<
    HasRequiredVertexComponents CompList,
    std::unsigned_integral Index = uint,
    typename ParentMesh          = void>
class VertexContainer :
    protected ElementContainer<
        typename comp::detail::ComponentListHelper<CompList>::type,
        ParentMesh,
        Index,
        Vertex>
{
    using Base = ElementContainer<
        typename comp::detail::ComponentListHelper<CompList>::type,
        ParentMesh,
        Index,
        Vertex>;

public:
    using VertexComponents = CompList;
    using Vertex           = typename Base::ProxyType;
    using ConstVertex      = typename Base::ConstProxyType;
    using VertexType_      = Vertex;
    using ConstVertexType  = ConstVertex;

    using Base::Base;

    Vertex vertex(Index i) { return Base::element(i); }
    ConstVertex vertex(Index i) const { return Base::element(i); }

    std::size_t vertexCount() const { return Base::elementCount(); }
    std::size_t vertexContainerSize() const { return Base::elementContainerSize(); }
    std::size_t deletedVertexCount() const { return Base::deletedElementCount(); }

    void resizeVertices(std::size_t n) { Base::resize(n); }

    auto vertices(bool jumpDeleted = true)
    {
        return Base::elements(jumpDeleted);
    }

    auto vertices(bool jumpDeleted = true) const
    {
        return Base::elements(jumpDeleted);
    }
};

} // namespace vcl::meshv2

#endif // VCL_MESH_V2_CONTAINERS_VERTEX_CONTAINER_H
