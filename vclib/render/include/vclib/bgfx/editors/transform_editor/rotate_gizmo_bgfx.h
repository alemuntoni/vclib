// VCLib - Visual Computing Library
// Copyright (C) 2021-2026 Visual Computing Lab, ISTI - CNR.
//
// This Source Code Form is subject to the terms of the Mozilla Public License,
// v. 2.0. If a copy of the MPL was not distributed with this file, You can
// obtain one at https://mozilla.org/MPL/2.0/.

#ifndef VCL_BGFX_EDITORS_TRANSFORM_EDITOR_ROTATE_GIZMO_BGFX_H
#define VCL_BGFX_EDITORS_TRANSFORM_EDITOR_ROTATE_GIZMO_BGFX_H

#include <vclib/bgfx/primitives/lines.h>
#include <vclib/bgfx/primitives/points.h>
#include <vclib/render/drawable/abstract_drawable_mesh.h>

#include <vclib/algorithms/core.h>
#include <vclib/space/core.h>

namespace vcl {

class RotateGizmoBGFX
{
    Lines  mCircles[3];
    Points mHandles;

    uint    mGizmoElementClicked = USHORT_NULL;
    Point3d mLocalAnchorPoint;
    double  mRadius = 1.0;

    double  mTotalAngle   = 0.0;
    bool    mIsFirstFrame = true;
    Point3d mLastMousePos3D;

public:
    RotateGizmoBGFX()
    {
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
    }

    void draw(uint viewId, const vcl::Matrix44f& gizmoTransform)
    {
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

    void drawId(uint viewId, const vcl::Matrix44f& gizmoTransform, ushort meshId)
    {
        uint32_t baseId = (0xFFFD << 16) | meshId;
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
        mTotalAngle       = 0.0;
        mIsFirstFrame     = true;
    }

    vcl::Point3d anchorPointInWorld(
        std::shared_ptr<AbstractDrawableMesh> mesh) const
    {
        // Anchor is just the center of the bounding box
        return mLocalAnchorPoint * mesh->meshProvider().transformMatrix();
    }

    Point3d pointOnArcballLocal(
        const Point3d& localP,
        const Point3d& localC,
        const Point3d& localViewNormal,
        double         radius) const
    {
        Point3d V = localP - localC;
        // Ensure V is orthogonal to the view normal
        V = V - V.dot(localViewNormal) * localViewNormal;

        double h = V.norm();
        double z = 0.0;

        if (h < (M_SQRT1_2 * radius)) {
            z = std::sqrt(std::max(0.0, radius * radius - h * h));
        }
        else {
            z = (radius * radius) / (2.0 * std::max(h, 1e-6));
        }

        return localC + V + localViewNormal * z;
    }

    Matrix44d calculateNewTransformArcball(
        const Point3d&        newPoint3D,
        const Point3d&        viewNormalWorld,
        const vcl::Matrix44d& originalMatrix)
    {
        if (mIsFirstFrame) {
            mLastMousePos3D = newPoint3D;
            mIsFirstFrame   = false;
            return originalMatrix;
        }

        Matrix44d invModel = originalMatrix.inverse();

        Point3d localC      = mLocalAnchorPoint;
        Point3d centerWorld = localC * originalMatrix;

        // Convert world view normal to local space direction
        Point3d localViewNormal =
            (Point3d(centerWorld + viewNormalWorld) * invModel - localC)
                .normalized();

        Point3d localNew  = newPoint3D * invModel;
        Point3d localPrev = mLastMousePos3D * invModel;

        Point3d pCurr =
            pointOnArcballLocal(localNew, localC, localViewNormal, mRadius);
        Point3d pPrev =
            pointOnArcballLocal(localPrev, localC, localViewNormal, mRadius);

        Point3d vCurr = pCurr - localC;
        Point3d vPrev = pPrev - localC;

        if (vCurr.norm() > 1e-6 && vPrev.norm() > 1e-6) {
            vCurr.normalize();
            vPrev.normalize();

            Eigen::Quaterniond q =
                Eigen::Quaterniond::FromTwoVectors(vPrev, vCurr);

            double deltaAngle = 0.0;

            if (mGizmoElementClicked == 0) { // X
                double norm = std::sqrt(q.w() * q.w() + q.x() * q.x());
                if (norm > 1e-6)
                    deltaAngle = 2.0 * std::atan2(q.x(), q.w());
            }
            else if (mGizmoElementClicked == 1) { // Y
                double norm = std::sqrt(q.w() * q.w() + q.y() * q.y());
                if (norm > 1e-6)
                    deltaAngle = 2.0 * std::atan2(q.y(), q.w());
            }
            else if (mGizmoElementClicked == 2) { // Z
                double norm = std::sqrt(q.w() * q.w() + q.z() * q.z());
                if (norm > 1e-6)
                    deltaAngle = 2.0 * std::atan2(q.z(), q.w());
            }

            mTotalAngle += deltaAngle;
        }

        mLastMousePos3D = newPoint3D;

        Matrix44d rotationMat = Matrix44d::Identity();
        if (mGizmoElementClicked == 0) {
            vcl::setTransformMatrixRotation(
                rotationMat, Point3d(1, 0, 0), mTotalAngle);
        }
        else if (mGizmoElementClicked == 1) {
            vcl::setTransformMatrixRotation(
                rotationMat, Point3d(0, 1, 0), mTotalAngle);
        }
        else if (mGizmoElementClicked == 2) {
            vcl::setTransformMatrixRotation(
                rotationMat, Point3d(0, 0, 1), mTotalAngle);
        }

        Matrix44d Tanchor = Matrix44d::Identity();
        vcl::setTransformMatrixTranslation(Tanchor, localC);

        Matrix44d TanchorInv = Matrix44d::Identity();
        vcl::setTransformMatrixTranslation(TanchorInv, Point3d(-localC));

        return originalMatrix * Tanchor * rotationMat * TanchorInv;
    }
};

} // namespace vcl

#endif // VCL_BGFX_EDITORS_TRANSFORM_EDITOR_ROTATE_GIZMO_BGFX_H
