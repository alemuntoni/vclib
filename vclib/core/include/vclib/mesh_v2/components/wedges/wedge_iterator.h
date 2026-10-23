// VCLib - Visual Computing Library
// Copyright (C) 2021-2026 Visual Computing Lab, ISTI - CNR.
//
// This Source Code Form is subject to the terms of the Mozilla Public License,
// v. 2.0. If a copy of the MPL was not distributed with this file, You can
// obtain one at https://mozilla.org/MPL/2.0/.

#ifndef VCL_MESH_V2_COMPONENTS_WEDGES_WEDGE_ITERATOR_H
#define VCL_MESH_V2_COMPONENTS_WEDGES_WEDGE_ITERATOR_H

#include "wedge_proxy.h"

#include <vclib/base.h>

#include <iterator>
#include <utility>

namespace vcl::meshv2::comp {

/**
 * @brief Random-access iterator over the wedges of a Face.
 *
 * Dereferences to a lightweight WedgeProxy<FaceType>.
 *
 * @tparam FaceType The type of the parent Face (mutable or const).
 */
template<typename FaceType>
class WedgeIterator
{
    FaceType mFace;
    uint     mIndex = 0;

public:
    using iterator_category = std::random_access_iterator_tag;
    using value_type        = WedgeProxy<FaceType>;
    using difference_type   = std::ptrdiff_t;
    using reference        = WedgeProxy<FaceType>;
    using pointer          = void;

    WedgeIterator() = default;
    WedgeIterator(FaceType face, uint index) : mFace(face), mIndex(index) {}

    reference operator*() const { return reference(mFace, mIndex); }
    reference operator[](difference_type n) const
    {
        return reference(mFace, static_cast<uint>(static_cast<difference_type>(mIndex) + n));
    }

    WedgeIterator& operator++()
    {
        ++mIndex;
        return *this;
    }

    WedgeIterator operator++(int)
    {
        auto tmp = *this;
        ++mIndex;
        return tmp;
    }

    WedgeIterator& operator--()
    {
        --mIndex;
        return *this;
    }

    WedgeIterator operator--(int)
    {
        auto tmp = *this;
        --mIndex;
        return tmp;
    }

    WedgeIterator& operator+=(difference_type n)
    {
        mIndex = static_cast<uint>(static_cast<difference_type>(mIndex) + n);
        return *this;
    }

    WedgeIterator& operator-=(difference_type n)
    {
        mIndex = static_cast<uint>(static_cast<difference_type>(mIndex) - n);
        return *this;
    }

    friend WedgeIterator operator+(WedgeIterator it, difference_type n)
    {
        return it += n;
    }

    friend WedgeIterator operator+(difference_type n, WedgeIterator it)
    {
        return it += n;
    }

    friend WedgeIterator operator-(WedgeIterator it, difference_type n)
    {
        return it -= n;
    }

    friend difference_type operator-(
        const WedgeIterator& a,
        const WedgeIterator& b)
    {
        return static_cast<difference_type>(a.mIndex) -
               static_cast<difference_type>(b.mIndex);
    }

    uint index() const { return mIndex; }

    template<typename OtherFace>
    friend class WedgeIterator;

    template<typename OtherFace>
    bool operator==(const WedgeIterator<OtherFace>& other) const
    {
        return mIndex == other.mIndex;
    }

    template<typename OtherFace>
    auto operator<=>(const WedgeIterator<OtherFace>& other) const
    {
        return mIndex <=> other.mIndex;
    }
};

} // namespace vcl::meshv2::comp

#endif // VCL_MESH_V2_COMPONENTS_WEDGES_WEDGE_ITERATOR_H
