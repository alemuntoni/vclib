/*****************************************************************************
 * VCLib                                                                     *
 * Visual Computing Library                                                  *
 *                                                                           *
 * Copyright(C) 2021-2025                                                    *
 * Visual Computing Lab                                                      *
 * ISTI - Italian National Research Council                                  *
 *                                                                           *
 * All rights reserved.                                                      *
 *                                                                           *
 * This program is free software; you can redistribute it and/or modify      *
 * it under the terms of the Mozilla Public License Version 2.0 as published *
 * by the Mozilla Foundation; either version 2 of the License, or            *
 * (at your option) any later version.                                       *
 *                                                                           *
 * This program is distributed in the hope that it will be useful,           *
 * but WITHOUT ANY WARRANTY; without even the implied warranty of            *
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the              *
 * Mozilla Public License Version 2.0                                        *
 * (https://www.mozilla.org/en-US/MPL/2.0/) for more details.                *
 ****************************************************************************/

#ifndef VCL_RENDER_DRAWABLE_DRAWABLE_MESH_H
#define VCL_RENDER_DRAWABLE_DRAWABLE_MESH_H

#include <vclib/render/config.h>

#ifdef VCLIB_RENDER_BACKEND_BGFX
#include <vclib/bgfx/drawable/drawable_mesh.h>
#endif

#ifdef VCLIB_RENDER_BACKEND_OPENGL2
#include <vclib/opengl2/drawable/drawable_mesh.h>
#endif

namespace vcl {

#ifdef VCLIB_RENDER_BACKEND_BGFX
template<MeshConcept MeshType>
using DrawableMesh = DrawableMeshBGFX<MeshType>;
#endif

#ifdef VCLIB_RENDER_BACKEND_OPENGL2
template<MeshConcept MeshType>
using DrawableMesh = DrawableMeshOpenGL2<MeshType>;
#endif

} // namespace vcl

#endif // VCL_RENDER_DRAWABLE_DRAWABLE_MESH_H
