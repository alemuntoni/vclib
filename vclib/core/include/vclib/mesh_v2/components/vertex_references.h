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
 * @brief The VertexReferences class represents a component that stores a
 * container of indices or pointers to vertices that will be part of an Element
 * (e.g. Face, Edge, Tetrahedron, etc.).
 *
 * The container of Vertex references is a random access container that could
 * have static or dynamic size, depending on the value of the template argument
 * N (a negative number means dynamic).
 *
 * For example, if a Face element should represent only triangles, N could be
 * 3. In this case, the container will have static size and it can't be resized
 * in any way. If N is -1, the container will have dynamic size and it will be
 * possible to resize it (e.g. if the Face element should represent a polygon).
 *
 * The member functions of this class will be available in the instance of any
 * Element that will contain this component.
 *
 * For example, if you have a Face Element `f` that has the VertexReferences
 * component, you'll be able to access to this component member functions from
 * `f`:
 *
 * @code{.cpp}
 * uint n = f.vertexCount();
 * auto* v = f.vertex(0);
 * uint vi = f.vertexIndex(0);
 * @endcode
 *
 * @note This component is usually the main component of an Element, and
 * therefore is automatically enabled in the Element's container and cannot be
 * disabled.
 *
 * @note Several components are *Tied To Vertex Count*: in other words, they
 * are composed by a container that has the same size of the VertexReferences
 * and, when the VertexReferences container is resized, also the container of
 * these components is resized automatically.
 *
 * @tparam N: The size of the container, that will represent the number of
 * storable vertices. If negative, the container is dynamic.
 * @tparam Index: The unsigned integral type used to store indices (default
 * uint).
 *
 * @ingroup components
 */
template<int N, std::unsigned_integral Index = uint>
class VertexReferences :
        public ContainerReferencesComponent<
            CompId::VERTEX_REFERENCES,
            Index,
            ElemId::VERTEX,
            N,
            false>
{
    using Base = ContainerReferencesComponent<
        CompId::VERTEX_REFERENCES,
        Index,
        ElemId::VERTEX,
        N,
        false>;

public:
    using IndexType                   = Index;
    static constexpr int VERTEX_COUNT = N;

    /* Constructors */

    /**
     * @brief Empty constructor.
     *
     * If the VertexReferences container size is static, initializes all the
     * pointers/indices to `nullptr`/`UINT_NULL`, otherwise the container will
     * be empty.
     */
    VertexReferences() = default;

    /* Member functions */

    uint vertexCount(this auto&& self)
    {
        return Base::containerData(self).size();
    }

    // --- Direct Index Access (works standalone without parent Mesh) ---

    decltype(auto) vertexIndices(this auto&& self)
    {
        return Base::containerData(self);
    }

    IndexType vertexIndex(this auto&& self, uint i)
    {
        assert(i < self.vertexCount());
        return self.vertexIndices()[i];
    }

    void setVertexIndex(this auto&& self, uint i, IndexType vIdx)
    {
        assert(i < self.vertexCount());
        self.vertexIndices()[i] = vIdx;
    }

    void setVertexIndices(
        this auto&& self,
        IndexType   v0,
        IndexType   v1,
        IndexType   v2)
    {
        auto& refs = self.vertexIndices();
        refs[0]    = v0;
        refs[1]    = v1;
        refs[2]    = v2;
    }

    // --- Element Proxy Access (via Parent Mesh) ---

    decltype(auto) vertex(this auto&& self, uint i)
    {
        assert(i < self.vertexCount());
        return self.referencedElement(i);
    }

    decltype(auto) vertices(this auto&& self)
    {
        return self.referencedElements();
    }
};

// Specialization Aliases
template<std::unsigned_integral Index = uint>
using TriangleVertexReferences = VertexReferences<3, Index>;

template<std::unsigned_integral Index = uint>
using PolygonVertexReferences = VertexReferences<-1, Index>;

/**
 * @brief Concept that checks whether a type T (which can be a TypeWrapper of
 * components, an ElementProxy, an ElementContainer, or a Mesh) has the
 * VertexReferences component (COMP_ID = CompId::VERTEX_REFERENCES).
 */
template<typename T>
concept HasVertexReferences = HasComponent<CompId::VERTEX_REFERENCES, T>;

template<typename T>
concept HasTriangleVertexReferences =
    HasComponent<CompId::VERTEX_REFERENCES, T> &&
    (RemoveRef<T>::VERTEX_COUNT == 3);

template<typename T>
concept HasPolygonVertexReferences =
    HasComponent<CompId::VERTEX_REFERENCES, T> &&
    (RemoveRef<T>::VERTEX_COUNT == -1);

} // namespace vcl::meshv2::comp

#endif // VCL_MESH_V2_COMPONENTS_VERTEX_REFERENCES_H
