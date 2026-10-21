// VCLib - Visual Computing Library
// Copyright (C) 2021-2026 Visual Computing Lab, ISTI - CNR.
//
// This Source Code Form is subject to the terms of the Mozilla Public License,
// v. 2.0. If a copy of the MPL was not distributed with this file, You can
// obtain one at https://mozilla.org/MPL/2.0/.

#ifndef VCL_MESH_V2_CONTAINERS_BASE_CUSTOM_COMPONENT_VECTOR_HANDLE_H
#define VCL_MESH_V2_CONTAINERS_BASE_CUSTOM_COMPONENT_VECTOR_HANDLE_H

#include <vclib/base.h>

#include <any>
#include <vector>

namespace vcl::meshv2 {

/**
 * @brief Allows to access directly to a custom component.
 *
 * The class allows to access a custom component stored in a Contaner of
 * Elements without having to use the Container itself and avoiding copies, and
 * it can be used as a normal std::vector. The class stores references to the
 * custom components, therefore it allows to modify them.
 *
 * It is meant to be created by a Container, that constructs it from the vector
 * of custom components and then returns it to the user.
 *
 * @note A CustomComponentVectorHandle object is meant to be used to access the
 * custom components. It does not make sense to modify the size of the container
 * or to add or remove elements. Member functions that modify the size of the
 * container are not implemented.
 *
 * @note If the Element Container is modified after the creation of a
 * CustomComponentVectorHandle, the CustomComponentVectorHandle is not updated
 * and still contains the old references to the custom components (that may be
 * invalidated).
 *
 * @tparam T: The type of the custom component.
 */
template<typename T>
class CustomComponentVectorHandle
{
    std::vector<std::reference_wrapper<T>> mVec;

public:
    class Iterator : public std::vector<std::reference_wrapper<T>>::iterator
    {
        using Base = std::vector<std::reference_wrapper<T>>::iterator;

    public:
        using value_type = T;
        using reference  = value_type&;
        using pointer    = value_type*;

        using Base::Base;

        Iterator(Base it) : Base(it) {}

        reference operator*() const { return Base::operator*().get(); }

        pointer operator->() const { return &(Base::operator*().get()); }
    };

    class ConstIterator :
            public std::vector<std::reference_wrapper<T>>::const_iterator
    {
        using Base = std::vector<std::reference_wrapper<T>>::const_iterator;

    public:
        using value_type = T;
        using reference  = const value_type&;
        using pointer    = const value_type*;

        using Base::Base;

        ConstIterator(Base it) : Base(it) {}

        reference operator*() const { return Base::operator*().get(); }

        pointer operator->() const { return &(Base::operator*().get()); }
    };

    CustomComponentVectorHandle() {}

    CustomComponentVectorHandle(std::vector<std::any>& cc)
    {
        mVec.reserve(cc.size());
        for (std::size_t i = 0; i < cc.size(); ++i) {
            mVec.push_back(std::any_cast<T&>(cc[i]));
        }
    }

    CustomComponentVectorHandle(const std::vector<std::any>& cc)
        requires std::is_const_v<T>
    {
        mVec.reserve(cc.size());
        for (std::size_t i = 0; i < cc.size(); ++i) {
            mVec.push_back(std::any_cast<const std::remove_const_t<T>&>(cc[i]));
        }
    }

    template<typename OtherT>
    requires std::is_same_v<const OtherT, T>
    CustomComponentVectorHandle(
        const CustomComponentVectorHandle<OtherT>& other)
    {
        mVec.reserve(other.size());
        for (std::size_t i = 0; i < other.size(); ++i) {
            mVec.push_back(other[i]);
        }
    }

    bool empty() const { return mVec.empty(); }

    T& at(std::size_t i) { return mVec.at(i).get(); }

    const T& at(std::size_t i) const
    {
        return mVec.at(i).get();
    }

    T& front() { return mVec.front().get(); }

    const T& front() const { return mVec.front().get(); }

    T& back() { return mVec.back().get(); }

    const T& back() const { return mVec.back().get(); }

    std::size_t size() const { return mVec.size(); }

    T& operator[](std::size_t i) { return mVec[i].get(); }

    const T& operator[](std::size_t i) const
    {
        return mVec[i].get();
    }

    Iterator begin() { return Iterator(mVec.begin()); }

    Iterator end() { return Iterator(mVec.end()); }

    ConstIterator begin() const { return ConstIterator(mVec.begin()); }

    ConstIterator end() const { return ConstIterator(mVec.end()); }
};

template<typename T>
using ConstCustomComponentVectorHandle = CustomComponentVectorHandle<const T>;

} // namespace vcl::meshv2

#endif // VCL_MESH_V2_CONTAINERS_BASE_CUSTOM_COMPONENT_VECTOR_HANDLE_H
