// VCLib - Visual Computing Library
// Copyright (C) 2021-2026 Visual Computing Lab, ISTI - CNR.
//
// This Source Code Form is subject to the terms of the Mozilla Public License,
// v. 2.0. If a copy of the MPL was not distributed with this file, You can
// obtain one at https://mozilla.org/MPL/2.0/.

#ifndef VCL_MESH_V2_PARALLEL_VECTORS_TUPLE_H
#define VCL_MESH_V2_PARALLEL_VECTORS_TUPLE_H

#include <vclib/base.h>

#include <array>
#include <tuple>
#include <utility>
#include <vector>

namespace vcl::meshv2 {

/**
 * @brief A tuple of parallel vectors, where each vector stores a specific type.
 *
 * This class provides an interface similar to `std::tuple`, but it wraps a
 * collection of `std::vector`s. The vectors are meant to be used in parallel
 * (i.e. having the same size). It allows enabling and disabling individual
 * vectors at runtime. When a vector is enabled, it is automatically resized
 * to match the global size of the tuple.
 *
 * Elements can be accessed by their Type or by their Index. If a type is
 * used multiple times in the `Types` variadic template pack, access by Type
 * will result in a compile-time error due to ambiguity, and access by Index
 * must be used instead. By default, all vectors are initially disabled and
 * the tuple size is 0.
 *
 * @tparam Types: The types of the elements stored in the parallel vectors.
 */
template<typename... Types>
class ParallelVectorsTuple
{
    static constexpr uint VECTOR_COUNT = sizeof...(Types);

    std::tuple<std::vector<Types>...> mVecTuple;
    std::array<bool, VECTOR_COUNT>    mVecEnabled {};
    std::size_t                       mSize = 0;

public:
    /**
     * @brief Default constructor. Initializes an empty tuple with all vectors
     * disabled.
     */
    ParallelVectorsTuple() = default;

    /**
     * @brief Returns the total number of types (both enabled and disabled) in
     * the tuple.
     * @return The number of vectors.
     */
    static constexpr uint vectorCount() { return VECTOR_COUNT; }

    /**
     * @brief Checks if a specific Type is present in the variadic template
     * pack.
     * @tparam T The type to check.
     * @return true if the type is found, false otherwise.
     */
    template<typename T>
    constexpr bool hasType() const
    {
        return IndexInTypes<T, Types...>::value != UINT_NULL;
    }

    /**
     * @brief Gets a reference to the vector storing elements of Type T.
     *
     * This will cause a compile-time error if the type T is duplicated in the
     * tuple.
     *
     * @tparam T The type of the vector to retrieve.
     * @return A reference to the vector.
     */
    template<typename T>
    std::vector<T>& get()
    {
        constexpr uint count = (0 + ... + (std::is_same_v<T, Types> ? 1 : 0));
        static_assert(count > 0, "Type not found in ParallelVectorsTuple");
        static_assert(
            count == 1,
            "Ambiguous type in ParallelVectorsTuple: type is duplicated");
        return std::get<std::vector<T>>(mVecTuple);
    }

    /**
     * @brief Gets a const reference to the vector storing elements of Type T.
     *
     * This will cause a compile-time error if the type T is duplicated in the
     * tuple.
     *
     * @tparam T The type of the vector to retrieve.
     * @return A const reference to the vector.
     */
    template<typename T>
    const std::vector<T>& get() const
    {
        constexpr uint count = (0 + ... + (std::is_same_v<T, Types> ? 1 : 0));
        static_assert(count > 0, "Type not found in ParallelVectorsTuple");
        static_assert(
            count == 1,
            "Ambiguous type in ParallelVectorsTuple: type is duplicated");
        return std::get<std::vector<T>>(mVecTuple);
    }

    /**
     * @brief Gets a reference to the vector at the specified Index I.
     * @tparam I The index of the vector to retrieve.
     * @return A reference to the vector.
     */
    template<uint I>
    auto& get()
    {
        static_assert(I < VECTOR_COUNT, "Index out of bounds");
        return std::get<I>(mVecTuple);
    }

    /**
     * @brief Gets a const reference to the vector at the specified Index I.
     * @tparam I The index of the vector to retrieve.
     * @return A const reference to the vector.
     */
    template<uint I>
    const auto& get() const
    {
        static_assert(I < VECTOR_COUNT, "Index out of bounds");
        return std::get<I>(mVecTuple);
    }

    /**
     * @brief Returns the current parallel size of the enabled vectors.
     * @return The number of elements in the enabled vectors.
     */
    std::size_t size() const { return mSize; }

    /**
     * @brief Resizes all currently enabled vectors to the specified size.
     *
     * Note: This operation is only applied to enabled vectors. Disabled vectors
     * remain untouched.
     *
     * @param size The new size for the enabled vectors.
     */
    void resize(std::size_t size)
    {
        if constexpr (vectorCount() > 0) {
            vectorResize<vectorCount() - 1>(size);
        }
        mSize = size;
    }

    /**
     * @brief Reserves capacity for all currently enabled vectors.
     *
     * Note: This operation is only applied to enabled vectors. Disabled vectors
     * remain untouched.
     *
     * @param size The capacity to reserve.
     */
    void reserve(std::size_t size)
    {
        if constexpr (vectorCount() > 0) {
            vectorReserve<vectorCount() - 1>(size);
        }
    }

    /**
     * @brief Clears the contents of all vectors and sets the global size to 0.
     *
     * Note: Unlike resize and reserve, this operation clears ALL vectors,
     * including the disabled ones, to ensure no memory is leaked.
     */
    void clear()
    {
        auto function = [](auto&... args) {
            ((args.clear()), ...);
        };
        std::apply(function, mVecTuple);
        mSize = 0;
    }

