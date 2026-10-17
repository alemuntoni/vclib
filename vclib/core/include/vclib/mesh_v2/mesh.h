// VCLib - Visual Computing Library
// Copyright (C) 2021-2026 Visual Computing Lab, ISTI - CNR.
//
// This Source Code Form is subject to the terms of the Mozilla Public License,
// v. 2.0. If a copy of the MPL was not distributed with this file, You can
// obtain one at https://mozilla.org/MPL/2.0/.

#ifndef VCL_MESH_V2_MESH_H
#define VCL_MESH_V2_MESH_H

#include <vclib/base.h>
#include <vclib/mesh_v2/containers/base/base.h>
#include <vclib/mesh_v2/containers/face_container.h>
#include <vclib/mesh_v2/containers/vertex_container.h>
#include <vclib/mesh_v2/elements/base/base.h>
#include <vclib/mesh_v2/elements/face.h>
#include <vclib/mesh_v2/elements/vertex.h>

#include <concepts>
#include <type_traits>
#include <utility>

namespace vcl::meshv2 {

/**
 * @brief The Mesh class represents a generic mesh composed of element
 * containers (VertexContainer, FaceContainer, etc.) and optional mesh-level
 * components.
 *
 * Mesh serves as the unifying glue:
 * - Automatically rebinds containers with its own type as ParentMesh so that
 *   element proxies (e.g. Face) can seamlessly navigate across containers (e.g.
 *   face.vertex(i)).
 * - Coordinates whole-mesh operations such as isCompact(), clear(), empty(),
 *   and component enablement across all containers.
 * - Provides generic access by ELEM_ID (container<ELEM_ID>(),
 * element<ELEM_ID>(), elements<ELEM_ID>(), count<ELEM_ID>()).
 *
 * @tparam Args Container types (e.g. VertexContainer<...>, FaceContainer<...>)
 * and mesh-level components.
 *
 * @ingroup mesh
 */
template<typename... Args>
class Mesh : public detail::RebindContainer<Args, Mesh<Args...>>...
{
    template<typename T>
    using Rebound = detail::RebindContainer<T, Mesh<Args...>>;

    static_assert(
        detail::FindContainer<ElemId::VERTEX, Rebound<Args>...>::found,
        "A Mesh must have a VertexContainer.");

    using VertexContainerType =
        typename detail::FindContainer<ElemId::VERTEX, Rebound<Args>...>::type;

    template<typename T, typename ExpectedIndex>
    static constexpr bool ContainerIndexMatches =
        (!requires { typename T::IndexType; }) ||
        std::is_same_v<typename T::IndexType, ExpectedIndex>;

public:
    using IndexType = typename VertexContainerType::IndexType;

    static_assert(
        (ContainerIndexMatches<Rebound<Args>, IndexType> && ...),
        "All element containers in a Mesh must share the exact same IndexType.");

    /**
     * @brief Constructs an empty Mesh and connects all child containers.
     */
    Mesh() { setParentMeshPointersToContainers(); }

    /**
     * @brief Copy constructor: deep copies containers and resets parent
     * pointers.
     */
    Mesh(const Mesh& other) :
            Rebound<Args>(static_cast<const Rebound<Args>&>(other))...
    {
        setParentMeshPointersToContainers();
    }

    /**
     * @brief Move constructor: moves containers and resets parent pointers.
     */
    Mesh(Mesh&& other) noexcept :
            Rebound<Args>(static_cast<Rebound<Args>&&>(other))...
    {
        setParentMeshPointersToContainers();
        other.setParentMeshPointersToContainers();
    }

    /**
     * @brief Assignment operator of the Mesh using the Copy & Swap idiom.
     * Handles both copy assignment and move assignment seamlessly and provides
     * the strong exception safety guarantee.
     *
     * @param other The mesh from which to copy or move data.
     * @return Reference to this Mesh after assignment.
     */
    Mesh& operator=(Mesh other) noexcept
    {
        swap(other);
        return *this;
    }

    /**
     * @brief Swaps the content of this Mesh with another.
     */
    void swap(Mesh& other) noexcept
    {
        using std::swap;
        (swap(
             static_cast<Rebound<Args>&>(*this),
             static_cast<Rebound<Args>&>(other)),
         ...);
        setParentMeshPointersToContainers();
        other.setParentMeshPointersToContainers();
    }

    friend void swap(Mesh& a, Mesh& b) noexcept { a.swap(b); }

    /* Container introspection and retrieval */

    /**
     * @brief Checks at compile-time if this Mesh has a container for ELEM_ID.
     */
    template<uint ELEM_ID>
    static constexpr bool hasContainerOf()
    {
        return detail::FindContainer<ELEM_ID, Rebound<Args>...>::found;
    }

    /**
     * @brief Type alias for the container of the given ELEM_ID.
     */
    template<uint ELEM_ID>
    using ContainerType =
        typename detail::FindContainer<ELEM_ID, Rebound<Args>...>::type;

