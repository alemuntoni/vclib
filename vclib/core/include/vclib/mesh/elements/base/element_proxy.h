// VCLib - Visual Computing Library
// Copyright (C) 2021-2026 Visual Computing Lab, ISTI - CNR.
//
// This Source Code Form is subject to the terms of the Mozilla Public License,
// v. 2.0. If a copy of the MPL was not distributed with this file, You can
// obtain one at https://mozilla.org/MPL/2.0/.

#ifndef VCL_MESH_ELEMENTS_BASE_ELEMENT_PROXY_H
#define VCL_MESH_ELEMENTS_BASE_ELEMENT_PROXY_H

#include <vclib/mesh/components/base/component.h>
#include <vclib/mesh/components/base/component_predicates.h>
#include <vclib/mesh/components/bit_flags.h>

#include <vclib/base.h>

#include <concepts>
#include <limits>
#include <type_traits>

namespace vcl {

namespace mesh {
template<
    typename ComponentList,
    typename ParentMesh,
    std::unsigned_integral Index,
    template<typename...> class Element>
class ElementContainer;

template<typename ContainerType>
class ElementIterator;
} // namespace mesh

template<
    uint ELEM_ID,
    typename ContainerType,
    typename ComponentListWrapper =
        typename std::remove_cv_t<ContainerType>::Components>
class ElementProxy;

/**
 * @brief Lightweight proxy object to access an element's data in the Structure
 * of Arrays (SoA) ElementContainer.
 *
 * The ElementProxy acts as an Object-Oriented view over the Structure of
 * Arrays. It is meant to be instantiated on the fly (passed by value) and
 * provides access to the components' data by delegating the calls to the
 * underlying Container.
 *
 * It will provide all the member functions of the components in the
 * ComponentListWrapper.
 */
template<uint ELEM_ID, typename ContainerType, typename... Comps>
class ElementProxy<ELEM_ID, ContainerType, vcl::TypeWrapper<Comps...>> :
        public Comps...
{
public:
    using Components = vcl::TypeWrapper<Comps...>;
    using IndexType  = typename std::remove_cv_t<ContainerType>::IndexType;

    /**
     * @brief The null/invalid sentinel value for this element's index type.
     */
    static constexpr IndexType NULL_INDEX = vcl::NULL_INDEX<IndexType>;

    static_assert(
        comp::HasBitFlags<Components>,
        "An Element must have the BitFlags component "
        "(vcl::comp::BitFlags).");

private:
    ContainerType* mContainer = nullptr;
    IndexType      mIndex     = NULL_INDEX;

public:
    static constexpr uint ELEMENT_ID = ELEM_ID;

    // Methods accessed by the components ("deducing this"
    // self.parentContainer()).
    // TODO: When C++26 variadic friends are fully supported on MSVC, move
    // parentContainer() to protected so that ElementProxy is fully encapsulated
    ContainerType& parentContainer() { return *mContainer; }

    const ContainerType& parentContainer() const { return *mContainer; }

    /**
     * @brief Returns the index of this element in the container.
     */
    IndexType index() const { return mIndex; }

    /**
     * @brief Checks if this proxy points to a valid element.
     * A proxy is considered invalid if it points to a valid container and its
     * index is the NULL_INDEX value. This naturally represents missing topology
     * references (e.g., boundary faces).
     */
    bool isValid() const
    {
        return mContainer && mIndex != NULL_INDEX;
    }

    /**
     * @brief Implicit conversion to bool to check if the proxy is valid.
     */
    explicit operator bool() const { return isValid(); }

    /**
     * @brief Checks if the component with COMP_ID is available (present and enabled).
     */
    template<uint COMP_ID>
    bool isComponentAvailable() const
    {
        return mContainer && mContainer->template isComponentAvailable<COMP_ID>();
    }

    /**
     * @brief Imports the components of the other element into this element.
     *
     * Iterates over all components that compose this element and calls their
     * importFrom method on the other element if available/enabled.
     *
     * @param[in] other: The other element to import from.
     * @param[in] importRefs: Boolean flag indicating if references (e.g. vertex
     * indices) should be imported (default: true).
     */
    template<typename OtherElement>
    void importFrom(const OtherElement& other, bool importRefs = true)
        requires (!std::is_const_v<ContainerType>)
    {
        static_assert(
            ELEMENT_ID == OtherElement::ELEMENT_ID,
            "Cannot import from an element of a different type.");

        auto importSingleComponent = [&]<typename Comp>() {
            if constexpr (requires { this->Comp::importFrom(other, importRefs); }) {
                if constexpr (requires { Comp::COMPONENT_ID; }) {
                    if (this->template isComponentAvailable<Comp::COMPONENT_ID>()) {
                        this->Comp::importFrom(other, importRefs);
                    }
                }
                else {
                    this->Comp::importFrom(other, importRefs);
                }
            }
        };

        (importSingleComponent.template operator()<Comps>(), ...);
    }

protected:
    ElementProxy() = default;

    ElementProxy(ContainerType& container, IndexType index) :
            mContainer(&container), mIndex(index)
    {
    }

    // Implicit conversion constructor to allow conversion from a mutable
    // ElementProxy to a const ElementProxy.
    template<typename OtherContainerType>
    requires std::is_same_v<ContainerType, const OtherContainerType>
    ElementProxy(
        const ElementProxy<
            ELEM_ID,
            OtherContainerType,
            vcl::TypeWrapper<Comps...>>& other) :
            mContainer(other.mContainer), mIndex(other.mIndex)
    {
    }

    template<uint TARGET_ELEM_ID>
    void updateReferences(const std::vector<IndexType>& newIndices)
    {
        auto updateIfMatching = [&]<typename Comp>() {
            if constexpr (
                comp::ComponentReferencesElement<Comp, TARGET_ELEM_ID>) {
                if constexpr (requires { Comp::COMPONENT_ID; }) {
                    if (this->template isComponentAvailable<
                            Comp::COMPONENT_ID>()) {
                        this->Comp::template updateReferences<TARGET_ELEM_ID>(
                            newIndices);
                    }
                }
                else {
                    this->Comp::template updateReferences<TARGET_ELEM_ID>(
                        newIndices);
                }
            }
        };

        (updateIfMatching.template operator()<Comps>(), ...);
    }

    template<uint TARGET_ELEM_ID>
    void shiftReferences(IndexType offset)
    {
        auto shiftIfMatching = [&]<typename Comp>() {
            if constexpr (
                comp::ComponentReferencesElement<Comp, TARGET_ELEM_ID>) {
                if constexpr (requires { Comp::COMPONENT_ID; }) {
                    if (this->template isComponentAvailable<
                            Comp::COMPONENT_ID>()) {
                        this->Comp::template shiftReferences<TARGET_ELEM_ID>(
                            offset);
                    }
                }
                else {
                    this->Comp::template shiftReferences<TARGET_ELEM_ID>(
                        offset);
                }
            }
        };

        (shiftIfMatching.template operator()<Comps>(), ...);
    }

    template<uint TARGET_ELEM_ID>
    static constexpr bool referencesElement()
    {
        return (comp::ComponentReferencesElement<Comps, TARGET_ELEM_ID> || ...);
    }

private:
    /// friend classes declarations

    template<uint, typename, typename>
    friend class ElementProxy;

    template<
        typename,
        typename,
        std::unsigned_integral,
        template<typename...> class>
    friend class mesh::ElementContainer;

    template<typename>
    friend class mesh::ElementIterator;

    // TODO: When C++26 (P2893 Variadic Friends) is fully supported across all
    // compilers (including MSVC), uncomment:
    // friend Comps...;
};

/**
 * @brief Concept that checks whether a type T is an Element proxy.
 *
 * An element proxy must have an ELEMENT_ID, an associated Components list,
 * an IndexType, and member functions index() and isValid().
 */
template<typename T>
concept ElementConcept = requires (T&& obj) {
    { std::remove_cvref_t<T>::ELEMENT_ID } -> std::convertible_to<uint>;
    typename std::remove_cvref_t<T>::Components;
    typename std::remove_cvref_t<T>::IndexType;
    { obj.index() } -> std::same_as<typename std::remove_cvref_t<T>::IndexType>;
    { obj.isValid() } -> std::same_as<bool>;
};

} // namespace vcl

#endif // VCL_MESH_ELEMENTS_BASE_ELEMENT_PROXY_H
