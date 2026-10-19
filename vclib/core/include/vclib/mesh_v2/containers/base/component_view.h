// VCLib - Visual Computing Library
// Copyright (C) 2021-2026 Visual Computing Lab, ISTI - CNR.
//
// This Source Code Form is subject to the terms of the Mozilla Public License,
// v. 2.0. If a copy of the MPL was not distributed with this file, You can
// obtain one at https://mozilla.org/MPL/2.0/.

#ifndef VCL_MESH_V2_CONTAINERS_BASE_COMPONENT_VIEW_H
#define VCL_MESH_V2_CONTAINERS_BASE_COMPONENT_VIEW_H

#include <vclib/base.h>
#include <vclib/mesh_v2/iterators/component_iterator.h>

#include <cassert>
#include <ranges>
#include <type_traits>

namespace vcl::meshv2 {

/**
 * @brief A view over a single component column in an ElementContainer.
 *
 * It provides random access via `operator[]`, raw buffer access via `data()`,
 * and bidirectional iteration via `begin()` and `end()` that respects the
 * container's deleted flags (jumping deleted elements if jumpDeleted is true).
 *
 * It does not allow mutating the container size or capacity, protecting the
 * Structure of Arrays (SoA) synchronization invariants.
 *
 * @tparam COMP_ID The component ID.
 * @tparam ContainerType The type of the container (mutable or const).
 */
template<uint COMP_ID, typename Container>
class ComponentView :
        public std::ranges::view_interface<ComponentView<COMP_ID, Container>>
{
private:
    Container* mContainer   = nullptr;
    bool       mJumpDeleted = true;

public:
    using ContainerType = std::remove_cv_t<Container>;
    using IndexType     = ContainerType::IndexType;
    using CompType      = comp::ComponentTypeT<COMP_ID, Container>;
    using RawDataType   = CompType::DataType;
    using DataType      = std::conditional_t<
        std::is_const_v<Container>,
        const RawDataType,
        RawDataType>;

    using value_type      = RawDataType;
    using reference       = DataType&;
    using const_reference = const RawDataType&;
    using pointer         = DataType*;
    using const_pointer   = const RawDataType*;

    using Iterator      = ComponentIterator<COMP_ID, Container>;
    using ConstIterator = ComponentIterator<COMP_ID, const Container>;

public:
    ComponentView() = default;

    ComponentView(Container& container, bool jumpDeleted = true) :
            mContainer(&container), mJumpDeleted(jumpDeleted)
    {
    }

    // Implicit conversion from mutable ComponentView to const ComponentView
    template<typename OtherContainerType>
    requires std::is_same_v<Container, const OtherContainerType>
    ComponentView(const ComponentView<COMP_ID, OtherContainerType>& other) :
            mContainer(other.mContainer), mJumpDeleted(other.mJumpDeleted)
    {
    }

    /**
     * @brief Checks if the component is enabled in the container.
     */
    bool isEnabled() const
    {
        return mContainer && mContainer->template isEnabled<COMP_ID>();
    }

    Iterator begin() const
    {
        if (!mContainer || !isEnabled()) {
            return end();
        }
        bool needJump =
            mJumpDeleted && (mContainer->size() != mContainer->elementCount());
        return Iterator(*mContainer, 0, needJump);
    }

    Iterator end() const
    {
        IndexType endIdx = (mContainer && isEnabled()) ?
                               mContainer->elementContainerSize() :
                               0;
        return Iterator(*mContainer, endIdx, false);
    }

    reference operator[](IndexType i) const
    {
        assert(mContainer != nullptr);
        assert(isEnabled() && "Component is not enabled!");
        assert(i < mContainer->elementContainerSize());
        return mContainer->template get<COMP_ID>()[i];
    }

    pointer data() const
    {
        if (!mContainer || !isEnabled()) {
            return nullptr;
        }
        return mContainer->template get<COMP_ID>().data();
    }

    std::size_t size() const
    {
        if (!mContainer || !isEnabled()) {
            return 0;
        }
        return mJumpDeleted ? mContainer->elementCount() :
                              mContainer->elementContainerSize();
    }

    bool empty() const { return size() == 0; }
};

} // namespace vcl::meshv2

#endif // VCL_MESH_V2_CONTAINERS_BASE_COMPONENT_VIEW_H
