/*****************************************************************************
 * VCLib                                                                     *
 * Visual Computing Library                                                  *
 *                                                                           *
 * Copyright(C) 2021-2024                                                    *
 * Visual Computing Lab                                                      *
 * ISTI - Italian National Research Council                                  *
 *                                                                           *
 * All rights reserved.                                                      *
 *                                                                           *
 * This program is free software; you can redistribute it and/or modify      *
 * it under the terms of the Mozilla Public License Version 2.0 as published *
 * by the Mozilla Foundation; either version 2 of the License, or            *
 * (at your option) any later version.                                       *
 *                                                                           *
 * This program is distributed in the hope that it will be useful,           *
 * but WITHOUT ANY WARRANTY; without even the implied warranty of            *
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the              *
 * Mozilla Public License Version 2.0                                        *
 * (https://www.mozilla.org/en-US/MPL/2.0/) for more details.                *
 ****************************************************************************/

#ifndef VCL_ALGORITHMS_CORE_DISTANCE_FUNCTIONS_H
#define VCL_ALGORITHMS_CORE_DISTANCE_FUNCTIONS_H

#include "element.h"
#include "misc.h"

#include <vclib/concepts/mesh.h>

namespace vcl {

// generic case - nothing is done here. It will fail because a specialization
// must exist
template<typename Obj1, typename Obj2, typename ScalarType>
struct DistFunctionStruct
{
    static_assert(
        sizeof(Obj1) != sizeof(Obj1), // always fail
        "There is no a DistFunctionStruct specialization for the given types.");
};

/**
 * @brief Return a proper dist function between a Obj1 object and an Obj2
 * object.
 *
 * The function will return a function called `distFun` defined in a
 * specialization of the struct DistFunctionStruct.
 *
 * The returned function is a std::function that takes in input two const
 * references of the desired types and returns a scalar that is the distance
 * between the two objects.
 *
 * If the distance function for your types is not defined, you can write your
 * own DistFunctionStruct specialization that defines a proper `static const
 * inline` object called `distFun` of `std::function` type.
 *
 * @note The distFunction works only for non-pointer types. This means that you
 * cannot ask for the distance function between a vcl::Point3d and a
 * vcl::Vertex*. If you are working with template types that you don't know if
 * they are pointers or not, you can do the following:
 * @code{.cpp}
 * // don't know if T1 and T2 are pointers or non-pointers
 * auto f = distFunction<RemoveCVRefAndPointer<T1>,
 * RemoveCVRefAndPointer<T2>>();
 * // obj1 is of type T1, obj2 is of type T2 (may be pointers or non-pointers)
 * auto dist = f(dereferencePtr(obj1), dereferencePtr(obj2));
 * @endcode
 *
 * @ingroup distance
 */
template<typename Obj1, typename Obj2>
auto distFunction()
{
    return DistFunctionStruct<Obj1, Obj2, double>::distFun;
}

/**
 * @brief Return a proper dist function between a Obj1 object and an Obj2
 * object.
 *
 * The function will return a function called `boundDistFun` defined in a
 * specialization of the struct DistFunctionStruct.
 *
 * This function is different w.r.t. the distFunction because the returned
 * function here takes three arguments: the two objects on which compute the
 * distance, and a scalar that represent the maximum distance that can be
 * returned. This value is used by some distance functions for performance
 * reasons: it allows to avoid several computation if a first distance does
 * already exceed the maximum value.
 *
 * The returned function is a std::function that takes in input two const
 * references of the desired types and a scalar that is the maximum distance
 * expected and returns a scalar that is the distance between the two objects.
 * If the returned value is >= the input maximum distance, the distance was not
 * computed totally and should be discarded.
 *
 * If the distance function for your types is not defined, you can write your
 * own DistFunctionStruct specialization that defines a proper `static const
 * inline` object called `boundDistFun` of `std::function` type.
 *
 * @note The boundedDistFunction works only for non-pointer types. This means
 * that you cannot ask for the distance function between a vcl::Point3d and a
 * vcl::Vertex*. If you are working with template types that you don't know if
 * they are pointers or not, you can do the following:
 * @code{.cpp}
 * // don't know if T1 and T2 are pointers or non-pointers
 * auto f = boundedDistFunction<RemoveCVRefAndPointer<T1>,
 * RemoveCVRefAndPointer<T2>, double>();
 * // obj1 is of type T1, obj2 is of type T2 (may be pointers or non-pointers)
 * auto dist = f(dereferencePtr(obj1), dereferencePtr(obj2), maxDist);
 * @endcode
 *
 * @ingroup distance
 */
template<typename Obj1, typename Obj2, typename ScalarType = double>
auto boundedDistFunction()
{
    return DistFunctionStruct<Obj1, Obj2, ScalarType>::boundDistFun;
}

/********* DistFunctionStruct Specializations *********/

// Specialization for distance between two points
template<PointConcept Obj1, PointConcept Obj2, typename ScalarType>
requires (Obj1::DIM == Obj2::DIM)
struct DistFunctionStruct<Obj1, Obj2, ScalarType>
{
    static inline const auto distFun = [](const Obj1& o1, const Obj2& o2) {
        return o1.dist(o2);
    };

