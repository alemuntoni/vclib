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
 * @tparam ContainerType: The type of the container (mutable or const).
 */
template<typename ContainerType>
class ElementIterator
{
public:
    using ContainerTraits = std::remove_cv_t<ContainerType>;
    using IndexType       = typename ContainerTraits::IndexType;
    using Components      = typename ContainerTraits::Components;
    using ElementType     = std::conditional_t<
        std::is_const_v<ContainerType>,
        typename ContainerTraits::ConstElementType,
        typename ContainerTraits::ElementType>;

    // Modern C++ standard iterator traits
    using difference_type   = std::ptrdiff_t;
    using value_type        = ElementType;
    using reference         = ElementType;
    using pointer           = FakePointerWithValue<ElementType>;
    using iterator_concept  = std::random_access_iterator_tag;
    using iterator_category = std::random_access_iterator_tag;

private:
    ContainerType* mContainer   = nullptr;
    IndexType      mIndex       = 0;
    bool           mJumpDeleted = true;

    void advanceForward()
    {
        if (mContainer) {
            std::size_t sz = mContainer->elementContainerSize();
            while (mIndex < sz && mContainer->isElementDeleted(mIndex)) {
                ++mIndex;
            }
        }
    }

    void advanceBackward()
    {
        if (mContainer) {
            while (mIndex > 0 && mContainer->isElementDeleted(mIndex)) {
                --mIndex;
            }
        }
    }

    template<typename OtherIt>
    difference_type differenceImpl(const OtherIt& other) const
    {
        if (!mJumpDeleted || (mContainer && mContainer->isElementContainerCompact())) {
            return static_cast<difference_type>(mIndex) -
                   static_cast<difference_type>(other.index());
        }
        if (mIndex == other.index()) {
            return 0;
        }
        if (mIndex > other.index()) {
            auto it = other;
            difference_type diff = 0;
            while (it.index() < mIndex) {
                ++it;
                ++diff;
            }
            return diff;
        } else {
            auto it = *this;
            difference_type diff = 0;
            while (it.index() < other.index()) {
                ++it;
                ++diff;
            }
            return -diff;
        }
    }

public:
    ElementIterator() = default;

    ElementIterator(
        ContainerType& container,
        IndexType      index,
        bool           jumpDeleted = true) :
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
            mContainer(other.parentContainer()), mIndex(other.index()),
            mJumpDeleted(other.jumpDeleted())
    {
    }

    ContainerType* parentContainer() const { return mContainer; }

    IndexType index() const { return mIndex; }

    bool jumpDeleted() const { return mJumpDeleted; }

    reference operator*() const { return ElementType(*mContainer, mIndex); }

    pointer operator->() const { return pointer(operator*()); }

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

    ElementIterator& operator+=(difference_type n)
    {
        if (!mJumpDeleted || (mContainer && mContainer->isElementContainerCompact())) {
            mIndex = static_cast<IndexType>(static_cast<difference_type>(mIndex) + n);
            return *this;
        }
        if (n > 0) {
            while (n--) {
                ++(*this);
            }
        } else if (n < 0) {
            while (n++) {
                --(*this);
            }
        }
        return *this;
    }

    ElementIterator& operator-=(difference_type n)
    {
        return *this += -n;
    }

    ElementIterator operator+(difference_type n) const
    {
        ElementIterator tmp = *this;
        tmp += n;
        return tmp;
    }

    friend ElementIterator operator+(
        difference_type n,
        const ElementIterator& it)
    {
        return it + n;
    }

    ElementIterator operator-(difference_type n) const
    {
        ElementIterator tmp = *this;
        tmp -= n;
        return tmp;
    }

    difference_type operator-(const ElementIterator& other) const
    {
        return differenceImpl(other);
    }

    template<typename OtherContainer>
    difference_type operator-(
        const ElementIterator<OtherContainer>& other) const
    {
        return differenceImpl(other);
    }

    reference operator[](difference_type n) const
    {
        return *(*this + n);
    }

    bool operator==(const ElementIterator& other) const
    {
        return mIndex == other.mIndex && mContainer == other.mContainer;
    }

    template<typename OtherContainer>
    bool operator==(const ElementIterator<OtherContainer>& other) const
    {
        return mIndex == other.index() && mContainer == other.parentContainer();
    }

    auto operator<=>(const ElementIterator& other) const
    {
        return mIndex <=> other.mIndex;
    }

    template<typename OtherContainer>
    auto operator<=>(const ElementIterator<OtherContainer>& other) const
    {
        return mIndex <=> other.index();
    }

    bool operator<(const ElementIterator& other) const
    {
        return mIndex < other.mIndex;
    }

    template<typename OtherContainer>
    bool operator<(const ElementIterator<OtherContainer>& other) const
    {
        return mIndex < other.index();
    }

    bool operator<=(const ElementIterator& other) const
    {
        return mIndex <= other.mIndex;
    }

    template<typename OtherContainer>
    bool operator<=(const ElementIterator<OtherContainer>& other) const
    {
        return mIndex <= other.index();
    }

    bool operator>(const ElementIterator& other) const
    {
        return mIndex > other.mIndex;
    }

    template<typename OtherContainer>
    bool operator>(const ElementIterator<OtherContainer>& other) const
    {
        return mIndex > other.index();
    }

    bool operator>=(const ElementIterator& other) const
    {
        return mIndex >= other.mIndex;
    }

    template<typename OtherContainer>
    bool operator>=(const ElementIterator<OtherContainer>& other) const
    {
        return mIndex >= other.index();
    }
};

} // namespace vcl::meshv2

#endif // VCL_MESH_V2_ITERATORS_ELEMENT_ITERATOR_H
