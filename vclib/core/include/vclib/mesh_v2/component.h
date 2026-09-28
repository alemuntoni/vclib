// VCLib - Visual Computing Library
// Copyright (C) 2021-2026 Visual Computing Lab, ISTI - CNR.
//
// This Source Code Form is subject to the terms of the Mozilla Public License,
// v. 2.0. If a copy of the MPL was not distributed with this file, You can
// obtain one at https://mozilla.org/MPL/2.0/.

#ifndef VCL_MESH_V2_COMPONENT_H
#define VCL_MESH_V2_COMPONENT_H

#include <vclib/base.h>

namespace vcl::meshv2 {

template<
    uint COMP_ID,                // component id
    typename DataType,           // data stored by the component
    typename ParentElemType,     // parent element type
    typename DerivedComponent   // CRTP pattern, derived class
    >
class Component {

protected:
    ParentElemType* parentElement()
    {
        static_assert(
            !std::is_same_v<ParentElemType, void>,
            "The component should know its parent element type to get access "
            "to its pointer. You should define the component by passing the "
            "element type as template parameter. E.G., for a Face element:\n"
            "vcl::face::TriangleVertexPtrs<Vertex<Scalar>, Face<Scalar>>\n"
            "                                              ^^^^^^^^^^^^ \n");

        return static_cast<ParentElemType*>(this);
    }

    const ParentElemType* parentElement() const
    {
        static_assert(
            !std::is_same_v<ParentElemType, void>,
            "The component should know its parent element type to get access "
            "to its pointer. You should define the component by passing the "
            "element type as template parameter. E.G., for a Face element:\n"
            "vcl::face::TriangleVertexPtrs<Vertex<Scalar>, Face<Scalar>>\n"
            "                                              ^^^^^^^^^^^^ \n");

        return static_cast<const ParentElemType*>(this);
    }

    DataType& data()
    {
        ParentElemType* parent = parentElement();
    }
};

} // namespace vcl::meshv2

#endif // VCL_MESH_V2_COMPONENT_H
