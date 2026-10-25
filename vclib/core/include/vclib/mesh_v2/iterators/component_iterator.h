// VCLib - Visual Computing Library
// Copyright (C) 2021-2026 Visual Computing Lab, ISTI - CNR.
//
// This Source Code Form is subject to the terms of the Mozilla Public License,
// v. 2.0. If a copy of the MPL was not distributed with this file, You can
// obtain one at https://mozilla.org/MPL/2.0/.

#ifndef VCL_MESH_V2_ITERATORS_COMPONENT_ITERATOR_H
#define VCL_MESH_V2_ITERATORS_COMPONENT_ITERATOR_H

#include "container_index_iterator.h"

#include <vclib/base.h>
#include <vclib/mesh_v2/components/base/component_predicates.h>

namespace vcl::meshv2 {

/**
 * @brief Random-access iterator over a single component column in an
 * ElementContainer.
 *
 * It yields references to the component data. By default, it jumps elements
 * that are marked as deleted (jumpDeleted = true).
 *
 * @tparam COMP_ID: The ID of the component to iterate over.
 * @tparam ContainerType: The type of the container (mutable or const).
 */
template<uint COMP_ID, typename ContainerType>
class ComponentIterator :
        public ContainerIndexIterator<
            ContainerType,
            typename comp::ComponentTypeT<COMP_ID, ContainerType>::DataType,
            std::conditional_t<
                std::is_const_v<ContainerType>,
                const typename comp::ComponentTypeT<COMP_ID, ContainerType>::
                    DataType&,
                typename comp::ComponentTypeT<COMP_ID, ContainerType>::
                    DataType&>,
            std::conditional_t<
                std::is_const_v<ContainerType>,
                const typename comp::ComponentTypeT<COMP_ID, ContainerType>::
                    DataType*,
                typename comp::ComponentTypeT<COMP_ID, ContainerType>::
                    DataType*>>
{
    using Base = ContainerIndexIterator<
        ContainerType,
        typename comp::ComponentTypeT<COMP_ID, ContainerType>::DataType,
        std::conditional_t<
            std::is_const_v<ContainerType>,
            const typename comp::ComponentTypeT<COMP_ID, ContainerType>::
                DataType&,
            typename comp::ComponentTypeT<COMP_ID, ContainerType>::DataType&>,
        std::conditional_t<
            std::is_const_v<ContainerType>,
            const typename comp::ComponentTypeT<COMP_ID, ContainerType>::
                DataType*,
            typename comp::ComponentTypeT<COMP_ID, ContainerType>::DataType*>>;

public:
    using Base::Base;

    using ContainerTraits = Base::ContainerTraits;
    using IndexType       = Base::IndexType;
    using CompType        = comp::ComponentTypeT<COMP_ID, ContainerType>;
    using RawDataType     = CompType::DataType;
    using DataType        = std::conditional_t<
        std::is_const_v<ContainerType>,
        const RawDataType,
        RawDataType>;
    using reference       = Base::reference;
    using pointer         = Base::pointer;
    using difference_type = Base::difference_type;

    ComponentIterator() = default;

    // Implicit conversion from mutable iterator to const iterator
    template<typename OtherContainerType>
    requires std::is_same_v<ContainerType, const OtherContainerType>
    ComponentIterator(
        const ComponentIterator<COMP_ID, OtherContainerType>& other) :
            Base(other)
    {
    }

    reference operator*() const
    {
        assert(this->mContainer != nullptr);
        assert(this->mIndex < this->mContainer->elementContainerSize());
        return this->mContainer->template vector<COMP_ID>()[this->mIndex];
    }

    pointer operator->() const { return &(operator*()); }

    friend ComponentIterator operator+(
        difference_type          n,
        const ComponentIterator& it)
    {
        return it + n;
    }
};

} // namespace vcl::meshv2

#endif // VCL_MESH_V2_ITERATORS_COMPONENT_ITERATOR_H
