// VCLib - Visual Computing Library
// Copyright (C) 2021-2026 Visual Computing Lab, ISTI - CNR.
//
// This Source Code Form is subject to the terms of the Mozilla Public License,
// v. 2.0. If a copy of the MPL was not distributed with this file, You can
// obtain one at https://mozilla.org/MPL/2.0/.

#ifndef VCL_MESH_V2_CONTAINERS_BASE_ELEMENT_CONTAINER_H
#define VCL_MESH_V2_CONTAINERS_BASE_ELEMENT_CONTAINER_H

#include "base.h"
#include "component_view.h"
#include "custom_component_vector_handle.h"
#include "custom_components_vector_map.h"
#include "element_container_proxy.h"

#include <vclib/mesh_v2/components/base/component.h>
#include <vclib/mesh_v2/components/base/component_predicates.h>
#include <vclib/mesh_v2/components/bit_flags.h>
#include <vclib/mesh_v2/components/custom_components.h>
#include <vclib/mesh_v2/components/vertex_references.h>
#include <vclib/mesh_v2/elements/base/element_proxy.h>
#include <vclib/mesh_v2/iterators/element_iterator.h>

#include <vclib/base.h>
#include <vclib/space/core.h>

#include <algorithm>
#include <cassert>
#include <concepts>
#include <istream>
#include <ostream>

namespace vcl::meshv2 {

/**
 * @brief The ElementContainer class is the low-level base container that
 * manages the Structure of Arrays (SoA) storage for a specific mesh element
 * type.
 *
 * It acts as the backbone for concrete element containers (such as
 * VertexContainer, FaceContainer, and EdgeContainer). For each component in the
 * ComponentList, ElementContainer maintains a dedicated, contiguous vector in a
 * ParallelVectorsTuple.
 *
 * Key responsibilities:
 * - Manages memory allocation, resizing, and element count tracking (both total
 *   allocated size and active non-deleted count).
 * - Tracks deleted elements via the mandatory BitFlags component and provides
 *   compacting routines (compactElements, elementCompactIndices).
 * - Controls dynamic enabling and disabling of optional components.
 * - Coordinates topological updates and reference remapping across containers
 *   when elements are compacted (updateReferences).
 * - Exposes low-level access to elements as strongly-typed flyweight proxies
 *   via the template template parameter Element (e.g. Vertex, Face).
 *
 * @tparam ComponentList: A vcl::TypeWrapper wrapping the list of components
 * that compose the element (e.g. VertexComponents, FaceComponents). Each
 * component determines a parallel vector column in the underlying SoA storage.
 * Must include the BitFlags component.
 *
 * @tparam ParentMesh: The enclosing Mesh class type to which this container
 * belongs, or void if used as an unbound placeholder prior to mesh rebinding.
 * Allows element proxies to navigate across containers (e.g. from Face to
 * Vertex) through container.parentMesh().
 *
 * @tparam Index: An unsigned integral type used for element indices.
 * Determines index representation width across the container (e.g. uint32_t
 * or uint64_t).
 *
 * @tparam Element: A template template parameter representing the concrete
 * element proxy class template (e.g. Vertex, Face, Edge), instantiated
 * internally as Element<ElementContainer> (ElementType) and
 * Element<const ElementContainer> (ConstElementType). This allows
 * ElementContainer and its iterators to instantiate and return strongly-typed
 * element proxies directly without intermediate type erasure.
 *
 * @ingroup containers
 */
template<
    typename ComponentList,
    typename ParentMesh,
    std::unsigned_integral Index,
    template<typename...> class Element>
class ElementContainer
{
    static_assert(
        comp::HasBitFlags<ComponentList>,
        "An Element must have the BitFlags component "
        "(vcl::meshv2::comp::BitFlags).");

    using VectorComponents = typename FilterTypesByCondition<
        detail::HasVectorColumnPred,
        ComponentList>::type;

    using VectorTupleType = typename detail::ParallelVectorsTupleFromComponents<
        VectorComponents>::type;

    ParentMesh* mParentMesh = nullptr;

    /**
     * @brief The number of elements in the container. Could be different
     * from elements.size() due to elements marked as deleted into the
     * container.
     */
    std::size_t mElemCount = 0;

    /**
     * @brief The tuple of vectors that stores the components of the elements.
     *
     * For each component in the Components list, there is a corresponding
     * vector in this tuple that stores the data for that component for all
     * elements in the container.
     */
    VectorTupleType mElemComponents;

    /**
     * @brief The map that associates a string to a vector of custom
     * components
     */
    [[no_unique_address]] detail::CustomComponentsVectorMap<
        comp::HasCustomComponents<ComponentList>> mCustomComponents;

protected:
    using Iterator      = ElementIterator<ElementContainer>;
    using ConstIterator = ElementIterator<const ElementContainer>;

public:
    using Components       = ComponentList;
    using IndexType        = Index;
    using ParentMeshType   = ParentMesh;
    using ElementType      = Element<ElementContainer>;
    using ConstElementType = Element<const ElementContainer>;

