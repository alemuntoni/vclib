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

#ifndef VCL_RENDER_SETTINGS_CROSS_SECTION_SETTINGS_H
#define VCL_RENDER_SETTINGS_CROSS_SECTION_SETTINGS_H

#include <vclib/algorithms/mesh.h>
#include <vclib/mesh.h>
#include <vclib/space/core.h>

namespace vcl {

class CrossSectionSettings
{
public:
    enum class CrossSectionType { NONE, PER_VERTEX, PER_FRAGMENT };

private:
    CrossSectionType mType = CrossSectionType::NONE;

    Box3f mBBox = Box3f(Point3f::min(), Point3f::max());

    Point3f mLower = Point3f::min();
    Point3f mUpper = Point3f::max();

public:
    CrossSectionSettings() = default;

    template<MeshConcept MeshType>
    CrossSectionSettings(const MeshType& m)
    {
        mBBox = vcl::boundingBox(m).template cast<float>();
        if constexpr (HasTransformMatrix<MeshType>) {
            Matrix44f transform = m.transformMatrix().template cast<float>();
            mBBox               = transformBox(mBBox, transform);
        }

        // inflate bbox by epsilon of its diagonal
        float eps = mBBox.diagonal() * 0.02f;

        mBBox.min() -= eps;
        mBBox.max() += eps;

        mLower = mBBox.min();
        mUpper = mBBox.max();
    }

    bool isEnabled() const { return mType != CrossSectionType::NONE; }

    CrossSectionType& type() { return mType; }

    CrossSectionType type() const { return mType; }

    const Box3f& boundingBox() const { return mBBox; }

    const Point3f& lower() const { return mLower; }

    const Point3f& upper() const { return mUpper; }

    /**
     * @brief Sets the bounding box for the cross-section settings.
     *
     * The bounding box is updated to the provided value, and the lower and
     * upper points are adjusted to be within the new bounding box if necessary.
     *
     * @param[in] bbox: The new bounding box to set.
     */
    void setBoundingBox(const Box3f& bbox)
    {
        if (!bbox.isNull()) {
            mBBox = bbox;

            Point3f clampedLower =
                mLower.cwiseMax(bbox.min()).cwiseMin(bbox.max());
            Point3f clampedUpper =
                mUpper.cwiseMax(bbox.min()).cwiseMin(bbox.max());

            mLower = clampedLower.cwiseMin(clampedUpper);
            mUpper = clampedLower.cwiseMax(clampedUpper);

            if (!mLower.isStrictlyBelow(mUpper)) {
                mLower = bbox.min();
                mUpper = bbox.max();
            }
        }
        else {
            mBBox = Box3f(Point3f::min(), Point3f::max());
        }
    }

    /**
     * @brief Sets the lower point of the cross-section settings.
     *
     * The lower point is updated only if it is inside the bounding box and
     * strictly below the upper point.
     *
     * @param[in] lower: The new lower point to set.
     */
    void setLower(const Point3f& lower)
    {
        if (mBBox.isInside(lower) && lower.isStrictlyBelow(mUpper)) {
            mLower = lower;
        }
    }

    /**
     * @brief Sets the upper point of the cross-section settings.
     *
     * The upper point is updated only if it is inside the bounding box and
     * strictly above the lower point.
     *
     * @param[in] upper: The new upper point to set.
     */
    void setUpper(const Point3f& upper)
    {
        if (mBBox.isInside(upper) && upper.isStrictlyAbove(mLower)) {
            mUpper = upper;
        }
    }

    /**
     * @brief Sets both the lower and upper points of the cross-section
     * settings.
     *
     * Both points are updated only if they are inside the bounding box and
     * the lower point is strictly below the upper point.
     *
     * @param[in] lower: The new lower point to set.
     * @param[in] upper: The new upper point to set.
     */
    void setLowerUpper(const Point3f& lower, const Point3f& upper)
    {
        if (mBBox.isInside(lower) && mBBox.isInside(upper) &&
            lower.isStrictlyBelow(upper)) {
            mLower = lower;
            mUpper = upper;
        }
    }
};

} // namespace vcl

#endif // VCL_RENDER_SETTINGS_CROSS_SECTION_SETTINGS_H
