// VCLib - Visual Computing Library
// Copyright (C) 2021-2026 Visual Computing Lab, ISTI - CNR.
//
// This Source Code Form is subject to the terms of the Mozilla Public License,
// v. 2.0. If a copy of the MPL was not distributed with this file, You can
// obtain one at https://mozilla.org/MPL/2.0/.

#ifndef VCL_MESH_COMPONENTS_WEDGES_WEDGE_CONTAINER_PROXY_H
#define VCL_MESH_COMPONENTS_WEDGES_WEDGE_CONTAINER_PROXY_H

#include "wedge_iterator.h"
#include "wedge_proxy.h"
#include "wedge_traits.h"

#include <vclib/base.h>
#include <vclib/base/views/view.h>

#include <cassert>
#include <concepts>
#include <utility>

namespace vcl::comp {

/**
 * @brief The WedgeContainerProxy class represents the container view of all
 * wedges (corners) of a specific Face.
 *
 * It provides:
 * 1. Range iteration over WedgeProxy (for auto w : f.wedges())
 * 2. Component views for direct column access (f.wedges().colors(), f.wedges().normals(), etc.)
 *
 * @tparam FaceType: The type of the parent Face (mutable or const).
 *
 * @ingroup elements
 */
template<typename FaceType>
class WedgeContainerProxy
{
    FaceType mFace;

public:
    WedgeContainerProxy(FaceType face) : mFace(face) {}

    /**
     * @brief Number of wedges in the face (matches vertex count).
     */
    uint size() const { return mFace.vertexCount(); }

    /**
     * @brief True if the face has no wedges.
     */
    bool empty() const { return size() == 0; }

    /**
     * @brief Access the i-th wedge proxy.
     */
    decltype(auto) operator[](this auto&& self, uint i)
    {
        return WedgeProxy<FaceType>(self.mFace, i);
    }

    /**
     * @brief Bounds-checked access to the i-th wedge proxy.
     */
    decltype(auto) at(this auto&& self, uint i)
    {
        assert(i < self.size());
        return WedgeProxy<FaceType>(self.mFace, i);
    }

    decltype(auto) front(this auto&& self) { return self[0]; }
    decltype(auto) back(this auto&& self) { return self[self.size() - 1]; }

    /* Range Iteration over WedgeProxy */

    auto begin(this auto&& self)
    {
        return WedgeIterator<FaceType>(self.mFace, 0);
    }

    auto end(this auto&& self)
    {
        return WedgeIterator<FaceType>(self.mFace, self.size());
    }

    /* Component Views */

    /**
     * @brief View of the face's wedge colors.
     */
    auto colors(this auto&& self)
        requires HasWedgeColors<FaceType>
    {
        using Comp = ComponentTypeT<CompId::WEDGE_COLORS, typename FaceType::Components>;
        auto&& d = Comp::wedgeData(self.mFace);
        return vcl::View(d.begin(), d.end());
    }

    /**
     * @brief View of the face's wedge texture coordinates.
     */
    auto texCoords(this auto&& self)
        requires HasWedgeTexCoords<FaceType>
    {
        using Comp = ComponentTypeT<CompId::WEDGE_TEX_COORDS, typename FaceType::Components>;
        auto&& d = Comp::wedgeData(self.mFace);
        return vcl::View(d.begin(), d.end());
    }

    /**
     * @brief View of the face's wedge normals.
     */
    auto normals(this auto&& self)
        requires HasWedgeNormals<FaceType>
    {
        using Comp = ComponentTypeT<CompId::WEDGE_NORMALS, typename FaceType::Components>;
        auto&& d = Comp::wedgeData(self.mFace);
        return vcl::View(d.begin(), d.end());
    }

    /* Availability checks */

    bool isColorsAvailable() const
        requires HasWedgeColors<FaceType>
    {
        return mFace.template isComponentAvailable<CompId::WEDGE_COLORS>();
    }

    bool isTexCoordsAvailable() const
        requires HasWedgeTexCoords<FaceType>
    {
        return mFace.template isComponentAvailable<CompId::WEDGE_TEX_COORDS>();
    }

    bool isNormalsAvailable() const
        requires HasWedgeNormals<FaceType>
    {
        return mFace.template isComponentAvailable<CompId::WEDGE_NORMALS>();
    }
};

} // namespace vcl::comp

#endif // VCL_MESH_COMPONENTS_WEDGES_WEDGE_CONTAINER_PROXY_H