    /**
     * @brief Returns a reference to the container for the given ELEM_ID.
     */
    template<uint ELEM_ID>
    requires (hasContainerOf<ELEM_ID>())
    ContainerType<ELEM_ID>& container()
    {
        return static_cast<ContainerType<ELEM_ID>&>(*this);
    }

    /**
     * @brief Returns a const reference to the container for the given ELEM_ID.
     */
    template<uint ELEM_ID>
    requires (hasContainerOf<ELEM_ID>())
    const ContainerType<ELEM_ID>& container() const
    {
        return static_cast<const ContainerType<ELEM_ID>&>(*this);
    }

    /* Component Enable / Disable */

    /**
     * @brief Enables the component COMP_ID on container ELEM_ID.
     */
    template<uint ELEM_ID, uint COMP_ID>
    requires (hasContainerOf<ELEM_ID>())
    void enable()
    {
        ContainerType<ELEM_ID>::template enable<COMP_ID>();
    }

    /**
     * @brief Disables the component COMP_ID on container ELEM_ID.
     */
    template<uint ELEM_ID, uint COMP_ID>
    requires (hasContainerOf<ELEM_ID>())
    void disable()
    {
        ContainerType<ELEM_ID>::template disable<COMP_ID>();
    }

    /**
     * @brief Checks if component COMP_ID is enabled on container ELEM_ID.
     */
    template<uint ELEM_ID, uint COMP_ID>
    requires (hasContainerOf<ELEM_ID>())
    bool isEnabled() const
    {
        return ContainerType<ELEM_ID>::template isEnabled<COMP_ID>();
    }

    /**
     * @brief Enables all components on container ELEM_ID.
     */
    template<uint ELEM_ID>
    requires (hasContainerOf<ELEM_ID>())
    void enableAll()
    {
        ContainerType<ELEM_ID>::enableAll();
    }

    /**
     * @brief Disables all non-mandatory components on container ELEM_ID.
     */
    template<uint ELEM_ID>
    requires (hasContainerOf<ELEM_ID>())
    void disableAll()
    {
        ContainerType<ELEM_ID>::disableAll();
    }

    /**
     * @brief Enables all components across all element containers.
     */
    void enableAll() { (enableAllContainer<Rebound<Args>>(), ...); }

    /**
     * @brief Disables all non-mandatory components across all element
     * containers.
     */
    void disableAll() { (disableAllContainer<Rebound<Args>>(), ...); }

    /* Unified mesh state */

    /**
     * @brief Checks if all containers in the mesh are compact (no deleted
     * elements).
     */
    bool isCompact() const
    {
        return (isContainerCompact<Rebound<Args>>() && ...);
    }

    /**
     * @brief Checks if the container of elements having ID ELEM_ID is compact.
     */
    template<uint ELEM_ID>
    requires (hasContainerOf<ELEM_ID>())
    bool isCompact() const
    {
        return this->ContainerType<ELEM_ID>::isCompact();
    }

    /**
     * @brief Compacts all containers in the mesh, removing all deleted elements
     * and updating all cross-container references.
     */
    void compact()
    {
        (compactContainer<Rebound<Args>>(), ...);
    }

    /**
     * @brief Compacts the container of elements having ID ELEM_ID.
     */
    template<uint ELEM_ID>
    requires (hasContainerOf<ELEM_ID>())
    auto compact()
    {
        return this->ContainerType<ELEM_ID>::compact();
    }

    /**
     * @brief Broadcasts updated element indices of ELEM_ID to all containers
     * so that any referencing components are updated.
     */
    template<uint ELEM_ID>
    void updateAllReferences(const std::vector<IndexType>& newIndices)
    {
        (updateContainerReferences<Rebound<Args>, ELEM_ID>(newIndices), ...);
    }

    /**
     * @brief Clears all elements across all containers in the mesh.
     */
    void clear() { (clearContainer<Rebound<Args>>(), ...); }

    /**
     * @brief Checks if all containers in the mesh are empty.
     */
    bool empty() const { return (isContainerEmpty<Rebound<Args>>() && ...); }

    /* Generic element access by ELEM_ID */

    template<uint ELEM_ID>
    requires (hasContainerOf<ELEM_ID>())
    decltype(auto) element(uint i)
    {
        return ContainerType<ELEM_ID>::element(i);
    }

    template<uint ELEM_ID>
    requires (hasContainerOf<ELEM_ID>())
    decltype(auto) element(uint i) const
    {
        return ContainerType<ELEM_ID>::element(i);
    }

    template<uint ELEM_ID>
    requires (hasContainerOf<ELEM_ID>())
    decltype(auto) elements(bool jumpDeleted = true)
    {
        return ContainerType<ELEM_ID>::elements(jumpDeleted);
    }

    template<uint ELEM_ID>
    requires (hasContainerOf<ELEM_ID>())
    decltype(auto) elements(bool jumpDeleted = true) const
    {
        return ContainerType<ELEM_ID>::elements(jumpDeleted);
    }

    template<uint ELEM_ID>
    requires (hasContainerOf<ELEM_ID>())
    std::size_t count() const
    {
        return ContainerType<ELEM_ID>::elementCount();
    }

