// VCLib - Visual Computing Library
// Copyright (C) 2021-2026 Visual Computing Lab, ISTI - CNR.
//
// This Source Code Form is subject to the terms of the Mozilla Public License,
// v. 2.0. If a copy of the MPL was not distributed with this file, You can
// obtain one at https://mozilla.org/MPL/2.0/.

#ifndef VCL_BGFX_EDITORS_TRANSFORM_EDITOR_ROTATE_GIZMO_BGFX_H
#define VCL_BGFX_EDITORS_TRANSFORM_EDITOR_ROTATE_GIZMO_BGFX_H

#include <vclib/algorithms/core/create.h>
#include <vclib/bgfx/primitives/lines.h>
#include <vclib/bgfx/primitives/points.h>
#include <vclib/render/drawable/abstract_drawable_mesh.h>
#include <vclib/space/core.h>

namespace vcl {

class RotateGizmoBGFX
{
    Lines  mCircles[3];
    Points mHandles;

    bool mIsInitialized = false;

    uint    mGizmoElementClicked = USHORT_NULL;
    Point3d mLocalAnchorPoint;
    double  mRadius = 1.0;

public:
    RotateGizmoBGFX() = default;

    void init()
    {
        if (mIsInitialized)
            return;

        auto circle2d = createCircle<Polygon2f>(128, 1.0f);

        std::vector<Point3f> ptsX, ptsY, ptsZ;
        std::vector<uint>    indices;

        ptsX.reserve(circle2d.size());
        ptsY.reserve(circle2d.size());
        ptsZ.reserve(circle2d.size());
        indices.reserve(circle2d.size() * 2);

        for (uint i = 0; i < circle2d.size(); ++i) {
            auto p = circle2d.point(i);
            ptsX.push_back(Point3f(0.0f, p.x(), p.y()));
            ptsY.push_back(Point3f(p.x(), 0.0f, p.y()));
            ptsZ.push_back(Point3f(p.x(), p.y(), 0.0f));
            indices.push_back(i);
            indices.push_back((i + 1) % circle2d.size());
        }

        mCircles[0].setVertices(ptsX);
        mCircles[0].setIndices(indices);
        mCircles[0].setGeneralColor(Color::Red);

        mCircles[1].setVertices(ptsY);
        mCircles[1].setIndices(indices);
        mCircles[1].setGeneralColor(Color::Green);

        mCircles[2].setVertices(ptsZ);
        mCircles[2].setIndices(indices);
        mCircles[2].setGeneralColor(Color::Blue);

        std::vector<Point3f> handles = {
            Point3f(
                0.0f, 1.0f, 0.0f), // Red handle (rotate around X), placed at +Y
            Point3f(
                0.0f,
                0.0f,
                1.0f), // Green handle (rotate around Y), placed at +Z
            Point3f(
                1.0f, 0.0f, 0.0f) // Blue handle (rotate around Z), placed at +X
        };
        std::vector<Color> handleColors = {
            Color::Red, Color::Green, Color::Blue};

        mHandles.setVertices(handles);
        mHandles.setVertexColors(handleColors);
        mHandles.setWidth(20.0f); // Make handles visible
        mHandles.setColorSetting(Points::ColorSetting::PER_VERTEX);
        mHandles.setDepthOffset(
            0.001f); // Slightly offset to avoid z-fighting with the circles

        mIsInitialized = true;
    }

    void draw(uint viewId, const vcl::Matrix44f& gizmoTransform)
    {
        init();
        bgfx::setTransform(gizmoTransform.data());
        mCircles[0].draw(viewId);

        bgfx::setTransform(gizmoTransform.data());
        mCircles[1].draw(viewId);

        bgfx::setTransform(gizmoTransform.data());
        mCircles[2].draw(viewId);

        bgfx::setTransform(gizmoTransform.data());
        // Temporarily disable depth testing for handles if you want them always
        // on top? For now, let bgfx state handle it.
        mHandles.draw(viewId);
    }

