// VCLib - Visual Computing Library
// Copyright (C) 2021-2026 Visual Computing Lab, ISTI - CNR.
//
// This Source Code Form is subject to the terms of the Mozilla Public License,
// v. 2.0. If a copy of the MPL was not distributed with this file, You can
// obtain one at https://mozilla.org/MPL/2.0/.

#ifndef VCL_MESH_V2_ITERATORS_CONTAINER_INDEX_ITERATOR_H
#define VCL_MESH_V2_ITERATORS_CONTAINER_INDEX_ITERATOR_H

#include <vclib/base.h>

#include <cassert>
#include <compare>
#include <iterator>
#include <type_traits>

namespace vcl::meshv2 {

/**
 * @brief Base class for random-access iterators that index into an ElementContainer.
 *
 * It manages the container pointer, current element index, skipping of deleted elements,
 * random-access movement arithmetic, and comparisons using C++23 deducing this.
 *
 * Derived classes (such as ElementIterator and ComponentIterator) only need to provide
 * `operator*()` and `operator->()`.
 *
 * @tparam ContainerType: The container type (const or non-const).
 * @tparam ValueType: The value_type of the iterator.
 * @tparam ReferenceType: The reference type returned by operator*().
 * @tparam PointerType: The pointer type returned by operator->().
 */
template<
    typename ContainerType,
    typename ValueType,
    typename ReferenceType,
    typename PointerType = ReferenceType*>
class ContainerIndexIterator
{
public:
    using ContainerTraits = std::remove_cv_t<ContainerType>;
    using IndexType       = typename ContainerTraits::IndexType;

    // Modern C++ standard iterator traits
    using difference_type   = std::ptrdiff_t;
    using value_type        = ValueType;
    using reference         = ReferenceType;
    using pointer           = PointerType;
    using iterator_concept  = std::random_access_iterator_tag;
    using iterator_category = std::random_access_iterator_tag;

protected:
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
    ContainerIndexIterator() = default;

    ContainerIndexIterator(
        ContainerType& container,
        IndexType      index,
        bool           jumpDeleted = true) :
            mContainer(&container), mIndex(index), mJumpDeleted(jumpDeleted)
    {
        if (mJumpDeleted) [[unlikely]] {
            advanceForward();
        }
    }

    // Conversion constructor from compatible (e.g. mutable to const) iterator base
    template<typename OtherContainerType, typename OtherV, typename OtherR, typename OtherP>
    requires std::is_same_v<ContainerType, const OtherContainerType>
    ContainerIndexIterator(
        const ContainerIndexIterator<OtherContainerType, OtherV, OtherR, OtherP>& other) :
            mContainer(other.parentContainer()),
            mIndex(other.index()),
            mJumpDeleted(other.jumpDeleted())
    {
    }

    ContainerType* parentContainer() const { return mContainer; }

    IndexType index() const { return mIndex; }

    bool jumpDeleted() const { return mJumpDeleted; }

    // Pre-increment
    decltype(auto) operator++(this auto&& self)
    {
        ++self.mIndex;
        if (self.mJumpDeleted) [[unlikely]] {
            self.advanceForward();
        }
        return (self);
    }

    // Post-increment
    auto operator++(this auto&& self, int)
    {
        auto tmp = self;
        ++self;
        return tmp;
    }

    // Pre-decrement
    decltype(auto) operator--(this auto&& self)
    {
        --self.mIndex;
        if (self.mJumpDeleted) [[unlikely]] {
            self.advanceBackward();
        }
        return (self);
    }

    // Post-decrement
    auto operator--(this auto&& self, int)
    {
        auto tmp = self;
        --self;
        return tmp;
    }

    // Addition assignment
    decltype(auto) operator+=(this auto&& self, difference_type n)
    {
        if (!self.mJumpDeleted || (self.mContainer && self.mContainer->isElementContainerCompact())) {
            self.mIndex = static_cast<IndexType>(static_cast<difference_type>(self.mIndex) + n);
            return (self);
        }
        if (n > 0) {
            while (n--) {
                ++self;
            }
        } else if (n < 0) {
            while (n++) {
                --self;
            }
        }
        return (self);
    }

    // Subtraction assignment
    decltype(auto) operator-=(this auto&& self, difference_type n)
    {
        return (self += -n);
    }

    // Addition
    auto operator+(this const auto& self, difference_type n)
    {
        auto tmp = self;
        tmp += n;
        return tmp;
    }

    // Subtraction (offset)
    auto operator-(this const auto& self, difference_type n)
    {
        auto tmp = self;
        tmp -= n;
        return tmp;
    }

    // Subtraction (difference between iterators)
    template<typename OtherIt>
    difference_type operator-(this const auto& self, const OtherIt& other)
    {
        return self.differenceImpl(other);
    }

    // Subscript
    decltype(auto) operator[](this const auto& self, difference_type n)
    {
        return *(self + n);
    }

    // Comparisons
    bool operator==(this const auto& self, const auto& other)
    {
        return self.mIndex == other.index() && self.mContainer == other.parentContainer();
    }

    auto operator<=>(this const auto& self, const auto& other)
    {
        return self.mIndex <=> other.index();
    }

    bool operator<(this const auto& self, const auto& other)
    {
        return self.mIndex < other.index();
    }

    bool operator<=(this const auto& self, const auto& other)
    {
        return self.mIndex <= other.index();
    }

    bool operator>(this const auto& self, const auto& other)
    {
        return self.mIndex > other.index();
    }

    bool operator>=(this const auto& self, const auto& other)
    {
        return self.mIndex >= other.index();
    }
};

} // namespace vcl::meshv2

#endif // VCL_MESH_V2_ITERATORS_CONTAINER_INDEX_ITERATOR_H
