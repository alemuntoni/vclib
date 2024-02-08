#*****************************************************************************
#* VCLib                                                                     *
#* Visual Computing Library                                                  *
#*                                                                           *
#* Copyright(C) 2021-2024                                                    *
#* Visual Computing Lab                                                      *
#* ISTI - Italian National Research Council                                  *
#*                                                                           *
#* All rights reserved.                                                      *
#*                                                                           *
#* This program is free software; you can redistribute it and/or modify      *
#* it under the terms of the GNU General Public License as published by      *
#* the Free Software Foundation; either version 3 of the License, or         *
#* (at your option) any later version.                                       *
#*                                                                           *
#* This program is distributed in the hope that it will be useful,           *
#* but WITHOUT ANY WARRANTY; without even the implied warranty of            *
#* MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the              *
#* GNU General Public License (http://www.gnu.org/licenses/gpl.txt)          *
#* for more details.                                                         *
#****************************************************************************/

cmake_minimum_required(VERSION 3.24)
project(vclib-examples)

set(CMAKE_COMPILE_WARNING_AS_ERROR ${VCLIB_COMPILE_WARNINGS_AS_ERRORS})

# examples
if (VCLIB_BUILD_EXAMPLES)
    add_subdirectory(distance)
    add_subdirectory(edge_mesh_basic)
    add_subdirectory(grid)
    add_subdirectory(misc)
    add_subdirectory(mesh_basic)
    add_subdirectory(mesh_clean)
    add_subdirectory(mesh_conversion)
    add_subdirectory(mesh_copy)
    add_subdirectory(mesh_create)
    add_subdirectory(mesh_custom_components)
    add_subdirectory(mesh_export_to_matrix)
    add_subdirectory(mesh_iterators)
    add_subdirectory(mesh_io_obj)
    add_subdirectory(mesh_io_off)
    add_subdirectory(mesh_io_ply)
    add_subdirectory(mesh_io_stl)
    add_subdirectory(mesh_new_user_component)
    add_subdirectory(mesh_normal)
    add_subdirectory(mesh_principal_curvature)
    add_subdirectory(mesh_sampling)
    add_subdirectory(mesh_smooth)
    add_subdirectory(mesh_sphere_intersection)
    add_subdirectory(mesh_stat)
    add_subdirectory(mesh_topology)
    add_subdirectory(mesh_transform)
    add_subdirectory(polygon_earcut)
    add_subdirectory(polymesh_basic)
endif()