    void drawId(uint viewId, const vcl::Matrix44f& gizmoTransform)
    {
        init();
        uint32_t baseId = (0xFFFD << 16);
        bgfx::setTransform(gizmoTransform.data());
        mHandles.drawId(viewId, baseId);
    }

    void calculateAnchor(
        ushort                                elemId,
        uint                                  primitiveId,
        std::shared_ptr<AbstractDrawableMesh> mesh)
    {
        mGizmoElementClicked = primitiveId; // 0, 1, or 2

        Point3d center = mesh->meshProvider().boundingBox().center();
        Point3d sizes  = mesh->meshProvider().boundingBox().size();
        mRadius        = sizes.norm() / 2.0;

        mLocalAnchorPoint = center;
    }

    vcl::Point3d anchorPointInWorld(
        std::shared_ptr<AbstractDrawableMesh> mesh) const
    {
        // Anchor is just the center of the bounding box
        return mLocalAnchorPoint * mesh->meshProvider().transformMatrix();
    }

    vcl::Matrix44d calculateNewTransform(
        const Point3d&        newPoint3D,
        const Point3d&        oldPoint3D,
        const vcl::Matrix44d& originalMatrix)
    {
        Matrix44d invModel = originalMatrix.inverse();

        Point3d localNew = newPoint3D * invModel;
        Point3d localOld = oldPoint3D * invModel;

        Point3d vNew = localNew - mLocalAnchorPoint;
        Point3d vOld = localOld - mLocalAnchorPoint;

        // Project the vectors onto the plane of rotation
        // If element == 0 (Red, rotate around X), plane is YZ. Normal = (1,0,0)
        // If element == 1 (Green, rotate around Y), plane is XZ. Normal =
        // (0,1,0) If element == 2 (Blue, rotate around Z), plane is XY. Normal
        // = (0,0,1)
        Point3d normal = Point3d::Zero();
        if (mGizmoElementClicked == 0)
            normal.x() = 1.0;
        else if (mGizmoElementClicked == 1)
            normal.y() = 1.0;
        else if (mGizmoElementClicked == 2)
            normal.z() = 1.0;

        // Project vOld and vNew on the plane defined by normal
        Point3d pOld = vOld - (vOld.dot(normal)) * normal;
        Point3d pNew = vNew - (vNew.dot(normal)) * normal;

        pOld.normalize();
        pNew.normalize();

        // Cross product gives the direction of rotation (parallel to normal)
        Point3d cross    = pOld.cross(pNew);
        double  sinAngle = cross.dot(normal);
        double  cosAngle = pOld.dot(pNew);
        double  angle    = std::atan2(sinAngle, cosAngle);

        Matrix44d rotationMat = Matrix44d::Identity();
        if (mGizmoElementClicked == 0) {
            vcl::setTransformMatrixRotation(
                rotationMat, Point3d(1, 0, 0), angle);
        }
        else if (mGizmoElementClicked == 1) {
            vcl::setTransformMatrixRotation(
                rotationMat, Point3d(0, 1, 0), angle);
        }
        else if (mGizmoElementClicked == 2) {
            vcl::setTransformMatrixRotation(
                rotationMat, Point3d(0, 0, 1), angle);
        }

        Matrix44d Tanchor = Matrix44d::Identity();
        vcl::setTransformMatrixTranslation(Tanchor, mLocalAnchorPoint);

        Matrix44d TanchorInv = Matrix44d::Identity();
        vcl::setTransformMatrixTranslation(
            TanchorInv, Point3d(-mLocalAnchorPoint));

        return originalMatrix * Tanchor * rotationMat * TanchorInv;
    }
};

} // namespace vcl

#endif // VCL_BGFX_EDITORS_TRANSFORM_EDITOR_ROTATE_GIZMO_BGFX_H
