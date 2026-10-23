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
#include <vclib/mesh_v2/containers/base/custom_component_vector_handle.h>
#include <vclib/mesh_v2/containers/face_container.h>
#include <vclib/mesh_v2/containers/vertex_container.h>
#include <vclib/mesh_v2/elements/base/base.h>
#include <vclib/mesh_v2/elements/face.h>
#include <vclib/mesh_v2/elements/vertex.h>
#include <vclib/mesh_v2/mesh_components.h>

#include <array>
#include <concepts>
#include <cstring>
#include <istream>
#include <ostream>
#include <type_traits>
#include <utility>

namespace vcl::meshv2 {

/**
 * @brief The Mesh class represents a generic mesh composed of element
 * containers (VertexContainer, FaceContainer, etc.) and mesh-level components.
 *
 * The Mesh class will expose all the public members of its containers and is
 * components, and its role is to implement all the functionalities that allow
 * these containers to comunicate (e.g. an operation on the vertex container
 * that requires to update also some face information).
 *
 * Therefore, in this page are documented only the functions that in some way
 * need to modify the status of more than one Container of the Mesh. Other
 * functions are inherited by the Container classes of the Elements of the Mesh,
 * or from its Components, and depend on all the templates that compose a
 * specific Mesh definition.
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
    static constexpr bool ContainerIndexMatches = []() {
        if constexpr (ElementContainerConcept<T>) {
            return std::is_same_v<typename T::IndexType, ExpectedIndex>;
        }
        else {
            return true;
        }
    }();

    template<typename T>
    struct IsMeshComponentPred
    {
        static constexpr bool value =
            !ElementContainerConcept<T> && ComponentConcept<T>;
    };

public:
    using IndexType = typename VertexContainerType::IndexType;

    static_assert(
        (ContainerIndexMatches<Rebound<Args>, IndexType> && ...),
        "All element containers in a Mesh must share the exact same "
        "IndexType.");

    /**
     * @brief Components is an alias to a vcl::TypeWrapper that wraps all the
     * types from which the Mesh inherits (Args) that are Components (they
     * satisfy the ComponentConcept).
     */
    using Components = typename FilterTypesByCondition<
        IsMeshComponentPred,
        TypeWrapper<Rebound<Args>...>>::type;

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
     * @brief Returns true if this Mesh has a container for ELEM_ID and that
     * container contains the component COMP_ID.
     *
     * @tparam ELEM_ID The ID of the element (e.g. ElemId::VERTEX).
     * @tparam COMP_ID The ID of the component (e.g. CompId::CUSTOM_COMPONENTS).
     */
    template<uint ELEM_ID, uint COMP_ID>
    static constexpr bool hasPerElementComponent()
    {
        if constexpr (hasContainerOf<ELEM_ID>()) {
            return comp::HasComponent<COMP_ID, ContainerType<ELEM_ID>>;
        }
        else {
            return false;
        }
    }

    /* Constructors */

    /**
     * @brief Empty constructor, constructs an empty mesh.
     */
    Mesh() { setParentMeshPointersToContainers(); }

    /**
     * @brief Copy constructor: deep copies containers and components.
     *
     * @param oth: the mesh from which constructo this Mesh.
     */
    Mesh(const Mesh& other) :
            Rebound<Args>(static_cast<const Rebound<Args>&>(other))...
    {
        setParentMeshPointersToContainers();
    }

    /**
     * @brief Move constructor, moves the given mesh into this one, without any
     * other resource acquisition.
     *
     * @param other: the mesh that will be moved into this Mesh.
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
     * @param other: The mesh from which to copy or move data.
     * @return Reference to this Mesh after assignment.
     */
    Mesh& operator=(Mesh other) noexcept
    {
        swap(other);
        return *this;
    }

    /* Member functions */

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

    /* Component Enable / Disable */

    /**
     * @brief Enables the component COMP_ID on container ELEM_ID.
     */
    template<uint ELEM_ID, uint COMP_ID>
    requires (hasContainerOf<ELEM_ID>())
    void enable()
    {
        ContainerType<ELEM_ID>::template enableOptionalComponent<COMP_ID>();
    }

