// VCLib - Visual Computing Library
// Copyright (C) 2021-2026 Visual Computing Lab, ISTI - CNR.
//
// This Source Code Form is subject to the terms of the Mozilla Public License,
// v. 2.0. If a copy of the MPL was not distributed with this file, You can
// obtain one at https://mozilla.org/MPL/2.0/.

#ifndef VCL_BGFX_EDITORS_TRANSFORM_EDITOR_ROTATE_GIZMO_BGFX_H
#define VCL_BGFX_EDITORS_TRANSFORM_EDITOR_ROTATE_GIZMO_BGFX_H

#include <vclib/bgfx/primitives/lines.h>
#include <vclib/bgfx/shapes/cone_shape.h>
#include <vclib/bgfx/shapes/cube_shape.h>
#include <vclib/render/drawable/abstract_drawable_mesh.h>

#include <vclib/algorithms/core.h>
#include <vclib/space/core.h>

namespace vcl {

class RotateGizmoBGFX
{
    Lines  mCircles[3];
    CubeShape mHandleCenter = CubeShape(
        vcl::Point3d(-0.025, -0.025, -0.025),
        vcl::Point3d(0.025, 0.025, 0.025));

    ConeShape mHandleCone1 = ConeShape(
        vcl::Point3d(0.0, 0.0, 0.05),
        vcl::Point3d(0.0, 0.0, 0.1),
        0.025,
        0.0,
        16);

    ConeShape mHandleCone2 = ConeShape(
        vcl::Point3d(0.0, 0.0, -0.05),
        vcl::Point3d(0.0, 0.0, -0.1),
        0.025,
        0.0,
        16);

    uint    mGizmoElementClicked = USHORT_NULL;
    Point3d mLocalAnchorPoint;
    double  mRadius = 1.0;

    double    mTotalAngle   = 0.0;
    bool      mIsFirstFrame = true;
    Point3d   mLastMousePos3D;
    Matrix44d mStartNoScaleBase = Matrix44d::Identity();

    static const uint64_t DRAW_STATE =
        0 | BGFX_STATE_WRITE_RGB | BGFX_STATE_WRITE_A | BGFX_STATE_WRITE_Z |
        BGFX_STATE_DEPTH_TEST_ALWAYS | BGFX_STATE_BLEND_ALPHA;

    static const uint64_t DRAW_HANDLE_STATE =
        0 | BGFX_STATE_WRITE_RGB | BGFX_STATE_WRITE_A | BGFX_STATE_WRITE_Z |
        BGFX_STATE_CULL_CW | BGFX_STATE_BLEND_ALPHA;

    static const uint64_t DRAW_ID_STATE =
        0 | BGFX_STATE_WRITE_RGB | BGFX_STATE_WRITE_A | BGFX_STATE_WRITE_Z |
        BGFX_STATE_DEPTH_TEST_ALWAYS |
        BGFX_STATE_BLEND_FUNC(BGFX_STATE_BLEND_ONE, BGFX_STATE_BLEND_ZERO);

    static constexpr float VISUAL_SCALE_FACTOR = 0.35f;

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

    }

    void draw(
        uint                  viewId,
        const vcl::Matrix44f& baseTransform,
        const vcl::Matrix44f& viewMatrix,
        const vcl::Matrix44f& projMatrix)
    {
        vcl::Matrix44f gizmoTransform =
            getGizmoTransform(baseTransform, viewMatrix, projMatrix);

        bgfx::setTransform(gizmoTransform.data());
        mCircles[0].draw(viewId, DRAW_STATE);

        bgfx::setTransform(gizmoTransform.data());
        mCircles[1].draw(viewId, DRAW_STATE);

        bgfx::setTransform(gizmoTransform.data());
        mCircles[2].draw(viewId, DRAW_STATE);

        drawHandle(viewId, gizmoTransform, 0, DRAW_HANDLE_STATE);
        drawHandle(viewId, gizmoTransform, 1, DRAW_HANDLE_STATE);
        drawHandle(viewId, gizmoTransform, 2, DRAW_HANDLE_STATE);
    }

