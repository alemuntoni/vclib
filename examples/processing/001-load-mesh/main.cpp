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
#include <memory>

#ifndef VCLIB_WITH_MODULES
#include <vclib/processing.h>

#include <vclib/load_save.h>
#else
#include <Eigen/Core>
#include <Eigen/Geometry>
#include <ranges>
import vclib.core;
import vclib.processing;
#endif

int main()
{
    vcl::proc::ActionManager manager;

    manager.add(vcl::proc::vclibActions());

    std::shared_ptr<vcl::proc::MeshI> pm0 = manager.loadMeshAction("obj")->load(
        VCLIB_EXAMPLE_MESHES_PATH "/TextureDouble.obj");

    assert(pm0->is<vcl::proc::TriMesh>());

    auto pm1 = manager.loadMeshAction("obj")->load(VCLIB_EXAMPLE_MESHES_PATH
                                                   "/greek_helmet.obj");

    assert(pm1->is<vcl::proc::PolyMesh>());

    auto params = manager.loadMeshAction("obj")->parameters();
    params.get("mesh_type")->setUintValue(1);

    auto pm2 = manager.loadMeshAction("obj")->load(
        VCLIB_EXAMPLE_MESHES_PATH "/greek_helmet.obj", params);

    assert(pm2->is<vcl::proc::TriMesh>());

    manager.saveMeshAction("obj")->save("td1.obj", *pm0);

    manager.saveMeshAction("obj")->save("greek_poly.obj", *pm1);

    manager.saveMeshAction("obj")->save("greek_tri.obj", *pm2);

    return 0;
}
