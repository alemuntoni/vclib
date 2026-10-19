// VCLib - Visual Computing Library
// Copyright (C) 2021-2026 Visual Computing Lab, ISTI - CNR.
//
// This Source Code Form is subject to the terms of the Mozilla Public License,
// v. 2.0. If a copy of the MPL was not distributed with this file, You can
// obtain one at https://mozilla.org/MPL/2.0/.

#ifndef VCL_MESH_V2_CONTAINERS_BASE_ELEMENT_CONTAINER_H
#define VCL_MESH_V2_CONTAINERS_BASE_ELEMENT_CONTAINER_H

#include <vclib/mesh_v2/components/base/component.h>
#include <vclib/mesh_v2/components/base/component_predicates.h>
#include <vclib/mesh_v2/components/bit_flags.h>
#include <vclib/mesh_v2/elements/base/element_proxy.h>
#include <vclib/mesh_v2/iterators/element_iterator.h>

#include <vclib/base.h>
#include <vclib/space/core.h>

#include <cassert>
#include <concepts>

namespace vcl::meshv2 {

namespace comp {
template<int, std::unsigned_integral>
class VertexReferences;
}

namespace detail {

// Trait to convert TypeWrapper<Components...> in a ParallelVectorsTuple
template<typename ComponentListWrapper>
struct ParallelVectorsTupleFromComponents;

template<typename... Components>
struct ParallelVectorsTupleFromComponents<vcl::TypeWrapper<Components...>>
{
    using type = ParallelVectorsTuple<typename Components::DataType...>;
};

} // namespace detail

template<
    typename ComponentList,
    typename ParentMesh,
    std::unsigned_integral Index        = uint,
    template<typename...> class Element = ElementProxy>
class ElementContainer
{
    static_assert(
        comp::HasBitFlags<ComponentList>,
        "An Element must have the BitFlags component "
        "(vcl::meshv2::comp::BitFlags).");

    using VectorTupleType =
        detail::ParallelVectorsTupleFromComponents<ComponentList>::type;

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

public:
    using Components     = ComponentList;
    using IndexType      = Index;
    using ParentMeshType = ParentMesh;
    using ProxyType      = Element<ElementContainer>;
    using ConstProxyType = Element<const ElementContainer>;
    using Iterator       = ElementIterator<ElementContainer>;
    using ConstIterator  = ElementIterator<const ElementContainer>;

    static constexpr uint ELEMENT_ID = ProxyType::ELEMENT_ID;

    /**
     * @brief Returns the vector for the given component ID.
     */
    template<uint COMP_ID>
    auto& get()
    {
        constexpr uint N = comp::IndexOfCompId<COMP_ID, Components>::value;
        return mElemComponents.template get<N>();
    }

    template<uint COMP_ID>
    const auto& get() const
    {
        constexpr uint N = comp::IndexOfCompId<COMP_ID, Components>::value;
        return mElemComponents.template get<N>();
    }

protected:
    ElementContainer()
    {
        // BitFlags is mandatory and always enabled
        enable<CompId::BIT_FLAGS>();
    }

    decltype(auto) mesh() requires (!std::is_void_v<ParentMesh>)
    {
        assert(mParentMesh != nullptr && "Parent mesh pointer is null");
        return *mParentMesh;
    }

    decltype(auto) mesh() const requires (!std::is_void_v<ParentMesh>)
    {
        assert(mParentMesh != nullptr && "Parent mesh pointer is null");
        return std::as_const(*mParentMesh);
    }

    void setMesh(ParentMesh* mesh) { mParentMesh = mesh; }

    /**
     * @brief Enables the component with the given COMP_ID.
     */
    template<uint COMP_ID>
    void enable()
    {
        constexpr uint N = comp::IndexOfCompId<COMP_ID, Components>::value;
        mElemComponents.template enable<N>();
    }