    /**
     * @brief Disables the component COMP_ID on container ELEM_ID.
     */
    template<uint ELEM_ID, uint COMP_ID>
    requires (hasContainerOf<ELEM_ID>())
    void disable()
    {
        ContainerType<ELEM_ID>::template disableOptionalComponent<COMP_ID>();
    }

    /**
     * @brief Checks if component COMP_ID is available on container ELEM_ID.
     */
    template<uint ELEM_ID, uint COMP_ID>
    requires (hasContainerOf<ELEM_ID>())
    bool isComponentAvailable() const
    {
        return ContainerType<ELEM_ID>::template isComponentAvailable<COMP_ID>();
    }

    /**
     * @brief Alias of isComponentAvailable.
     */
    template<uint ELEM_ID, uint COMP_ID>
    requires (hasContainerOf<ELEM_ID>())
    bool isAvailable() const
    {
        return isComponentAvailable<ELEM_ID, COMP_ID>();
    }

    /**
     * @brief Checks if component COMP_ID is enabled on container ELEM_ID.
     */
    template<uint ELEM_ID, uint COMP_ID>
    requires (hasContainerOf<ELEM_ID>())
    bool isEnabled() const
    {
        return isComponentAvailable<ELEM_ID, COMP_ID>();
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

    template<uint ELEM_ID>
    requires (hasContainerOf<ELEM_ID>())
    void enableAllOptionalComponents()
    {
        enableAll<ELEM_ID>();
    }

    template<uint ELEM_ID>
    requires (hasContainerOf<ELEM_ID>())
    void disableAllOptionalComponents()
    {
        disableAll<ELEM_ID>();
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

    void enableAllOptionalComponents() { enableAll(); }

    void disableAllOptionalComponents() { disableAll(); }

    /**
     * @brief Enables all the optional components of this Mesh that are also
     * available and enabled in the other mesh m.
     *
     * @tparam OtherMeshType The type of the mesh from which to copy component
     * states.
     * @param[in] m The other mesh to check.
     */
    template<typename OtherMeshType>
    void enableSameOptionalComponentsOf(const OtherMeshType& m)
    {
        (enableSameOptionalComponentsOfContainer<Rebound<Args>>(m), ...);
    }

    /**
     * @brief Imports all the data (elements and mesh-level components) from the
     * input Mesh m to this Mesh.
     *
     * This function calls, for each Container and Component of the mesh, its
     * importFrom function.
     *
     * @param[in] m The mesh from which to import all the data.
     */
    template<typename OtherMeshType>
    void importFrom(const OtherMeshType& m)
    {
        (importFromHelper<Rebound<Args>>(m), ...);
    }

    /**
     * @brief Appends the contents of another Mesh of the exact same type to
     * this Mesh.
     *
     * Appends the elements of all containers (vertices, faces, etc.) from the
     * input Mesh m to this Mesh, preserving active and deleted element states,
     * and shifts all topological references (e.g. face vertex indices) in the
     * newly appended elements so that they correctly point to the appended
     * elements.
     *
     * @note This operation is only allowed between meshes of the exact same type.
     *
     * @param[in] m The mesh to append to this mesh.
     */
    void append(const Mesh& m)
    {
        if (this == &m) {
            Mesh copy = m;
            append(copy);
            return;
        }

        std::array<IndexType, ElemId::ELEMENT_COUNT> initialSizes = {0};
        (saveInitialContainerSize<Rebound<Args>>(initialSizes), ...);

        (appendContainer<Rebound<Args>>(m), ...);

        (shiftReferencesOfContainer<Rebound<Args>>(initialSizes), ...);

        // TODO: When mesh::TransformMatrix is implemented, apply relative
        // transformation matrix to the newly appended vertex positions and
        // normals: matrix = this->transformMatrix().inverse() *
        // m.transformMatrix(); transform positions and normals from
        // initialSizes[ElemId::VERTEX] to end.

        // TODO: When mesh::Materials is implemented, merge materials and
        // textures and remap material indices of the newly appended faces.
    }

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
        return this->ContainerType<ELEM_ID>::isElementContainerCompact();
    }

    /**
     * @brief Compacts all containers in the mesh, removing all deleted elements
     * and updating all cross-container references.
     */
    void compact() { (compactContainer<Rebound<Args>>(), ...); }

    /**
     * @brief Compacts the container of elements having ID ELEM_ID.
     */
    template<uint ELEM_ID>
    requires (hasContainerOf<ELEM_ID>())
    auto compact()
    {
        return this->ContainerType<ELEM_ID>::compactElements();
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

    /**
     * @brief Serializes the mesh to the given binary output stream.
     *
     * Writes the binary format magic header ("vcl_mesh", 8 bytes), the format
     * version (uint32_t = 1), and then sequentially serializes all element
     * containers (with their SoA column buffers and component enable states)
     * and serializable mesh-level components.
     *
     * @param[out] os The binary output stream to write to.
     */
    void serialize(std::ostream& os) const
    {
        constexpr char MAGIC[8] = {'v', 'c', 'l', '_', 'm', 'e', 's', 'h'};
        os.write(MAGIC, 8);
        constexpr uint32_t VERSION = 1;
        vcl::serialize(os, VERSION);

        (serializeBase<Rebound<Args>>(os), ...);
    }

    /**
     * @brief Deserializes the mesh from the given binary input stream.
     *
     * Validates the binary format magic header ("vcl_mesh") and format
     * version (= 1), deserializes all element containers and mesh-level
     * components, and reconnects all child container parent pointers back
     * to this mesh.
     *
     * @param[in] is The binary input stream to read from.
     * @throws std::runtime_error If the magic header or format version is invalid.
     */
    void deserialize(std::istream& is)
    {
        char magic[8];
        is.read(magic, 8);
        if (std::memcmp(magic, "vcl_mesh", 8) != 0) {
            throw std::runtime_error(
                "Invalid mesh file header: expected 'vcl_mesh'");
        }
        uint32_t version = 0;
        vcl::deserialize(is, version);
        if (version != 1) {
            throw std::runtime_error(
                "Unsupported mesh binary format version: " +
                std::to_string(version));
        }

        (deserializeBase<Rebound<Args>>(is), ...);

        setParentMeshPointersToContainers();
    }

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
    decltype(auto) add()
    {
        return ContainerType<ELEM_ID>::addElement();
    }

    template<uint ELEM_ID>
    requires (hasContainerOf<ELEM_ID>())
    decltype(auto) add(std::size_t n)
    {
        return ContainerType<ELEM_ID>::addElements(n);
    }

    template<uint ELEM_ID>
    requires (hasContainerOf<ELEM_ID>())
    void resize(std::size_t n)
    {
        ContainerType<ELEM_ID>::resizeElements(n);
    }

    template<uint ELEM_ID>
    requires (hasContainerOf<ELEM_ID>())
    void reserve(std::size_t n)
    {
        ContainerType<ELEM_ID>::reserveElements(n);
    }

    template<uint ELEM_ID>
    requires (hasContainerOf<ELEM_ID>())
    void clear()
    {
        ContainerType<ELEM_ID>::clearElements();
    }

    template<uint ELEM_ID>
    requires (hasContainerOf<ELEM_ID>())
    void deleteElement(uint i)
    {
        ContainerType<ELEM_ID>::deleteElement(i);
    }

    /* Element operations by proxy */

    template<ElementConcept El>
    requires (hasContainerOf<std::remove_cvref_t<El>::ELEMENT_ID>())
    void deleteElement(const El& e)
    {
        deleteElement<std::remove_cvref_t<El>::ELEMENT_ID>(e.index());
    }

    /* Per-Element Custom Components */

    /**
     * @brief Checks if the Element container with ELEM_ID has a custom
     * component with the given name.
     *
     * @tparam ELEM_ID The ID of the element (e.g. ElemId::VERTEX).
     * @param[in] name The name of the custom component to check.
     * @return true if the element has the custom component, false otherwise.
     */
    template<uint ELEM_ID>
    requires (hasPerElementComponent<ELEM_ID, CompId::CUSTOM_COMPONENTS>())
    bool hasPerElementCustomComponent(const std::string& name) const
    {
        return ContainerType<ELEM_ID>::hasElemCustomComponent(name);
    }

    /**
     * @brief Returns a vector containing all the names of the custom components
     * associated to the Element with ELEM_ID.
     *
     * @tparam ELEM_ID The ID of the element.
     * @return A vector of strings with all custom component names.
     */
    template<uint ELEM_ID>
    requires (hasPerElementComponent<ELEM_ID, CompId::CUSTOM_COMPONENTS>())
    std::vector<std::string> perElementCustomComponentNames() const
    {
        return ContainerType<ELEM_ID>::elemCustomComponentNames();
    }

    /**
     * @brief Checks if the custom component of the Element with ELEM_ID is of
     * type K.
     *
     * @tparam ELEM_ID The ID of the element.
     * @tparam K The type of the custom component to check.
     * @param[in] name The name of the custom component.
     * @return true if the custom component is of type K, false otherwise.
     */
    template<uint ELEM_ID, typename K>
    requires (hasPerElementComponent<ELEM_ID, CompId::CUSTOM_COMPONENTS>())
    bool isPerElementCustomComponentOfType(const std::string& name) const
    {
        return ContainerType<ELEM_ID>::template isElemCustomComponentOfType<K>(name);
    }

    /**
     * @brief Returns the std::type_index of the custom component of the Element
     * with ELEM_ID.
     *
     * @tparam ELEM_ID The ID of the element.
     * @param[in] name The name of the custom component.
     * @return The std::type_index of the custom component.
     */
    template<uint ELEM_ID>
    requires (hasPerElementComponent<ELEM_ID, CompId::CUSTOM_COMPONENTS>())
    std::type_index perElementCustomComponentType(const std::string& name) const
    {
        return ContainerType<ELEM_ID>::elemCustomComponentType(name);
    }

    /**
     * @brief Returns a vector of strings containing the names of all custom
     * components of type K associated to the Element with ELEM_ID.
     *
     * @tparam ELEM_ID The ID of the element.
     * @tparam K The type of the custom components.
     * @return A vector of strings with custom component names of type K.
     */
    template<uint ELEM_ID, typename K>
    requires (hasPerElementComponent<ELEM_ID, CompId::CUSTOM_COMPONENTS>())
    std::vector<std::string> perElementCustomComponentNamesOfType() const
    {
        return ContainerType<ELEM_ID>::template elemCustomComponentNamesOfType<K>();
    }

    /**
     * @brief Adds a custom component of type K with the given name to the
     * Element container with ELEM_ID.
     *
     * @tparam ELEM_ID The ID of the element.
     * @tparam K The type of the custom component.
     * @param[in] name The name of the custom component to add.
     */
    template<uint ELEM_ID, typename K>
    requires (hasPerElementComponent<ELEM_ID, CompId::CUSTOM_COMPONENTS>())
    void addPerElementCustomComponent(const std::string& name)
    {
        ContainerType<ELEM_ID>::template addElemCustomComponent<K>(name);
    }

    /**
     * @brief Deletes the custom component with the given name from the Element
     * container with ELEM_ID.
     *
     * @tparam ELEM_ID The ID of the element.
     * @param[in] name The name of the custom component to delete.
     */
    template<uint ELEM_ID>
    requires (hasPerElementComponent<ELEM_ID, CompId::CUSTOM_COMPONENTS>())
    void deletePerElementCustomComponent(const std::string& name)
    {
        ContainerType<ELEM_ID>::deleteElemCustomComponent(name);
    }

    /**
     * @brief Returns a vector handle to access the custom component of type K
     * with the given name on the Element container with ELEM_ID.
     *
     * @tparam ELEM_ID The ID of the element.
     * @tparam K The type of the custom component.
     * @param[in] name The name of the custom component.
     * @return A CustomComponentVectorHandle<K>.
     */
    template<uint ELEM_ID, typename K>
    requires (hasPerElementComponent<ELEM_ID, CompId::CUSTOM_COMPONENTS>())
    CustomComponentVectorHandle<K> perElementCustomComponentVectorHandle(
        const std::string& name)
    {
        return ContainerType<ELEM_ID>::template customComponentVectorHandle<K>(name);
    }

    /**
     * @brief Returns a const vector handle to access the custom component of
     * type K with the given name on the Element container with ELEM_ID.
     *
     * @tparam ELEM_ID The ID of the element.
     * @tparam K The type of the custom component.
     * @param[in] name The name of the custom component.
     * @return A ConstCustomComponentVectorHandle<K>.
     */
    template<uint ELEM_ID, typename K>
    requires (hasPerElementComponent<ELEM_ID, CompId::CUSTOM_COMPONENTS>())
    ConstCustomComponentVectorHandle<K> perElementCustomComponentVectorHandle(
        const std::string& name) const
    {
        return ContainerType<ELEM_ID>::template customComponentVectorHandle<K>(name);
    }

    /**
     * @brief Serializes all per-element custom components of type CompType
     * for the container having ID ELEM_ID to the output stream.
     *
     * @tparam ELEM_ID The ID of the element (e.g. ElemId::VERTEX).
     * @tparam CompType The type of custom components to serialize.
     * @param[out] os The output stream.
     */
    template<uint ELEM_ID, typename CompType>
    requires (hasPerElementComponent<ELEM_ID, CompId::CUSTOM_COMPONENTS>())
    void serializePerElementCustomComponentsOfType(std::ostream& os) const
    {
        ContainerType<ELEM_ID>::template serializeElemCustomComponentsOfType<CompType>(os);
    }

    /**
     * @brief Deserializes all per-element custom components of type CompType
     * for the container having ID ELEM_ID from the input stream.
     *
     * @tparam ELEM_ID The ID of the element (e.g. ElemId::VERTEX).
     * @tparam CompType The type of custom components to deserialize.
     * @param[in] is The input stream.
     */
    template<uint ELEM_ID, typename CompType>
    requires (hasPerElementComponent<ELEM_ID, CompId::CUSTOM_COMPONENTS>())
    void deserializePerElementCustomComponentsOfType(std::istream& is)
    {
        ContainerType<ELEM_ID>::template deserializeElemCustomComponentsOfType<CompType>(is);
    }


private:
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

    template<typename T>
    void setParentMeshPointer()
    {
        if constexpr (ElementContainerConcept<T>) {
            this->T::setParentMesh(this);
        }
    }

    void setParentMeshPointersToContainers()
    {
        (setParentMeshPointer<Rebound<Args>>(), ...);
    }

    template<typename T>
    void enableAllContainer()
    {
        if constexpr (ElementContainerConcept<T>) {
            this->T::enableAllOptionalComponents();
        }
    }

    template<typename T>
    void disableAllContainer()
    {
        if constexpr (ElementContainerConcept<T>) {
            this->T::disableAllOptionalComponents();
        }
    }

    template<typename Cont, typename OtherMeshType>
    void enableSameOptionalComponentsOfContainer(const OtherMeshType& m)
    {
        if constexpr (ElementContainerConcept<Cont>) {
            if constexpr (OtherMeshType::template hasContainerOf<Cont::ELEMENT_ID>()) {
                this->Cont::enableSameOptionalComponentsOf(
                    m.template container<Cont::ELEMENT_ID>());
            }
        }
    }

    template<typename BaseType, typename OtherMeshType>
    void importFromHelper(const OtherMeshType& m)
    {
        this->BaseType::importFrom(m);
    }

    template<typename Cont>
    void saveInitialContainerSize(
        std::array<IndexType, ElemId::ELEMENT_COUNT>& sizes) const
    {
        if constexpr (ElementContainerConcept<Cont>) {
            sizes[Cont::ELEMENT_ID] = this->Cont::elementContainerSize();
        }
    }

    template<typename Cont>
    void appendContainer(const Mesh& m)
    {
        if constexpr (ElementContainerConcept<Cont>) {
            this->Cont::append(static_cast<const Cont&>(m));
        }
    }

    template<typename Cont, uint TARGET_ELEM_ID>
    void shiftContainerReferences(IndexType startFrom, IndexType offset)
    {
        if constexpr (ElementContainerConcept<Cont>) {
            this->Cont::template shiftReferences<TARGET_ELEM_ID>(
                startFrom, offset);
        }
    }

    template<typename Cont>
    void shiftReferencesOfContainer(
        const std::array<IndexType, ElemId::ELEMENT_COUNT>& initialSizes)
    {
        if constexpr (ElementContainerConcept<Cont>) {
            const IndexType startFrom = initialSizes[Cont::ELEMENT_ID];
            [&]<uint... TargetIDs>(std::integer_sequence<uint, TargetIDs...>) {
                (shiftContainerReferences<Cont, TargetIDs>(
                     startFrom, initialSizes[TargetIDs]),
                 ...);
            }(std::make_integer_sequence<uint, ElemId::ELEMENT_COUNT>{});
        }
    }

    template<typename T>
    bool isContainerCompact() const
    {
        if constexpr (ElementContainerConcept<T>) {
            return this->T::deletedElementCount() == 0;
        }
        else {
            return true;
        }
    }

    template<typename T>
    void clearContainer()
    {
        if constexpr (ElementContainerConcept<T>) {
            this->T::clearElements();
        }
    }

    template<typename T>
    void compactContainer()
    {
        if constexpr (ElementContainerConcept<T>) {
            this->T::compactElements();
        }
    }

    template<typename T, uint ELEM_ID>
    void updateContainerReferences(const std::vector<IndexType>& newIndices)
    {
        if constexpr (ElementContainerConcept<T>) {
            this->T::template updateReferences<ELEM_ID>(newIndices);
        }
    }

    template<typename T>
    bool isContainerEmpty() const
    {
        if constexpr (ElementContainerConcept<T>) {
            return this->T::elementCount() == 0;
        }
        else {
            return true;
        }
    }

    template<typename T>
    void serializeBase(std::ostream& os) const
    {
        if constexpr (requires { this->T::serialize(os); }) {
            this->T::serialize(os);
        }
    }

    template<typename T>
    void deserializeBase(std::istream& is)
    {
        if constexpr (requires { this->T::deserialize(is); }) {
            this->T::deserialize(is);
        }
    }

    template<typename...>
    friend class Mesh;

    template<
        typename ComponentList,
        typename ParentMesh,
        std::unsigned_integral Index,
        template<typename...> class Element>
    friend class ElementContainer;
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
 * @brief Concept that checks whether a Mesh has a container for the given
 * ELEM_ID with the given component COMP_ID.
 */
template<typename MeshType, uint ELEM_ID, uint COMP_ID>
concept HasPerElementComponent =
    HasContainerOf<MeshType, ELEM_ID> &&
    comp::HasComponent<COMP_ID, typename MeshType::template ContainerType<ELEM_ID>>;

/**
 * @brief Concept that checks whether a Mesh has per-element custom components
 * on container ELEM_ID.
 */
template<typename MeshType, uint ELEM_ID>
concept HasPerElementCustomComponents =
    HasPerElementComponent<MeshType, ELEM_ID, CompId::CUSTOM_COMPONENTS>;

/**
 * @brief Concept that checks whether a Mesh has per-vertex custom components.
 */
template<typename MeshType>
concept HasPerVertexCustomComponents =
    HasPerElementCustomComponents<MeshType, ElemId::VERTEX>;

/**
 * @brief Concept that checks whether a Mesh has per-face custom components.
 */
template<typename MeshType>
concept HasPerFaceCustomComponents =
    HasPerElementCustomComponents<MeshType, ElemId::FACE>;

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
