// VCLib - Visual Computing Library
// Copyright (C) 2021-2026 Visual Computing Lab, ISTI - CNR.
//
// This Source Code Form is subject to the terms of the Mozilla Public License,
// v. 2.0. If a copy of the MPL was not distributed with this file, You can
// obtain one at https://mozilla.org/MPL/2.0/.

#ifndef VCL_MESH_V2_ELEMENT_CONTAINER_H
#define VCL_MESH_V2_ELEMENT_CONTAINER_H

#include <cassert>
#include <concepts>
#include <vclib/base.h>

#include <vclib/mesh_v2/components/base/component.h>
#include <vclib/mesh_v2/components/bit_flags.h>
#include <vclib/mesh_v2/element_proxy.h>
#include <vclib/mesh_v2/iterators/element_iterator.h>
#include <vclib/mesh_v2/parallel_vectors_tuple.h>

namespace vcl::meshv2 {

template<
    typename ComponentList,
    std::unsigned_integral Index = uint,
    typename ParentMesh          = void>
class ElementContainer
{
    static_assert(
        HasBitFlags<ComponentList>,
        "An Element must have the BitFlags component "
        "(vcl::meshv2::comp::BitFlags).");

public:
    using Components     = ComponentList;
    using IndexType      = Index;
    using MeshType       = ParentMesh;
    using ProxyType      = ElementProxy<ElementContainer>;
    using ConstProxyType = ElementProxy<const ElementContainer>;
    using Iterator       = ElementIterator<ElementContainer>;
    using ConstIterator  = ElementIterator<const ElementContainer>;

private:
    using TupleType =
        typename ParallelVectorsTupleFromComponents<Components>::type;
    TupleType   mElemComponents;
    std::size_t mElemCount = 0; // Number of non-deleted elements
    std::conditional_t<std::is_void_v<ParentMesh>, void*, ParentMesh*> mMesh =
        nullptr;

public:
    ElementContainer()
    {
        // BitFlags is mandatory and always enabled
        enable<CompId::BIT_FLAGS>();
    }

    void setMesh(auto* mesh) { mMesh = mesh; }

    template<typename M = ParentMesh>
    M& mesh()
    {
        assert(mMesh != nullptr && "Parent mesh pointer is null");
        if constexpr (std::is_void_v<M>) {
            static_assert(
                !std::is_void_v<M>,
                "ParentMesh type is void; please specify mesh type in mesh<MeshType>()");
        }
        else {
            return *static_cast<M*>(mMesh);
        }
    }

    template<typename M = ParentMesh>
    const M& mesh() const
    {
        assert(mMesh != nullptr && "Parent mesh pointer is null");
        if constexpr (std::is_void_v<M>) {
            static_assert(
                !std::is_void_v<M>,
                "ParentMesh type is void; please specify mesh type in mesh<MeshType>()");
        }
        else {
            return *static_cast<const M*>(mMesh);
        }
    }

    /**
     * @brief Returns the vector for the given component ID.
     */
    template<uint COMP_ID>
    auto& get()
    {
        constexpr uint N = IndexOfCompId<COMP_ID, Components>::value;
        return mElemComponents.template get<N>();
    }

    template<uint COMP_ID>
    const auto& get() const
    {
        constexpr uint N = IndexOfCompId<COMP_ID, Components>::value;
        return mElemComponents.template get<N>();
    }

    /**
     * @brief Enables the component with the given COMP_ID.
     */
    template<uint COMP_ID>
    void enable()
    {
        constexpr uint N = IndexOfCompId<COMP_ID, Components>::value;
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
        constexpr uint N = IndexOfCompId<COMP_ID, Components>::value;
        mElemComponents.template disable<N>();
    }

    /**
     * @brief Checks if the component with the given COMP_ID is enabled.
     */
    template<uint COMP_ID>
    bool isEnabled() const
    {
        constexpr uint N = IndexOfCompId<COMP_ID, Components>::value;
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
     * @brief Returns the total number of elements in the container, including
     * deleted ones.
     */
    std::size_t size() const { return mElemComponents.size(); }

    std::size_t elementContainerSize() const { return mElemComponents.size(); }

    /**
     * @brief Returns the number of non-deleted elements in the container.
     */
    std::size_t elementCount() const { return mElemCount; }

    /**
     * @brief Returns the number of deleted elements in the container.
     */
    std::size_t deletedElementCount() const { return size() - elementCount(); }

    bool empty() const { return mElemCount == 0; }

    /**
     * @brief Marks as deleted the element with the given id.
     */
    void deleteElement(IndexType i)
    {
        assert(i < size());
        auto& flags = get<CompId::BIT_FLAGS>()[i];
        if (!flags[comp::BitFlags::DELETED]) {
            flags[comp::BitFlags::DELETED] = true;
            --mElemCount;
        }
    }

    /**
     * @brief Un-marks as deleted the element with the given id.
     */
    void undeleteElement(IndexType i)
    {
        assert(i < size());
        auto& flags = get<CompId::BIT_FLAGS>()[i];
        if (flags[comp::BitFlags::DELETED]) {
            flags[comp::BitFlags::DELETED] = false;
            ++mElemCount;
        }
    }

    bool isDeleted(IndexType i) const
    {
        assert(i < size());
        return get<CompId::BIT_FLAGS>()[i][comp::BitFlags::DELETED];
    }

    void resize(std::size_t newSize)
    {
        std::size_t oldSize = mElemComponents.size();
        mElemComponents.resize(newSize);
        if (newSize > oldSize) {
            mElemCount += (newSize - oldSize);
        }
        else if (newSize < oldSize) {
            mElemCount           = 0;
            const auto& flagsVec = get<CompId::BIT_FLAGS>();
            for (std::size_t i = 0; i < newSize; ++i) {
                if (!flagsVec[i][comp::BitFlags::DELETED]) {
                    ++mElemCount;
                }
            }
        }
    }

    void reserve(std::size_t capacity) { mElemComponents.reserve(capacity); }

    void clear()
    {
        mElemComponents.clear();
        mElemCount = 0;
    }

    // Element proxy access
    ProxyType operator[](IndexType i)
    {
        assert(i < size());
        return ProxyType(*this, i);
    }

    ConstProxyType operator[](IndexType i) const
    {
        assert(i < size());
        return ConstProxyType(*this, i);
    }

    ProxyType element(IndexType i)
    {
        assert(i < size());
        return ProxyType(*this, i);
    }

    ConstProxyType element(IndexType i) const
    {
        assert(i < size());
        return ConstProxyType(*this, i);
    }

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

    ConstIterator cbegin(bool jumpDeleted = true) const
    {
        return begin(jumpDeleted);
    }

    ConstIterator cend() const { return end(); }
};

} // namespace vcl::meshv2

#endif // VCL_MESH_V2_ELEMENT_CONTAINER_H
