// VCLib - Visual Computing Library
// Copyright (C) 2021-2026 Visual Computing Lab, ISTI - CNR.
//
// This Source Code Form is subject to the terms of the Mozilla Public License,
// v. 2.0. If a copy of the MPL was not distributed with this file, You can
// obtain one at https://mozilla.org/MPL/2.0/.

#ifndef VCL_MESH_V2_ITERATORS_ELEMENT_ITERATOR_H
#define VCL_MESH_V2_ITERATORS_ELEMENT_ITERATOR_H

#include "container_index_iterator.h"

#include <vclib/base.h>
#include <vclib/mesh_v2/components/bit_flags.h>
#include <vclib/mesh_v2/elements/base/element_proxy.h>

namespace vcl::meshv2 {

/**
 * @brief Random-access iterator over the elements of an ElementContainer.
 *
 * It yields lightweight ElementProxy objects by value. By default, it jumps
 * elements that are marked as deleted (jumpDeleted = true).
 *
 * @tparam ContainerType: The type of the container (mutable or const).
 */
template<typename ContainerType>
class ElementIterator :
        public ContainerIndexIterator<
            ContainerType,
            std::conditional_t<
                std::is_const_v<ContainerType>,
                typename std::remove_cv_t<ContainerType>::ConstElementType,
                typename std::remove_cv_t<ContainerType>::ElementType>,
            std::conditional_t<
                std::is_const_v<ContainerType>,
                typename std::remove_cv_t<ContainerType>::ConstElementType,
                typename std::remove_cv_t<ContainerType>::ElementType>,
            FakePointerWithValue<std::conditional_t<
                std::is_const_v<ContainerType>,
                typename std::remove_cv_t<ContainerType>::ConstElementType,
                typename std::remove_cv_t<ContainerType>::ElementType>>>
{
    using Base = ContainerIndexIterator<
        ContainerType,
        std::conditional_t<
            std::is_const_v<ContainerType>,
            typename std::remove_cv_t<ContainerType>::ConstElementType,
            typename std::remove_cv_t<ContainerType>::ElementType>,
        std::conditional_t<
            std::is_const_v<ContainerType>,
            typename std::remove_cv_t<ContainerType>::ConstElementType,
            typename std::remove_cv_t<ContainerType>::ElementType>,
        FakePointerWithValue<std::conditional_t<
            std::is_const_v<ContainerType>,
            typename std::remove_cv_t<ContainerType>::ConstElementType,
            typename std::remove_cv_t<ContainerType>::ElementType>>>;

public:
    using Base::Base;

    using ContainerTraits = Base::ContainerTraits;
    using IndexType       = Base::IndexType;
    using Components      = ContainerTraits::Components;
    using ElementType     = Base::value_type;
    using reference       = Base::reference;
    using pointer         = Base::pointer;
    using difference_type = Base::difference_type;

    ElementIterator() = default;

    // Implicit conversion from mutable iterator to const iterator
    template<typename OtherContainerType>
    requires std::is_same_v<ContainerType, const OtherContainerType>
    ElementIterator(const ElementIterator<OtherContainerType>& other) :
            Base(other)
    {
    }

    reference operator*() const
    {
        return ElementType(*this->mContainer, this->mIndex);
    }

    pointer operator->() const { return pointer(operator*()); }

    friend ElementIterator operator+(
        difference_type        n,
        const ElementIterator& it)
    {
        return it + n;
    }
};

} // namespace vcl::meshv2

#endif // VCL_MESH_V2_ITERATORS_ELEMENT_ITERATOR_H