    void drawId(
        uint                  viewId,
        const vcl::Matrix44f& baseTransform,
        const vcl::Matrix44f& viewMatrix,
        const vcl::Matrix44f& projMatrix,
        ushort                meshId)
    {
        vcl::Matrix44f gizmoTransform =
            getGizmoTransform(baseTransform, viewMatrix, projMatrix);

        uint32_t xId = (0xFFFD << 16) | ((0 << 14) | meshId);
        uint32_t yId = (0xFFFD << 16) | ((1 << 14) | meshId);
        uint32_t zId = (0xFFFD << 16) | ((2 << 14) | meshId);

        drawHandle(viewId, gizmoTransform, 0, DRAW_ID_STATE, xId);
        drawHandle(viewId, gizmoTransform, 1, DRAW_ID_STATE, yId);
        drawHandle(viewId, gizmoTransform, 2, DRAW_ID_STATE, zId);
    }

    void calculateAnchor(
        ushort                                axisId,
        std::shared_ptr<AbstractDrawableMesh> mesh,
        const vcl::Matrix44f&                 viewMatrix,
        const vcl::Matrix44f&                 projMatrix)
    {
        mGizmoElementClicked = axisId; // 0, 1, or 2

        Point3d center = mesh->meshProvider().boundingBox().center();

        vcl::Matrix44f model =
            mesh->meshProvider().transformMatrix().template cast<float>();
        vcl::Matrix44f transMat = vcl::Matrix44f::Identity();
        vcl::setTransformMatrixTranslation(transMat, center.cast<float>());
        vcl::Matrix44f baseTransform = model * transMat;

        vcl::Point3f centerWorld =
            vcl::Point3f(0.0f, 0.0f, 0.0f) * baseTransform;
        vcl::Point3f centerView = centerWorld * viewMatrix;

        vcl::Point3f col0(viewMatrix(0, 0), viewMatrix(1, 0), viewMatrix(2, 0));
        float        viewScale = std::max(0.0001f, col0.norm());

        float projScale = 1.0f / std::max(0.0001f, std::abs(projMatrix(1, 1)));
        float visualScale = VISUAL_SCALE_FACTOR;
        if (projMatrix(3, 3) == 1.0f) {
            visualScale = (projScale / viewScale) * VISUAL_SCALE_FACTOR;
        }
        else {
            float depth = std::max(0.1f, std::abs(centerView.z()));
            visualScale = (depth * projScale / viewScale) * VISUAL_SCALE_FACTOR;
        }

        mRadius = visualScale;

        vcl::Matrix44d baseTransformD = baseTransform.template cast<double>();
        mStartNoScaleBase             = baseTransformD;
        mStartNoScaleBase.block<3, 1>(0, 0).normalize();
        mStartNoScaleBase.block<3, 1>(0, 1).normalize();
        mStartNoScaleBase.block<3, 1>(0, 2).normalize();

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

        Matrix44d invGizmo = mStartNoScaleBase.inverse();

        Point3d localC      = Point3d(0, 0, 0);
        Point3d centerWorld = localC * mStartNoScaleBase;

        // Convert world view normal to gizmo space direction
        Point3d localViewNormal =
            (Point3d(centerWorld + viewNormalWorld) * invGizmo - localC)
                .normalized();

        Point3d localNew  = newPoint3D * invGizmo;
        Point3d localPrev = mLastMousePos3D * invGizmo;

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
        vcl::setTransformMatrixTranslation(Tanchor, mLocalAnchorPoint);

        Matrix44d TanchorInv = Matrix44d::Identity();
        vcl::setTransformMatrixTranslation(
            TanchorInv, Point3d(-mLocalAnchorPoint));

        return originalMatrix * Tanchor * rotationMat * TanchorInv;
    }

private:
    void drawHandle(
        uint                  viewId,
        const vcl::Matrix44f& gizmoTransform,
        int                   axis,
        uint64_t              state,
        uint                  id = UINT_NULL)
    {
        vcl::Matrix44f localTransform = vcl::Matrix44f::Identity();
        float          offset         = 1.15f;
        vcl::Color color = vcl::Color::Red;

        if (axis == 0) { // Red handle (X rotation, YZ circle)
            vcl::setTransformMatrixTranslation(
                localTransform, vcl::Point3f(0.0f, offset, 0.0f));
        }
        else if (axis == 1) { // Green handle (Y rotation, XZ circle)
            color = vcl::Color::Green;
            vcl::Matrix44f rot = vcl::Matrix44f::Identity();
            vcl::setTransformMatrixRotation(
                rot, vcl::Point3f(0, 1, 0), float(M_PI / 2.0));
            vcl::Matrix44f trans = vcl::Matrix44f::Identity();
            vcl::setTransformMatrixTranslation(
                trans, vcl::Point3f(0.0f, 0.0f, offset));
            localTransform = trans * rot;
        }
        else if (axis == 2) { // Blue handle (Z rotation, XY circle)
            color = vcl::Color::Blue;
            vcl::Matrix44f rot = vcl::Matrix44f::Identity();
            vcl::setTransformMatrixRotation(
                rot, vcl::Point3f(1, 0, 0), float(-M_PI / 2.0));
            vcl::Matrix44f trans = vcl::Matrix44f::Identity();
            vcl::setTransformMatrixTranslation(
                trans, vcl::Point3f(offset, 0.0f, 0.0f));
            localTransform = trans * rot;
        }

        vcl::Matrix44f finalTransform = gizmoTransform * localTransform;

        if (id == UINT_NULL) {
            mHandleCenter.draw(viewId, color, finalTransform, state);
            mHandleCone1.draw(viewId, color, finalTransform, state);
            mHandleCone2.draw(viewId, color, finalTransform, state);
        }
        else {
            mHandleCenter.drawId(viewId, id, finalTransform, state);
            mHandleCone1.drawId(viewId, id, finalTransform, state);
            mHandleCone2.drawId(viewId, id, finalTransform, state);
        }
    }

