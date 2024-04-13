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
 * it under the terms of the GNU General Public License as published by      *
 * the Free Software Foundation; either version 3 of the License, or         *
 * (at your option) any later version.                                       *
 *                                                                           *
 * This program is distributed in the hope that it will be useful,           *
 * but WITHOUT ANY WARRANTY; without even the implied warranty of            *
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the              *
 * GNU General Public License (http://www.gnu.org/licenses/gpl.txt)          *
 * for more details.                                                         *
 ****************************************************************************/

#include <iostream>

#include <vector>

#include <Eigen/Core>

import vclib;

static_assert(
    vcl::IteratorConcept<std::vector<int>::iterator>,
    "std::vector<int>::iterator is not an iterator");

static_assert(
    vcl::IsPointer<int*>, "int* is not a pointer");

static_assert(
    !vcl::LoggerConcept<int>, "int is a Logger");

static_assert(
    vcl::IntegralOrEnum<vcl::PrimitiveType>, "PrimitiveType is not an integral or enum");

static_assert(
    !vcl::NonBoolIntegralOrEnum<bool>, "bool is a NonBoolIntegralOrEnum");

static_assert(
    vcl::NonBoolIntegralOrEnum<int>, "int is not a NonBoolIntegralOrEnum");

static_assert(
    !vcl::ArrayConcept<std::vector<int>>, "std::vector<int> is an array");

static_assert(
    vcl::EigenMatrixConcept<Eigen::Matrix4f>, "Eigen::Matrix4f is not an Eigen matrix");

static_assert(
    !vcl::PointConcept<Eigen::Vector3d>, "Eigen::Vector3d is a point");

static_assert(
    !vcl::SphereConcept<Eigen::Vector3d>, "Eigen::Vector3d is a sphere");

static_assert(
    vcl::Range<std::vector<int>>, "std::vector<int> is not a Range");

int main()
{
    std::cerr << "Is NAN degenerate: " << vcl::isDegenerate(std::numeric_limits<double>::quiet_NaN()) << std::endl;

    std::vector<int> v(1000000000);

    vcl::Timer t("parallel");

    vcl::parallelFor(v, [](int& el) {
        el = 1;
    });

    t.stopAndPrint();

    t.setCaption("sequential");

    t.start();

    for (int i = 0; i < v.size(); i++) {
        v[i] = 2;
    }

    t.stopAndPrint();

    return 0;
}
