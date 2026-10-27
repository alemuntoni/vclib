// VCLib - Visual Computing Library
// Copyright (C) 2021-2026 Visual Computing Lab, ISTI - CNR.
//
// This Source Code Form is subject to the terms of the Mozilla Public License,
// v. 2.0. If a copy of the MPL was not distributed with this file, You can
// obtain one at https://mozilla.org/MPL/2.0/.

#ifndef TEST_OPTIONAL_COMPONENTS_H
#define TEST_OPTIONAL_COMPONENTS_H

#include "types.h"

inline void testEnableDisable()
{
    MyMesh mesh;
    // By default optional components are disabled
    assert((!mesh.isElementComponentAvailable<ElemId::VERTEX, CompId::NORMAL>()));
    assert((!mesh.isElementComponentAvailable<ElemId::VERTEX, CompId::COLOR>()));
    assert((!mesh.isElementComponentAvailable<ElemId::FACE, CompId::COLOR>()));

    // Enable normal
    mesh.enableElementComponent<ElemId::VERTEX, CompId::NORMAL>();
    assert((mesh.isElementComponentAvailable<ElemId::VERTEX, CompId::NORMAL>()));

    // Disable normal
    mesh.disableElementComponent<ElemId::VERTEX, CompId::NORMAL>();
    assert((!mesh.isElementComponentAvailable<ElemId::VERTEX, CompId::NORMAL>()));

    // Enable all
    mesh.enableAllOptionalComponents();
    assert((mesh.isElementComponentAvailable<ElemId::VERTEX, CompId::NORMAL>()));
    assert((mesh.isElementComponentAvailable<ElemId::VERTEX, CompId::COLOR>()));
    assert((mesh.isElementComponentAvailable<ElemId::FACE, CompId::COLOR>()));

    // Disable all
    mesh.disableAllOptionalComponents();
    assert((!mesh.isElementComponentAvailable<ElemId::VERTEX, CompId::NORMAL>()));
    assert((!mesh.isElementComponentAvailable<ElemId::VERTEX, CompId::COLOR>()));
    assert((!mesh.isElementComponentAvailable<ElemId::FACE, CompId::COLOR>()));

    // CurvatureMesh optional principal curvature
    CurvatureMesh cmesh;
    assert((!cmesh.isElementComponentAvailable<ElemId::FACE, CompId::PRINCIPAL_CURVATURE>()));
    cmesh.enableElementComponent<ElemId::FACE, CompId::PRINCIPAL_CURVATURE>();
    assert((cmesh.isElementComponentAvailable<ElemId::FACE, CompId::PRINCIPAL_CURVATURE>()));
    cmesh.disableElementComponent<ElemId::FACE, CompId::PRINCIPAL_CURVATURE>();
    assert((!cmesh.isElementComponentAvailable<ElemId::FACE, CompId::PRINCIPAL_CURVATURE>()));

    // QualityMesh optional quality
    QualityMesh qmesh;
    assert((!qmesh.isElementComponentAvailable<ElemId::FACE, CompId::QUALITY>()));
    qmesh.enableElementComponent<ElemId::FACE, CompId::QUALITY>();
    assert((qmesh.isElementComponentAvailable<ElemId::FACE, CompId::QUALITY>()));
    qmesh.disableElementComponent<ElemId::FACE, CompId::QUALITY>();
    assert((!qmesh.isElementComponentAvailable<ElemId::FACE, CompId::QUALITY>()));

    // TangentMesh optional tangent
    TangentMesh tmesh;
    assert((!tmesh.isElementComponentAvailable<ElemId::FACE, CompId::TANGENT>()));
    tmesh.enableElementComponent<ElemId::FACE, CompId::TANGENT>();
    assert((tmesh.isElementComponentAvailable<ElemId::FACE, CompId::TANGENT>()));
    tmesh.disableElementComponent<ElemId::FACE, CompId::TANGENT>();
    assert((!tmesh.isElementComponentAvailable<ElemId::FACE, CompId::TANGENT>()));
}

inline void testEnableSameOptionalComponentsOf()
{
    MyMesh meshWithNormals;
    meshWithNormals.enableElementComponent<ElemId::VERTEX, CompId::NORMAL>();
    assert((meshWithNormals.isElementComponentAvailable<ElemId::VERTEX, CompId::NORMAL>()));

    MyMesh meshWithoutNormals;
    assert((!meshWithoutNormals.isElementComponentAvailable<ElemId::VERTEX, CompId::NORMAL>()));

    // 1. Mesh-level enableSameOptionalComponentsOf
    meshWithoutNormals.enableSameOptionalComponentsOf(meshWithNormals);
    assert((meshWithoutNormals.isElementComponentAvailable<ElemId::VERTEX, CompId::NORMAL>()));

    // 2. Test with a mesh that does not have the component (remains disabled)
    SimpleMesh simpleMesh;
    MyMesh meshTestedAgainstSimple;
    assert((!meshTestedAgainstSimple.isElementComponentAvailable<ElemId::VERTEX, CompId::NORMAL>()));
    meshTestedAgainstSimple.enableSameOptionalComponentsOf(simpleMesh);
    assert((!meshTestedAgainstSimple.isElementComponentAvailable<ElemId::VERTEX, CompId::NORMAL>()));
}

inline void testNonOptionalComponents()
{
    FullMesh mesh;
    // By default, non-optional components are always available
    assert((mesh.isElementComponentAvailable<ElemId::VERTEX, CompId::POSITION>()));
    assert((mesh.isElementComponentAvailable<ElemId::VERTEX, CompId::NORMAL>()));
    assert((mesh.isElementComponentAvailable<ElemId::VERTEX, CompId::COLOR>()));
    assert((mesh.isElementComponentAvailable<ElemId::FACE, CompId::VERTEX_REFERENCES>()));
    assert((mesh.isElementComponentAvailable<ElemId::FACE, CompId::COLOR>()));

    // disableAllOptionalComponents does not disable non-optional components
    mesh.disableAllOptionalComponents();
    assert((mesh.isElementComponentAvailable<ElemId::VERTEX, CompId::NORMAL>()));
    assert((mesh.isElementComponentAvailable<ElemId::VERTEX, CompId::COLOR>()));
    assert((mesh.isElementComponentAvailable<ElemId::FACE, CompId::COLOR>()));
}

inline void testOptionalComponents()
{
    std::println("--- Testing Optional & Non-Optional Components ---");
    testEnableDisable();
    testEnableSameOptionalComponentsOf();
    testNonOptionalComponents();
    std::println("Optional & Non-Optional components verified successfully!\n");
}

#endif // TEST_OPTIONAL_COMPONENTS_H
