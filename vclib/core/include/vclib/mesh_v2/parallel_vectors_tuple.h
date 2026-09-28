// VCLib - Visual Computing Library
// Copyright (C) 2021-2026 Visual Computing Lab, ISTI - CNR.
//
// This Source Code Form is subject to the terms of the Mozilla Public License,
// v. 2.0. If a copy of the MPL was not distributed with this file, You can
// obtain one at https://mozilla.org/MPL/2.0/.

#ifndef VCL_MESH_V2_PARALLEL_VECTORS_TUPLE_H
#define VCL_MESH_V2_PARALLEL_VECTORS_TUPLE_H

namespace vcl::meshv2 {

template<typename ...Types>
class ParallelVectorsTuple {
    // Implementation of a tuple of parallel vectors for storing types.
    // each vector is automatically disabled
    // apis should be as similar as possible to std::tuple, but with the ability
    // to enable/disable vectors at runtime.
    // access to the vectors should be through a get function that takes the
    // type as template parameter.
    // when same type is used multiple times, user should use a tag to
    // differentiate them.
};

} // namespace vcl::meshv2

#endif // VCL_MESH_V2_PARALLEL_VECTORS_TUPLE_H