    static constexpr uint ELEMENT_ID = ElementType::ELEMENT_ID;

protected:
    ElementContainer()
    {
        enableNonOptionalComponents(Components {});
    }

    /* Members that are inherited and wrapped by derived element containers */

    /**
     * @brief Returns a const proxy of the element at the i-th position in
     * the Element Container of the Mesh, which will be the element having index
     * = i.
     *
     * This function does not perform any sanity check: if i is less than
     * elementContainerSize(), this function will return a valid Element
     * reference (note that the Element may have been flagged as deleted).
     *
     * @param[in] i: the index of the element that will be returned.
     */
    ConstElementType element(IndexType i) const
    {
        assert(i < size() || i == std::numeric_limits<IndexType>::max());
        return ConstElementType(*this, i);
    }

    /**
     * @brief Returns a proxy of the element at the i-th position in the
     * Element Container of the Mesh, which will be the element having index =
     * i.
     *
     * This function does not perform any sanity check: if i is less than
     * elementContainerSize(), this function will return a valid Element
     * reference (note that the Element may have been flagged as deleted).
     *
     * @param[in] i: the index of the element that will be returned.
     */
    ElementType element(IndexType i)
    {
        assert(i < size() || i == std::numeric_limits<IndexType>::max());
        return ElementType(*this, i);
    }

    /**
     * @brief Returns the number of **non-deleted** elements contained in the
     * Element container of the Mesh.
     *
     * If elementCount() != elementContainerSize(), it means that there are
     * some elements that are flagged as deleted.
     *
     * @return The number of non-deleted elements of the Mesh.
     */
    std::size_t elementCount() const { return mElemCount; }

    /**
     * @brief Returns the number of elements (also deleted) contained in the
     * Element container of the Mesh.
     *
     * If elementCount() != elementContainerSize(), it means that there are
     * some elements that are flagged as deleted.
     *
     * @return The number of all the elements contained in the Mesh.
     */
    std::size_t elementContainerSize() const { return mElemComponents.size(); }

    /**
     * @brief Returns the number of deleted elements in the Element container,
     * that is elementContainerSize() - elementCount().
     *
     * @return The number of deleted elements in the container.
     */
    std::size_t deletedElementCount() const { return size() - elementCount(); }

    /**
     * @brief Adds an element to the Element Container.
     *
     * Returns the index of the added element.
     *
     * @return The index of the added element.
     */
    IndexType addElement()
    {
        std::size_t id = mElemComponents.size();
        mElemComponents.resize(id + 1);
        if constexpr (comp::HasCustomComponents<Components>) {
            mCustomComponents.resize(id + 1);
        }
        ++mElemCount;
        return static_cast<IndexType>(id);
    }

    /**
     * @brief Adds n elements to the Element Container.
     *
     * Returns the index of the first added element.
     *
     * @param[in] n: the number of elements to add.
     * @return The index of the first added element.
     */
    IndexType addElements(std::size_t n)
    {
        std::size_t baseId = mElemComponents.size();
        mElemComponents.resize(baseId + n);
        if constexpr (comp::HasCustomComponents<Components>) {
            mCustomComponents.resize(baseId + n);
        }
        mElemCount += n;
        return static_cast<IndexType>(baseId);
    }

    void clearElements()
    {
        mElemComponents.clear();
        if constexpr (comp::HasCustomComponents<Components>) {
            mCustomComponents.clear();
        }
        mElemCount = 0;
    }

    /**
     * @brief Resizes the element container to contain `size` elements.
     *
     * If the new size is greater than the old one, new elements are added to
     * the container, and a reallocation may happen. If the new size is smaller
     * than the old one, the container will keep its first non-deleted `size`
     * elements, and the remaining elements are marked as deleted.
     *
     * If the call of this function will cause a reallocation of the Element
     * container, the function will automatically take care of updating all the
     * Element pointers contained in the Mesh.
     *
     * @warning The given `size` is relative to the number of non-deleted
     * elements, not to the size of the element container. For example, if you
     * have a mesh with 10 elements and elementContainerSize() == 20, calling
     * resizeElements(5) will not cause a reallocation of the container, but
     * will mark as deleted the least 5 non-deleted elements of the container.
     * In the same scenario, calling resizeElements(15) will result in a element
     * container having 15 elements and elementContainerSize() == 25. The
     * latest 5 elements will be the newly added.
     *
     * @warning Any index to deleted elements in the Mesh will be left
     * unchanged, and therefore will point to invalid elements. This means that
     * if you call this member function with a lower number of elements, you'll
     * need to manually manage the pointers/indices to the deleted elements.
     *
     * @param[in] size: the new size of the Element container.
     */
    void resizeElements(std::size_t newSize)
    {
        std::size_t oldSize = mElemComponents.size();
        mElemComponents.resize(newSize);
        if constexpr (comp::HasCustomComponents<Components>) {
            mCustomComponents.resize(newSize);
        }
        if (newSize > oldSize) {
            mElemCount += (newSize - oldSize);
        }
        else if (newSize < oldSize) {
            mElemCount = 0;
            for (std::size_t i = 0; i < newSize; ++i) {
                if (!isDeleted(i)) {
                    ++mElemCount;
                }
            }
        }
    }

