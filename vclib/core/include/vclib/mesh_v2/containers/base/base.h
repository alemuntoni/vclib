// VCLib - Visual Computing Library
// Copyright (C) 2021-2026 Visual Computing Lab, ISTI - CNR.
//
// This Source Code Form is subject to the terms of the Mozilla Public License,
// v. 2.0. If a copy of the MPL was not distributed with this file, You can
// obtain one at https://mozilla.org/MPL/2.0/.

#ifndef VCL_MESH_V2_CONTAINERS_BASE_BASE_H
#define VCL_MESH_V2_CONTAINERS_BASE_BASE_H

#include <vclib/base.h>

namespace vcl::meshv2 {

namespace detail {

/// Rebind

// Helper to rebind a container's ParentMesh to the actual Mesh class
template<typename T, typename NewParent>
struct RebindContainerHelper
{
    using type = T;
};

template<typename T, typename NewParent>
requires requires { typename T::template Rebind<NewParent>; }
struct RebindContainerHelper<T, NewParent>
{
    using type = typename T::template Rebind<NewParent>;
};

template<typename T, typename NewParent>
using RebindContainer = typename RebindContainerHelper<T, NewParent>::type;

/// Find Container

// Compile-time search for a container with the given ELEMENT_ID
template<uint ELEM_ID, typename... Types>
struct FindContainer;

template<uint ELEM_ID>
struct FindContainer<ELEM_ID>
{
    using type                  = void;
    static constexpr bool found = false;
};

template<uint ELEM_ID, typename Head, typename... Tail>
struct FindContainer<ELEM_ID, Head, Tail...>
{
private:
    static constexpr bool matches = requires {
        requires Head::ELEMENT_ID == ELEM_ID;
    };

public:
    using type = std::conditional_t<
        matches,
        Head,
        typename FindContainer<ELEM_ID, Tail...>::type>;
    static constexpr bool found =
        matches || FindContainer<ELEM_ID, Tail...>::found;
};

} // namespace detail

} // namespace vcl::meshv2

#endif // VCL_MESH_V2_CONTAINERS_BASE_BASE_H
