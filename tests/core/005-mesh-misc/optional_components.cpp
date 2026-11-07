// VCLib - Visual Computing Library
// Copyright (C) 2021-2026 Visual Computing Lab, ISTI - CNR.
//
// This Source Code Form is subject to the terms of the Mozilla Public License,
// v. 2.0. If a copy of the MPL was not distributed with this file, You can
// obtain one at https://mozilla.org/MPL/2.0/.

#include "types.h"

#include <catch2/catch_test_macros.hpp>

TEST_CASE("Optional Components - Enable Disable")
{
    MyMesh mesh;
    // By default optional components are disabled
    REQUIRE(
        (!mesh.isElementComponentAvailable<ElemId::VERTEX, CompId::NORMAL>()));
    REQUIRE(
        (!mesh.isElementComponentAvailable<ElemId::VERTEX, CompId::COLOR>()));
    REQUIRE((!mesh.isElementComponentAvailable<ElemId::FACE, CompId::COLOR>()));

    // Enable normal
    mesh.enableElementComponent<ElemId::VERTEX, CompId::NORMAL>();
    REQUIRE(
        (mesh.isElementComponentAvailable<ElemId::VERTEX, CompId::NORMAL>()));

    // Disable normal
    mesh.disableElementComponent<ElemId::VERTEX, CompId::NORMAL>();
    REQUIRE(
        (!mesh.isElementComponentAvailable<ElemId::VERTEX, CompId::NORMAL>()));

    // Enable all
    mesh.enableAllOptionalComponents();
    REQUIRE(
        (mesh.isElementComponentAvailable<ElemId::VERTEX, CompId::NORMAL>()));
    REQUIRE(
        (mesh.isElementComponentAvailable<ElemId::VERTEX, CompId::COLOR>()));
    REQUIRE((mesh.isElementComponentAvailable<ElemId::FACE, CompId::COLOR>()));

    // Disable all
    mesh.disableAllOptionalComponents();
    REQUIRE(
        (!mesh.isElementComponentAvailable<ElemId::VERTEX, CompId::NORMAL>()));
    REQUIRE(
        (!mesh.isElementComponentAvailable<ElemId::VERTEX, CompId::COLOR>()));
    REQUIRE((!mesh.isElementComponentAvailable<ElemId::FACE, CompId::COLOR>()));

    // CurvatureMesh optional principal curvature
    CurvatureMesh cmesh;
    REQUIRE((!cmesh.isElementComponentAvailable<
              ElemId::FACE,
              CompId::PRINCIPAL_CURVATURE>()));
    cmesh.enableElementComponent<ElemId::FACE, CompId::PRINCIPAL_CURVATURE>();
    REQUIRE((cmesh.isElementComponentAvailable<
             ElemId::FACE,
             CompId::PRINCIPAL_CURVATURE>()));
    cmesh.disableElementComponent<ElemId::FACE, CompId::PRINCIPAL_CURVATURE>();
    REQUIRE((!cmesh.isElementComponentAvailable<
              ElemId::FACE,
              CompId::PRINCIPAL_CURVATURE>()));

    // QualityMesh optional quality
    QualityMesh qmesh;
    REQUIRE(
        (!qmesh.isElementComponentAvailable<ElemId::FACE, CompId::QUALITY>()));
    qmesh.enableElementComponent<ElemId::FACE, CompId::QUALITY>();
    REQUIRE(
        (qmesh.isElementComponentAvailable<ElemId::FACE, CompId::QUALITY>()));
    qmesh.disableElementComponent<ElemId::FACE, CompId::QUALITY>();
    REQUIRE(
        (!qmesh.isElementComponentAvailable<ElemId::FACE, CompId::QUALITY>()));

    // TangentMesh optional tangent
    TangentMesh tmesh;
    REQUIRE(
        (!tmesh.isElementComponentAvailable<ElemId::FACE, CompId::TANGENT>()));
    tmesh.enableElementComponent<ElemId::FACE, CompId::TANGENT>();
    REQUIRE(
        (tmesh.isElementComponentAvailable<ElemId::FACE, CompId::TANGENT>()));
    tmesh.disableElementComponent<ElemId::FACE, CompId::TANGENT>();
    REQUIRE(
        (!tmesh.isElementComponentAvailable<ElemId::FACE, CompId::TANGENT>()));
}

TEST_CASE("Optional Components - Enable Same Optional Components Of")
{
    MyMesh meshWithNormals;
    meshWithNormals.enableElementComponent<ElemId::VERTEX, CompId::NORMAL>();
    REQUIRE(
        (meshWithNormals
             .isElementComponentAvailable<ElemId::VERTEX, CompId::NORMAL>()));

    MyMesh meshWithoutNormals;
    REQUIRE(
        (!meshWithoutNormals
              .isElementComponentAvailable<ElemId::VERTEX, CompId::NORMAL>()));

    // 1. Mesh-level enableSameOptionalComponentsOf
    meshWithoutNormals.enableSameOptionalComponentsOf(meshWithNormals);
    REQUIRE(
        (meshWithoutNormals
             .isElementComponentAvailable<ElemId::VERTEX, CompId::NORMAL>()));

    // 2. Test with a mesh that does not have the component (remains disabled)
    SimpleMesh simpleMesh;
    MyMesh     meshTestedAgainstSimple;
    REQUIRE(
        (!meshTestedAgainstSimple
              .isElementComponentAvailable<ElemId::VERTEX, CompId::NORMAL>()));
    meshTestedAgainstSimple.enableSameOptionalComponentsOf(simpleMesh);
    REQUIRE(
        (!meshTestedAgainstSimple
              .isElementComponentAvailable<ElemId::VERTEX, CompId::NORMAL>()));
}

TEST_CASE("Optional Components - Non Optional Components")
{
    FullMesh mesh;
    // By default, non-optional components are always available
    REQUIRE(
        (mesh.isElementComponentAvailable<ElemId::VERTEX, CompId::POSITION>()));
    REQUIRE(
        (mesh.isElementComponentAvailable<ElemId::VERTEX, CompId::NORMAL>()));
    REQUIRE(
        (mesh.isElementComponentAvailable<ElemId::VERTEX, CompId::COLOR>()));
    REQUIRE((mesh.isElementComponentAvailable<
             ElemId::FACE,
             CompId::VERTEX_REFERENCES>()));
    REQUIRE((mesh.isElementComponentAvailable<ElemId::FACE, CompId::COLOR>()));

    // disableAllOptionalComponents does not disable non-optional components
    mesh.disableAllOptionalComponents();
    REQUIRE(
        (mesh.isElementComponentAvailable<ElemId::VERTEX, CompId::NORMAL>()));
    REQUIRE(
        (mesh.isElementComponentAvailable<ElemId::VERTEX, CompId::COLOR>()));
    REQUIRE((mesh.isElementComponentAvailable<ElemId::FACE, CompId::COLOR>()));
}
