// VCLib - Visual Computing Library
// Copyright (C) 2021-2026 Visual Computing Lab, ISTI - CNR.
//
// This Source Code Form is subject to the terms of the Mozilla Public License,
// v. 2.0. If a copy of the MPL was not distributed with this file, You can
// obtain one at https://mozilla.org/MPL/2.0/.

#ifndef VCL_BGFX_EDITORS_TRANSFORM_EDITOR_SCALE_GIZMO_BGFX_H
#define VCL_BGFX_EDITORS_TRANSFORM_EDITOR_SCALE_GIZMO_BGFX_H

#include <vclib/algorithms/core.h>
#include <vclib/bgfx/primitives/lines.h>
#include <vclib/bgfx/primitives/points.h>
#include <vclib/render/drawable/abstract_drawable_mesh.h>
#include <vector>

namespace vcl {

class ScaleGizmoBGFX
{
    vcl::Lines  mBBoxEdges;
    vcl::Points mScaleCorners;
    vcl::Points mScaleEdgeCenters;
    vcl::Points mScaleFaceCenters;

    ushort       mGizmoTypeClicked    = USHORT_NULL;
    uint         mGizmoElementClicked = UINT_NULL;
    vcl::Point3d mLocalAnchorPoint    = vcl::Point3d::Zero();

    inline static const std::array<vcl::Point3d, 8> sCorners = {
        {{-0.5, -0.5, -0.5},
         {0.5, -0.5, -0.5},
         {0.5, 0.5, -0.5},
         {-0.5, 0.5, -0.5},
         {-0.5, -0.5, 0.5},
         {0.5, -0.5, 0.5},
         {0.5, 0.5, 0.5},
         {-0.5, 0.5, 0.5}}
    };

    inline static const std::array<vcl::Point3d, 12> sEdgeCenters = {
        {{0, -0.5, -0.5},
         {0, 0.5, -0.5},
         {0, -0.5, 0.5},
         {0, 0.5, 0.5}, // X-aligned
         {-0.5, 0, -0.5},
         {0.5, 0, -0.5},
         {-0.5, 0, 0.5},
         {0.5, 0, 0.5}, // Y-aligned
         {-0.5, -0.5, 0},
         {0.5, -0.5, 0},
         {-0.5, 0.5, 0},
         {0.5, 0.5, 0}}  // Z-aligned
    };

    inline static const std::array<vcl::Point3d, 6> sFaceCenters = {
        {{-0.5, 0, 0},
         {0.5, 0, 0}, // X faces
         {0, -0.5, 0},
         {0, 0.5, 0}, // Y faces
         {0, 0, -0.5},
         {0, 0, 0.5}}  // Z faces
    };

public:
    ScaleGizmoBGFX()
    {
        mScaleCorners.setVertices(sCorners);
        mScaleCorners.setWidth(10.0f);
        mScaleCorners.setGeneralColor(vcl::Color::White);
        mScaleCorners.setShape(vcl::Points::Shape::CIRCLE);
        mScaleCorners.setDepthOffset(0.001f);

        mScaleEdgeCenters.setVertices(sEdgeCenters);
        mScaleEdgeCenters.setWidth(10.0f);
        mScaleEdgeCenters.setShape(Points::Shape::CIRCLE);
        mScaleEdgeCenters.setDepthOffset(0.001f);

        std::vector<Color> edgeColors;
        for (int i = 0; i < 4; ++i)
            edgeColors.push_back(Color(0, 255, 255));
        for (int i = 0; i < 4; ++i)
            edgeColors.push_back(Color(255, 0, 255));
        for (int i = 0; i < 4; ++i)
            edgeColors.push_back(Color(255, 255, 0));
        mScaleEdgeCenters.setVertexColors(edgeColors);
        mScaleEdgeCenters.setColorSetting(Points::ColorSetting::PER_VERTEX);

        mScaleFaceCenters.setVertices(sFaceCenters);
        mScaleFaceCenters.setWidth(10.0f);
        mScaleFaceCenters.setShape(vcl::Points::Shape::CIRCLE);
        mScaleFaceCenters.setDepthOffset(0.001f);

        std::vector<vcl::Color> faceColors = {
            vcl::Color::Red,
            vcl::Color::Red,
            vcl::Color::Green,
            vcl::Color::Green,
            vcl::Color::Blue,
            vcl::Color::Blue};
        mScaleFaceCenters.setVertexColors(faceColors);
        mScaleFaceCenters.setColorSetting(
            vcl::Points::ColorSetting::PER_VERTEX);

        int edges[12][2] = {
            {0, 1},
            {1, 2},
            {2, 3},
            {3, 0},
            {4, 5},
            {5, 6},
            {6, 7},
            {7, 4},
            {0, 4},
            {1, 5},
            {2, 6},
            {3, 7}
        };
        std::vector<vcl::Point3d> edgeVerts;
        for (int i = 0; i < 12; ++i) {
            edgeVerts.push_back(sCorners[edges[i][0]]);
            edgeVerts.push_back(sCorners[edges[i][1]]);
        }
        mBBoxEdges.setVertices(edgeVerts);
        mBBoxEdges.setWidth(1.5f);
        mBBoxEdges.setGeneralColor(vcl::Color::Black);
    }

