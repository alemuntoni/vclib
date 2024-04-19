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

#include <cassert>
#include <iostream>

#ifndef VCLIB_WITH_MODULES
#include <vclib/load_save.h>
#include <vclib/meshes.h>
#include <vclib/misc/timer.h>
#else
#include <ranges>
import vclib;
#endif

int main()
{
    vcl::TriMesh m =
        vcl::loadPly<vcl::TriMesh>(VCLIB_ASSETS_PATH "/TextureDouble.ply");

    vcl::PolyMesh pm;

    vcl::Timer t("import");
    pm.enableSameOptionalComponentsOf(m);
    pm.importFrom(m);
    t.stopAndPrint();

    assert(pm.isPerFaceWedgeTexCoordsEnabled());

    vcl::save(pm, VCLIB_RESULTS_PATH "/TextureDouble_converted.ply", false);

    pm = vcl::loadPly<vcl::PolyMesh>(VCLIB_ASSETS_PATH "/cube_poly.ply");

    m.clear();
    m.disableAllPerFaceOptionalComponents();
    m.disableAllPerVertexOptionalComponents();
    m.enableSameOptionalComponentsOf(pm);
    m.importFrom(pm);

    vcl::save(m, VCLIB_RESULTS_PATH "/cube_from_poly.ply", false);

    return 0;
}
