// VCLib - Visual Computing Library
// Copyright (C) 2021-2026 Visual Computing Lab, ISTI - CNR.
//
// This Source Code Form is subject to the terms of the Mozilla Public License,
// v. 2.0. If a copy of the MPL was not distributed with this file, You can
// obtain one at https://mozilla.org/MPL/2.0/.

#ifndef VCL_MESH_V2_CONTAINERS_FACE_CONTAINER_H
#define VCL_MESH_V2_CONTAINERS_FACE_CONTAINER_H

#include <vclib/mesh_v2/components/base/component_predicates.h>
#include <vclib/mesh_v2/containers/base/element_container.h>
#include <vclib/mesh_v2/elements/face.h>

#include <vclib/base.h>

namespace vcl::meshv2 {

/**
 * @brief FaceContainer stores the per-face data in a Structure of Arrays (SoA).
 *
 * It accepts the Face component list (satisfying HasRequiredFaceComponents),
 * instantiates the underlying storage and exposes the Face element view.
 *
 * @tparam CompList The face components type (e.g. FaceComponents<BitFlags, TriangleVertexReferences<uint>>).
 * @tparam Index The index type (default uint).
 * @tparam ParentMesh The parent mesh type (default void).
 *
 * @ingroup containers
 */
template<
    HasRequiredFaceComponents CompList,
    std::unsigned_integral Index = uint,
    typename ParentMesh          = void>
class FaceContainer :
    public ElementContainer<
        typename comp::detail::ComponentListHelper<CompList>::type,
        ParentMesh,
        Index,
        Face>
{
    using Base = ElementContainer<
        typename comp::detail::ComponentListHelper<CompList>::type,
        ParentMesh,
        Index,
        Face>;

protected:
    using Base::get;

public:
    using FaceComponents = CompList;
    using Face           = typename Base::ProxyType;
    using ConstFace      = typename Base::ConstProxyType;
    using FaceType_      = Face;
    using ConstFaceType  = ConstFace;

    template<typename NewParent>
    using Rebind = FaceContainer<CompList, Index, NewParent>;

    FaceContainer()
    {
        Base::template enable<CompId::VERTEX_REFERENCES>();
    }

    FaceContainer(ParentMesh* parent) : FaceContainer()
    {
        Base::setMesh(parent);
    }

    Face face(Index i) { return Base::element(i); }
    ConstFace face(Index i) const { return Base::element(i); }

    std::size_t faceCount() const { return Base::elementCount(); }
    std::size_t faceContainerSize() const { return Base::elementContainerSize(); }
    std::size_t deletedFaceCount() const { return Base::deletedElementCount(); }

    std::vector<Index> compactFaces() { return Base::compactElements(); }

    void resizeFaces(std::size_t n) { Base::resizeElements(n); }

    void deleteFace(Index i) { Base::deleteElement(i); }
    void deleteFace(const Face& f) { Base::deleteElement(f.index()); }

    auto faces(bool jumpDeleted = true)
    {
        return Base::elements(jumpDeleted);
    }

    auto faces(bool jumpDeleted = true) const
    {
        return Base::elements(jumpDeleted);
    }
};

} // namespace vcl::meshv2

#endif // VCL_MESH_V2_CONTAINERS_FACE_CONTAINER_H