    template<uint ELEM_ID>
    requires (hasContainerOf<ELEM_ID>())
    std::size_t containerSize() const
    {
        return ContainerType<ELEM_ID>::elementContainerSize();
    }

    template<uint ELEM_ID>
    requires (hasContainerOf<ELEM_ID>())
    std::size_t deletedCount() const
    {
        return ContainerType<ELEM_ID>::deletedElementCount();
    }

    template<uint ELEM_ID>
    requires (hasContainerOf<ELEM_ID>())
    void resize(std::size_t n)
    {
        ContainerType<ELEM_ID>::resize(n);
    }

    template<uint ELEM_ID>
    requires (hasContainerOf<ELEM_ID>())
    void reserve(std::size_t n)
    {
        ContainerType<ELEM_ID>::reserve(n);
    }

    template<uint ELEM_ID>
    requires (hasContainerOf<ELEM_ID>())
    void deleteElement(uint i)
    {
        ContainerType<ELEM_ID>::deleteElement(i);
    }

    template<uint ELEM_ID>
    requires (hasContainerOf<ELEM_ID>())
    bool isDeleted(uint i) const
    {
        return ContainerType<ELEM_ID>::isDeleted(i);
    }

    /* Element operations by proxy */

    template<ElementConcept El>
    requires (hasContainerOf<std::remove_cvref_t<El>::ELEMENT_ID>())
    void deleteElement(const El& e)
    {
        deleteElement<std::remove_cvref_t<El>::ELEMENT_ID>(e.index());
    }

    template<ElementConcept El>
    requires (hasContainerOf<std::remove_cvref_t<El>::ELEMENT_ID>())
    bool isDeleted(const El& e) const
    {
        return isDeleted<std::remove_cvref_t<El>::ELEMENT_ID>(e.index());
    }

private:
    template<typename T>
    void setParentMeshPointer()
    {
        if constexpr (requires { this->T::setMesh(this); }) {
            this->T::setMesh(this);
        }
    }

    void setParentMeshPointersToContainers()
    {
        (setParentMeshPointer<Rebound<Args>>(), ...);
    }

    template<typename T>
    void enableAllContainer()
    {
        if constexpr (requires { this->T::enableAll(); }) {
            this->T::enableAll();
        }
    }

    template<typename T>
    void disableAllContainer()
    {
        if constexpr (requires { this->T::disableAll(); }) {
            this->T::disableAll();
        }
    }

    template<typename T>
    bool isContainerCompact() const
    {
        if constexpr (requires {
                          {
                              this->T::deletedElementCount()
                          } -> std::convertible_to<std::size_t>;
                      }) {
            return this->T::deletedElementCount() == 0;
        }
        else {
            return true;
        }
    }

    template<typename T>
    void clearContainer()
    {
        if constexpr (requires { this->T::clear(); }) {
            this->T::clear();
        }
    }

    template<typename T>
    void compactContainer()
    {
        if constexpr (requires { this->T::compact(); }) {
            this->T::compact();
        }
    }

    template<typename T, uint ELEM_ID>
    void updateContainerReferences(const std::vector<IndexType>& newIndices)
    {
        if constexpr (requires { this->T::template updateReferences<ELEM_ID>(newIndices); }) {
            this->T::template updateReferences<ELEM_ID>(newIndices);
        }
    }

    template<typename T>
    bool isContainerEmpty() const
    {
        if constexpr (requires {
                          {
                              this->T::elementCount()
                          } -> std::convertible_to<std::size_t>;
                      }) {
            return this->T::elementCount() == 0;
        }
        else {
            return true;
        }
    }
};

/* Mesh Concepts */

/**
 * @brief Concept that checks whether a Mesh has a container for the given
 * ELEM_ID.
 */
template<typename MeshType, uint ELEM_ID>
concept HasContainerOf =
    requires { requires MeshType::template hasContainerOf<ELEM_ID>(); };

/**
 * @brief Concept that checks whether a Mesh has a VertexContainer.
 */
template<typename MeshType>
concept HasVertices = HasContainerOf<MeshType, ElemId::VERTEX>;

/**
 * @brief Concept that checks whether a Mesh has a FaceContainer.
 */
template<typename MeshType>
concept HasFaces = HasContainerOf<MeshType, ElemId::FACE>;

/**
 * @brief Concept that checks whether a Mesh has an EdgeContainer.
 */
template<typename MeshType>
concept HasEdges = HasContainerOf<MeshType, ElemId::EDGE>;

/**
 * @brief Concept that checks whether a Mesh has triangular faces.
 */
template<typename MeshType>
concept HasTriangles = HasFaces<MeshType> && requires {
    typename MeshType::Face;
    requires TriangleFaceConcept<typename MeshType::Face>;
};

/**
 * @brief Concept that checks whether a type T satisfies the Mesh concept.
 */
template<typename T>
concept MeshConcept = requires (T& m) {
    { m.isCompact() } -> std::same_as<bool>;
    { m.empty() } -> std::same_as<bool>;
    m.clear();
};

} // namespace vcl::meshv2

#endif // VCL_MESH_V2_MESH_H
