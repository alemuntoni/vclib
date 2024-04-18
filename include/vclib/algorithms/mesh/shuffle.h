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

#ifndef VCL_ALGORITHMS_MESH_SHUFFLE_H
#define VCL_ALGORITHMS_MESH_SHUFFLE_H

#ifndef VCLIB_WITH_MODULES
#include <random>

#include <vclib/mesh/requirements.h>
#endif

namespace vcl {

namespace detail {

/*
 * Generic implementation of fillAndShuffleVertexPointerVector, to avoid code
 * duplication.
 *
 * Templates M and V can be both const and non-const MeshType and VertexType
 */
template<typename M, typename V>
std::vector<V*> genericFASVPV(M m, bool deterministic)
{
    std::vector<V*> vec;
    vec.reserve(m.vertexNumber());

    for (V& v : m.vertices()) {
        vec.push_back(&v);
    }

    std::random_device rd;
    std::mt19937       gen(rd());
    if (deterministic)
        gen = std::mt19937(0);
    std::shuffle(vec.begin(), vec.end(), gen);
    return vec;
}

/*
 * Generic implementation of fillAndShuffleFacePointerVector, to avoid code
 * duplication.
 *
 * Templates M and F can be both const and non-const MeshType and FaceType
 */
template<typename M, typename F>
std::vector<F*> genericFASFPV(M m, bool deterministic)
{
    std::vector<F*> vec;
    vec.reserve(m.faceNumber());

    for (F& f : m.faces()) {
        vec.push_back(&f);
    }

    std::random_device rd;
    std::mt19937       gen(rd());
    if (deterministic)
        gen = std::mt19937(0);
    std::shuffle(vec.begin(), vec.end(), gen);
    return vec;
}

} // namespace detail

template<MeshConcept MeshType>
std::vector<typename MeshType::VertexType*> fillAndShuffleVertexPointerVector(
    MeshType& m,
    bool      deterministic = false)
{
    using VertexType = MeshType::VertexType;

    return detail::genericFASVPV<MeshType&, VertexType>(m, deterministic);
}

template<MeshConcept MeshType>
std::vector<const typename MeshType::VertexType*>
fillAndShuffleVertexPointerVector(const MeshType& m, bool deterministic = false)
{
    using VertexType = MeshType::VertexType;
    return detail::genericFASVPV<const MeshType&, const VertexType>(
        m, deterministic);
}

template<MeshConcept MeshType>
std::vector<uint> fillAndShuffleVertexIndexVector(
    const MeshType& m,
    bool            deterministic = false)
{
    using VertexType = MeshType::VertexType;

    std::vector<uint> vec;
    vec.reserve(m.vertexNumber());

    for (const VertexType& v : m.vertices()) {
        vec.push_back(m.index(v));
    }

    std::random_device rd;
    std::mt19937       gen(rd());
    if (deterministic)
        gen = std::mt19937(0);
    std::shuffle(vec.begin(), vec.end(), gen);
    return vec;
}

template<FaceMeshConcept MeshType>
std::vector<typename MeshType::FaceType*> fillAndShuffleFacePointerVector(
    MeshType& m,
    bool      deterministic = false)
{
    using FaceType = MeshType::FaceType;

    return detail::genericFASFPV<MeshType&, FaceType>(m, deterministic);
}

template<FaceMeshConcept MeshType>
std::vector<const typename MeshType::FaceType*> fillAndShuffleFacePointerVector(
    const MeshType& m,
    bool            deterministic = false)
{
    using FaceType = MeshType::FaceType;

    return detail::genericFASFPV<const MeshType&, const FaceType>(
        m, deterministic);
}

template<FaceMeshConcept MeshType>
std::vector<uint> fillAndShuffleFaceIndexVector(
    const MeshType& m,
    bool            deterministic = false)
{
    using FaceType = MeshType::FaceType;

    std::vector<uint> vec;
    vec.reserve(m.faceNumber());

    for (const FaceType& f : m.faces()) {
        vec.push_back(m.index(f));
    }

    std::random_device rd;
    std::mt19937       gen(rd());
    if (deterministic)
        gen = std::mt19937(0);
    std::shuffle(vec.begin(), vec.end(), gen);
    return vec;
}

} // namespace vcl

#endif // VCL_ALGORITHMS_MESH_SHUFFLE_H
