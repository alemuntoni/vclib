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

#ifndef VCL_SPACE_MATRIX_H
#define VCL_SPACE_MATRIX_H

#include <Eigen/Core>

#include <vclib/concepts/space/matrix.h>
#include <vclib/concepts/space/point.h>

namespace vcl {

template<typename Scalar>
using Matrix33 = Eigen::Matrix<Scalar, 3, 3>;

using Matrix33i = Matrix33<int>;
using Matrix33f = Matrix33<float>;
using Matrix33d = Matrix33<double>;

template<typename Scalar>
using Matrix44 = Eigen::Matrix<Scalar, 4, 4>;

using Matrix44i = Matrix44<int>;
using Matrix44f = Matrix44<float>;
using Matrix44d = Matrix44<double>;

template<typename Scalar>
using Affine3 = Eigen::Transform<Scalar, 3, Eigen::Affine>;

using Affine3i = Affine3<int>;
using Affine3f = Affine3<float>;
using Affine3d = Affine3<double>;

/**
 * @brief Multiplies a matrix and a point
 *
 * This function multiplies a matrix and a point and returns the resulting
 * point.
 *
 * @tparam MatrixType: A type that satisfies the EigenMatrixConcept
 * @tparam PointType: A type that satisfies the PointConcept
 *
 * @param[in] m: The matrix to be multiplied with the point
 * @param[in] p: The point to be multiplied with the matrix
 * @return The resulting point after multiplying the matrix and point
 *
 * @requires MatrixType::RowsAtCompileTime == PointType::DIM &&
 *           MatrixType::ColsAtCompileTime == PointType::DIM
 */
template<EigenMatrixConcept MatrixType, PointConcept PointType>
PointType operator*(const MatrixType& m, const PointType& p) requires (
    MatrixType::RowsAtCompileTime == PointType::DIM &&
    MatrixType::ColsAtCompileTime == PointType::DIM)
{
    PointType res;
    for (uint i = 0; i < PointType::DIM; i++) {
        for (uint j = 0; j < PointType::DIM; j++) {
            res(i) += m(i, j) * p(i);
        }
    }
    return res;
}

} // namespace vcl

#endif // VCL_SPACE_MATRIX_H