    void reserveElements(std::size_t capacity)
    {
        mElemComponents.reserve(capacity);
        if constexpr (comp::HasCustomComponents<Components>) {
            mCustomComponents.reserve(capacity);
        }
    }

    /**
     * @brief Compacts the element container, keeping only the non-deleted
     * elements.
     *
     * @return a vector that tells, for each old element index, the new index of
     * the element. Will contain std::numeric_limits<IndexType>::max() if the
     * element has been deleted.
     */
    std::vector<IndexType> compactElements()
    {
        std::vector<IndexType> newIndices = elementCompactIndices();
        if (!isElementContainerCompact()) {
            mElemComponents.compact(newIndices);
            if constexpr (comp::HasCustomComponents<Components>) {
                mCustomComponents.compact(newIndices);
            }
            assert(mElemComponents.size() == mElemCount);

            if constexpr (!std::is_void_v<ParentMesh>) {
                if (mParentMesh != nullptr) {
                    mParentMesh->template updateAllReferences<ELEMENT_ID>(
                        newIndices);
                }
            }
        }
        return newIndices;
    }

    /**
     * @brief Marks as deleted the element with the given id.
     *
     * This member function does not perform any reallocation of the elements:
     * the deleted elements will stay in the Element Container, but will be
     * marked as deleted.
     *
     * Deleted elements are automatically jumped by the iterators provided by
     * the Element Container.
     *
     * @warning If there were pointers/indices to the deleted element in this or
     * other containers, they will not be updated.
     *
     * @param[in] i: the id of the element that will be marked as deleted.
     */
    void deleteElement(IndexType i)
    {
        assert(i < size());
        auto e = element(i);
        if (!e.deleted()) {
            e.setDeleted(true);
            --mElemCount;
        }
    }

    /**
     * @brief Marks as deleted the given element.
     *
     * This member function does not perform any reallocation of the elements:
     * the deleted elements will stay in the Element Container, but will be
     * marked as deleted.
     *
     * Deleted elements are automatically jumped by the iterators provided by
     * the Element Container.
     *
     * @warning If there were pointers/indices to the deleted element in this or
     * other containers, they will not be updated.
     *
     * @param[in] e: the element that will be marked as deleted.
     */
    void deleteElement(const ElementType& e) { deleteElement(e.index()); }

    /**
     * @brief Returns true if the container is compact, i.e. there are no
     * deleted elements.
     *
     * @return true if the container is compact, false otherwise.
     */
    bool isElementContainerCompact() const
    {
        return deletedElementCount() == 0;
    }

    /**
     * @brief This is an utility member function that returns the index of an
     * element if the container would be compact, that is the number of
     * non-deleted elements before the element with the given index.
     *
     * Complexity: O(n), with n the number of elements in the container.
     *
     * This function does not perform any sanity check on the given index.
     *
     * @param[in] i: the index of a element of the container.
     * @return The index that the element with index i would have if this
     * container would be compact.
     */
    IndexType elementIndexIfCompact(IndexType i) const
    {
        assert(i < size());
        if (isElementContainerCompact()) {
            return i;
        }
        else {
            IndexType cnt = 0;
            for (IndexType ii = 0; ii < i; ++ii) {
                if (!isDeleted(ii))
                    ++cnt;
            }
            return cnt;
        }
    }

    /**
     * @brief Returns a vector that tells, for each actual element index, the
     * new index that the element would have in a compacted container. For each
     * deleted element index, the value of the vector will be
     * std::numeric_limits<IndexType>::max().
     *
     * This is useful if you need to know the indices of the elements that they
     * would have in a compact container, without considering the deleted ones.
     *
     * @return A vector containing, for each element index, its index if the
     * container would be compact.
     */
    std::vector<IndexType> elementCompactIndices() const
    {
        std::vector<IndexType> newIndices(size());
        if (isElementContainerCompact()) {
            for (IndexType i = 0; i < size(); ++i) {
                newIndices[i] = i;
            }
            return newIndices;
        }

        IndexType currentIndex = 0;
        for (IndexType i = 0; i < size(); ++i) {
            if (!isDeleted(i)) {
                newIndices[i] = currentIndex++;
            }
            else {
                newIndices[i] = std::numeric_limits<IndexType>::max();
            }
        }
        return newIndices;
    }

    /**
     * @brief Appends all elements from another container of the same type.
     *
     * @param[in] other The other container whose elements will be appended.
     */
    void append(const ElementContainer& other)
    {
        appendImpl(Components {}, other);
    }

