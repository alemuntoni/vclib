// VCLib - Visual Computing Library
// Copyright (C) 2021-2026 Visual Computing Lab, ISTI - CNR.
//
// This Source Code Form is subject to the terms of the Mozilla Public License,
// v. 2.0. If a copy of the MPL was not distributed with this file, You can
// obtain one at https://mozilla.org/MPL/2.0/.

#ifndef VCL_MESH_COMPONENTS_WEDGES_WEDGE_PROXY_H
#define VCL_MESH_COMPONENTS_WEDGES_WEDGE_PROXY_H

#include "wedge_traits.h"

#include <cassert>

namespace vcl {
namespace comp {

/**
 * @brief The WedgeProxy class represents a proxy (view) of a single wedge (corner)
 * of a Face element.
 *
 * It provides object-oriented access to all attributes associated with this wedge/corner:
 * - Topological vertex and vertex index
 * - Color (if HasWedgeColors<FaceType>)
 * - TexCoord (if HasWedgeTexCoords<FaceType>)
 * - Normal (if HasWedgeNormals<FaceType>)
 *
 * It automatically propagates constness from the parent Face.
 *
 * @tparam FaceType: The type of the parent Face (mutable or const).
 *
 * @ingroup elements
 */
template<typename FaceType>
class WedgeProxy
{
    FaceType mFace;
    uint     mIndex = 0;

public:
    WedgeProxy() = default;

    WedgeProxy(FaceType face, uint index) : mFace(face), mIndex(index) {}

    /**
     * @brief Returns the local corner index of this wedge in the face (0, 1, 2...).
     */
    uint index() const { return mIndex; }

    /**
     * @brief Returns a reference to the parent face.
     */
    decltype(auto) parentFace(this auto&& self) { return (self.mFace); }

    /**
     * @brief Returns a proxy/reference to the vertex associated with this wedge corner.
     */
    decltype(auto) vertex(this auto&& self)
    {
        return self.mFace.vertex(self.mIndex);
    }

    /**
     * @brief Returns the index of the vertex associated with this wedge corner.
     */
    uint vertexIndex() const
    {
        return mFace.vertexIndex(mIndex);
    }

    /**
     * @brief Returns a reference to the color of this wedge.
     */
    decltype(auto) color(this auto&& self)
        requires HasWedgeColors<FaceType>
    {
        using Comp = ComponentTypeT<CompId::WEDGE_COLORS, typename FaceType::Components>;
        return Comp::wedgeData(self.mFace)[self.mIndex];
    }

    /**
     * @brief Returns a reference to the texture coordinate of this wedge.
     */
    decltype(auto) texCoord(this auto&& self)
        requires HasWedgeTexCoords<FaceType>
    {
        using Comp = ComponentTypeT<CompId::WEDGE_TEX_COORDS, typename FaceType::Components>;
        return Comp::wedgeData(self.mFace)[self.mIndex];
    }

    /**
     * @brief Returns a reference to the normal of this wedge.
     */
    decltype(auto) normal(this auto&& self)
        requires HasWedgeNormals<FaceType>
    {
        using Comp = ComponentTypeT<CompId::WEDGE_NORMALS, typename FaceType::Components>;
        return Comp::wedgeData(self.mFace)[self.mIndex];
    }

    /**
     * @brief Checks if color is available on this wedge.
     */
    bool isColorAvailable() const
        requires HasWedgeColors<FaceType>
    {
        return mFace.template isComponentAvailable<CompId::WEDGE_COLORS>();
    }

    /**
     * @brief Checks if texture coordinate is available on this wedge.
     */
    bool isTexCoordAvailable() const
        requires HasWedgeTexCoords<FaceType>
    {
        return mFace.template isComponentAvailable<CompId::WEDGE_TEX_COORDS>();
    }

    /**
     * @brief Checks if normal is available on this wedge.
     */
    bool isNormalAvailable() const
        requires HasWedgeNormals<FaceType>
    {
        return mFace.template isComponentAvailable<CompId::WEDGE_NORMALS>();
    }
};

/**
 * @brief Concept that checks whether a type T is an instance of WedgeProxy.
 *
 * @tparam T: The type to check.
 * @ingroup element_concepts
 */
template<typename T>
concept WedgeConcept =
    IsSpecializationOfV<std::remove_cvref_t<T>, WedgeProxy>;

} // namespace comp

using comp::WedgeConcept;

} // namespace vcl

#endif // VCL_MESH_COMPONENTS_WEDGES_WEDGE_PROXY_H
