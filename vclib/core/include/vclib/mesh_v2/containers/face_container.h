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

    /**
     * @brief Adds a new face into the Face container, returning the index of the
     * added face.
     *
     * @return The index of the new face.
     */
    Index addFace() { return Base::addElement(); }

    /**
     * @brief Adds a new face with the given vertices or vertex indices into the
     * Face container, returning the index of the added face.
     *
     * @param[in] args: The vertices or vertex indices of the new face.
     * @return The index of the new face.
     */
    template<typename... Args>
    Index addFace(const Args&... args)
        requires (sizeof...(Args) >= 3 &&
                  (sizeof...(Args) == Face::VERTEX_COUNT || Face::VERTEX_COUNT < 0))
    {
        Index fid = addFace();
        face(fid).setVertices(args...);
        return fid;
    }

    /**
     * @brief Adds a new face with the vertices or vertex indices contained in
     * the given range, returning the index of the added face.
     *
     * @param[in] rng: The range of vertices or vertex indices of the new face.
     * @return The index of the new face.
     */
    template<std::ranges::input_range Rng>
    Index addFace(Rng&& rng)
    {
        Index fid = addFace();
        face(fid).setVertices(std::forward<Rng>(rng));
        return fid;
    }

    /**
     * @brief Adds an arbitrary number of n faces, returning the index of the first
     * added face.
     *
     * @param[in] n: the number of faces to add to the container.
     * @return The index of the first added face.
     */
    Index addFaces(std::size_t n) { return Base::addElements(n); }

    /**
     * @brief Adds an arbitrary number of faces with the vertices or vertex
     * indices contained in the given range of ranges, returning the index of
     * the first added face.
     *
     * @param[in] rng: A range of ranges containing the vertices or vertex
     * indices for each face.
     * @return The index of the first added face.
     */
    template<std::ranges::input_range Rng>
    requires std::ranges::input_range<std::ranges::range_value_t<Rng>>
    Index addFaces(Rng&& rng)
    {
        Index baseId = faceContainerSize();
        if constexpr (std::ranges::sized_range<Rng>) {
            reserveFaces(baseId + std::ranges::size(rng));
        }
        for (auto&& f : rng) {
            addFace(f);
        }
        return baseId;
    }

    std::vector<Index> compactFaces() { return Base::compactElements(); }

    void resizeFaces(std::size_t n) { Base::resizeElements(n); }
    void reserveFaces(std::size_t capacity) { Base::reserveElements(capacity); }
    void clearFaces() { Base::clearElements(); }

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