    /**
     * @brief Serializes the element container to the given output stream.
     *
     * Writes the active element count and the contents of the underlying
     * parallel vectors tuple (including size, component enable flags, and
     * data).
     *
     * @param os The output stream to write to.
     */
    void serialize(std::ostream& os) const
    {
        vcl::serialize(os, mElemCount);
        mElemComponents.serialize(os);
    }

    /**
     * @brief Deserializes the element container from the given input stream.
     *
     * Reads the active element count and the contents of the underlying
     * parallel vectors tuple. If custom components are supported, existing
     * custom components are cleared.
     *
     * @param is The input stream to read from.
     */
    void deserialize(std::istream& is)
    {
        vcl::deserialize(is, mElemCount);
        mElemComponents.deserialize(is);
        if constexpr (comp::HasCustomComponents<ComponentList>) {
            mCustomComponents.clear();
        }
    }

    /**
     * @brief Returns an iterator to the beginning of the container.
     *
     * The iterator is automatically initialized to jump deleted elements of the
     * container. You can change this option by calling this function with
     * jumpDeleted=false.
     *
     * @param[in] jumpDeleted (def: true): boolean that tells if the iterator
     * should jump deleted elements.
     * @return An iterator the the first element of the container.
     */
    Iterator begin(bool jumpDeleted = true)
    {
        bool needJump = jumpDeleted && (size() != elementCount());
        return Iterator(*this, 0, needJump);
    }

    /**
     * @brief Returns an iterator to the end of the container.
     * @return An iterator to the end of the container.
     */
    Iterator end() { return Iterator(*this, size(), false); }

    /**
     * @brief Returns a const iterator to the beginning of the container.
     *
     * The iterator is automatically initialized to jump deleted elements of the
     * container. You can change this option by calling this function with
     * jumpDeleted=false.
     *
     * @param[in] jumpDeleted (def: true): boolean that tells if the iterator
     * should jump deleted elements.
     * @return A const iterator the the first element of the container.
     */
    ConstIterator begin(bool jumpDeleted = true) const
    {
        bool needJump = jumpDeleted && (size() != elementCount());
        return ConstIterator(*this, 0, needJump);
    }

    /**
     * @brief Returns a const iterator to the end of the container.
     * @return A const iterator to the end of the container.
     */
    ConstIterator end() const { return ConstIterator(*this, size(), false); }

    /**
     * @brief Returns a ElementContainerProxy object that allows to iterate over
     * the elements of the containers, providing two member functions begin()
     * and end(), and allows access to the vector of components.
     *
     * This member function is very useful when you want to iterate over the
     * elements using the C++ foreach syntax:
     *
     * @code{.cpp}
     * for (Element& f : m.elements()){
     *     // do something with this element
     * }
     * @endcode
     *
     * The iterator used to iterate over elements is automatically initialized
     * to jump deleted elements of the container. You can change this option by
     * calling this function with jumpDeleted=false.
     *
     * @param[in] jumpDeleted (def: true): boolean that tells if the iterator
     * should jump deleted elements.
     * @return An object having begin() and end() function, allowing to iterate
     * over the container.
     */
    auto elements(bool jumpDeleted = true)
    {
        return ElementContainerProxy<ElementContainer>(*this, jumpDeleted);
    }

    /**
     * @brief Returns a ElementContainerProxy object that allows to iterate over
     * the elements of the containers, providing two member functions begin()
     * and end(), and allows access to the vector of components.
     *
     * This member function is very useful when you want to iterate over the
     * elements using the C++ foreach syntax:
     *
     * @code{.cpp}
     * for (Element& f : m.elements()){
     *     // do something with this element
     * }
     * @endcode
     *
     * The iterator used to iterate over elements is automatically initialized
     * to jump deleted elements of the container. You can change this option by
     * calling this function with jumpDeleted=false.
     *
     * @param[in] jumpDeleted (def: true): boolean that tells if the iterator
     * should jump deleted elements.
     * @return An object having begin() and end() function, allowing to iterate
     * over the container.
     */
    auto elements(bool jumpDeleted = true) const
    {
        return ElementContainerProxy<const ElementContainer>(
            *this, jumpDeleted);
    }

    // TODO: missing elements(begin, end)

    // ### Enable - Disable

    /**
     * @brief Checks if the component with the given COMP_ID is available
     * (present at compile time and enabled at run time).
     *
     * Non-optional components are always available at compile-time (true).
     * Optional components return true if currently enabled, false otherwise.
     */
    template<uint COMP_ID>
    bool isComponentAvailable() const
    {
        if constexpr (comp::HasComponent<COMP_ID, Components>) {
            if constexpr (comp::HasOptionalComponent<COMP_ID, Components>) {
                constexpr uint N =
                    comp::IndexOfCompId<COMP_ID, VectorComponents>::value;
                return mElemComponents.template isEnabled<N>();
            }
            else {
                return true;
            }
        }
        else {
            return false;
        }
    }

