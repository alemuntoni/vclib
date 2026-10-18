// VCLib - Visual Computing Library
// Copyright (C) 2021-2026 Visual Computing Lab, ISTI - CNR.
//
// This Source Code Form is subject to the terms of the Mozilla Public License,
// v. 2.0. If a copy of the MPL was not distributed with this file, You can
// obtain one at https://mozilla.org/MPL/2.0/.

#ifndef VCL_MESH_V2_COMPONENTS_BASE_REFERENCE_VECTOR_H
#define VCL_MESH_V2_COMPONENTS_BASE_REFERENCE_VECTOR_H

#include <vclib/space/core.h>

namespace vcl::meshv2 {

template<std::unsigned_integral Index, int N>
class ReferenceVector : public vcl::Vector<Index, N>
{
    using Base = vcl::Vector<Index, N>;

public:
    using Base::Base;
    static constexpr Index NULL_INDEX = std::numeric_limits<Index>::max();

    ReferenceVector()
    {
        if constexpr (N >= 0) {
            this->fill(NULL_INDEX);
        }
    }

    explicit ReferenceVector(std::size_t size) : Base(size, NULL_INDEX) {}

    void resize(std::size_t size, Index value = NULL_INDEX) requires (N < 0)
    {
        Base::resize(size, value);
    }
};

} // namespace vcl::meshv2

#endif // VCL_MESH_V2_COMPONENTS_BASE_REFERENCE_VECTOR_H
