// VCLib - Visual Computing Library
// Copyright (C) 2021-2026 Visual Computing Lab, ISTI - CNR.
//
// This Source Code Form is subject to the terms of the Mozilla Public License,
// v. 2.0. If a copy of the MPL was not distributed with this file, You can
// obtain one at https://mozilla.org/MPL/2.0/.

#ifndef VCL_MESH_V2_ELEMENT_PROXY_H
#define VCL_MESH_V2_ELEMENT_PROXY_H

#include <limits>
#include <type_traits>
#include <vclib/base.h>
#include <vclib/mesh_v2/components/base/component.h>
#include <vclib/mesh_v2/components/bit_flags.h>

namespace vcl::meshv2 {

template <typename ContainerType, typename ComponentListWrapper = typename std::remove_cv_t<ContainerType>::Components>
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
template<typename ContainerType, typename... Components>
class ElementProxy<ContainerType, vcl::TypeWrapper<Components...>> :
        public Components...
{
    static_assert(
        HasBitFlags<vcl::TypeWrapper<Components...>>::value,
        "An Element must have the BitFlags component (vcl::meshv2::comp::BitFlags).");

public:
    using IndexType = typename std::remove_cv_t<ContainerType>::IndexType;

private:
    ContainerType* mContainer = nullptr;
    IndexType mIndex = std::numeric_limits<IndexType>::max();

public:
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
        const ElementProxy<OtherContainerType, vcl::TypeWrapper<Components...>>&
            other) : mContainer(&other.container()), mIndex(other.index())
    {
    }

    // Methods accessed by the components (via "deducing this" self.container())
    ContainerType& container() { return *mContainer; }
    const ContainerType& container() const { return *mContainer; }

    IndexType index() const { return mIndex; }

    /**
     * @brief Checks if this proxy points to a valid element.
     * A proxy is considered invalid if it points to a valid container and its
     * index is the maximum value of IndexType (NULL index). This naturally
     * represents missing topology references (e.g., boundary faces).
     */
    bool isValid() const
    {
        return mContainer && mIndex != std::numeric_limits<IndexType>::max();
    }
    explicit operator bool() const { return isValid(); }
};

} // namespace vcl::meshv2

#endif // VCL_MESH_V2_ELEMENT_PROXY_H