    /**
     * @brief Alias of isComponentAvailable.
     */
    template<uint COMP_ID>
    bool isAvailable() const
    {
        return isComponentAvailable<COMP_ID>();
    }

    /**
     * @brief Checks if the component with the given COMP_ID is enabled/available.
     */
    template<uint COMP_ID>
    bool isEnabled() const
    {
        return isComponentAvailable<COMP_ID>();
    }

    /**
     * @brief Enables the component with the given COMP_ID.
     */
    template<uint COMP_ID>
    void enable()
    {
        static_assert(
            comp::HasComponent<COMP_ID, Components>,
            "Component not found in container");
        if constexpr (comp::HasOptionalComponent<COMP_ID, Components>) {
            constexpr uint N =
                comp::IndexOfCompId<COMP_ID, VectorComponents>::value;
            mElemComponents.template enable<N>();
        }
    }

    /**
     * @brief Disables the component with the given COMP_ID.
     * Note: Non-optional components cannot be disabled.
     */
    template<uint COMP_ID>
    void disable()
    {
        static_assert(
            comp::HasComponent<COMP_ID, Components>,
            "Component not found in container");
        static_assert(
            comp::HasOptionalComponent<COMP_ID, Components>,
            "Cannot disable a non-optional component!");
        constexpr uint N =
            comp::IndexOfCompId<COMP_ID, VectorComponents>::value;
        mElemComponents.template disable<N>();
    }

    void enableAll() { mElemComponents.enableAll(); }

    void disableAll()
    {
        disableOptionalComponents(Components {});
    }

    void enableAllOptionalComponents() { enableAll(); }

    void disableAllOptionalComponents() { disableAll(); }

    /**
     * @brief Enables in this container all the optional components that are
     * available and enabled in the given other container.
     *
     * @tparam OtherContainer The type of the container to check against.
     * @param[in] other The other container.
     */
    template<typename OtherContainer>
    void enableSameOptionalComponentsOf(const OtherContainer& other)
    {
        enableSameOptionalComponentsOfImpl(Components {}, other);
    }

    /**
     * @brief Returns the vector for the given component ID.
     */
    template<uint COMP_ID>
    auto& vector()
    {
        static_assert(
            COMP_ID != CompId::CUSTOM_COMPONENTS,
            "CustomComponents do not have a column in the parallel vectors "
            "tuple.");
        constexpr uint N =
            comp::IndexOfCompId<COMP_ID, VectorComponents>::value;
        return mElemComponents.template get<N>();
    }

    template<uint COMP_ID>
    const auto& vector() const
    {
        static_assert(
            COMP_ID != CompId::CUSTOM_COMPONENTS,
            "CustomComponents do not have a column in the parallel vectors "
            "tuple.");
        constexpr uint N =
            comp::IndexOfCompId<COMP_ID, VectorComponents>::value;
        return mElemComponents.template get<N>();
    }

    /**
     * @brief Imports all elements from the corresponding container of the
     * other mesh into this container.
     *
     * @param[in] m The other mesh to import from.
     */
    template<typename OtherMesh>
    void importFrom(const OtherMesh& m)
    {
        if constexpr (OtherMesh::template hasContainerOf<ELEMENT_ID>()) {
            const auto& c = m.template container<ELEMENT_ID>();
            enableSameOptionalComponentsOf(c);
            clearElements();
            const std::size_t n = c.elementContainerSize();
            addElements(n);
            for (IndexType i = 0; i < n; ++i) {
                element(i).importFrom(c.element(i));
            }
            mElemCount = c.elementCount();
            if constexpr (
                comp::HasCustomComponents<Components> &&
                comp::HasCustomComponents<
                    typename std::remove_cvref_t<decltype(c)>::Components>) {
                mCustomComponents = c.mCustomComponents;
            }
        }
    }

    decltype(auto) parentMesh() requires (!std::is_void_v<ParentMesh>)
    {
        assert(mParentMesh != nullptr && "Parent mesh pointer is null");
        return *mParentMesh;
    }

    decltype(auto) parentMesh() const requires (!std::is_void_v<ParentMesh>)
    {
        assert(mParentMesh != nullptr && "Parent mesh pointer is null");
        return std::as_const(*mParentMesh);
    }

    void setParentMesh(ParentMesh* mesh) { mParentMesh = mesh; }



    template<uint TARGET_ELEM_ID>
    void updateReferences(const std::vector<IndexType>& newIndices)
    {
        if constexpr (
            ElementType::template referencesElement<TARGET_ELEM_ID>()) {
            for (std::size_t i = 0; i < size(); ++i) {
                element(i).template updateReferences<TARGET_ELEM_ID>(
                    newIndices);
            }
        }
    }

    template<uint TARGET_ELEM_ID>
    void shiftReferences(IndexType startFrom, IndexType offset)
    {
        if constexpr (
            ElementType::template referencesElement<TARGET_ELEM_ID>()) {
            if (offset != 0) {
                for (std::size_t i = startFrom; i < size(); ++i) {
                    element(i).template shiftReferences<TARGET_ELEM_ID>(offset);
                }
            }
        }
    }