    /**
     * @brief Disables the component with the given COMP_ID.
     * Note: BitFlags cannot be disabled.
     */
    template<uint COMP_ID>
    void disable()
    {
        static_assert(
            COMP_ID != CompId::BIT_FLAGS,
            "BitFlags component cannot be disabled");
        constexpr uint N = comp::IndexOfCompId<COMP_ID, Components>::value;
        mElemComponents.template disable<N>();
    }

    /**
     * @brief Checks if the component with the given COMP_ID is enabled.
     */
    template<uint COMP_ID>
    bool isEnabled() const
    {
        constexpr uint N = comp::IndexOfCompId<COMP_ID, Components>::value;
        return mElemComponents.template isEnabled<N>();
    }

    void enableAll() { mElemComponents.enableAll(); }

    void disableAll()
    {
        mElemComponents.disableAll();
        // Keep BitFlags enabled
        enable<CompId::BIT_FLAGS>();
    }

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
        enableSameOptionalComponentsOfImpl(Components{}, other);
    }

    template<uint TARGET_ELEM_ID>
    void updateReferences(const std::vector<IndexType>& newIndices)
    {
        if constexpr (ProxyType::template referencesElement<TARGET_ELEM_ID>()) {
            for (std::size_t i = 0; i < size(); ++i) {
                element(i).template updateReferences<TARGET_ELEM_ID>(
                    newIndices);
            }
        }
    }