    vcl::Matrix44f getGizmoTransform(
        const vcl::Matrix44f& baseTransform,
        const vcl::Matrix44f& viewMatrix,
        const vcl::Matrix44f& projMatrix) const
    {
        vcl::Point3f centerWorld =
            vcl::Point3f(0.0f, 0.0f, 0.0f) * baseTransform;
        vcl::Point3f centerView = centerWorld * viewMatrix;

        vcl::Point3f col0(viewMatrix(0, 0), viewMatrix(1, 0), viewMatrix(2, 0));
        float        viewScale = std::max(0.0001f, col0.norm());

        float projScale = 1.0f / std::max(0.0001f, std::abs(projMatrix(1, 1)));
        float visualScale = VISUAL_SCALE_FACTOR;
        if (projMatrix(3, 3) == 1.0f) {
            visualScale = (projScale / viewScale) * VISUAL_SCALE_FACTOR;
        }
        else {
            float depth = std::max(0.1f, std::abs(centerView.z()));
            visualScale = (depth * projScale / viewScale) * VISUAL_SCALE_FACTOR;
        }

        vcl::Matrix44f noScaleBase = baseTransform;
        noScaleBase.block<3, 1>(0, 0).normalize();
        noScaleBase.block<3, 1>(0, 1).normalize();
        noScaleBase.block<3, 1>(0, 2).normalize();

        vcl::Matrix44f scaleMat = vcl::Matrix44f::Identity();
        vcl::setTransformMatrixScale(
            scaleMat, vcl::Point3f(visualScale, visualScale, visualScale));

        return noScaleBase * scaleMat;
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
};

} // namespace vcl

#endif // VCL_BGFX_EDITORS_TRANSFORM_EDITOR_ROTATE_GIZMO_BGFX_H