    // --- Custom Components ---

    /**
     * @brief Returns true if the element has a custom component with the given
     * name, false otherwise.
     *
     * @param[in] name The name of the custom component.
     * @return true if the custom component exists, false otherwise.
     */
    bool hasElemCustomComponent(const std::string& name) const
        requires comp::HasCustomComponents<Components>
    {
        return mCustomComponents.componentExists(name);
    }

    /**
     * @brief Returns a vector containing the names of all the custom components
     * associated to the elements of this container.
     *
     * @return A vector of strings containing the names of all custom
     * components.
     */
    std::vector<std::string> elemCustomComponentNames() const
        requires comp::HasCustomComponents<Components>
    {
        return mCustomComponents.allComponentNames();
    }

    /**
     * @brief Returns true if the custom component of the given name is of the
     * type given as template argument, false otherwise.
     *
     * @tparam K The type of the custom component to check.
     * @param[in] name The name of the custom component.
     * @return true if the custom component is of type K, false otherwise.
     */
    template<typename K>
    bool isElemCustomComponentOfType(const std::string& name) const
        requires comp::HasCustomComponents<Components>
    {
        return mCustomComponents.template isComponentOfType<K>(name);
    }

    /**
     * @brief Returns the `std::type_index` of the custom component of the given
     * name.
     *
     * @param[in] name The name of the custom component.
     * @return The `std::type_index` of the custom component.
     */
    std::type_index elemComponentType(const std::string& name) const
        requires comp::HasCustomComponents<Components>
    {
        return mCustomComponents.componentType(name);
    }

    std::type_index elemCustomComponentType(const std::string& name) const
        requires comp::HasCustomComponents<Components>
    {
        return elemComponentType(name);
    }

    /**
     * @brief Returns a vector of strings containing the names of all the custom
     * components of type K.
     *
     * @tparam K The type of the custom components.
     * @return A vector of strings containing the names of custom components of
     * type K.
     */
    template<typename K>
    std::vector<std::string> elemCustomComponentNamesOfType() const
        requires comp::HasCustomComponents<Components>
    {
        return mCustomComponents.template allComponentNamesOfType<K>();
    }

    /**
     * @brief Adds a new vector of custom components with the given name and
     * type. All existing elements are initialized with the default constructor
     * of K.
     *
     * @tparam K The type of the custom component.
     * @param[in] name The name of the custom component.
     */
    template<typename K>
    void addElemCustomComponent(const std::string& name)
        requires comp::HasCustomComponents<Components>
    {
        mCustomComponents.template addNewComponent<K>(
            name, elementContainerSize());
    }

    /**
     * @brief Deletes the custom component with the given name.
     *
     * @param[in] name The name of the custom component to delete.
     */
    void deleteElemCustomComponent(const std::string& name)
        requires comp::HasCustomComponents<Components>
    {
        mCustomComponents.deleteComponent(name);
    }

    /**
     * @brief Returns a vector handle to access the custom component of type K
     * with the given name.
     *
     * @tparam K The type of the custom component.
     * @param[in] name The name of the custom component.
     * @return A CustomComponentVectorHandle<K>.
     */
    template<typename K>
    CustomComponentVectorHandle<K> customComponentVectorHandle(
        const std::string& name) requires comp::HasCustomComponents<Components>
    {
        return CustomComponentVectorHandle<K>(
            mCustomComponents.template componentVector<K>(name));
    }

    /**
     * @brief Returns a const vector handle to access the custom component of
     * type K with the given name.
     *
     * @tparam K The type of the custom component.
     * @param[in] name The name of the custom component.
     * @return A ConstCustomComponentVectorHandle<K>.
     */
    template<typename K>
    ConstCustomComponentVectorHandle<K> customComponentVectorHandle(
        const std::string& name) const
        requires comp::HasCustomComponents<Components>
    {
        return ConstCustomComponentVectorHandle<K>(
            mCustomComponents.template componentVector<K>(name));
    }

    /**
     * @brief Returns a reference to the custom component of type K at index i.
     *
     * @tparam K The type of the custom component.
     * @param[in] name The name of the custom component.
     * @param[in] i The index of the element.
     * @return A reference to the custom component value.
     */
    template<typename K>
    K& elemCustomComponent(const std::string& name, IndexType i)
        requires comp::HasCustomComponents<Components>
    {
        return std::any_cast<K&>(
            mCustomComponents.template componentVector<K>(name)[i]);
    }

    template<typename K>
    const K& elemCustomComponent(const std::string& name, IndexType i) const
        requires comp::HasCustomComponents<Components>
    {
        return std::any_cast<const K&>(
            mCustomComponents.template componentVector<K>(name)[i]);
    }

