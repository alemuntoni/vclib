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

#ifndef VCLIB_WITH_MODULES
#include <vclib/processing.h>

#include <vclib/load_save.h>
#else
import vclib;
#endif

int main()
{
    vcl::LoadSettings s;
    s.loadTextureImages = true;

    vcl::proc::TriMesh mesh = vcl::load<vcl::proc::TriMesh>(
        VCLIB_ASSETS_PATH "/TextureDouble.ply", s);

    vcl::proc::ActionManager manager;

    manager.add(vcl::proc::vclibActions());

    // saving obj
    manager.saveMeshAction("obj")->save("td.obj", mesh);

    // saving off
    manager.saveMeshAction("off")->save("td.off", mesh);

    // saving ply
    manager.saveMeshAction("ply")->save("td.ply", mesh);

    // saving stl
    manager.saveMeshAction("stl")->save("td.stl", mesh);

    return 0;
}
