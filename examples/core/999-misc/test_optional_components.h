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
    assert((!mesh.isPerElementComponentAvailable<meshv2::ElemId::VERTEX, meshv2::CompId::NORMAL>()));
    assert((!mesh.isPerElementComponentAvailable<meshv2::ElemId::VERTEX, meshv2::CompId::COLOR>()));
    assert((!mesh.isPerElementComponentAvailable<meshv2::ElemId::FACE, meshv2::CompId::COLOR>()));

    // Enable normal
    mesh.enablePerElementComponent<meshv2::ElemId::VERTEX, meshv2::CompId::NORMAL>();
    assert((mesh.isPerElementComponentAvailable<meshv2::ElemId::VERTEX, meshv2::CompId::NORMAL>()));

    // Disable normal
    mesh.disablePerElementComponent<meshv2::ElemId::VERTEX, meshv2::CompId::NORMAL>();
    assert((!mesh.isPerElementComponentAvailable<meshv2::ElemId::VERTEX, meshv2::CompId::NORMAL>()));

    // Enable all
    mesh.enableAllOptionalComponents();
    assert((mesh.isPerElementComponentAvailable<meshv2::ElemId::VERTEX, meshv2::CompId::NORMAL>()));
    assert((mesh.isPerElementComponentAvailable<meshv2::ElemId::VERTEX, meshv2::CompId::COLOR>()));
    assert((mesh.isPerElementComponentAvailable<meshv2::ElemId::FACE, meshv2::CompId::COLOR>()));

    // Disable all
    mesh.disableAllOptionalComponents();
    assert((!mesh.isPerElementComponentAvailable<meshv2::ElemId::VERTEX, meshv2::CompId::NORMAL>()));
    assert((!mesh.isPerElementComponentAvailable<meshv2::ElemId::VERTEX, meshv2::CompId::COLOR>()));
    assert((!mesh.isPerElementComponentAvailable<meshv2::ElemId::FACE, meshv2::CompId::COLOR>()));

    // CurvatureMesh optional principal curvature
    CurvatureMesh cmesh;
    assert((!cmesh.isPerElementComponentAvailable<meshv2::ElemId::FACE, meshv2::CompId::PRINCIPAL_CURVATURE>()));
    cmesh.enablePerElementComponent<meshv2::ElemId::FACE, meshv2::CompId::PRINCIPAL_CURVATURE>();
    assert((cmesh.isPerElementComponentAvailable<meshv2::ElemId::FACE, meshv2::CompId::PRINCIPAL_CURVATURE>()));
    cmesh.disablePerElementComponent<meshv2::ElemId::FACE, meshv2::CompId::PRINCIPAL_CURVATURE>();
    assert((!cmesh.isPerElementComponentAvailable<meshv2::ElemId::FACE, meshv2::CompId::PRINCIPAL_CURVATURE>()));
}

inline void testEnableSameOptionalComponentsOf()
{
    MyMesh meshWithNormals;
    meshWithNormals.enablePerElementComponent<meshv2::ElemId::VERTEX, meshv2::CompId::NORMAL>();
    assert((meshWithNormals.isPerElementComponentAvailable<meshv2::ElemId::VERTEX, meshv2::CompId::NORMAL>()));

    MyMesh meshWithoutNormals;
    assert((!meshWithoutNormals.isPerElementComponentAvailable<meshv2::ElemId::VERTEX, meshv2::CompId::NORMAL>()));

    // 1. Mesh-level enableSameOptionalComponentsOf
    meshWithoutNormals.enableSameOptionalComponentsOf(meshWithNormals);
    assert((meshWithoutNormals.isPerElementComponentAvailable<meshv2::ElemId::VERTEX, meshv2::CompId::NORMAL>()));

    // 2. Test with a mesh that does not have the component (remains disabled)
    SimpleMesh simpleMesh;
    MyMesh meshTestedAgainstSimple;
    assert((!meshTestedAgainstSimple.isPerElementComponentAvailable<meshv2::ElemId::VERTEX, meshv2::CompId::NORMAL>()));
    meshTestedAgainstSimple.enableSameOptionalComponentsOf(simpleMesh);
    assert((!meshTestedAgainstSimple.isPerElementComponentAvailable<meshv2::ElemId::VERTEX, meshv2::CompId::NORMAL>()));
}

inline void testNonOptionalComponents()
{
    FullMesh mesh;
    // By default, non-optional components are always available
    assert((mesh.isPerElementComponentAvailable<meshv2::ElemId::VERTEX, meshv2::CompId::POSITION>()));
    assert((mesh.isPerElementComponentAvailable<meshv2::ElemId::VERTEX, meshv2::CompId::NORMAL>()));
    assert((mesh.isPerElementComponentAvailable<meshv2::ElemId::VERTEX, meshv2::CompId::COLOR>()));
    assert((mesh.isPerElementComponentAvailable<meshv2::ElemId::FACE, meshv2::CompId::VERTEX_REFERENCES>()));
    assert((mesh.isPerElementComponentAvailable<meshv2::ElemId::FACE, meshv2::CompId::COLOR>()));

    // disableAllOptionalComponents does not disable non-optional components
    mesh.disableAllOptionalComponents();
    assert((mesh.isPerElementComponentAvailable<meshv2::ElemId::VERTEX, meshv2::CompId::NORMAL>()));
    assert((mesh.isPerElementComponentAvailable<meshv2::ElemId::VERTEX, meshv2::CompId::COLOR>()));
    assert((mesh.isPerElementComponentAvailable<meshv2::ElemId::FACE, meshv2::CompId::COLOR>()));
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
