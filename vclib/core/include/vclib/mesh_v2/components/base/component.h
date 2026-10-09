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
