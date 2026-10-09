// VCLib - Visual Computing Library
// Copyright (C) 2021-2026 Visual Computing Lab, ISTI - CNR.
//
// This Source Code Form is subject to the terms of the Mozilla Public License,
// v. 2.0. If a copy of the MPL was not distributed with this file, You can
// obtain one at https://mozilla.org/MPL/2.0/.

#ifndef VCL_MESH_V2_COMPONENTS_BASE_COMPONENT_H
#define VCL_MESH_V2_COMPONENTS_BASE_COMPONENT_H

#include <vclib/base.h>

#include <vclib/mesh_v2/parallel_vectors_tuple.h>

namespace vcl::meshv2 {

/**
 * @brief Base class for all components.
 * It exposes the DataType (used by the container to build the tuple)
 * and the COMPONENT_ID (for metaprogramming checks).
 */
template<uint COMP_ID, typename Data>
struct Component
{
    using DataType                     = Data;
    static constexpr uint COMPONENT_ID = COMP_ID;
};

namespace detail {

// Helper details for finding the index of a Component by COMP_ID in a list of
// Components. It recursively checks each component's COMPONENT_ID against the
// target COMP_ID, incrementing the CurrentIndex until a match is found or the
// list is exhausted.
template<uint COMP_ID, uint CurrentIndex, typename... Components>
struct IndexOfCompIdImpl;

template<uint COMP_ID, uint CurrentIndex, typename First, typename... Rest>
struct IndexOfCompIdImpl<COMP_ID, CurrentIndex, First, Rest...>
{
    static constexpr uint value =
        (First::COMPONENT_ID == COMP_ID) ?
            CurrentIndex :
            IndexOfCompIdImpl<COMP_ID, CurrentIndex + 1, Rest...>::value;
};

// Base case when the component is not found
template<uint COMP_ID, uint CurrentIndex>
struct IndexOfCompIdImpl<COMP_ID, CurrentIndex>
{
    static constexpr uint value = UINT_NULL;
};

} // namespace detail

/**
 * @brief Trait to find the index of a Component by its COMP_ID inside a
 * TypeWrapper.
 *
 * Usage: IndexOfCompId<COMP_ID, TypeWrapper<...>>::value
 */
template <uint COMP_ID, typename ComponentListWrapper>
struct IndexOfCompId;

template<uint COMP_ID, typename... Components>
struct IndexOfCompId<COMP_ID, vcl::TypeWrapper<Components...>>
{
    static constexpr uint value =
        detail::IndexOfCompIdImpl<COMP_ID, 0, Components...>::value;
    static_assert(
        value != UINT_NULL,
        "Component ID not found in the ComponentList");
};

// Trait to convert TypeWrapper<Components...> in a ParallelVectorsTuple
template<typename ComponentListWrapper>
struct ParallelVectorsTupleFromComponents;

template<typename... Components>
struct ParallelVectorsTupleFromComponents<vcl::TypeWrapper<Components...>>
{
    using type = ParallelVectorsTuple<typename Components::DataType...>;
};

} // namespace vcl::meshv2

#endif // VCL_MESH_V2_COMPONENTS_BASE_COMPONENT_H