    static inline const auto boundDistFun =
        [](const Obj1& o1, const Obj2& o2, ScalarType) {
            return o1.dist(o2);
        };
};

// Specialization for distance between a point and a segment
template<PointConcept Obj1, SegmentConcept Obj2, typename ScalarType>
requires (Obj1::DIM == Obj2::DIM)
struct DistFunctionStruct<Obj1, Obj2, ScalarType>
{
    static inline const auto distFun = [](const Obj1& o1, const Obj2& o2) {
        return pointSegmentDistance(o1, o2);
    };

    static inline const auto boundDistFun =
        [](const Obj1& o1, const Obj2& o2, ScalarType) {
            return pointSegmentDistance(o1, o2);
        };
};

// Specialization for distance between a segment and a point
template<SegmentConcept Obj1, PointConcept Obj2, typename ScalarType>
requires (Obj1::DIM == Obj2::DIM)
struct DistFunctionStruct<Obj1, Obj2, ScalarType>
{
    static inline const auto distFun = [](const Obj1& o1, const Obj2& o2) {
        return pointSegmentDistance(o2, o1);
    };

    static inline const auto boundDistFun =
        [](const Obj1& o1, const Obj2& o2, ScalarType) {
            return pointSegmentDistance(o2, o1);
        };
};

// Specialization for distance between Vertex and Point
template<VertexConcept Obj1, Point3Concept Obj2, typename ScalarType>
struct DistFunctionStruct<Obj1, Obj2, ScalarType>
{
    static inline const auto distFun = [](const Obj1& o1, const Obj2& o2) {
        return o1.coord().dist(o2);
    };

    static inline const auto boundDistFun =
        [](const Obj1& o1, const Obj2& o2, ScalarType) {
            return o1.coord().dist(o2);
        };
};

// Specialization for distance between Point and Vertex
template<Point3Concept Obj1, VertexConcept Obj2, typename ScalarType>
struct DistFunctionStruct<Obj1, Obj2, ScalarType>
{
    static inline auto distFun = [](const Obj1& o1, const Obj2& o2) {
        return o1.dist(o2.coord());
    };

    static inline const std::function<
        ScalarType(const Obj1&, const Obj2&, ScalarType)>
        boundDistFun = [](const Obj1& o1, const Obj2& o2, ScalarType) {
            return o1.dist(o2.coord());
        };
};

// Specialization for distance between Vertex and Vertex
template<VertexConcept Obj1, VertexConcept Obj2, typename ScalarType>
struct DistFunctionStruct<Obj1, Obj2, ScalarType>
{
    static inline const auto distFun = [](const Obj1& o1, const Obj2& o2) {
        return o1.coord().dist(o2.coord());
    };

    static inline const auto boundDistFun =
        [](const Obj1& o1, const Obj2& o2, ScalarType) {
            return o1.coord().dist(o2.coord());
        };
};

// Specialization for distance between Point and Face
template<Point3Concept Obj1, FaceConcept Obj2, typename ScalarType>
struct DistFunctionStruct<Obj1, Obj2, ScalarType>
{
    static const inline auto distFun = [](const Obj1& o1, const Obj2& o2) {
        return pointFaceDistance(o1, o2);
    };

    static const inline auto boundDistFun =
        [](const Obj1& o1, const Obj2& o2, ScalarType m) {
            return pointFaceDistance(o1, o2, m);
        };
};

// Specialization for distance between Face and Point
template<FaceConcept Obj1, Point3Concept Obj2, typename ScalarType>
struct DistFunctionStruct<Obj1, Obj2, ScalarType>
{
    static const inline auto distFun = [](const Obj1& o1, const Obj2& o2) {
        return pointFaceDistance(o2, o1);
    };

    static const inline auto boundDistFun =
        [](const Obj1& o1, const Obj2& o2, ScalarType m) {
            return pointFaceDistance(o2, o1, m);
        };
};

// Specialization for distance between Vertex and Face
template<VertexConcept Obj1, FaceConcept Obj2, typename ScalarType>
struct DistFunctionStruct<Obj1, Obj2, ScalarType>
{
    static const inline auto distFun = [](const Obj1& o1, const Obj2& o2) {
        return pointFaceDistance(o1.coord(), o2);
    };

    static const inline auto boundDistFun =
        [](const Obj1& o1, const Obj2& o2, ScalarType m) {
            return pointFaceDistance(o1.coord(), o2, m);
        };
};

// Specialization for distance between Face and Vertex
template<FaceConcept Obj1, VertexConcept Obj2, typename ScalarType>
struct DistFunctionStruct<Obj1, Obj2, ScalarType>
{
    static const inline auto distFun = [](const Obj1& o1, const Obj2& o2) {
        return pointFaceDistance(o2.coord(), o1);
    };

    static const inline auto boundDistFun =
        [](const Obj1& o1, const Obj2& o2, ScalarType m) {
            return pointFaceDistance(o2.coord(), o1, m);
        };
};

} // namespace vcl

#endif // VCL_ALGORITHMS_CORE_DISTANCE_FUNCTIONS_H
