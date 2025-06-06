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

#ifndef HELLO_TRIANGLE_DRAWER_H
#define HELLO_TRIANGLE_DRAWER_H

#include "common.h"

#include <vclib/render/drawers/plain_drawer.h>

template<typename DerivedRenderApp>
class HelloTriangleDrawer : public vcl::PlainDrawer<DerivedRenderApp>
{
public:
    HelloTriangleDrawer(vcl::uint width = 1024, vcl::uint height = 768) {}

    ~HelloTriangleDrawer() {}

    void onResize(vcl::uint width, vcl::uint height) override
    {
        std::cout << "Resize: " << width << "; " << height
                  << ". Nothing to do\n";
    }

    void onDrawContent(vcl::uint) override
    {
        glClearColor(0.f, 0.f, 0.f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);

        glBegin(GL_TRIANGLES);
        glColor4ubv((GLubyte*) &(vertices[0].abgr));
        glVertex2fv((GLfloat*) &(vertices[0].pos));
        glColor4ubv((GLubyte*) &(vertices[1].abgr));
        glVertex2fv((GLfloat*) &(vertices[1].pos));
        glColor4ubv((GLubyte*) &(vertices[2].abgr));
        glVertex2fv((GLfloat*) &(vertices[2].pos));
        glEnd();
    }
};

#endif // HELLO_TRIANGLE_DRAWER_H
