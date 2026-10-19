// VCLib - Visual Computing Library
// Copyright (C) 2021-2026 Visual Computing Lab, ISTI - CNR.
//
// This Source Code Form is subject to the terms of the Mozilla Public License,
// v. 2.0. If a copy of the MPL was not distributed with this file, You can
// obtain one at https://mozilla.org/MPL/2.0/.

#ifndef VCL_MESH_V2_ITERATORS_COMPONENT_ITERATOR_H
#define VCL_MESH_V2_ITERATORS_COMPONENT_ITERATOR_H

#include <vclib/base.h>
#include <vclib/mesh_v2/components/base/component_predicates.h>

#include <cassert>
#include <compare>
#include <iterator>
#include <type_traits>

namespace vcl::meshv2 {

/**
 * @brief Bidirectional iterator over a single component column in an ElementContainer.
 *
 * It yields references to the component data. By default, it jumps elements
 * that are marked as deleted (jumpDeleted = true).
 *
 * @tparam COMP_ID The ID of the component to iterate over.
 * @tparam ContainerType The type of the container (mutable or const).
 */
template<uint COMP_ID, typename ContainerType>
class ComponentIterator
{
public:
    using ContainerTraits = std::remove_cv_t<ContainerType>;
    using IndexType       = typename ContainerTraits::IndexType;
    using CompType        = comp::ComponentTypeT<COMP_ID, ContainerType>;
    using RawDataType     = typename CompType::DataType;
    using DataType        = std::conditional_t<std::is_const_v<ContainerType>, const RawDataType, RawDataType>;

    // Modern C++ standard iterator traits
    using difference_type   = std::ptrdiff_t;
    using value_type        = RawDataType;
    using reference         = DataType&;
    using pointer           = DataType*;
    using iterator_concept  = std::bidirectional_iterator_tag;
    using iterator_category = std::bidirectional_iterator_tag;

private:
    ContainerType* mContainer   = nullptr;
    IndexType      mIndex       = 0;
    bool           mJumpDeleted = true;

    void advanceForward()
    {
        if (mContainer) {
            std::size_t sz = mContainer->elementContainerSize();
            while (mIndex < sz && mContainer->isDeleted(mIndex)) {
                ++mIndex;
            }
        }
    }

    void advanceBackward()
    {
        if (mContainer) {
            while (mIndex > 0 && mContainer->isDeleted(mIndex)) {
                --mIndex;
            }
        }
    }

public:
    ComponentIterator() = default;

    ComponentIterator(ContainerType& container, IndexType index, bool jumpDeleted = true) :
        mContainer(&container), mIndex(index), mJumpDeleted(jumpDeleted)
    {
        if (mJumpDeleted) [[unlikely]] {
            advanceForward();
        }
    }

    // Implicit conversion from mutable iterator to const iterator
    template<typename OtherContainerType>
        requires std::is_same_v<ContainerType, const OtherContainerType>
    ComponentIterator(const ComponentIterator<COMP_ID, OtherContainerType>& other) :
        mContainer(other.container()), mIndex(other.index()), mJumpDeleted(other.jumpDeleted())
    {
    }

    ContainerType* container() const { return mContainer; }
    IndexType index() const { return mIndex; }
    bool jumpDeleted() const { return mJumpDeleted; }

    reference operator*() const
    {
        assert(mContainer != nullptr);
        assert(mIndex < mContainer->elementContainerSize());
        return mContainer->template get<COMP_ID>()[mIndex];
    }

    pointer operator->() const
    {
        return &(operator*());
    }

    ComponentIterator& operator++()
    {
        ++mIndex;
        if (mJumpDeleted) [[unlikely]] {
            advanceForward();
        }
        return *this;
    }

    ComponentIterator operator++(int)
    {
        ComponentIterator tmp = *this;
        ++(*this);
        return tmp;
    }

    ComponentIterator& operator--()
    {
        --mIndex;
        if (mJumpDeleted) [[unlikely]] {
            advanceBackward();
        }
        return *this;
    }

    ComponentIterator operator--(int)
    {
        ComponentIterator tmp = *this;
        --(*this);
        return tmp;
    }

    template<typename OtherContainer>
    bool operator==(const ComponentIterator<COMP_ID, OtherContainer>& other) const
    {
        return mIndex == other.index() && mContainer == other.container();
    }

    template<typename OtherContainer>
    auto operator<=>(const ComponentIterator<COMP_ID, OtherContainer>& other) const
    {
        return mIndex <=> other.index();
    }
};

} // namespace vcl::meshv2

#endif // VCL_MESH_V2_ITERATORS_COMPONENT_ITERATOR_H
