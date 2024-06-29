/*****************************************************************************
 * VCLib                                                                     *
 * Visual Computing Library                                                  *
 *                                                                           *
 * Copyright(C) 2021-2024                                                    *
 * Visual Computing Lab                                                      *
 * ISTI - Italian National Research Council                                  *
 *                                                                           *
 * All rights reserved.                                                      *
 *                                                                           *
 * This program is free software; you can redistribute it and/or modify      *
 * it under the terms of the GNU General Public License as published by      *
 * the Free Software Foundation; either version 3 of the License, or         *
 * (at your option) any later version.                                       *
 *                                                                           *
 * This program is distributed in the hope that it will be useful,           *
 * but WITHOUT ANY WARRANTY; without even the implied warranty of            *
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the              *
 * GNU General Public License (http://www.gnu.org/licenses/gpl.txt)          *
 * for more details.                                                         *
 ****************************************************************************/

#ifndef MONTECARLO_SAMPLING_H
#define MONTECARLO_SAMPLING_H

#ifndef VCLIB_WITH_MODULES
#include <vclib/algorithms.h>
#include <vclib/meshes.h>
#else
#include <fstream>
#include <ranges> // needed by msvc...
import vclib;
#endif

void montecarloSampling(const vcl::TriMesh& m, vcl::TriMesh& samples)
{
    uint nSamples = 50;

    vcl::MeshSampler<vcl::TriMesh> s =
        vcl::montecarloPointSampling<vcl::MeshSampler<vcl::TriMesh>>(
            m, nSamples);

    samples = s.samples();
}

#endif // MONTECARLO_SAMPLING_H
