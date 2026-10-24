// VCLib - Visual Computing Library
// Copyright (C) 2021-2026 Visual Computing Lab, ISTI - CNR.
//
// This Source Code Form is subject to the terms of the Mozilla Public License,
// v. 2.0. If a copy of the MPL was not distributed with this file, You can
// obtain one at https://mozilla.org/MPL/2.0/.

#ifndef VCL_MESH_V2_COMPONENTS_WEDGES_H
#define VCL_MESH_V2_COMPONENTS_WEDGES_H

#include "base/component_predicates.h"
#include "base/container_component.h"
#include "color.h"
#include "normal.h"
#include "tex_coord.h"
#include "wedges/wedge_container_proxy.h"
#include "wedges/wedge_iterator.h"
#include "wedges/wedge_proxy.h"
#include "wedges/wedge_traits.h"

#include <vclib/base.h>
#include <vclib/space/core.h>

#include <cassert>
#include <concepts>
#include <utility>

namespace vcl::meshv2::comp {

/**
 * @brief The Wedges component represents a collection of per-wedge attributes
 * associated to the vertices/corners of an Element (typically Face).
 *
 * It wraps a container of attributes (e.g. Color, TexCoord, Normal) whose size
 * matches the vertex count of the face (e.g. 3 for triangles, dynamic for polygons).
 *
 * For dynamic polygons (N < 0), Wedges is automatically marked as
 * TIED_TO_VERTEX_COUNT = true, so resizing the polygon's vertices automatically
 * resizes the wedge buffers.
 *
 * Access to individual wedges is provided via Face::wedge(i) (returning WedgeProxy)
 * or Face::wedges() (returning WedgeContainerProxy).
 *
 * @tparam Comp: The base component type (e.g. Color, TexCoordf, Normal3f) or raw value type.
 * @tparam N: The number of wedges per element (default 3; negative for dynamic/polygonal faces).
 * @tparam OPT: True if the component is optional (defaults to Comp::IS_OPTIONAL if Comp is a Component).
 *
 * @ingroup components
 */
template<typename Comp, int N = 3, bool OPT = IsCompOptional<Comp>::value>
requires WedgeableComponent<Comp>
class Wedges : public ContainerComponent<
    WedgeTraits<Comp>::COMP_ID,
    typename WedgeTraits<Comp>::ValueType,
    N,
    (N < 0),
    OPT>
{
    using Base = ContainerComponent<
        WedgeTraits<Comp>::COMP_ID,
        typename WedgeTraits<Comp>::ValueType,
        N,
        (N < 0),
        OPT>;

public:
    /* Constructors */
    Wedges() = default;

    // wedge accessors (wedge() and wedges()) are provided by Face proxy
    // they cannot be provided here because Face may inherit from multiple
    // Wedges components (e.g. Color, TexCoord, Normal).

private:
    template<typename>
    friend class WedgeProxy;

    template<typename>
    friend class WedgeContainerProxy;

    /**
     * @brief Internal static accessor for friend proxies to retrieve the vector of wedges.
     */
    static decltype(auto) wedgeData(auto&& self)
    {
        return Base::compData(self);
    }

protected:
    template<typename OtherElement>
    void importFrom(this auto&& self, const OtherElement& other, bool = true);
};

/* Member function implementations */

template<typename Comp, int N, bool OPT>
requires WedgeableComponent<Comp>
template<typename OtherElement>
void Wedges<Comp, N, OPT>::importFrom(
    this auto&&         self,
    const OtherElement& other,
    bool)
{
    static constexpr uint COMP_ID = Base::COMPONENT_ID;
    using ValueType = typename Base::ValueType;

    if constexpr (HasComponent<COMP_ID, OtherElement>) {
        if (isComponentAvailableOn<COMP_ID>(other)) {
            auto&& selfContainer = Base::container(self);
            if constexpr (COMP_ID == CompId::WEDGE_COLORS) {
                if constexpr (N < 0) {
                    selfContainer.resize(other.wedges().colors().size());
                }
                const uint n = std::min(
                    (uint) selfContainer.size(),
                    (uint) other.wedges().colors().size());
                for (uint i = 0; i < n; ++i) {
                    selfContainer[i] = other.wedges().colors()[i];
                }
            }
            else if constexpr (COMP_ID == CompId::WEDGE_TEX_COORDS) {
                if constexpr (N < 0) {
                    selfContainer.resize(other.wedges().texCoords().size());
                }
                const uint n = std::min(
                    (uint) selfContainer.size(),
                    (uint) other.wedges().texCoords().size());
                for (uint i = 0; i < n; ++i) {
                    using S = typename ValueType::ScalarType;
                    selfContainer[i] =
                        other.wedges().texCoords()[i].template cast<S>();
                }
            }
            else if constexpr (COMP_ID == CompId::WEDGE_NORMALS) {
                if constexpr (N < 0) {
                    selfContainer.resize(other.wedges().normals().size());
                }
                const uint n = std::min(
                    (uint) selfContainer.size(),
                    (uint) other.wedges().normals().size());
                for (uint i = 0; i < n; ++i) {
                    using S = typename ValueType::ScalarType;
                    selfContainer[i] =
                        other.wedges().normals()[i].template cast<S>();
                }
            }
        }
    }
}

/* Component Aliases */

template<typename Comp, int N = 3, bool OPT = IsCompOptional<Comp>::value>
using Wedge = Wedges<Comp, N, OPT>;

template<int N = 3, bool OPT = false>
using WedgesColors = Wedges<Color<false>, N, OPT>;

template<int N = 3, bool OPT = false>
using WedgeColors = WedgesColors<N, OPT>;

template<typename Scalar = float, int N = 3, bool OPT = false>
using WedgesTexCoords = Wedges<TexCoord<Scalar, false>, N, OPT>;

template<typename Scalar = float, int N = 3, bool OPT = false>
using WedgeTexCoords = WedgesTexCoords<Scalar, N, OPT>;

template<typename Point = Point3f, int N = 3, bool OPT = false>
using WedgesNormals = Wedges<Normal<Point, false>, N, OPT>;

template<typename Point = Point3f, int N = 3, bool OPT = false>
using WedgeNormals = WedgesNormals<Point, N, OPT>;

// Triangle wedge aliases (N = 3)
using TriangleWedgesColors             = WedgesColors<3, false>;
using OptionalTriangleWedgesColors     = WedgesColors<3, true>;
using TriangleWedgeColors              = TriangleWedgesColors;
using OptionalTriangleWedgeColors      = OptionalTriangleWedgesColors;

using TriangleWedgesTexCoordsf         = WedgesTexCoords<float, 3, false>;
using OptionalTriangleWedgesTexCoordsf = WedgesTexCoords<float, 3, true>;
using TriangleWedgesTexCoordsd         = WedgesTexCoords<double, 3, false>;
using OptionalTriangleWedgesTexCoordsd = WedgesTexCoords<double, 3, true>;
using TriangleWedgeTexCoordsf          = TriangleWedgesTexCoordsf;
using OptionalTriangleWedgeTexCoordsf  = OptionalTriangleWedgesTexCoordsf;
using TriangleWedgeTexCoordsd          = TriangleWedgesTexCoordsd;
using OptionalTriangleWedgeTexCoordsd  = OptionalTriangleWedgesTexCoordsd;

using TriangleWedgesNormals3f          = WedgesNormals<Point3f, 3, false>;
using OptionalTriangleWedgesNormals3f  = WedgesNormals<Point3f, 3, true>;
using TriangleWedgesNormals3d          = WedgesNormals<Point3d, 3, false>;
using OptionalTriangleWedgesNormals3d  = WedgesNormals<Point3d, 3, true>;
using TriangleWedgeNormals3f           = TriangleWedgesNormals3f;
using OptionalTriangleWedgeNormals3f   = OptionalTriangleWedgesNormals3f;
using TriangleWedgeNormals3d           = TriangleWedgesNormals3d;
using OptionalTriangleWedgeNormals3d   = OptionalTriangleWedgesNormals3d;

// Polygon wedge aliases (N = -1)
using PolygonWedgesColors              = WedgesColors<-1, false>;
using OptionalPolygonWedgesColors      = WedgesColors<-1, true>;
using PolygonWedgeColors               = PolygonWedgesColors;
using OptionalPolygonWedgeColors       = OptionalPolygonWedgesColors;

using PolygonWedgesTexCoordsf          = WedgesTexCoords<float, -1, false>;
using OptionalPolygonWedgesTexCoordsf  = WedgesTexCoords<float, -1, true>;
using PolygonWedgesTexCoordsd          = WedgesTexCoords<double, -1, false>;
using OptionalPolygonWedgesTexCoordsd  = WedgesTexCoords<double, -1, true>;
using PolygonWedgeTexCoordsf           = PolygonWedgesTexCoordsf;
using OptionalPolygonWedgeTexCoordsf   = OptionalPolygonWedgesTexCoordsf;
using PolygonWedgeTexCoordsd           = PolygonWedgesTexCoordsd;
using OptionalPolygonWedgeTexCoordsd   = OptionalPolygonWedgesTexCoordsd;

using PolygonWedgesNormals3f           = WedgesNormals<Point3f, -1, false>;
using OptionalPolygonWedgesNormals3f   = WedgesNormals<Point3f, -1, true>;
using PolygonWedgesNormals3d           = WedgesNormals<Point3d, -1, false>;
using OptionalPolygonWedgesNormals3d   = WedgesNormals<Point3d, -1, true>;
using PolygonWedgeNormals3f            = PolygonWedgesNormals3f;
using OptionalPolygonWedgeNormals3f    = OptionalPolygonWedgesNormals3f;
using PolygonWedgeNormals3d            = PolygonWedgesNormals3d;
using OptionalPolygonWedgeNormals3d    = OptionalPolygonWedgesNormals3d;

} // namespace vcl::meshv2::comp

#endif // VCL_MESH_V2_COMPONENTS_WEDGES_H
