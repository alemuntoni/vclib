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

#include <iostream>

#ifndef VCLIB_WITH_MODULES
#include <vclib/algorithms.h>
#include <vclib/load_save.h>
#include <vclib/meshes.h>
#include <vclib/misc/timer.h>
#else
#include <Eigen/Core>
import vclib;
#endif

int main()
{
    vcl::TriMesh m =
        vcl::loadPly<vcl::TriMesh>(VCLIB_ASSETS_PATH "/bunny_textured.ply");

    vcl::TriMesh mSmooth(m);

    vcl::Timer t1("Laplacian Smoothing");

    vcl::laplacianSmoothing(mSmooth, 30);
    t1.stopAndPrint();

    vcl::savePly(mSmooth, VCLIB_RESULTS_PATH "/bunny_lapl_smooth.ply");

    vcl::Timer t2("Taubin Smoothing");

    vcl::taubinSmoothing(m, 300, 0.5, -0.53);
    t2.stopAndPrint();

    vcl::savePly(m, VCLIB_RESULTS_PATH "/bunny_taub_smooth.ply");

    return 0;
}
