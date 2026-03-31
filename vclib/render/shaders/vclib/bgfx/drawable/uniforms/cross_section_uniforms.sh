/*****************************************************************************
 * VCLib                                                                     *
 * Visual Computing Library                                                  *
 *                                                                           *
 * Copyright(C) 2021-2026                                                    *
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

#ifndef VCL_EXT_BGFX_UNIFORMS_CROSS_SECTION_UNIFORMS_SH
#define VCL_EXT_BGFX_UNIFORMS_CROSS_SECTION_UNIFORMS_SH

uniform vec4 u_crossSectionMinData;
uniform vec4 u_crossSectionMaxData;

#define u_crossSectionMinX u_crossSectionMinData.x
#define u_crossSectionMinY u_crossSectionMinData.y
#define u_crossSectionMinZ u_crossSectionMinData.z

#define u_crossSectionMaxX u_crossSectionMaxData.x
#define u_crossSectionMaxY u_crossSectionMaxData.y
#define u_crossSectionMaxZ u_crossSectionMaxData.z

#endif // VCL_EXT_BGFX_UNIFORMS_CROSS_SECTION_UNIFORMS_SH