    /**
     * @brief Imports the custom components from another container for a
     * specific element index.
     *
     * @param[in] thisIndex The index in this container.
     * @param[in] otherContainer The other container to import from.
     * @param[in] otherIndex The index in the other container.
     */
    template<typename OtherContainer>
    void importElementCustomComponents(
        IndexType             thisIndex,
        const OtherContainer& otherContainer,
        IndexType otherIndex) requires comp::HasCustomComponents<Components>
    {
        if constexpr (
            comp::HasCustomComponents<typename OtherContainer::Components>) {
            for (const auto& name : mCustomComponents.allComponentNames()) {
                mCustomComponents.importSameCustomComponentFrom(
                    thisIndex,
                    otherIndex,
                    name,
                    otherContainer.mCustomComponents);
            }
        }
    }

    /**
     * @brief Serializes all custom components of type K stored in this
     * container to the given output stream.
     *
     * @tparam K The type of the custom components to serialize.
     * @param os The output stream to write to.
     */
    template<typename K>
    void serializeElemCustomComponentsOfType(std::ostream& os) const
        requires comp::HasCustomComponents<Components>
    {
        mCustomComponents.template serializeCustomComponentsOfType<K>(os);
    }

    /**
     * @brief Deserializes all custom components of type K from the given
     * input stream and stores them into this container.
     *
     * @tparam K The type of the custom components to deserialize.
     * @param is The input stream to read from.
     */
    template<typename K>
    void deserializeElemCustomComponentsOfType(std::istream& is)
        requires comp::HasCustomComponents<Components>
    {
        mCustomComponents.template deserializeCustomComponentsOfType<K>(is);
    }

    ElementType operator[](IndexType i)
    {
        assert(i < size() || i == std::numeric_limits<IndexType>::max());
        return ElementType(*this, i);
    }

    ConstElementType operator[](IndexType i) const
    {
        assert(i < size() || i == std::numeric_limits<IndexType>::max());
        return ConstElementType(*this, i);
    }

private:
    template<uint COMP_ID, typename Data, bool OPT>
    friend class Component;

    template<typename>
    friend class ElementIterator;

    template<uint, typename>
    friend class ComponentIterator;

    template<uint, typename>
    friend class ComponentView;

    template<typename>
    friend class ElementContainerProxy;

    // to allow TTVC resize, pushBack, insert, erase, clear members
    template<int, std::unsigned_integral>
    friend class comp::VertexReferences;

    template<uint, typename CT, typename CLW>
    friend class ElementProxy;

    friend class comp::CustomComponents;

    template<
        typename,
        typename,
        std::unsigned_integral,
        template<typename...> class>
    friend class ElementContainer;

    template<uint CID, std::unsigned_integral I, uint RID, int N, bool TTVC, bool OPT>
    friend class ContainerReferencesComponent;

    std::size_t size() const { return mElemComponents.size(); }

    bool isDeleted(IndexType i) const
    {
        assert(i < size());
        return vector<CompId::BIT_FLAGS>()[i][0];
    }

    // --- TTVC (Tied To Vertex Count) synchronization helpers ---

    template<typename... Comps>
    void resizeTTVCComponentsImpl(
        vcl::TypeWrapper<Comps...>,
        IndexType i,
        uint      n)
    {
        assert(i < size());
        auto p          = element(i);
        auto resizeComp = [&]<typename Comp>() {
            if constexpr (comp::IsTiedToVertexCount<Comp>) {
                if (this->template isComponentAvailable<Comp::COMPONENT_ID>()) {
                    p.Comp::resize(n);
                }
            }
        };
        (resizeComp.template operator()<Comps>(), ...);
    }

    template<typename... Comps>
    void pushBackTTVCComponentsImpl(vcl::TypeWrapper<Comps...>, IndexType i)
    {
        assert(i < size());
        auto p            = element(i);
        auto pushBackComp = [&]<typename Comp>() {
            if constexpr (comp::IsTiedToVertexCount<Comp>) {
                if (this->template isComponentAvailable<Comp::COMPONENT_ID>()) {
                    p.Comp::pushBack();
                }
            }
        };
        (pushBackComp.template operator()<Comps>(), ...);
    }

    template<typename... Comps>
    void insertTTVCComponentsImpl(
        vcl::TypeWrapper<Comps...>,
        IndexType i,
        uint      pos)
    {
        assert(i < size());
        auto p          = element(i);
        auto insertComp = [&]<typename Comp>() {
            if constexpr (comp::IsTiedToVertexCount<Comp>) {
                if (this->template isComponentAvailable<Comp::COMPONENT_ID>()) {
                    p.Comp::insert(pos);
                }
            }
        };
        (insertComp.template operator()<Comps>(), ...);
    }

    template<typename... Comps>
    void eraseTTVCComponentsImpl(
        vcl::TypeWrapper<Comps...>,
        IndexType i,
        uint      pos)
    {
        assert(i < size());
        auto p         = element(i);
        auto eraseComp = [&]<typename Comp>() {
            if constexpr (comp::IsTiedToVertexCount<Comp>) {
                if (this->template isComponentAvailable<Comp::COMPONENT_ID>()) {
                    p.Comp::erase(pos);
                }
            }
        };
        (eraseComp.template operator()<Comps>(), ...);
    }

