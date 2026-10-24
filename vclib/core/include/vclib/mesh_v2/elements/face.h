// VCLib - Visual Computing Library
// Copyright (C) 2021-2026 Visual Computing Lab, ISTI - CNR.
//
// This Source Code Form is subject to the terms of the Mozilla Public License,
// v. 2.0. If a copy of the MPL was not distributed with this file, You can
// obtain one at https://mozilla.org/MPL/2.0/.

#ifndef VCL_MESH_V2_ELEMENTS_FACE_H
#define VCL_MESH_V2_ELEMENTS_FACE_H

#include "face_components.h"

#include <vclib/mesh_v2/components/bit_flags.h>
#include <vclib/mesh_v2/components/vertex_references.h>
#include <vclib/mesh_v2/elements/base/base.h>
#include <vclib/mesh_v2/elements/base/element_proxy.h>

#include <ranges>

namespace vcl::meshv2 {

/**
 * @brief The Face class represents the runtime proxy (view) of a
 * Face element in a mesh container.
 *
 * It inherits from ElementProxy and provides object-oriented access to
 * the per-face components stored in the container's Structure of Arrays.
 *
 * @tparam ContainerType: The type of the container (mutable or const).
 *
 * @ingroup elements
 */
template<typename ContainerType>
class Face : public ElementProxy<ElemId::FACE, ContainerType>
{
    using Base = ElementProxy<ElemId::FACE, ContainerType>;

public:
    using typename Base::Components;
    using typename Base::IndexType;
    using Base::NULL_INDEX;

    static constexpr int VERTEX_COUNT =
        comp::ComponentTypeT<CompId::VERTEX_REFERENCES, Components>::VERTEX_COUNT;

    static_assert(
        comp::HasVertexReferences<Components>,
        "A Face must have the VertexReferences component (e.g. "
        "TriangleVertexReferences).");
    static_assert(
        comp::HasBitFlags<Components>,
        "A Face must have the BitFlags component "
        "(vcl::meshv2::comp::BitFlags).");

    Face() = default;

    // Implicit conversion constructor from mutable Face to const Face
    template<typename OtherContainerType>
    requires std::is_same_v<ContainerType, const OtherContainerType>
    Face(const Face<OtherContainerType>& other) : Base(other)
    {
    }

    template<typename OtherElement>
    void importFrom(const OtherElement& other, bool importRefs = true)
        requires (!std::is_const_v<ContainerType>)
    {
        if constexpr (
            comp::HasPolygonVertexReferences<Components> &&
            comp::HasVertexReferences<OtherElement>) {
            this->resizeVertices(other.vertexCount());
        }
        Base::importFrom(other, importRefs);
    }

    /**
     * @brief Returns a proxy to the i-th wedge (corner) of the face.
     * @param[in] i: The index of the wedge (0 <= i < vertexCount()).
     * @return A WedgeProxy for the i-th corner.
     */
    decltype(auto) wedge(this auto&& self, uint i)
    {
        assert(i < self.vertexCount());
        return comp::WedgeProxy(self, i);
    }

    /**
     * @brief Returns the container proxy for all wedges of the face.
     * Allows iterating over wedges (for auto w : f.wedges()) or direct
     * column container access (for auto& n : f.wedges().normals()).
     * @return A WedgeContainerProxy for this face.
     */
    decltype(auto) wedges(this auto&& self)
    {
        return comp::WedgeContainerProxy(self);
    }

protected:
    Face(ContainerType& container, IndexType index) : Base(container, index) {}

private:

    /// friend classes declarations

    template<uint, typename, typename>
    friend class ElementProxy;

    template<
        typename,
        typename,
        std::unsigned_integral,
        template<typename...> class>
    friend class ElementContainer;

    template<typename>
    friend class ElementIterator;

    template<typename>
    friend class Face;
};

/**
 * @brief The FaceComponents struct defines a type-wrapper for the components
 * of a Face, checking at compile-time that the required components (BitFlags,
 * VertexReferences) are present.
 *
 * @tparam Comps: The components that compose the Face.
 *
 * @ingroup elements
 */
template<typename... Comps>
struct FaceComponents : vcl::TypeWrapper<Comps...>
{
    static_assert(
        comp::HasVertexReferences<vcl::TypeWrapper<Comps...>>,
        "Face components must include VertexReferences (e.g. "
        "TriangleVertexReferences).");
    static_assert(
        comp::HasBitFlags<vcl::TypeWrapper<Comps...>>,
        "Face components must include BitFlags (vcl::meshv2::comp::BitFlags).");
};

template<typename... Comps>
struct FaceComponents<vcl::TypeWrapper<Comps...>> : FaceComponents<Comps...>
{
};

/* Concepts */

/**
 * @brief Concept that checks whether a type T is a Face element.
 *
 * @ingroup face_concepts
 */
template<typename T>
concept FaceConcept =
    ElementConcept<T> && (std::remove_cvref_t<T>::ELEMENT_ID == ElemId::FACE) &&
    comp::HasBitFlags<typename std::remove_cvref_t<T>::Components> &&
    comp::HasVertexReferences<typename std::remove_cvref_t<T>::Components>;

/**
 * @brief Concept that checks whether a type T is a triangular Face element.
 *
 * @ingroup face_concepts
 */
template<typename T>
concept TriangleFaceConcept = FaceConcept<T> && requires {
    requires std::remove_cvref_t<T>::VERTEX_COUNT == 3;
};

/**
 * @brief Concept that checks whether a type T is a valid component list for a
 * Face.
 *
 * @ingroup face_concepts
 */
template<typename T>
concept HasRequiredFaceComponents =
    comp::HasBitFlags<T> && comp::HasVertexReferences<T>;

} // namespace vcl::meshv2

#endif // VCL_MESH_V2_ELEMENTS_FACE_H
