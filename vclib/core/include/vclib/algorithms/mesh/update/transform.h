// VCLib - Visual Computing Library
// Copyright (C) 2021-2026 Visual Computing Lab, ISTI - CNR.
//
// This Source Code Form is subject to the terms of the Mozilla Public License,
// v. 2.0. If a copy of the MPL was not distributed with this file, You can
// obtain one at https://mozilla.org/MPL/2.0/.

#ifndef VCL_ALGORITHMS_MESH_UPDATE_TRANSFORM_H
#define VCL_ALGORITHMS_MESH_UPDATE_TRANSFORM_H

#include <vclib/mesh.h>
#include <vclib/space/core.h>

namespace vcl {

template<MeshConcept MeshType, typename ScalarM>
void applyTransformMatrix(
    MeshType&                mesh,
    const Matrix44<ScalarM>& matrix,
    bool                     updateNormals = true)
{
    multiplyPointsByMatrix(mesh.vertices() | vcl::views::positions, matrix);

    // TODO: automatize: for each element, check if it has normal and apply
    // the matrix to it
    if (updateNormals) {
        if constexpr (HasVertexNormals<MeshType>) {
            if (mesh.isVertexNormalAvailable()) {
                multiplyNormalsByMatrix(
                    mesh.vertices() | vcl::views::normals, matrix);
            }
        }
        if constexpr (HasFaceNormals<MeshType>) {
            if (mesh.isFaceNormalAvailable()) {
                multiplyNormalsByMatrix(
                    mesh.faces() | vcl::views::normals, matrix);
            }
        }
    }
}

template<MeshConcept MeshType, PointConcept PointType>
void translate(MeshType& mesh, const PointType& t)
{
    using Scalar = ScalarType<MeshType>;
    for (auto& p : mesh.vertices().positions()) {
        p += t.template cast<Scalar>();
    }
}

template<MeshConcept MeshType, PointConcept PointType>
void scale(MeshType& mesh, const PointType& s)
{
    for (auto& p : mesh.vertices().positions()) {
        p(0) *= s(0);
        p(1) *= s(1);
        p(2) *= s(2);
    }
}

template<MeshConcept MeshType, typename Scalar = double>
void scale(MeshType& mesh, const Scalar& s)
{
    for (auto& p : mesh.vertices().positions()) {
        p *= s;
    }
}

template<MeshConcept MeshType, typename Scalar>
void rotate(
    MeshType&               mesh,
    const Matrix33<Scalar>& m,
    bool                    updateNormals = true)
{
    for (auto&& v : mesh.vertices()) {
        v.position() = m * v.position();
    }

    if (updateNormals) {
        if constexpr (HasVertexNormals<MeshType>) {
            if (mesh.isVertexNormalAvailable()) {
                for (auto&& v : mesh.vertices()) {
                    v.normal() = m * v.normal();
                }
            }
        }

        if constexpr (HasFaceNormals<MeshType>) {
            if (mesh.isFaceNormalAvailable()) {
                for (auto&& f : mesh.faces()) {
                    f.normal() = m * f.normal();
                }
            }
        }
    }
}

template<MeshConcept MeshType, PointConcept PointType, typename Scalar = double>
void rotate(
    MeshType&        mesh,
    const PointType& axis,
    const Scalar&    angleRad,
    bool             updateNormals = true)
{
    using ScalarType = ScalarType<MeshType>;

    Matrix33<ScalarType> m;
    setTransformMatrixRotation(m, axis, angleRad);

    rotate(mesh, m, updateNormals);
}

template<MeshConcept MeshType, PointConcept PointType, typename Scalar = double>
void rotateDeg(
    MeshType&        mesh,
    const PointType& axis,
    const Scalar&    angleDeg,
    bool             updateNormals = true)
{
    rotate(mesh, axis, toRad(angleDeg), updateNormals);
}

} // namespace vcl

#endif // VCL_ALGORITHMS_MESH_UPDATE_TRANSFORM_H