    template<typename... Comps>
    void clearTTVCComponentsImpl(vcl::TypeWrapper<Comps...>, IndexType i)
    {
        assert(i < size());
        auto p         = element(i);
        auto clearComp = [&]<typename Comp>() {
            if constexpr (comp::IsTiedToVertexCount<Comp>) {
                if (this->template isComponentAvailable<Comp::COMPONENT_ID>()) {
                    p.Comp::clear();
                }
            }
        };
        (clearComp.template operator()<Comps>(), ...);
    }

    void resizeTTVCComponents(IndexType i, uint n)
    {
        resizeTTVCComponentsImpl(Components {}, i, n);
    }

    void pushBackTTVCComponents(IndexType i)
    {
        pushBackTTVCComponentsImpl(Components {}, i);
    }

    void insertTTVCComponents(IndexType i, uint pos)
    {
        insertTTVCComponentsImpl(Components {}, i, pos);
    }

    void eraseTTVCComponents(IndexType i, uint pos)
    {
        eraseTTVCComponentsImpl(Components {}, i, pos);
    }

    void clearTTVCComponents(IndexType i)
    {
        clearTTVCComponentsImpl(Components {}, i);
    }

    template<typename... Comps>
    void enableNonOptionalComponents(vcl::TypeWrapper<Comps...>)
    {
        auto enableIfNotOpt = [&]<typename Comp>() {
            if constexpr (
                detail::HasVectorColumnPred<Comp>::value &&
                !comp::IsOptionalComponent<Comp>) {
                constexpr uint N =
                    comp::IndexOfCompId<Comp::COMPONENT_ID, VectorComponents>::value;
                mElemComponents.template enable<N>();
            }
        };
        (enableIfNotOpt.template operator()<Comps>(), ...);
    }

    template<typename... Comps>
    void disableOptionalComponents(vcl::TypeWrapper<Comps...>)
    {
        auto disableIfOpt = [&]<typename Comp>() {
            if constexpr (comp::IsOptionalComponent<Comp>) {
                constexpr uint N =
                    comp::IndexOfCompId<Comp::COMPONENT_ID, VectorComponents>::value;
                mElemComponents.template disable<N>();
            }
        };
        (disableIfOpt.template operator()<Comps>(), ...);
    }

    template<typename... Comps, typename OtherContainer>
    void enableSameOptionalComponentsOfImpl(
        vcl::TypeWrapper<Comps...>,
        const OtherContainer& other)
    {
        auto enableIfAvailableInOther = [&]<typename Comp>() {
            if constexpr (comp::IsOptionalComponent<Comp>) {
                if (!this->template isComponentAvailable<Comp::COMPONENT_ID>()) {
                    if constexpr (
                        comp::
                            HasComponent<Comp::COMPONENT_ID, OtherContainer>) {
                        if (other.template isComponentAvailable<Comp::COMPONENT_ID>()) {
                            this->template enable<Comp::COMPONENT_ID>();
                        }
                    }
                }
            }
        };

        (enableIfAvailableInOther.template operator()<Comps>(), ...);
    }

    template<typename... Comps>
    void appendImpl(vcl::TypeWrapper<Comps...>, const ElementContainer& other)
    {
        const std::size_t on = other.elementContainerSize();
        if (on == 0) {
            return;
        }

        const std::size_t n = elementContainerSize();
        addElements(on);

        auto copyComponent = [&]<typename Comp>() {
            if constexpr (Comp::COMPONENT_ID != CompId::CUSTOM_COMPONENTS) {
                if (this->template isComponentAvailable<Comp::COMPONENT_ID>() &&
                    other.template isComponentAvailable<Comp::COMPONENT_ID>()) {
                    auto& thisVec = this->template vector<Comp::COMPONENT_ID>();
                    const auto& otherVec =
                        other.template vector<Comp::COMPONENT_ID>();
                    std::copy(
                        otherVec.begin(), otherVec.end(), thisVec.begin() + n);
                }
            }
        };

        (copyComponent.template operator()<Comps>(), ...);

        if constexpr (comp::HasCustomComponents<Components>) {
            mCustomComponents.append(other.mCustomComponents, n, on);
        }

        mElemCount = (mElemCount - on) + other.elementCount();
    }
};

/**
 * @brief Concept that checks whether a type T is an ElementContainer.
 */
template<typename T>
concept ElementContainerConcept = requires {
    { std::remove_cvref_t<T>::ELEMENT_ID } -> std::convertible_to<uint>;
    typename std::remove_cvref_t<T>::IndexType;
    typename std::remove_cvref_t<T>::ElementType;
    typename std::remove_cvref_t<T>::ConstElementType;
};

} // namespace vcl::meshv2

#endif // VCL_MESH_V2_CONTAINERS_BASE_ELEMENT_CONTAINER_H