    /**
     * @brief Swaps the elements at indices i and j across all currently enabled
     * vectors.
     *
     * Note: This operation is only applied to enabled vectors.
     *
     * @param i The first element index.
     * @param j The second element index.
     */
    void swapElements(uint i, uint j)
    {
        if constexpr (vectorCount() > 0) {
            vectorSwap<vectorCount() - 1>(i, j);
        }
    }

    /**
     * @brief Checks whether the vector of Type T is currently enabled.
     *
     * This will cause a compile-time error if the type T is duplicated in the
     * tuple.
     *
     * @tparam T The type to check.
     * @return true if enabled, false otherwise.
     */
    template<typename T>
    bool isEnabled() const
    {
        constexpr uint count = (0 + ... + (std::is_same_v<T, Types> ? 1 : 0));
        static_assert(count > 0, "Type not found in ParallelVectorsTuple");
        static_assert(
            count == 1,
            "Ambiguous type in ParallelVectorsTuple: type is duplicated");
        constexpr uint ind = IndexInTypes<T, Types...>::value;
        return mVecEnabled[ind];
    }

    /**
     * @brief Checks whether the vector at Index I is currently enabled.
     * @tparam I The index to check.
     * @return true if enabled, false otherwise.
     */
    template<uint I>
    bool isEnabled() const
    {
        static_assert(I < VECTOR_COUNT, "Index out of bounds");
        return mVecEnabled[I];
    }

    /**
     * @brief Enables all vectors and resizes them to the current global size.
     */
    void enableAll()
    {
        if constexpr (vectorCount() > 0) {
            vectorEnableAll<vectorCount() - 1>();
        }
    }

    /**
     * @brief Disables all vectors and clears their contents.
     */
    void disableAll()
    {
        if constexpr (vectorCount() > 0) {
            vectorDisableAll<vectorCount() - 1>();
        }
    }

    /**
     * @brief Enables the vector of Type T and resizes it to the current global
     * size.
     *
     * This will cause a compile-time error if the type T is duplicated in the
     * tuple.
     *
     * @tparam T The type to enable.
     */
    template<typename T>
    void enable()
    {
        constexpr uint count = (0 + ... + (std::is_same_v<T, Types> ? 1 : 0));
        static_assert(count > 0, "Type not found in ParallelVectorsTuple");
        static_assert(
            count == 1,
            "Ambiguous type in ParallelVectorsTuple: type is duplicated");
        constexpr uint ind = IndexInTypes<T, Types...>::value;
        mVecEnabled[ind]   = true;
        std::get<ind>(mVecTuple).resize(mSize);
    }

    /**
     * @brief Enables the vector at Index I and resizes it to the current global
     * size.
     * @tparam I The index to enable.
     */
    template<uint I>
    void enable()
    {
        static_assert(I < VECTOR_COUNT, "Index out of bounds");
        mVecEnabled[I] = true;
        std::get<I>(mVecTuple).resize(mSize);
    }

    /**
     * @brief Disables the vector of Type T and clears its contents.
     *
     * This will cause a compile-time error if the type T is duplicated in the
     * tuple.
     *
     * @tparam T The type to disable.
     */
    template<typename T>
    void disable()
    {
        constexpr uint count = (0 + ... + (std::is_same_v<T, Types> ? 1 : 0));
        static_assert(count > 0, "Type not found in ParallelVectorsTuple");
        static_assert(
            count == 1,
            "Ambiguous type in ParallelVectorsTuple: type is duplicated");
        constexpr uint ind = IndexInTypes<T, Types...>::value;
        mVecEnabled[ind]   = false;
        std::get<ind>(mVecTuple).clear();
    }

    /**
     * @brief Disables the vector at Index I and clears its contents.
     * @tparam I The index to disable.
     */
    template<uint I>
    void disable()
    {
        static_assert(I < VECTOR_COUNT, "Index out of bounds");
        mVecEnabled[I] = false;
        std::get<I>(mVecTuple).clear();
    }

private:
    template<std::size_t N>
    void vectorEnableAll()
    {
        enable<N>();
        if constexpr (N != 0) {
            vectorEnableAll<N - 1>();
        }
    }

    template<std::size_t N>
    void vectorDisableAll()
    {
        disable<N>();
        if constexpr (N != 0) {
            vectorDisableAll<N - 1>();
        }
    }

    template<std::size_t N>
    void vectorResize(std::size_t size)
    {
        if (mVecEnabled[N]) {
            std::get<N>(mVecTuple).resize(size);
        }
        if constexpr (N != 0) {
            vectorResize<N - 1>(size);
        }
    }

    template<std::size_t N>
    void vectorReserve(std::size_t size)
    {
        if (mVecEnabled[N]) {
            std::get<N>(mVecTuple).reserve(size);
        }
        if constexpr (N != 0) {
            vectorReserve<N - 1>(size);
        }
    }

    template<std::size_t N>
    void vectorSwap(uint i, uint j)
    {
        if (mVecEnabled[N]) {
            auto& vec = std::get<N>(mVecTuple);
            std::swap(vec[i], vec[j]);
        }
        if constexpr (N != 0) {
            vectorSwap<N - 1>(i, j);
        }
    }
};

template<typename... Types>
class ParallelVectorsTuple<TypeWrapper<Types...>> :
        public ParallelVectorsTuple<Types...>
{
};

} // namespace vcl::meshv2

#endif // VCL_MESH_V2_PARALLEL_VECTORS_TUPLE_H