    void draw(uint viewId, const vcl::Matrix44f& gizmoTransform)
    {
        bgfx::setTransform(gizmoTransform.data());
        mBBoxEdges.draw(viewId);

        bgfx::setTransform(gizmoTransform.data());
        mScaleCorners.draw(viewId);

        bgfx::setTransform(gizmoTransform.data());
        mScaleEdgeCenters.draw(viewId);

        bgfx::setTransform(gizmoTransform.data());
        mScaleFaceCenters.draw(viewId);
    }

    void drawId(uint viewId, const vcl::Matrix44f& gizmoTransform)
    {
        uint32_t cornerId     = (0xFFFE << 16) | 0;
        uint32_t edgeCenterId = (0xFFFE << 16) | 1;
        uint32_t faceCenterId = (0xFFFE << 16) | 2;

        bgfx::setTransform(gizmoTransform.data());
        mScaleCorners.drawId(viewId, cornerId);

        bgfx::setTransform(gizmoTransform.data());
        mScaleEdgeCenters.drawId(viewId, edgeCenterId);

        bgfx::setTransform(gizmoTransform.data());
        mScaleFaceCenters.drawId(viewId, faceCenterId);
    }

    void calculateAnchor(
        ushort                                elemId,
        uint                                  primitiveId,
        std::shared_ptr<AbstractDrawableMesh> mesh)
    {
        mGizmoTypeClicked    = elemId;
        mGizmoElementClicked = primitiveId;

        vcl::Point3d P = vcl::Point3d::Zero();
        if (elemId == 0)
            P = sCorners[primitiveId];
        else if (elemId == 1)
            P = sEdgeCenters[primitiveId];
        else if (elemId == 2)
            P = sFaceCenters[primitiveId];

        Point3d center = mesh->meshProvider().boundingBox().center();
        Point3d sizes  = mesh->meshProvider().boundingBox().size();

        Point3d offset(P.x() * sizes.x(), P.y() * sizes.y(), P.z() * sizes.z());

        // The anchor is the opposite point
        mLocalAnchorPoint = center - offset;
    }

    vcl::Point3d anchorPointInWorld(
        std::shared_ptr<AbstractDrawableMesh> mesh) const
    {
        // We return the clicked point in world space to use as anchor depth
        Point3d center       = mesh->meshProvider().boundingBox().center();
        Point3d localClicked = center + (center - mLocalAnchorPoint);
        return localClicked * mesh->meshProvider().transformMatrix();
    }

    vcl::Matrix44d calculateNewTransform(
        const Point3d&        newPoint3D,
        const Point3d&        oldPoint3D,
        const vcl::Matrix44d& originalMatrix)
    {
        Matrix44d invModel = originalMatrix.inverse();

        Point3d localNew = newPoint3D * invModel;
        Point3d localOld = oldPoint3D * invModel;

        Point3d dNew = localNew - mLocalAnchorPoint;
        Point3d dOld = localOld - mLocalAnchorPoint;

        Point3d scale(1.0, 1.0, 1.0);

        if (mGizmoTypeClicked == 0) {
            double s = dNew.dot(dOld) / std::max(1e-12, dOld.squaredNorm());
            s        = std::max(0.01, s);
            scale    = Point3d(s, s, s);
        }
        else if (mGizmoTypeClicked == 1) {
            uint e       = mGizmoElementClicked;
            bool isXEdge = (e < 4);
            bool isYEdge = (e >= 4 && e < 8);
            bool isZEdge = (e >= 8);

            if (isXEdge) {
                scale.y() = safeRatio(dNew.y(), dOld.y());
                scale.z() = safeRatio(dNew.z(), dOld.z());
            }
            else if (isYEdge) {
                scale.x() = safeRatio(dNew.x(), dOld.x());
                scale.z() = safeRatio(dNew.z(), dOld.z());
            }
            else if (isZEdge) {
                scale.x() = safeRatio(dNew.x(), dOld.x());
                scale.y() = safeRatio(dNew.y(), dOld.y());
            }
        }
        else if (mGizmoTypeClicked == 2) {
            uint f       = mGizmoElementClicked;
            bool isXFace = (f < 2);
            bool isYFace = (f >= 2 && f < 4);
            bool isZFace = (f >= 4);

            if (isXFace)
                scale.x() = safeRatio(dNew.x(), dOld.x());
            if (isYFace)
                scale.y() = safeRatio(dNew.y(), dOld.y());
            if (isZFace)
                scale.z() = safeRatio(dNew.z(), dOld.z());
        }

        Matrix44d scaleMat = Matrix44d::Identity();
        vcl::setTransformMatrixScale(scaleMat, scale);

        Matrix44d Tanchor = Matrix44d::Identity();
        vcl::setTransformMatrixTranslation(Tanchor, mLocalAnchorPoint);

        Matrix44d TanchorInv = Matrix44d::Identity();
        vcl::setTransformMatrixTranslation(
            TanchorInv, Point3d(-mLocalAnchorPoint));

        return originalMatrix * Tanchor * scaleMat * TanchorInv;
    }

private:
    static double safeRatio(double n, double d)
    {
        return std::abs(d) > 1e-6 ? std::max(0.01, n / d) : 1.0;
    }
};

} // namespace vcl

#endif // VCL_BGFX_EDITORS_TRANSFORM_EDITOR_SCALE_GIZMO_BGFX_H
