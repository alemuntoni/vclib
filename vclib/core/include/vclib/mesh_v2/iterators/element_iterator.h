// VCLib - Visual Computing Library
// Copyright (C) 2021-2026 Visual Computing Lab, ISTI - CNR.
//
// This Source Code Form is subject to the terms of the Mozilla Public License,
// v. 2.0. If a copy of the MPL was not distributed with this file, You can
// obtain one at https://mozilla.org/MPL/2.0/.

#ifndef VCL_MESH_V2_ITERATORS_ELEMENT_ITERATOR_H
#define VCL_MESH_V2_ITERATORS_ELEMENT_ITERATOR_H

#include <vclib/base.h>
#include <vclib/mesh_v2/components/bit_flags.h>
#include <vclib/mesh_v2/elements/base/element_proxy.h>

#include <compare>
#include <iterator>
#include <type_traits>

namespace vcl::meshv2 {

/**
 * @brief Bidirectional iterator over the elements of an ElementContainer.
 *
 * It yields lightweight ElementProxy objects by value. By default, it jumps
 * elements that are marked as deleted (jumpDeleted = true).
 *
 * @tparam ContainerType The type of the container (mutable or const).
 */
template<typename ContainerType>
class ElementIterator
{
public:
    using ContainerTraits = std::remove_cv_t<ContainerType>;
    using IndexType       = typename ContainerTraits::IndexType;
    using Components      = typename ContainerTraits::Components;
    using ProxyType       = std::conditional_t<
        std::is_const_v<ContainerType>,
        typename ContainerTraits::ConstProxyType,
        typename ContainerTraits::ProxyType>;

    // Modern C++ standard iterator traits
    using difference_type   = std::ptrdiff_t;
    using value_type        = ProxyType;
    using reference         = ProxyType;
    using pointer           = FakePointerWithValue<ProxyType>;
    using iterator_concept  = std::bidirectional_iterator_tag;
    using iterator_category = std::bidirectional_iterator_tag;

private:
    ContainerType* mContainer = nullptr;
    IndexType mIndex = 0;
    bool mJumpDeleted = true;

    void advanceForward()
    {
        if (mContainer) {
            std::size_t sz = mContainer->size();
            const auto& bitFlagsVec = mContainer->template get<CompId::BIT_FLAGS>();
            while (mIndex < sz && bitFlagsVec[mIndex][comp::BitFlags::DELETED]) {
                ++mIndex;
            }
        }
    }

    void advanceBackward()
    {
        if (mContainer) {
            const auto& bitFlagsVec = mContainer->template get<CompId::BIT_FLAGS>();
            while (mIndex > 0 && bitFlagsVec[mIndex][comp::BitFlags::DELETED]) {
                --mIndex;
            }
        }
    }

public:
    ElementIterator() = default;

    ElementIterator(ContainerType& container, IndexType index, bool jumpDeleted = true) :
        mContainer(&container), mIndex(index), mJumpDeleted(jumpDeleted)
    {
        if (mJumpDeleted) [[unlikely]] {
            advanceForward();
        }
    }

    // Implicit conversion from mutable iterator to const iterator
    template<typename OtherContainerType>
        requires std::is_same_v<ContainerType, const OtherContainerType>
    ElementIterator(const ElementIterator<OtherContainerType>& other) :
        mContainer(other.container()), mIndex(other.index()), mJumpDeleted(other.jumpDeleted())
    {
    }

    ContainerType* container() const { return mContainer; }
    IndexType index() const { return mIndex; }
    bool jumpDeleted() const { return mJumpDeleted; }

    reference operator*() const
    {
        return ProxyType(*mContainer, mIndex);
    }

    pointer operator->() const
    {
        return pointer(operator*());
    }

    ElementIterator& operator++()
    {
        ++mIndex;
        if (mJumpDeleted) [[unlikely]] {
            advanceForward();
        }
        return *this;
    }

    ElementIterator operator++(int)
    {
        ElementIterator tmp = *this;
        ++(*this);
        return tmp;
    }

    ElementIterator& operator--()
    {
        --mIndex;
        if (mJumpDeleted) [[unlikely]] {
            advanceBackward();
        }
        return *this;
    }

    ElementIterator operator--(int)
    {
        ElementIterator tmp = *this;
        --(*this);
        return tmp;
    }

    template<typename OtherContainer>
    bool operator==(const ElementIterator<OtherContainer>& other) const
    {
        return mIndex == other.index() && mContainer == other.container();
    }

    template<typename OtherContainer>
    auto operator<=>(const ElementIterator<OtherContainer>& other) const
    {
        return mIndex <=> other.index();
    }
};

} // namespace vcl::meshv2

#endif // VCL_MESH_V2_ITERATORS_ELEMENT_ITERATOR_H
