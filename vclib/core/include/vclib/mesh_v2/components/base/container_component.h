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
 * @tparam COMP_ID The unique identifier of the component.
 * @tparam T The type of the elements stored in the container.
 * @tparam N The size of the container. If N >= 0, static size. If N < 0,
 * dynamic size.
 * @tparam TTVC True if the component's container size is tied to the vertex
 * count of the parent element.
 */
template<
    uint COMP_ID,
    typename T,
    int  N,
    bool TTVC,
    typename Container = vcl::Vector<T, N>>
struct ContainerComponent : public Component<COMP_ID, Container>
{
    using ValueType                           = T;
    using ContainerType                       = Container;
    static constexpr int  SIZE                = N;
    static constexpr bool IS_DYNAMIC          = (N < 0);
    static constexpr bool TIED_TO_VERTEX_COUNT = TTVC;

protected:
    template<typename Self>
    static decltype(auto) containerData(Self&& self)
    {
        return self.container().template get<COMP_ID>()[self.index()];
    }
};

} // namespace vcl::meshv2

#endif // VCL_MESH_V2_COMPONENTS_BASE_CONTAINER_COMPONENT_H
