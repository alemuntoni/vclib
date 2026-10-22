// VCLib - Visual Computing Library
// Copyright (C) 2021-2026 Visual Computing Lab, ISTI - CNR.
//
// This Source Code Form is subject to the terms of the Mozilla Public License,
// v. 2.0. If a copy of the MPL was not distributed with this file, You can
// obtain one at https://mozilla.org/MPL/2.0/.

#ifndef VCL_MESH_V2_COMPONENTS_BASE_CONTAINER_COMPONENT_H
#define VCL_MESH_V2_COMPONENTS_BASE_CONTAINER_COMPONENT_H

#include <vclib/mesh_v2/components/base/component.h>
#include <vclib/space/core/vector/vector.h>

namespace vcl::meshv2 {

/**
 * @brief Base class for all components that store a collection of elements
 * (statically sized with std::array if N >= 0, or dynamically with std::vector
 * if N < 0).
 *
 * It wraps vcl::Vector<T, N> and provides type information and access helpers
 * for derived components, without polluting the ElementProxy interface with
 * generic methods that would cause collisions in multiple inheritance.
 *
 * @tparam COMP_ID: The unique identifier of the component.
 * @tparam T: The type of the elements stored in the container.
 * @tparam N: The size of the container. If N >= 0, static size. If N < 0,
 * dynamic size.
 * @tparam TTVC: True if the component's container size is tied to the vertex
 * count of the parent element.
 * @tparam Container: The container type to use. Defaults to vcl::Vector<T, N>.
 */
template<
    uint COMP_ID,
    typename T,
    int  N,
    bool TTVC,
    typename Container = vcl::Vector<T, N>>
class ContainerComponent : public Component<COMP_ID, Container>
{
    using Base = Component<COMP_ID, Container>;

protected:
    using ValueType                            = T;
    using ContainerType                        = Container;
    static constexpr int  SIZE                 = N;
    static constexpr bool TIED_TO_VERTEX_COUNT = TTVC;

    static decltype(auto) container(auto&& self)
    {
        return Base::compData(self);
    }

    void resize(this auto&& self, uint n) requires (N < 0)
    {
        container(self).resize(n);
    }

    void pushBack(this auto&& self, const T& v = T()) requires (N < 0)
    {
        container(self).pushBack(v);
    }

    void insert(this auto&& self, uint pos, const T& v = T()) requires (N < 0)
    {
        container(self).insert(pos, v);
    }

    void erase(this auto&& self, uint pos) requires (N < 0)
    {
        container(self).erase(pos);
    }

    void clear(this auto&& self) requires (N < 0) { container(self).clear(); }
};

} // namespace vcl::meshv2

#endif // VCL_MESH_V2_COMPONENTS_BASE_CONTAINER_COMPONENT_H
