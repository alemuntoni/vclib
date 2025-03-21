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

#ifndef VCL_WEBGPU_DRAWERS_VIEWER_DRAWER_H
#define VCL_WEBGPU_DRAWERS_VIEWER_DRAWER_H

#include <vclib/render/drawers/abstract_viewer_drawer.h>

namespace vcl {

template<typename DerivedRenderApp>
class ViewerDrawerWebGPU : public AbstractViewerDrawer<DerivedRenderApp>
{
    using ParentViewer = AbstractViewerDrawer<DerivedRenderApp>;
    using DTB          = ParentViewer::DTB;

public:
    ViewerDrawerWebGPU(uint width = 1024, uint height = 768) :
            ParentViewer(width, height)
    {
    }

    void onInit(uint viewId) override
    {
        // todo
    }

    void onResize(uint width, uint height) override
    {
        DTB::resizeViewer(width, height);
    }

    void onDraw(uint viewId) override { onDrawContent(viewId); }

    void onDrawContent(uint viewId) override
    {
        // todo
    }

    void onPostDraw() override
    {
        // todo
    }

    void toggleAxisVisibility() override
    {
        // todo
    }

    void toggleTrackBallVisibility() override
    {
        // todo
    }
};

} // namespace vcl

#endif // VCL_WEBGPU_DRAWERS_VIEWER_DRAWER_H
