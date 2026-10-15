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

#include <cassert>
#include <cstddef>

namespace vcl::meshv2 {

/**
 * @brief Base class for all components that store a collection of elements
 * (statically sized with std::array if N >= 0, or dynamically with std::vector
 * if N < 0).
 *
 * It wraps vcl::Vector<T, N> and provides member functions for the
 * ElementProxy to access the container data.
 *
 * @tparam COMP_ID The unique identifier of the component.
 * @tparam T The type of the elements stored in the container.
 * @tparam N The size of the container. If N >= 0, static size. If N < 0,
 * dynamic size.
 * @tparam TTVC True if the component's container size is tied to the vertex
 * count of the parent element.
 */
template<uint COMP_ID, typename T, int N, bool TTVC>
struct ContainerComponent : public Component<COMP_ID, vcl::Vector<T, N>>
{
    using ValueType                           = T;
    static constexpr int  SIZE                = N;
    static constexpr bool IS_DYNAMIC          = (N < 0);
    static constexpr bool TIED_TO_VERTEX_COUNT = TTVC;

    // Common container access via deducing this
    decltype(auto) size(this auto&& self)
    {
        if constexpr (N >= 0) {
            return static_cast<std::size_t>(N);
        }
        else {
            return self.container().template get<COMP_ID>()[self.index()].size();
        }
    }

    decltype(auto) empty(this auto&& self)
    {
        if constexpr (N >= 0) {
            return N == 0;
        }
        else {
            return self.container().template get<COMP_ID>()[self.index()].empty();
        }
    }

    decltype(auto) operator[](this auto&& self, uint i)
    {
        return self.container().template get<COMP_ID>()[self.index()][i];
    }

    decltype(auto) at(this auto&& self, uint i)
    {
        return self.container().template get<COMP_ID>()[self.index()].at(i);
    }

    decltype(auto) front(this auto&& self)
    {
        return self.container().template get<COMP_ID>()[self.index()].front();
    }

    decltype(auto) back(this auto&& self)
    {
        return self.container().template get<COMP_ID>()[self.index()].back();
    }

    decltype(auto) data(this auto&& self)
    {
        return self.container().template get<COMP_ID>()[self.index()].data();
    }

    decltype(auto) begin(this auto&& self)
    {
        return self.container().template get<COMP_ID>()[self.index()].begin();
    }

    decltype(auto) end(this auto&& self)
    {
        return self.container().template get<COMP_ID>()[self.index()].end();
    }

    decltype(auto) cbegin(this auto&& self)
    {
        return self.container().template get<COMP_ID>()[self.index()].cbegin();
    }

    decltype(auto) cend(this auto&& self)
    {
        return self.container().template get<COMP_ID>()[self.index()].cend();
    }

    // Dynamic operations only enabled if N < 0
    void resize(this auto&& self, std::size_t n)
        requires (N < 0)
    {
        self.container().template get<COMP_ID>()[self.index()].resize(n);
    }

    void pushBack(this auto&& self, const T& elem)
        requires (N < 0)
    {
        self.container().template get<COMP_ID>()[self.index()].pushBack(elem);
    }

    void popBack(this auto&& self)
        requires (N < 0)
    {
        self.container().template get<COMP_ID>()[self.index()].popBack();
    }

    void clear(this auto&& self)
        requires (N < 0)
    {
        self.container().template get<COMP_ID>()[self.index()].clear();
    }
};

} // namespace vcl::meshv2

#endif // VCL_MESH_V2_COMPONENTS_BASE_CONTAINER_COMPONENT_H
