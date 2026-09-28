// VCLib - Visual Computing Library
// Copyright (C) 2021-2026 Visual Computing Lab, ISTI - CNR.
//
// This Source Code Form is subject to the terms of the Mozilla Public License,
// v. 2.0. If a copy of the MPL was not distributed with this file, You can
// obtain one at https://mozilla.org/MPL/2.0/.

#include <iostream>

#include <vclib/mesh_v2/parallel_vectors_tuple.h>

int main()
{
    vcl::meshv2::ParallelVectorsTuple<int, float, std::pair<int, double>> vt;

    vt.enable<0>();
    vt.enable<2>();

    vt.resize(10);

    return 0;
}
