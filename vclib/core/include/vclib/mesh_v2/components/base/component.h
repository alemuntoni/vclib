// VCLib - Visual Computing Library
// Copyright (C) 2021-2026 Visual Computing Lab, ISTI - CNR.
//
// This Source Code Form is subject to the terms of the Mozilla Public License,
// v. 2.0. If a copy of the MPL was not distributed with this file, You can
// obtain one at https://mozilla.org/MPL/2.0/.

#ifndef VCL_MESH_V2_COMPONENTS_BASE_COMPONENT_H
#define VCL_MESH_V2_COMPONENTS_BASE_COMPONENT_H

#include <vclib/base.h>

#include <vclib/mesh_v2/components/base/base.h>

namespace vcl::meshv2 {

/**
 * @brief Base class for all components.
 * It exposes the DataType (used by the container to build the tuple)
 * and the COMPONENT_ID (for metaprogramming checks).
 */
template<uint COMP_ID, typename Data>
class Component
{
public:
    using DataType                     = Data;
    static constexpr uint COMPONENT_ID = COMP_ID;

protected:
    // needs to be static to be accessible from the derived classes
    static decltype(auto) compData(auto&& self)
    {
        return self.parentContainer().template vector<COMP_ID>()[self.index()];
    }
};

/**
 * @brief Concept that checks whether a type T is a Component (has COMPONENT_ID).
 */
template<typename T>
concept ComponentConcept = requires {
    { std::remove_cvref_t<T>::COMPONENT_ID } -> std::convertible_to<uint>;
};

} // namespace vcl::meshv2

#endif // VCL_MESH_V2_COMPONENTS_BASE_COMPONENT_H
