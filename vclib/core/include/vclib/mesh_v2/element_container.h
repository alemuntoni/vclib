// VCLib - Visual Computing Library
// Copyright (C) 2021-2026 Visual Computing Lab, ISTI - CNR.
//
// This Source Code Form is subject to the terms of the Mozilla Public License,
// v. 2.0. If a copy of the MPL was not distributed with this file, You can
// obtain one at https://mozilla.org/MPL/2.0/.

#ifndef VCL_MESH_V2_ELEMENT_CONTAINER_H
#define VCL_MESH_V2_ELEMENT_CONTAINER_H

#include <vclib/base.h>

#include <vclib/mesh_v2/components/base/component.h>
#include <vclib/mesh_v2/parallel_vectors_tuple.h>

namespace vcl::meshv2 {

template<typename ComponentList>
class ElementContainer
{
    using TupleType =
        typename ParallelVectorsTupleFromComponents<ComponentList>::type;
    TupleType mElemComponents;

public:
    ElementContainer() = default;

    /**
     * @brief Returns the vector for the given component ID.
     */
    template<uint COMP_ID>
    auto& get()
    {
        constexpr uint N = IndexOfCompId<COMP_ID, ComponentList>::value;
        return mElemComponents.template get<N>();
    }

    template<uint COMP_ID>
    const auto& get() const
    {
        constexpr uint N = IndexOfCompId<COMP_ID, ComponentList>::value;
        return mElemComponents.template get<N>();
    }

    /**
     * @brief Enables the component with the given COMP_ID.
     */
    template<uint COMP_ID>
    void enable()
    {
        constexpr uint N = IndexOfCompId<COMP_ID, ComponentList>::value;
        mElemComponents.template enable<N>();
    }

    /**
     * @brief Disables the component with the given COMP_ID.
     */
    template<uint COMP_ID>
    void disable()
    {
        constexpr uint N = IndexOfCompId<COMP_ID, ComponentList>::value;
        mElemComponents.template disable<N>();
    }

    /**
     * @brief Checks if the component with the given COMP_ID is enabled.
     */
    template<uint COMP_ID>
    bool isEnabled() const
    {
        constexpr uint N = IndexOfCompId<COMP_ID, ComponentList>::value;
        return mElemComponents.template isEnabled<N>();
    }

    void enableAll() { mElemComponents.enableAll(); }

    void disableAll() { mElemComponents.disableAll(); }

    std::size_t size() const { return mElemComponents.size(); }

    void resize(std::size_t size) { mElemComponents.resize(size); }

    void reserve(std::size_t capacity) { mElemComponents.reserve(capacity); }

    void clear() { mElemComponents.clear(); }
};

} // namespace vcl::meshv2

#endif // VCL_MESH_V2_ELEMENT_CONTAINER_H