    /* Members that are inherited and renamed/wrapped by derived element containers */

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
    ConstProxyType element(IndexType i) const
    {
        assert(i < size() || i == std::numeric_limits<IndexType>::max());
        return ConstProxyType(*this, i);
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
    ProxyType element(IndexType i)
    {
        assert(i < size() || i == std::numeric_limits<IndexType>::max());
        return ProxyType(*this, i);
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
     * @brief Returns true if the container is compact, i.e. there are no
     * deleted elements.
     *
     * @return true if the container is compact, false otherwise.
     */
    bool isElementContainerCompact() const
    {
        return deletedElementCount() == 0;
    }

    // TODO: missing addElement
    // TODO: missing addElements

    void clearElements()
    {
        mElemComponents.clear();
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
    void deleteElement(const ProxyType& e) { deleteElement(e.index()); }

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

    // TODO: append
    // TODO: serialize and deserialize

    // Iterators
    Iterator begin(bool jumpDeleted = true)
    {
        bool needJump = jumpDeleted && (size() != elementCount());
        return Iterator(*this, 0, needJump);
    }

    Iterator end() { return Iterator(*this, size(), false); }

    ConstIterator begin(bool jumpDeleted = true) const
    {
        bool needJump = jumpDeleted && (size() != elementCount());
        return ConstIterator(*this, 0, needJump);
    }

    ConstIterator end() const { return ConstIterator(*this, size(), false); }

    auto elements(bool jumpDeleted = true)
    {
        return vcl::View(begin(jumpDeleted), end());
    }

    auto elements(bool jumpDeleted = true) const
    {
        return vcl::View(begin(jumpDeleted), end());
    }


    ProxyType operator[](IndexType i)
    {
        assert(i < size() || i == std::numeric_limits<IndexType>::max());
        return ProxyType(*this, i);
    }

    ConstProxyType operator[](IndexType i) const
    {
        assert(i < size() || i == std::numeric_limits<IndexType>::max());
        return ConstProxyType(*this, i);
    }

private:
    template<typename>
    friend class ElementIterator;

    template<int, std::unsigned_integral>
    friend class comp::VertexReferences;

    template<typename CT, typename CLW>
    friend class ElementProxy;

    template<uint CID, std::unsigned_integral I, uint RID, int N, bool TTVC>
    friend class ContainerReferencesComponent;

    std::size_t size() const { return mElemComponents.size(); }

    bool isDeleted(IndexType i) const
    {
        assert(i < size());
        return get<CompId::BIT_FLAGS>()[i][0];
    }

    // --- TTVC (Tied To Vertex Count) synchronization helpers ---

    template<typename... Comps>
    void resizeTTVCComponentsImpl(vcl::TypeWrapper<Comps...>, IndexType i, uint n)
    {
        assert(i < size());
        auto p = element(i);
        ([&]() {
            if constexpr (comp::IsTiedToVertexCount<Comps>) {
                if (isEnabled<Comps::COMPONENT_ID>()) {
                    p.Comps::resize(n);
                }
            }
        }(), ...);
    }

    template<typename... Comps>
    void pushBackTTVCComponentsImpl(vcl::TypeWrapper<Comps...>, IndexType i)
    {
        assert(i < size());
        auto p = element(i);
        ([&]() {
            if constexpr (comp::IsTiedToVertexCount<Comps>) {
                if (isEnabled<Comps::COMPONENT_ID>()) {
                    p.Comps::pushBack();
                }
            }
        }(), ...);
    }

    template<typename... Comps>
    void insertTTVCComponentsImpl(vcl::TypeWrapper<Comps...>, IndexType i, uint pos)
    {
        assert(i < size());
        auto p = element(i);
        ([&]() {
            if constexpr (comp::IsTiedToVertexCount<Comps>) {
                if (isEnabled<Comps::COMPONENT_ID>()) {
                    p.Comps::insert(pos);
                }
            }
        }(), ...);
    }

    template<typename... Comps>
    void eraseTTVCComponentsImpl(vcl::TypeWrapper<Comps...>, IndexType i, uint pos)
    {
        assert(i < size());
        auto p = element(i);
        ([&]() {
            if constexpr (comp::IsTiedToVertexCount<Comps>) {
                if (isEnabled<Comps::COMPONENT_ID>()) {
                    p.Comps::erase(pos);
                }
            }
        }(), ...);
    }

    template<typename... Comps>
    void clearTTVCComponentsImpl(vcl::TypeWrapper<Comps...>, IndexType i)
    {
        assert(i < size());
        auto p = element(i);
        ([&]() {
            if constexpr (comp::IsTiedToVertexCount<Comps>) {
                if (isEnabled<Comps::COMPONENT_ID>()) {
                    p.Comps::clear();
                }
            }
        }(), ...);
    }

    void resizeTTVCComponents(IndexType i, uint n)
    {
        resizeTTVCComponentsImpl(Components{}, i, n);
    }

    void pushBackTTVCComponents(IndexType i)
    {
        pushBackTTVCComponentsImpl(Components{}, i);
    }

    void insertTTVCComponents(IndexType i, uint pos)
    {
        insertTTVCComponentsImpl(Components{}, i, pos);
    }

    void eraseTTVCComponents(IndexType i, uint pos)
    {
        eraseTTVCComponentsImpl(Components{}, i, pos);
    }

    void clearTTVCComponents(IndexType i)
    {
        clearTTVCComponentsImpl(Components{}, i);
    }

    template<typename... Comps, typename OtherContainer>
    void enableSameOptionalComponentsOfImpl(
        vcl::TypeWrapper<Comps...>,
        const OtherContainer& other)
    {
        auto enableIfAvailableInOther = [&]<typename Comp>() {
            if (!this->template isEnabled<Comp::COMPONENT_ID>()) {
                if constexpr (comp::HasComponent<Comp::COMPONENT_ID, OtherContainer>) {
                    if (comp::isComponentAvailableOn<Comp::COMPONENT_ID>(other)) {
                        this->template enable<Comp::COMPONENT_ID>();
                    }
                }
            }
        };

        (enableIfAvailableInOther.template operator()<Comps>(), ...);
    }
};

/**
 * @brief Concept that checks whether a type T is an ElementContainer.
 */
template<typename T>
concept ElementContainerConcept = requires {
    { std::remove_cvref_t<T>::ELEMENT_ID } -> std::convertible_to<uint>;
    typename std::remove_cvref_t<T>::IndexType;
    typename std::remove_cvref_t<T>::ProxyType;
    typename std::remove_cvref_t<T>::ConstProxyType;
};

} // namespace vcl::meshv2

#endif // VCL_MESH_V2_CONTAINERS_BASE_ELEMENT_CONTAINER_H
